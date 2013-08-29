#include "Material.h"

namespace Resource
{ 

	Material::Material(void):
	m_ambient( 0.1f , 0.1f, 0.1f , 1.0f ),
	m_diffuse( 0.5f , 0.5f , 0.5f , 1.0f ),
	m_specular( 1.0f , 1.0f , 1.0f , 1.0f ),
	m_shininess( 128.0f )
	{ 
		for( int iChannel = 0 ; iChannel < MAX_TEXTURE_CHANNEL ; iChannel++ )
		{
			m_texChannels[iChannel] = NULL;
		}
	}


	Material::~Material(void)
	{
	}  

	void Material::SetTexture( Texture2D* pTexture, int channel )
	{
		ZP_ASSERT( channel >= 0 && channel < MAX_TEXTURE_CHANNEL );
		m_texChannels[channel] = pTexture;
	}

	Texture2D* Material::GetTexture( int channel )
	{
		ZP_ASSERT( channel >= 0 && channel < MAX_TEXTURE_CHANNEL );
		return m_texChannels[channel];
	}
	 
}//namespace Resource