#include "NormalMapShadingPipeline.h"

namespace Render
{


NormalMapShadingPipeline::NormalMapShadingPipeline(void)
{
	m_pixelShader.SetShadeModel( NORMMAP_MODEL );
}


NormalMapShadingPipeline::~NormalMapShadingPipeline(void)
{
}

void NormalMapShadingPipeline::RunLightingStage( void )
{
	PrepareForPerPixelLightingInCameraSpace();
}

void NormalMapShadingPipeline::RunRasterizationStage( void )
{
	DrawTrianglesToFrameBuffer();
}

}//namespace Render