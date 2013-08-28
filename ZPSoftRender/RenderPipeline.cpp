#include "RenderPipeline.h"
#include "RenderPrimitive.h"
#include "FrameStackAllocator.h"
#include "RenderContext.h"
#include "Light.h"
#include "ZPMathDependency.h"
#include "Material.h"

namespace Render
{


	RenderPipeline::RenderPipeline(void):
	m_pRenderContext( NULL )
	{
	}


	RenderPipeline::~RenderPipeline(void)
	{
		m_pRenderContext = NULL;
	}

	void RenderPipeline::DrawElements( RenderPrimitive& renderPrimitive )
	{
		FrameStackAllocator::GetInstance()->Clear();
		m_renderList.Clear(); 
		m_renderList.CopyFromRenderPrimitive( renderPrimitive ); 

		this->RunVertexShaderStage();
		this->RunRasterizationStage();
		this->RunEarlyZPassStage();
		this->RunFragmentShaderStage();
		this->RunMergeStage();
	}

	void RenderPipeline::RunVertexShaderStage( void )
	{
		TransformFromLocalSpaceToCameraSpace();
		RemoveBackFaceInCameraSpace();
		CalcLightInCameraSpace();
		TransformFromCameraSpaceToProjectionSpace();
		TransformFromProjectionSpaceToScreenSpace();
	}

	void RenderPipeline::RunRasterizationStage( void )
	{

	}

	void RenderPipeline::RunEarlyZPassStage( void )
	{

	}

	void RenderPipeline::RunFragmentShaderStage( void )
	{

	}

	void RenderPipeline::RunMergeStage( void )
	{
		DrawFacesWireFrameToFrameBuffer();
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
			pVerts[uiVert].m_v3Normal = pVerts[uiVert].m_v3Normal * normalMatrix; 
		}
	}

	void RenderPipeline::RemoveBackFaceInCameraSpace( void )
	{
		//遍历面列表剔除背向面
		RFace* pFace = m_renderList.GetFaceList();

		while( NULL != pFace )
		{
			unsigned int uiVertIndx0 = pFace->m_uiIndices[0];
			unsigned int uiVertIndx1 = pFace->m_uiIndices[1];
			unsigned int uiVertIndx2 = pFace->m_uiIndices[2];

			Math::Vec3 v3U =m_renderList.GetRTransVerts()[uiVertIndx2].m_v3Pos - m_renderList.GetRTransVerts()[uiVertIndx0].m_v3Pos;
			Math::Vec3 v3V = m_renderList.GetRTransVerts()[uiVertIndx1].m_v3Pos - m_renderList.GetRTransVerts()[uiVertIndx0].m_v3Pos;

			//求出面法线
			Math::Vec3 v3N = v3U.CrossProduct( v3V );

			//将此阶段计算出的面法线存入面中
			pFace->m_v3Normal = v3N;
			pFace->m_v3Normal.Normalize();

			//若为背面
			if( v3N.DotProduct( -m_renderList.GetRTransVerts()[uiVertIndx1].m_v3Pos ) <= 0.0f  )
			{
				pFace->SetStateBit( RFACE_STATE_BACKFACE );
			}else{
				m_renderList.GetRTransVerts()[uiVertIndx0].SetStateBit( RVERT_STATE_ACTIVE );
				m_renderList.GetRTransVerts()[uiVertIndx1].SetStateBit( RVERT_STATE_ACTIVE );
				m_renderList.GetRTransVerts()[uiVertIndx2].SetStateBit( RVERT_STATE_ACTIVE );
			}

			pFace = pFace->m_pNext;
		}
	}

	void RenderPipeline::TransformFromCameraSpaceToProjectionSpace( void )
	{
		for( unsigned int uiVert = 0 ; uiVert < m_renderList.VertCount() ; uiVert++ )
		{
			//若当前顶点已被剔除则变换下一个顶点
			if( !m_renderList.GetRTransVerts()[uiVert].TestStateBit( RVERT_STATE_ACTIVE ) )
			{
				continue;
			}

			Math::Vec4 v4Pos( m_renderList.GetRTransVerts()[uiVert].m_v3Pos );
			v4Pos = v4Pos * m_pRenderContext->GetCurrProjectionMatrix();
			m_renderList.GetRTransVerts()[uiVert].m_v3Pos = Math::Vec3( v4Pos.x , v4Pos.y , v4Pos.z );

			//透视除z
			Real invW = 1.0f / v4Pos.w;

			m_renderList.GetRTransVerts()[uiVert].m_v3Pos.x *= invW;
			m_renderList.GetRTransVerts()[uiVert].m_v3Pos.y *= invW;
		}
	}

	void RenderPipeline::TransformFromProjectionSpaceToScreenSpace( void )
	{
		for( unsigned int uiVert = 0 ; uiVert < m_renderList.VertCount() ; uiVert++ )
		{
			//若当前顶点已被剔除则变换下一个顶点
			if( !m_renderList.GetRTransVerts()[uiVert].TestStateBit( RVERT_STATE_ACTIVE ) )
			{
				continue;
			}

			Math::Vec4 v4Pos( m_renderList.GetRTransVerts()[uiVert].m_v3Pos );
			v4Pos = v4Pos * m_pRenderContext->GetCurrProjectionToScreenMatrix();
			m_renderList.GetRTransVerts()[uiVert].m_v3Pos = Math::Vec3( v4Pos.x , v4Pos.y , v4Pos.z ); 
		}
	}

	void RenderPipeline::DrawFacesWireFrameToFrameBuffer( void )
	{
		RFace* pFace = m_renderList.GetFaceList();
		Math::BGRA8888_t lineColor = 
			Math::MathUtil::ColorVecToRGBA8888( Math::Vec4( 1.0f , 1.0f , 1.0f , 1.0f ) );

		while( NULL != pFace )
		{
			if( pFace->TestStateBit( RFACE_STATE_BACKFACE ) )
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
				m_pRenderContext->GetColorFrameBuffer().DrawLineMidPoint( v2P0 , v2P1 , static_cast<void*>( &lineColor ) );
			} 
			pFace = pFace->m_pNext;
		}
	}

	void RenderPipeline::DrawFacesSolidTrianglesToFrameBuffer( void )
	{
		RFace* pFace = m_renderList.GetFaceList();
		RVertex* pVerts = m_renderList.GetRTransVerts();

		while( NULL != pFace )
		{
			if( pFace->TestStateBit( RFACE_STATE_BACKFACE ) )
			{
				pFace = pFace->m_pNext;
				continue;
			}

			Math::BGRA8888_t faceColor = 
				Math::MathUtil::ColorVecToRGBA8888( pFace->m_v4Color );

			unsigned int uiVertIndices[3];  
			uiVertIndices[0] = pFace->m_uiIndices[0];
			uiVertIndices[1] = pFace->m_uiIndices[1];
			uiVertIndices[2] = pFace->m_uiIndices[2];

			Math::Vec2 v2P0( pVerts[uiVertIndices[0]].m_v3Pos.x , pVerts[uiVertIndices[0]].m_v3Pos.y );
			Math::Vec2 v2P1( pVerts[uiVertIndices[1]].m_v3Pos.x , pVerts[uiVertIndices[1]].m_v3Pos.y );
			Math::Vec2 v2P2( pVerts[uiVertIndices[2]].m_v3Pos.x , pVerts[uiVertIndices[2]].m_v3Pos.y ); 
			m_pRenderContext->GetColorFrameBuffer().DrawTriangle2DSolid( v2P0 , v2P1 , v2P2 ,  static_cast<void*>( &faceColor ) );

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

			//将光源位置变换到相机空间
			Math::Vec4 v4LightPos = ( pLight->Position() ) * m_pRenderContext->GetCurrWorldToCamMatrix();

			RFace* pFace = m_renderList.GetFaceList();
			RVertex* pVerts = m_renderList.GetRTransVerts();

			while( NULL != pFace )
			{
				if( pFace->TestStateBit( RFACE_STATE_BACKFACE ) )
				{  
					pFace = pFace->m_pNext;
					continue;
				}

				for( int iVert = 0 ; iVert < 3 ; iVert++ )
				{
					unsigned int uiVert = pFace->m_uiIndices[iVert];
					Math::Vec3 v3Viewer = -(pVerts[uiVert].m_v3Pos); 
					v3Viewer.Normalize();
					Math::Vec3 v3LightDir = Math::Vec3( v4LightPos.x , v4LightPos.y , v4LightPos.z ) - pVerts[uiVert].m_v3Pos; 
					v3LightDir.Normalize();

					Math::Vec3 v3H = ( v3Viewer + v3LightDir ).NormalisedCopy();

					Real fSpecFactor = pVerts[uiVert].m_v3Normal.DotProduct( v3H );
					fSpecFactor = fSpecFactor > 0.0f ? fSpecFactor : 0.0f;
					fSpecFactor = Math::MathUtil::Pow( fSpecFactor , pMaterial->GetShininess() );

					Real fDiffFactor = pVerts[uiVert].m_v3Normal.DotProduct( v3LightDir );
					fDiffFactor = fDiffFactor > 0.0f ? fDiffFactor : 0.0f;

					pVerts[uiVert].m_v4Color += fSpecFactor*( pMaterial->GetSpecular()* pLight->Specular() ) + 
						fDiffFactor*( pMaterial->GetDiffuse() * pLight->Diffuse() );
				} 

				pFace = pFace->m_pNext;
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

			//将光源位置变换到相机空间
			Math::Vec4 v4LightPos = ( pLight->Position() ) * m_pRenderContext->GetCurrWorldToCamMatrix();
			 
			RFace* pFace = m_renderList.GetFaceList();
			RVertex* pVerts = m_renderList.GetRTransVerts();

			while( NULL != pFace )
			{
				if( pFace->TestStateBit( RFACE_STATE_BACKFACE ) )
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
													fDiffFactor*( pMaterial->GetDiffuse() * pLight->Diffuse() );

				pFace = pFace->m_pNext;
			}
			itLight++;
		}
	}

	void RenderPipeline::DrawGouraudShadingTrianglesToFrameBuffer( void )
	{
		RFace* pFace = m_renderList.GetFaceList();

		while( NULL != pFace )
		{
			if( pFace->TestStateBit( RFACE_STATE_BACKFACE ) )
			{
				pFace = pFace->m_pNext;
				continue;
			}
			 

			unsigned int uiVertIndices[3];  
			uiVertIndices[0] = pFace->m_uiIndices[0];
			uiVertIndices[1] = pFace->m_uiIndices[1];
			uiVertIndices[2] = pFace->m_uiIndices[2];

			m_pRenderContext->GetColorFrameBuffer().DrawTriangle2D( 
				m_renderList.GetRTransVerts()[uiVertIndices[0]] , 
				m_renderList.GetRTransVerts()[uiVertIndices[1]] ,
				m_renderList.GetRTransVerts()[uiVertIndices[2]] );

			pFace = pFace->m_pNext;
		}
	}



}//namespace Render