#include "GouraudShadingPipeline.h"

namespace Render
{


GouraudShadingPipeline::GouraudShadingPipeline(void)
{
}


GouraudShadingPipeline::~GouraudShadingPipeline(void)
{
}

void GouraudShadingPipeline::RunMergeStage( void )
{
	DrawGouraudShadingTrianglesToFrameBuffer();
}

void GouraudShadingPipeline::RunVertexShaderStage( void )
{
	TransformFromLocalSpaceToCameraSpace();
	RemoveBackFaceInCameraSpace();
	CalcPerVertexLightInCameraSpace();
	TransformFromCameraSpaceToProjectionSpace();
	TransformFromProjectionSpaceToScreenSpace();
}


}//namespace Render