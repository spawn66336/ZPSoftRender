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

	void PhongShadingPipeline::RunLightingStage( void )
	{
		PrepareForPerPixelLightingInCameraSpace();
	}

	void PhongShadingPipeline::RunRasterizationStage( void )
	{
		DrawTrianglesToFrameBuffer();
	}

}//namespace Render