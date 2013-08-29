#include "PixelShader.h"
#include "RenderList.h"
#include "Texture2D.h"
#include "Material.h"

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
		 
		Math::Vec4 v4TexDiffColor(1.0f,1.0f,1.0f,1.0f);
		 
		if( m_diffuseTextureSampler.IsActive() )
		{
			Math::Vec2 v2Texcoord = v.m_v2Texcoord * fProjCorrectionFactor;
			v4TexDiffColor = m_diffuseTextureSampler.Sample( v2Texcoord ); 
		}

		if( GetShadeModel() == FLAT_MODEL )
		{
			return m_v4FaceColor * v4TexDiffColor;
		}

		if( GetShadeModel() == GOURAUD_MODEL )
		{ 
			return v4TexDiffColor * v.m_v4Color * fProjCorrectionFactor ;
		}

		if( GetShadeModel() == PHONG_MODEL )
		{ 
			
			return Math::Vec4();
		}

		if( GetShadeModel() == NORMMAP_MODEL )
		{
			return Math::Vec4();
		}
		return Math::Vec4();
	}

	void PixelShader::SetMaterial( Resource::Material* pMaterial )
	{
		m_pMaterial = pMaterial;
		if( NULL != m_pMaterial )
		{
			m_diffuseTextureSampler.SetTexture2D( m_pMaterial->GetTexture( DIFFUSE_CH ) );
		}
	}

	Texture2DSampler::Texture2DSampler( void ):
	m_uiBytesPerPixel(0),
	m_uiLineOffset(0),
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
			Real u = Math::MathUtil::Clamp( v2Texcoord.x , 0.0f , 1.0f );
			Real v = Math::MathUtil::Clamp( v2Texcoord.y , 0.0f , 1.0f );  
			int x = static_cast<int>( static_cast<Real>( m_pTexture->Width() - 1 ) * u );
			int y = static_cast<int>( static_cast<Real>( m_pTexture->Height() - 1 ) * v );

			Math::RGBA8888_t color = 0;
			memcpy( &color , &m_pTexture->Pixels()[y*m_uiLineOffset + x*m_uiBytesPerPixel] , m_uiBytesPerPixel );
			return Math::MathUtil::RGBA8888ToVec4( color );
		}
		return Math::Vec4(1.0f,1.0f,1.0f,1.0f);
	}



}//namespace Render