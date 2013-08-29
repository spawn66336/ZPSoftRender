#include "FlatShadingPipeline.h"
#include "RenderContext.h"

namespace Render
{ 

	FlatShadingPipeline::FlatShadingPipeline(void)
	{
		m_pixelShader.SetShadeModel( FLAT_MODEL );
	} 

	FlatShadingPipeline::~FlatShadingPipeline(void)
	{
	}


	void FlatShadingPipeline::RunLightingStage( void )
	{
		CalcLightInCameraSpace();
	}


	void FlatShadingPipeline::RunRasterizationStage( void )
	{ 
		DrawFacesSolidTrianglesToFrameBuffer();
	}

}//namespace Render
