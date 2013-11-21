#include "D3DRenderPipeline.h"
#include "D3DIncludeCallback.h"
#include "IRender.h"
#include "Material.h"
#include "Texture2D.h"
#include "D3DRenderOperation.h"

namespace Render
{


D3DRenderPipeline::D3DRenderPipeline(void):
m_pDevice(NULL),
m_pEffectPool(NULL),
m_pEffect(NULL)
{
}


D3DRenderPipeline::~D3DRenderPipeline(void)
{
	Destroy();
}

void D3DRenderPipeline::PushRenderOp( Resource::Material* pMat , D3DRenderOperation* pOp )
{
	auto itFindGroup = m_renderGroup.find( pMat );
	if( itFindGroup != m_renderGroup.end() )
	{
		itFindGroup->second.m_OpList.push_back( pOp );
		return;
	}

	auto itGroup = m_renderGroup.insert( std::make_pair( pMat , MaterialGroup() ) );
	itGroup.first->second.m_pMaterial = pMat;
	itGroup.first->second.m_OpList.push_back( pOp );
}

void D3DRenderPipeline::Render( void )
{ 
	auto itMatGroup = m_renderGroup.begin();
	while( itMatGroup != m_renderGroup.end() )
	{

		MaterialGroup& matGroup = itMatGroup->second; 
		Resource::Material* pMat = matGroup.m_pMaterial;

		if( NULL == pMat )
		{
			if( NULL == m_pEffect )
			{
				++itMatGroup;
				continue;
			}

			//渲染地形
			m_pEffect->SetTechnique( "TerrainShading" );
			 
			Math::Vec3 LightPos( 2000.0f , 8000.0f , 3000.f ); 
			Math::Vec3 DirLightDir( 0.5f , -1.0f , 0.5f );
			DirLightDir.Normalize();
			m_pEffect->SetValue( "f3LightPos"  , &(LightPos) , sizeof(LightPos) );  
			m_pEffect->SetValue( "f3DirLightDir" , &DirLightDir , sizeof(DirLightDir) );
			D3DMATERIAL9 material;
			Math::Vec4 v4Ambient( 0.1f , 0.1f, 0.1f , 1.0f );
			Math::Vec4 v4Diffuse( 1.0f , 1.0f , 1.0f , 1.0f );
			Math::Vec4 v4Specular( 1.0f , 1.0f , 1.0f , 1.0f );
			float fShininess = 128.0f;

			//应用材质  
			memset( &material , 0 , sizeof( material ) ); 
			memcpy( &material.Ambient , &v4Ambient , sizeof(D3DCOLORVALUE) );
			memcpy( &material.Diffuse , &v4Diffuse , sizeof(D3DCOLORVALUE) );
			memcpy( &material.Specular , &v4Specular , sizeof(D3DCOLORVALUE) );
			material.Power = fShininess;     
			int size = sizeof(material);
			m_pEffect->SetValue( "g_Material" , (LPCVOID)&material , sizeof(material) ); 

			UINT uiPassCount = 0;  
			m_pEffect->Begin( &uiPassCount ,  0 );  

			for( UINT uiPass = 0 ; uiPass < uiPassCount ; uiPass++ )
			{  
				m_pEffect->BeginPass( uiPass );   
				auto itOp = matGroup.m_OpList.begin();
				while( itOp != matGroup.m_OpList.end() )
				{ 
					D3DRenderOperation* pOp = *itOp;   

					m_pEffect->SetMatrix( "m4World" , (D3DXMATRIX*)&( pOp->m_worldMat ) ); 
					m_pEffect->CommitChanges();

					m_pDevice->SetVertexDeclaration( pOp->m_pVertexDecl );
					m_pDevice->SetStreamSource( pOp->m_streamIndex , pOp->m_pVB , 0 , pOp->m_stride  );
					m_pDevice->SetIndices( pOp->m_pIB ); 
					m_pDevice->DrawIndexedPrimitive( pOp->m_primitiveType , 0 , 0 , pOp->m_vertexCount , 0 , pOp->m_primCount );   

					++itOp;
				}
				m_pEffect->EndPass();
			}
			m_pEffect->End();

			++itMatGroup;
			continue;
		}

		Resource::Texture2D* pDiffTex = pMat->GetTexture( DIFFUSE_CH );
		Resource::Texture2D* pNormTex = pMat->GetTexture( BUMPMAP_CH ); 
	 
		String techniqueName = "NormalMapShading";
	  
		if( m_pEffect )
		{ 
			UINT uiPassCount = 0;  
			HRESULT hRes = m_pEffect->SetTechnique( techniqueName.c_str() );
			
			Math::Vec3 LightPos( 30.f , 30.f , 30.f ); 
			m_pEffect->SetValue( "f3LightPos"  , &(LightPos) , sizeof(LightPos) );  

			D3DMATERIAL9 material;
			//应用材质 
			memset( &material , 0 , sizeof( material ) ); 
			memcpy( &material.Ambient , &pMat->GetAmbient() , sizeof(D3DCOLORVALUE) );
			memcpy( &material.Diffuse , &pMat->GetDiffuse() , sizeof(D3DCOLORVALUE) );
			memcpy( &material.Specular , &pMat->GetSpecular() , sizeof(D3DCOLORVALUE) );
			material.Power = pMat->GetShininess();     
			int size = sizeof(material);
			m_pEffect->SetValue( "g_Material" , (LPCVOID)&material , sizeof(material) );  
			if( pDiffTex )
			{
				m_pEffect->SetTexture( "diffuseTex" , (IDirect3DTexture9*)pDiffTex->GetUserPointer() );  
			}

			if( pNormTex )
			{
				m_pEffect->SetTexture( "normalTex" , (IDirect3DTexture9*)pNormTex->GetUserPointer() );  
			}
			  
			m_pEffect->Begin( &uiPassCount ,  0 );  
			for( UINT uiPass = 0 ; uiPass < uiPassCount ; uiPass++ )
			{ 
				m_pEffect->BeginPass( uiPass );   
				auto itOp = matGroup.m_OpList.begin();
				while( itOp != matGroup.m_OpList.end() )
				{ 
					D3DRenderOperation* pOp = *itOp;
					m_pEffect->SetMatrix( "m4World" , (D3DXMATRIX*)&( pOp->m_worldMat ) ); 
					m_pEffect->CommitChanges();

					m_pDevice->SetVertexDeclaration( pOp->m_pVertexDecl );
					m_pDevice->SetStreamSource( pOp->m_streamIndex , pOp->m_pVB , 0 , pOp->m_stride  );
					m_pDevice->SetIndices( pOp->m_pIB ); 
					m_pDevice->DrawIndexedPrimitive( pOp->m_primitiveType , 0 , 0 , pOp->m_vertexCount, 0 , pOp->m_primCount );   
					++itOp;
				}
					m_pEffect->EndPass();
			} 
			m_pEffect->End(); 
		}//if( m_pEffect )
  
		++itMatGroup;
	}
}

void D3DRenderPipeline::Reset( void )
{
	auto itGroup = m_renderGroup.begin();
	while( itGroup != m_renderGroup.end() )
	{
		itGroup->second.m_OpList.clear();
		++itGroup;
	}
}

void D3DRenderPipeline::Init( IDirect3DDevice9* pDevice  , ID3DXEffectPool* pPool  )
{
	m_pDevice = pDevice;
	m_pEffectPool = pPool;

	//创建全局变量共享的Pool 
	ID3DXBuffer* pErrorBuf = NULL; 
	D3DIncludeCallback d3dIncludeCallback;

	DWORD dwShaderFlags = 0;

#ifdef ZP_DEBUG_D3D
	dwShaderFlags = D3DXSHADER_DEBUG|D3DXSHADER_SKIPOPTIMIZATION;
#endif
	    
	HRESULT hRes   = 
		D3DXCreateEffectFromFileA( 
		m_pDevice , 
		"shader.fx" , 
		NULL , 
		&d3dIncludeCallback , 
		dwShaderFlags ,
		m_pEffectPool ,
		&m_pEffect ,
		&pErrorBuf
		);

	String errStr;
	if( pErrorBuf )
	{
		errStr = (char*)pErrorBuf->GetBufferPointer();
	}
}

void D3DRenderPipeline::Destroy( void )
{
	m_renderGroup.clear();
	ZP_SAFE_RELEASE( m_pEffect );
	m_pDevice = NULL;
}

void D3DRenderPipeline::OnLostDevice( void )
{
	if( m_pEffect )
	{
		m_pEffect->OnLostDevice();
	}
}

void D3DRenderPipeline::OnResetDevice( void )
{
	if( m_pEffect )
	{
		m_pEffect->OnResetDevice();
	}
}


}//namespace Render