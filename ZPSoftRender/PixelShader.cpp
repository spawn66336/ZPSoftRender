#include "PixelShader.h"
#include "RenderList.h"
#include "Texture2D.h"
#include "Material.h"
#include "Light.h"

namespace Render
{
	 
	PixelShader::PixelShader(void):
	m_shadeModel( FLAT_MODEL)
	{
	}


	PixelShader::~PixelShader(void)
	{
	}

	Math::Vec4 PixelShader::Run( const RVertex& v   )
	{
		Real fProjCorrectionFactor = 1.0f / v.m_invZ;
		 
		bool bHasDiffTex = false;
		bool bHasBumpTex = false;
		Math::Vec4 v4TexDiffColor(1.0f,1.0f,1.0f,1.0f);
		Math::Vec4 v4BumpColor( 0.0f , 0.0f , 0.0f, 0.0f );

		Math::Vec2 v2Texcoord = v.m_v2Texcoord * fProjCorrectionFactor;

		if( m_diffuseTextureSampler.IsActive() )
		{ 
			v4TexDiffColor = m_diffuseTextureSampler.Sample( v2Texcoord ); 
			bHasDiffTex = true;
		}

		if( m_normTextureSampler.IsActive() )
		{
			v4BumpColor = m_normTextureSampler.Sample( v2Texcoord );
			bHasBumpTex = true;
		}
		  
		if( GetShadeModel() == FLAT_MODEL )
		{
			return m_v4FaceColor * v4TexDiffColor;
		}

		if( GetShadeModel() == GOURAUD_MODEL )
		{ 
			return v4TexDiffColor * v.m_v4Color * fProjCorrectionFactor ;
		}

		bool bNormmapShading = false;
		if( GetShadeModel() == NORMMAP_MODEL && 
			bHasBumpTex )
		{
			bNormmapShading = true;
		}

		if( bNormmapShading )
		{
			Math::Vec4 v4FinalColor;
			Math::Vec3 v3Normal = v.m_v3Normal /** fProjCorrectionFactor*/;
			Math::Vec3 v3Tangent = v.m_v3Tangent /** fProjCorrectionFactor*/;
			Math::Vec3 v3Binormal = v.m_v3Binormal /** fProjCorrectionFactor*/;

			lightTable_t::iterator itLight = m_lights.begin();
			while( itLight != m_lights.end() )
			{
				Math::Vec4 v4LightPos =  (*itLight)->GetPosInCamera(); 

				Math::Vec3 View = -(v.m_v3PosInCam); 
				Math::Vec3 v3Viewer;
				v3Viewer.x = View.DotProduct( v3Tangent );
				v3Viewer.y = View.DotProduct( v3Binormal );
				v3Viewer.z = View.DotProduct( v3Normal );
				v3Viewer.Normalize(); 
				  
				Math::Vec3 LightVec = Math::Vec3( v4LightPos.x , v4LightPos.y , v4LightPos.z ) - v.m_v3PosInCam;
				Math::Vec3 v3LightDir;
				v3LightDir.x = LightVec.DotProduct( v3Tangent );
				v3LightDir.y = LightVec.DotProduct( v3Binormal );
				v3LightDir.z = LightVec.DotProduct( v3Normal ); 
				v3LightDir.Normalize();

				Math::Vec3 v3H = ( v3Viewer + v3LightDir ).NormalisedCopy();

				Math::Vec3 v3BumpNorm( v4BumpColor.x , v4BumpColor.y , v4BumpColor.z );
				v3BumpNorm *= 2.0f;
				v3BumpNorm -= ( Math::Vec3( 1.0f ));
				v3BumpNorm.Normalize();

				Real fSpecFactor = v3BumpNorm.DotProduct( v3H );
				fSpecFactor = fSpecFactor > 0.0f ? fSpecFactor : 0.0f;
				fSpecFactor = Math::MathUtil::Pow( fSpecFactor , m_pMaterial->GetShininess() );

				Real fDiffFactor = v3BumpNorm.DotProduct( v3LightDir );
				fDiffFactor = fDiffFactor > 0.0f ? fDiffFactor : 0.0f;

				//��û����������������������е�������ɫ
				if( !bHasDiffTex )
				{
					v4TexDiffColor = m_pMaterial->GetDiffuse();
				}

				v4FinalColor += fSpecFactor*( m_pMaterial->GetSpecular()* (*itLight)->Specular() ) + 
					fDiffFactor*( v4TexDiffColor * (*itLight)->Diffuse() );


				++itLight;
			}
			return v4FinalColor;

		}

		if( !bNormmapShading )
		{ 
			Math::Vec4 v4FinalColor;
			Math::Vec3 v3Normal = v.m_v3Normal;
			/*v3Normal *= fProjCorrectionFactor;*/
			v3Normal.Normalize();

			lightTable_t::iterator itLight = m_lights.begin();
			while( itLight != m_lights.end() )
			{
				Math::Vec4 v4LightPos =  (*itLight)->GetPosInCamera();
				 
				 Math::Vec3 v3Viewer = -(v.m_v3PosInCam); 
				 v3Viewer.Normalize();
				 Math::Vec3 v3LightDir = Math::Vec3( v4LightPos.x , v4LightPos.y , v4LightPos.z ) - v.m_v3PosInCam; 
				 v3LightDir.Normalize();

				 Math::Vec3 v3H = ( v3Viewer + v3LightDir ).NormalisedCopy();

				 Real fSpecFactor = v3Normal.DotProduct( v3H );
				 fSpecFactor = fSpecFactor > 0.0f ? fSpecFactor : 0.0f;
				 fSpecFactor = Math::MathUtil::Pow( fSpecFactor , m_pMaterial->GetShininess() );

				 Real fDiffFactor = v3Normal.DotProduct( v3LightDir );
				 fDiffFactor = fDiffFactor > 0.0f ? fDiffFactor : 0.0f;

				 //��û����������������������е�������ɫ
				 if( !bHasDiffTex )
				 {
					 v4TexDiffColor = m_pMaterial->GetDiffuse();
				 }

				 v4FinalColor += fSpecFactor*( m_pMaterial->GetSpecular()* (*itLight)->Specular() ) + 
					 fDiffFactor*( v4TexDiffColor * (*itLight)->Diffuse() );
				 
				++itLight;
			}
			return v4FinalColor;
		}

		 
		return Math::Vec4();
	}

	void PixelShader::SetMaterial( Resource::Material* pMaterial )
	{
		m_pMaterial = pMaterial;
		if( NULL != m_pMaterial )
		{
			m_diffuseTextureSampler.SetTexture2D( m_pMaterial->GetTexture( DIFFUSE_CH ) );
			m_normTextureSampler.SetTexture2D( m_pMaterial->GetTexture( BUMPMAP_CH ) );
		}
	}

	Texture2DSampler::Texture2DSampler( void ):
	m_uiBytesPerPixel(0),
	m_uiLineOffset(0),
	m_uiWrapType( WRAP_REPEAT ),
	m_pTexture( NULL)
	{  
	}

	Texture2DSampler::~Texture2DSampler()
	{
		m_pTexture = NULL;
	}


	void Texture2DSampler::SetTexture2D( Resource::Texture2D* pTexture )
	{
		m_pTexture = pTexture;
		if( NULL != m_pTexture )
		{
			m_uiBytesPerPixel = m_pTexture->Bpp() / 8;
			m_uiLineOffset = m_pTexture->Width() * m_uiBytesPerPixel;
		}
	}


	Math::Vec4 Texture2DSampler::Sample( Math::Vec2& v2Texcoord )
	{
		if( NULL != m_pTexture )
		{
			int iu = 0 , iv = 0;
			Math::Vec2 v2FinalUV;
			CalcTexcoord( v2Texcoord , v2FinalUV , iu , iv ); 

			Real du = v2FinalUV.x - static_cast<Real>( static_cast<int>( v2FinalUV.x ) );
			Real dv = v2FinalUV.y - static_cast<Real>( static_cast<int>( v2FinalUV.y ) );
			Real one_mius_du = 1.0f - du;
			Real one_mius_dv = 1.0f - dv;
			int one_plus_x = ( 1+iu ) % static_cast<int>( m_pTexture->Width() );
			int one_plus_y = ( 1+iv ) % static_cast<int>( m_pTexture->Height() );
			 
			Math::RGBA8888_t color = 0;
			memcpy( &color , &m_pTexture->Pixels()[iv*m_uiLineOffset + iu*m_uiBytesPerPixel] , m_uiBytesPerPixel );
			Math::Vec4 v4Color0 = Math::MathUtil::RGBA8888ToVec4( color );
			memcpy( &color , &m_pTexture->Pixels()[iv*m_uiLineOffset + one_plus_x*m_uiBytesPerPixel] , m_uiBytesPerPixel );
			Math::Vec4 v4Color1 = Math::MathUtil::RGBA8888ToVec4( color );
			memcpy( &color , &m_pTexture->Pixels()[one_plus_y*m_uiLineOffset + one_plus_x*m_uiBytesPerPixel] , m_uiBytesPerPixel );
			Math::Vec4 v4Color2 = Math::MathUtil::RGBA8888ToVec4( color );
			memcpy( &color , &m_pTexture->Pixels()[one_plus_y*m_uiLineOffset + iu*m_uiBytesPerPixel] , m_uiBytesPerPixel );
			Math::Vec4 v4Color3 = Math::MathUtil::RGBA8888ToVec4( color );

			return one_mius_du*one_mius_dv*v4Color0 + 
					   du*one_mius_dv*v4Color1 + 
				       du*dv*v4Color2 + 
				       one_mius_du*dv*v4Color3;
		}
		return Math::Vec4(1.0f,1.0f,1.0f,1.0f);
	}

	void Texture2DSampler::CalcTexcoord( Math::Vec2& v2Texcoord , Math::Vec2& v2FinalUV , int& iu , int& iv )
	{
		if( m_uiWrapType == WRAP_CLAMP )
		{

			Real u = Math::MathUtil::Clamp( v2Texcoord.x , 0.0f , 1.0f );
			Real v = Math::MathUtil::Clamp( v2Texcoord.y , 0.0f , 1.0f );  

			v2FinalUV.x = static_cast<Real>( m_pTexture->Width() - 1 ) * u;
			v2FinalUV.y = static_cast<Real>( m_pTexture->Height() - 1 ) * v;

			iu = static_cast<int>( v2FinalUV.x );
			iv = static_cast<int>( v2FinalUV.y );
		}else if( m_uiWrapType == WRAP_REPEAT ){

			Real u = Math::MathUtil::Fract( v2Texcoord.x );
			Real v = Math::MathUtil::Fract( v2Texcoord.y );

			if( u < 0.0f ) 
			{
				u += 1.0f;
			}

			if( v < 0.0f )
			{
				v += 1.0f;
			}

			v2FinalUV.x = static_cast<Real>( m_pTexture->Width() - 1 ) * u;
			v2FinalUV.y = static_cast<Real>( m_pTexture->Height() - 1 ) * v;

			iu = static_cast<int>( v2FinalUV.x );
			iv = static_cast<int>( v2FinalUV.y );

			iu %= (int)( m_pTexture->Width() );
			iv %= (int)( m_pTexture->Height() );
		}
	}



}//namespace Render