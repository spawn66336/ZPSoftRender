#include "Light.h"

namespace Render
{ 

	Light::Light(void):
	m_name(),
	m_v4Ambient(),
	m_v4Diffuse(),
	m_v4Specular(),
	m_v4Pos(),
	m_v4PosInCam(),
	m_active(false)
	{
		m_v4Ambient.Set( 0.25f , 0.25f , 0.25f , 1.0f );
		m_v4Diffuse.Set( 0.75f ,0.75f ,0.75f ,1.0f );
		m_v4Specular.Set( 1.0f , 1.0f , 1.0f ,1.0f );
		m_v4Pos.Set( 100.0f ,100.0f , 100.0f ,0.0f );
	}


	Light::~Light(void)
	{
	}

}//namespace Render