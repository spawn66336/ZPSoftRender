#include "PhongShadingPipeline.h"

namespace Render
{


	PhongShadingPipeline::PhongShadingPipeline(void)
	{
		m_pixelShader.SetShadeModel( PHONG_MODEL );
	}


	PhongShadingPipeline::~PhongShadingPipeline(void)
	{
	}

}//namespace Render