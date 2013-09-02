#include "RenderPipeline.h"
#include "RenderPrimitive.h"
#include "FrameStackAllocator.h"
#include "RenderContext.h"
#include "Light.h"
#include "ZPMathDependency.h"
#include "Material.h"
#include "IRender.h"
#include "Plane.h"

namespace Render
{


	RenderPipeline::RenderPipeline(void):
	m_pRenderContext( NULL )
	{
		m_pixelShader.SetShadeModel( WIREFRAME_MODEL );
	}


	RenderPipeline::~RenderPipeline(void)
	{
		m_pRenderContext = NULL;
	}

	void RenderPipeline::DrawElements( RenderPrimitive& renderPrimitive )
	{ 
		FrameStackAllocator::GetInstance()->Clear();

		if( NULL == m_pRenderContext )
		{
			return;
		}

		m_rasterProcessor.SetColorBuffer( &( m_pRenderContext->GetColorFrameBuffer() ) );
		m_rasterProcessor.SetDepthBuffer( &( m_pRenderContext->GetDepthFrameBuffer() ) );
		m_pixelShader.SetMaterial( m_pRenderContext->GetCurrMaterial() );

		m_renderList.Clear(); 
		m_renderList.CopyFromRenderPrimitive( renderPrimitive ); 

		this->RunTransformStage();
		this->RunLightingStage();
		this->RunPostLightingStage();
		this->RunRasterizationStage();
	}

	void RenderPipeline::RunTransformStage( void )
	{
		InitRVerts();
		TransformFromLocalSpaceToCameraSpace();
		RemoveBackFaceInCameraSpace(); 
		ClipFacesInCameraSpace();
	}

	void RenderPipeline::RunLightingStage( void )
	{ 
	}

	void RenderPipeline::RunPostLightingStage( void )
	{
		TransformFromCameraSpaceToProjectionSpace();
		TransformFromProjectionSpaceToScreenSpace();
		PerspectiveCorrection();
	}


	void RenderPipeline::RunRasterizationStage( void )
	{
		DrawFacesWireFrameToFrameBuffer();
	}
	 

	void RenderPipeline::InitRVerts( void )
	{
		RVertex* pVerts = m_renderList.GetRTransVerts();
		for( unsigned int uiVert = 0 ; uiVert < m_renderList.RTransVertCount()  ; uiVert++ )
		{ 
			pVerts[uiVert].ClearAttriBits();
			switch( m_pixelShader.GetShadeModel() )
			{
			case WIREFRAME_MODEL:
				pVerts[uiVert].SetAttriBit( RVERT_ATTRI_SHADE_WIREFRAME );
				break;
			case FLAT_MODEL:
				pVerts[uiVert].SetAttriBit( RVERT_ATTRI_SHADE_FLAT|RVERT_ATTRI_SHADE_WITH_TEXTURE );
				break;
			case GOURAUD_MODEL:
				pVerts[uiVert].SetAttriBit( RVERT_ATTRI_SHADE_GOURAUD|RVERT_ATTRI_SHADE_WITH_TEXTURE );
				break;
			case PHONG_MODEL:
				pVerts[uiVert].SetAttriBit( RVERT_ATTRI_SHADE_PHONG|RVERT_ATTRI_SHADE_WITH_TEXTURE );
				break;
			case NORMMAP_MODEL:
				pVerts[uiVert].SetAttriBit( RVERT_ATTRI_SHADE_NORMMAP|RVERT_ATTRI_SHADE_WITH_TEXTURE );
				break;
			default:
				pVerts[uiVert].SetAttriBit( RVERT_ATTRI_SHADE_WIREFRAME );
				break;
			} 
		}
	}


	void RenderPipeline::TransformFromLocalSpaceToCameraSpace( void )
	{
		Math::Matrix3 normalMatrix = m_pRenderContext->GetCurrModelViewMatrix().GetRotatePart();
		RVertex* pVerts = m_renderList.GetRTransVerts();
		
		for( unsigned int uiVert = 0 ; uiVert < m_renderList.VertCount()  ; uiVert++ )
		{
			Math::Vec4 v4Pos( m_renderList.GetRVerts()[uiVert].m_v3Pos );
			v4Pos = v4Pos * m_pRenderContext->GetCurrModelViewMatrix();
			
			pVerts[uiVert].m_v3Pos = Math::Vec3( v4Pos.x , v4Pos.y , v4Pos.z );
			//������ռ��еĶ��㱣��һ�ݣ����������ع���ʹ��
			pVerts[uiVert].m_v3PosInCam = pVerts[uiVert].m_v3Pos;
			pVerts[uiVert].m_v3Normal = pVerts[uiVert].m_v3Normal * normalMatrix;

			if( m_pixelShader.GetShadeModel() == NORMMAP_MODEL )
			{
				pVerts[uiVert].m_v3Tangent = pVerts[uiVert].m_v3Tangent * normalMatrix;
				pVerts[uiVert].m_v3Binormal = pVerts[uiVert].m_v3Binormal * normalMatrix;
			}
		}
	}
	
	void RenderPipeline::ClipFacesInCameraSpace( void )
	{
		RFace* pFace = m_renderList.GetFaceList();
		RVertex* pVerts = m_renderList.GetRTransVerts();

		while( NULL != pFace )
		{
			if( !pFace->IsActive() )
			{
				pFace = pFace->m_pNext;
				continue;
			}

			unsigned int* pIndices =  pFace->m_uiIndices; 

			if( m_pRenderContext->GetFrustum().IsOutSide( 
				pVerts[pIndices[0]].m_v3Pos , 
				pVerts[pIndices[1]].m_v3Pos ,
				pVerts[pIndices[2]].m_v3Pos ) )
			{
				pFace->SetStateBit( RFACE_STATE_CLIPED );
				pFace = pFace->m_pNext;
				continue;
			}

			Math::Plane nearClipPlane = m_pRenderContext->GetFrustum().GetNearClipPlane();

			//����������û�����ƽ���ཻ ������ָ�
			if( nearClipPlane.IsFrontSide( 
				pVerts[pIndices[0]].m_v3Pos , 
				pVerts[pIndices[1]].m_v3Pos ,
				pVerts[pIndices[2]].m_v3Pos ) )
			{
				pFace = pFace->m_pNext;
				continue;
			}

			int iFrontSideVertCount = 0;
			int iClassifyCodes[3] = {0};
			
			//��������������������
			for( int iVert = 0 ; iVert < 3 ; iVert++ )
			{
				iClassifyCodes[iVert] = 
					nearClipPlane.ClassifyPoint( pVerts[pIndices[iVert]].m_v3Pos ) ;
				if( Math::FRONT_SIDE == iClassifyCodes[iVert] )
				{
					iFrontSideVertCount++;
				}
			}

			ZP_ASSERT( 0 != iFrontSideVertCount  );

			if( iFrontSideVertCount == 1 )
			{//���1ֻ��һ��������ƽ������

				int j = 0;
				unsigned int uiNewIndices[3] = {0}; 

				//�ֱ�������߼���
				for( int iLine = 0 ; iLine < 3 ; iLine++ )
				{
					int iP0 = iLine;
					int iP1 = ( iLine + 1 )%3; 
					 
					if(  iClassifyCodes[iP0] > Math::ON_THE_PLANE  )
					{
						uiNewIndices[ j++] = pIndices[iP0] ;  
					} 

					if( iClassifyCodes[iP0] < Math::ON_THE_PLANE )
					{
						pVerts[ pIndices[iP0] ].UnSetStateBit( RVERT_STATE_ACTIVE ); 
					}
					
					if( iClassifyCodes[iP1] == Math::ON_THE_PLANE )
					{
						continue;
					}

					if( iClassifyCodes[iP0] != iClassifyCodes[iP1] )
					{
						Real t = 0.0f;
						nearClipPlane.ClipLineSegment( 
								pVerts[ pIndices[iP0] ].m_v3Pos , 
								pVerts[ pIndices[iP1] ].m_v3Pos , 
								t
							); 

						RVertex rvNewVert = pVerts[ pIndices[iP0] ] + ( pVerts[ pIndices[iP1] ] - pVerts[ pIndices[iP0]] ) * t;
						rvNewVert.m_uiAttri =  pVerts[ pIndices[iP0] ].m_uiAttri;
						rvNewVert.m_uiState = RVERT_STATE_ACTIVE;

						unsigned int uiNewIndex = 
							m_renderList.AddTransVert( rvNewVert );

						uiNewIndices[j++] = uiNewIndex;
					}

				}//for( int iLine = 0 ; iLine < 3 ; iLine++ )
				 
				for( int iVert = 0 ; iVert < 3 ; iVert++ )
				{
					pIndices[iVert] = uiNewIndices[iVert];
				}

			}else{ //���2 ֻ������������ƽ������
				 
				RFace* pNewFace = (RFace*)FrameStackAllocator::GetInstance()->Alloc( sizeof(RFace) );
				new ( static_cast<void*>(pNewFace) ) RFace( *pFace );
				ZP_ASSERT( NULL != pNewFace ); 

				int j = 0;
				unsigned int uiNewIndices[4] = {0};

				for( int iLine = 0 ; iLine < 3 ; iLine++ )
				{
					int iP0 = iLine;
					int iP1 = ( iLine + 1 )%3; 

					if( iClassifyCodes[iP0] < Math::ON_THE_PLANE )
					{
						pVerts[ pIndices[iP0] ].UnSetStateBit( RVERT_STATE_ACTIVE ); 
					}
					
					if( iClassifyCodes[iP0] > Math::ON_THE_PLANE  )
					{
						uiNewIndices[j++] = pIndices[iP0];
					}


					if( iClassifyCodes[iP0] != iClassifyCodes[iP1] )
					{
						Real t = 0.0f;
						nearClipPlane.ClipLineSegment( 
							pVerts[ pIndices[iP0] ].m_v3Pos , 
							pVerts[ pIndices[iP1] ].m_v3Pos , 
							t
							);

						//����ָ��������ɵĶ��㲢�����б���
						RVertex rvNewVert = pVerts[ pIndices[iP0] ] + ( pVerts[ pIndices[iP1] ] - pVerts[ pIndices[iP0]] ) * t ;
						rvNewVert.m_uiAttri =  pVerts[ pIndices[iP0] ].m_uiAttri;
						rvNewVert.m_uiState = RVERT_STATE_ACTIVE;

						unsigned int uiNewIndex = 
							m_renderList.AddTransVert( rvNewVert );
						 
						uiNewIndices[j++] = uiNewIndex;
					}

				}//for( int iLine = 0 ; iLine < 3 ; iLine++ )

				pIndices[0] = uiNewIndices[0];
				pIndices[1] = uiNewIndices[1];
				pIndices[2] = uiNewIndices[2];

				pNewFace->m_uiIndices[0] = uiNewIndices[3];
				pNewFace->m_uiIndices[1] = uiNewIndices[0];
				pNewFace->m_uiIndices[2] = uiNewIndices[2];

				m_renderList.AddFace( *pNewFace );
			}

			pFace = pFace->m_pNext;
		}
	}

	void RenderPipeline::RemoveBackFaceInCameraSpace( void )
	{
		//�������б��޳�������
		RFace* pFace = m_renderList.GetFaceList();
		RVertex* pVerts = m_renderList.GetRTransVerts();

		while( NULL != pFace )
		{
			unsigned int uiVertIndx0 = pFace->m_uiIndices[0];
			unsigned int uiVertIndx1 = pFace->m_uiIndices[1];
			unsigned int uiVertIndx2 = pFace->m_uiIndices[2];

			Math::Vec3 v3U =pVerts[uiVertIndx2].m_v3Pos - pVerts[uiVertIndx0].m_v3Pos;
			Math::Vec3 v3V = pVerts[uiVertIndx1].m_v3Pos - pVerts[uiVertIndx0].m_v3Pos;

			//����淨��
			Math::Vec3 v3N = v3U.CrossProduct( v3V );

			//���˽׶μ�������淨�ߴ�������
			pFace->m_v3Normal = v3N;
			pFace->m_v3Normal.Normalize();

			//��Ϊ����
			if( pFace->m_v3Normal.DotProduct( -( pVerts[uiVertIndx1].m_v3Pos ) ) <= 0.0f  ) 
			{
				pFace->SetStateBit( RFACE_STATE_BACKFACE );
			}else{
				pVerts[uiVertIndx0].SetStateBit( RVERT_STATE_ACTIVE );
				pVerts[uiVertIndx1].SetStateBit( RVERT_STATE_ACTIVE );
				pVerts[uiVertIndx2].SetStateBit( RVERT_STATE_ACTIVE );
			}

			pFace = pFace->m_pNext;
		}
	}

	void RenderPipeline::TransformFromCameraSpaceToProjectionSpace( void )
	{

		RVertex* pVerts = m_renderList.GetRTransVerts();

		for( unsigned int uiVert = 0 ; uiVert < m_renderList.RTransVertCount() ; uiVert++ )
		{
			//����ǰ�����ѱ��޳���任��һ������
			if( !pVerts[uiVert].TestStateBit( RVERT_STATE_ACTIVE ) )
			{
				continue;
			}

			Math::Vec4 v4Pos( pVerts[uiVert].m_v3Pos );
			v4Pos = v4Pos * m_pRenderContext->GetCurrProjectionMatrix();
			pVerts[uiVert].m_v3Pos = Math::Vec3( v4Pos.x , v4Pos.y , v4Pos.z );

			//͸�ӳ�z
			Real invZ = 1.0f / v4Pos.w;

			pVerts[uiVert].m_v3Pos.x *= invZ;
			pVerts[uiVert].m_v3Pos.y *= invZ; 
			//��һ���� 1/Z
			pVerts[uiVert].m_v3Pos.z *= invZ; 
			pVerts[uiVert].m_invZ = invZ;
			
		}
	}

	void RenderPipeline::TransformFromProjectionSpaceToScreenSpace( void )
	{
		RVertex* pVerts = m_renderList.GetRTransVerts();
		for( unsigned int uiVert = 0 ; uiVert < m_renderList.RTransVertCount() ; uiVert++ )
		{
			//����ǰ�����ѱ��޳���任��һ������
			if( !pVerts[uiVert].TestStateBit( RVERT_STATE_ACTIVE ) )
			{
				continue;
			}

			Math::Vec4 v4Pos( pVerts[uiVert].m_v3Pos );
			v4Pos = v4Pos * m_pRenderContext->GetCurrProjectionToScreenMatrix();
			pVerts[uiVert].m_v3Pos = Math::Vec3( v4Pos.x , v4Pos.y , v4Pos.z ); 
		}
	}


	void RenderPipeline::PerspectiveCorrection( void )
	{
		RVertex* pVerts = m_renderList.GetRTransVerts();

		for( unsigned int uiVert = 0 ; uiVert < m_renderList.RTransVertCount() ; uiVert++ )
		{
			//����ǰ�����ѱ��޳���任��һ������
			if( !pVerts[uiVert].TestStateBit( RVERT_STATE_ACTIVE ) )
			{
				continue;
			}
			 
			Real fInvZ = pVerts[uiVert].m_invZ;
			pVerts[uiVert].m_v2Texcoord *= fInvZ;
			if( m_pixelShader.GetShadeModel() == GOURAUD_MODEL  )
			{
				pVerts[uiVert].m_v4Color *= fInvZ;
			}

		/*	if( m_pixelShader.GetShadeModel() == PHONG_MODEL ||
				m_pixelShader.GetShadeModel() == NORMMAP_MODEL )
			{
				pVerts[uiVert].m_v3Normal *= fInvZ;
			}
			
			if( m_pixelShader.GetShadeModel() == NORMMAP_MODEL )
			{
				pVerts[uiVert].m_v3Tangent *= fInvZ;
				pVerts[uiVert].m_v3Binormal *= fInvZ;
			}*/

			m_pRenderContext->IncCurrRenderVertexCount();

		}//for( unsigned int uiVert = 0 ; uiVert < m_renderList.RTransVertCount() ; uiVert++ )
	}


	void RenderPipeline::DrawFacesWireFrameToFrameBuffer( void )
	{
		RFace* pFace = m_renderList.GetFaceList();
		Math::BGRA8888_t lineColor = 
			Math::MathUtil::ColorVecToBGRA8888( Math::Vec4( 1.0f , 1.0f , 1.0f , 1.0f ) );
		  
		while( NULL != pFace )
		{
			if( !pFace->IsActive() )
			{
				pFace = pFace->m_pNext;
				continue;
			} 

			unsigned int uiVertIndices[3];  
			uiVertIndices[0] = pFace->m_uiIndices[0];
			uiVertIndices[1] = pFace->m_uiIndices[1];
			uiVertIndices[2] = pFace->m_uiIndices[2];

			for( unsigned int uiLine = 0 ; uiLine < 3 ; uiLine++ )
			{
				unsigned int uiStartIndx = uiVertIndices[ uiLine ];
				unsigned int uiEndIndx = uiVertIndices[ (uiLine+1)%3 ];
				Math::Vec2 v2P0( m_renderList.GetRTransVerts()[uiStartIndx].m_v3Pos.x ,  m_renderList.GetRTransVerts()[uiStartIndx].m_v3Pos.y );
				Math::Vec2 v2P1(  m_renderList.GetRTransVerts()[uiEndIndx].m_v3Pos.x ,  m_renderList.GetRTransVerts()[uiEndIndx].m_v3Pos.y );
				m_rasterProcessor.DrawLineMidPoint( v2P0 , v2P1 , Math::Vec4( 1.0f , 1.0f , 1.0f , 1.0f ) );
			} 

			m_pRenderContext->IncCurrRenderFaceCount();
			pFace = pFace->m_pNext;
		}
		 
	}

	void RenderPipeline::DrawFacesSolidTrianglesToFrameBuffer( void )
	{
		RFace* pFace = m_renderList.GetFaceList();
		RVertex* pVerts = m_renderList.GetRTransVerts();

		m_pixelShader.SetShadeModel( FLAT_MODEL );
		 
		while( NULL != pFace )
		{
			if( !pFace->IsActive() )
			{
				pFace = pFace->m_pNext;
				continue;
			}
			 

			m_pixelShader.SetFaceColor( pFace->m_v4Color );

			unsigned int uiVertIndices[3];  
			uiVertIndices[0] = pFace->m_uiIndices[0];
			uiVertIndices[1] = pFace->m_uiIndices[1];
			uiVertIndices[2] = pFace->m_uiIndices[2];

			Math::Vec2 v2P0( pVerts[uiVertIndices[0]].m_v3Pos.x , pVerts[uiVertIndices[0]].m_v3Pos.y );
			Math::Vec2 v2P1( pVerts[uiVertIndices[1]].m_v3Pos.x , pVerts[uiVertIndices[1]].m_v3Pos.y );
			Math::Vec2 v2P2( pVerts[uiVertIndices[2]].m_v3Pos.x , pVerts[uiVertIndices[2]].m_v3Pos.y ); 

			m_rasterProcessor.DrawTriangle2D( pVerts[uiVertIndices[0]] , pVerts[uiVertIndices[1]] , pVerts[uiVertIndices[2]] , m_pixelShader );

			m_pRenderContext->IncCurrRenderFaceCount();
			pFace = pFace->m_pNext;
		}
		 
	}


	void RenderPipeline::CalcPerVertexLightInCameraSpace( void )
	{
		if( !m_pRenderContext->IsLightingEnable() )
		{
			return;
		}

		RenderContext::lightTable_t& lights = m_pRenderContext->GetLightList();
		RenderContext::lightTable_t::iterator itLight = lights.begin(); 

		Resource::Material* pMaterial = m_pRenderContext->GetCurrMaterial();

		while( itLight != lights.end() )
		{
			Light* pLight = itLight->second;

			if( !pLight->IsActive() )
			{
				itLight++;
				continue;
			}

			//����Դλ�ñ任������ռ�
			Math::Vec4 v4LightPos = ( pLight->Position() ) * m_pRenderContext->GetCurrWorldToCamMatrix(); 
			RVertex* pVerts = m_renderList.GetRTransVerts();

			for( unsigned int uiVert = 0 ; uiVert < m_renderList.RTransVertCount() ; uiVert++ )
			{
				RVertex& v = pVerts[uiVert];

				if( !v.TestStateBit( RVERT_STATE_ACTIVE ) )
				{
					continue;
				}

				Math::Vec3 v3Viewer = -(v.m_v3Pos); 
				v3Viewer.Normalize();
				Math::Vec3 v3LightDir = Math::Vec3( v4LightPos.x , v4LightPos.y , v4LightPos.z ) - v.m_v3Pos; 
				v3LightDir.Normalize();

				Math::Vec3 v3H = ( v3Viewer + v3LightDir ).NormalisedCopy();

				Real fSpecFactor = v.m_v3Normal.DotProduct( v3H );
				fSpecFactor = fSpecFactor > 0.0f ? fSpecFactor : 0.0f;
				fSpecFactor = Math::MathUtil::Pow( fSpecFactor , pMaterial->GetShininess() );

				Real fDiffFactor = v.m_v3Normal.DotProduct( v3LightDir );
				fDiffFactor = fDiffFactor > 0.0f ? fDiffFactor : 0.0f;

				v.m_v4Color += fSpecFactor*( pMaterial->GetSpecular()* pLight->Specular() ) + 
					fDiffFactor*( pMaterial->GetDiffuse() * pLight->Diffuse() );
			} 
			 
			itLight++;
		}
	}


	void RenderPipeline::CalcLightInCameraSpace( void )
	{
		if( !m_pRenderContext->IsLightingEnable() )
		{
			return;
		}

		RenderContext::lightTable_t& lights = m_pRenderContext->GetLightList();
		RenderContext::lightTable_t::iterator itLight = lights.begin();

		Math::Vec4 v4FaceColor( 0.75f , 0.75f , 0.75f , 1.0f );

		Resource::Material* pMaterial = m_pRenderContext->GetCurrMaterial();

		while( itLight != lights.end() )
		{
			Light* pLight = itLight->second;

			if( !pLight->IsActive() )
			{
				itLight++;
				continue;
			}

			//����Դλ�ñ任������ռ�
			Math::Vec4 v4LightPos = ( pLight->Position() ) * m_pRenderContext->GetCurrWorldToCamMatrix();
			 
			RFace* pFace = m_renderList.GetFaceList();
			RVertex* pVerts = m_renderList.GetRTransVerts();

			while( NULL != pFace )
			{
				if( !pFace->IsActive() )
				{  
					pFace = pFace->m_pNext;
					continue;
				}

				unsigned int uiVert0 = pFace->m_uiIndices[0];
				Math::Vec3 v3Viewer = -(pVerts[uiVert0].m_v3Pos); 
				v3Viewer.Normalize();
				Math::Vec3 v3LightDir = Math::Vec3( v4LightPos.x , v4LightPos.y , v4LightPos.z ) - pVerts[uiVert0].m_v3Pos; 
				v3LightDir.Normalize();

				Math::Vec3 v3H = ( v3Viewer + v3LightDir ).NormalisedCopy();

				Real fSpecFactor = pFace->m_v3Normal.DotProduct( v3H );
				fSpecFactor = fSpecFactor > 0.0f ? fSpecFactor : 0.0f;
				fSpecFactor = Math::MathUtil::Pow( fSpecFactor , pMaterial->GetShininess() );

				Real fDiffFactor = pFace->m_v3Normal.DotProduct( v3LightDir );
				fDiffFactor = fDiffFactor > 0.0f ? fDiffFactor : 0.0f;

				pFace->m_v4Color += fSpecFactor*( pMaterial->GetSpecular()* pLight->Specular() ) + 
													fDiffFactor*( pMaterial->GetDiffuse() * pLight->Diffuse() ) + 
													pMaterial->GetAmbient()*pLight->Ambient();

				pFace = pFace->m_pNext;
			}
			itLight++;
		}
	}


	void RenderPipeline::PrepareForPerPixelLightingInCameraSpace( void )
	{ 
		m_pixelShader.ClearAllLights();

		RenderContext::lightTable_t& lights = m_pRenderContext->GetLightList();
		RenderContext::lightTable_t::iterator itLight = lights.begin(); 

		while( itLight != lights.end() )
		{
			Light* pLight = itLight->second; 
			if( !pLight->IsActive() )
			{
				++itLight;
				continue;
			}

			Math::Vec4 v4LightPos = ( pLight->Position() ) * m_pRenderContext->GetCurrWorldToCamMatrix();
			pLight->SetPosInCamera( v4LightPos );  

			//����Դ�б����������ɫ����
			m_pixelShader.AddLight( pLight ); 
			++itLight;
		} 
	}


	void RenderPipeline::DrawTrianglesToFrameBuffer( void )
	{
		RFace* pFace = m_renderList.GetFaceList(); 

		while( NULL != pFace )
		{
			if( !pFace->IsActive() )
			{
				pFace = pFace->m_pNext;
				continue;
			} 

			unsigned int uiVertIndices[3];  
			uiVertIndices[0] = pFace->m_uiIndices[0];
			uiVertIndices[1] = pFace->m_uiIndices[1];
			uiVertIndices[2] = pFace->m_uiIndices[2];

			m_rasterProcessor.DrawTriangle2D( 
				m_renderList.GetRTransVerts()[uiVertIndices[0]] , 
				m_renderList.GetRTransVerts()[uiVertIndices[1]] ,
				m_renderList.GetRTransVerts()[uiVertIndices[2]] , m_pixelShader );
			 
			m_pRenderContext->IncCurrRenderFaceCount();
			pFace = pFace->m_pNext;
		}
		 
	}




}//namespace Render