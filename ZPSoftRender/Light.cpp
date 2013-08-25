#include "Light.h"

namespace Render
{ 

	Light::Light(void):
	m_name(),
	m_ambient(),
	m_diffuse(),
	m_specular(),
	m_pos(),
	m_active(false)
	{
		m_ambient.Set( 0.0f , 0.0f , 0.0f , 1.0f );
		m_diffuse.Set( 0.75f ,0.75f ,0.75f ,1.0f );
		m_specular.Set( 1.0f , 1.0f , 1.0f ,1.0f );
		m_pos.Set( 100.0f ,100.0f , 100.0f ,0.0f );
	}


	Light::~Light(void)
	{
	}

}//namespace Render