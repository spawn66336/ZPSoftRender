#include "FlatShadingPipeline.h"
#include "RenderContext.h"

namespace Render
{ 

	FlatShadingPipeline::FlatShadingPipeline(void)
	{
	} 

	FlatShadingPipeline::~FlatShadingPipeline(void)
	{
	}

	void FlatShadingPipeline::RunMergeStage( void )
	{ 
		DrawFacesSolidTrianglesToFrameBuffer();
	}

}//namespace Render
