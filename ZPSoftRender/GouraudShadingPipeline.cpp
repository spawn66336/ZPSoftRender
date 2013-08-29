#include "GouraudShadingPipeline.h"

namespace Render
{


GouraudShadingPipeline::GouraudShadingPipeline(void)
{
	m_pixelShader.SetShadeModel( GOURAUD_MODEL );
}


GouraudShadingPipeline::~GouraudShadingPipeline(void)
{
}

 
void GouraudShadingPipeline::RunLightingStage( void )
{
	CalcPerVertexLightInCameraSpace();
}

void GouraudShadingPipeline::RunRasterizationStage( void )
{
	DrawTrianglesToFrameBuffer();
}


}//namespace Render