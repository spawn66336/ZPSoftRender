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
	}


	Light::~Light(void)
	{
	}

}//namespace Render