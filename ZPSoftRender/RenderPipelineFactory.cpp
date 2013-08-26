#include "RenderPipelineFactory.h"
#include "RenderPipeline.h"
#include "FlatShadingPipeline.h"
#include "GouraudShadingPipeline.h"
#include "PhongShadingPipeline.h"
#include "NormalMapShadingPipeline.h"


namespace Render
{ 

	RenderPipelineFactory::RenderPipelineFactory(void)
	{
	}


	RenderPipelineFactory::~RenderPipelineFactory(void)
	{
	}

	RenderPipeline* RenderPipelineFactory::Create( const SHADE_MODEL model )
	{
		RenderPipeline* pNewPipeline = NULL;

		switch( model )
		{ 
		case FLAT_MODEL:
			pNewPipeline = new FlatShadingPipeline;
			break;
		case GOURAUD_MODEL:
			pNewPipeline = new GouraudShadingPipeline;
			break;
		case PHONG_MODEL:
			pNewPipeline = new PhongShadingPipeline;
			break;
		case NORMMAP_MODEL:
			pNewPipeline = new NormalMapShadingPipeline;
			break;
		default:
			pNewPipeline = new RenderPipeline;
			break;
		}

		ZP_ASSERT( NULL != pNewPipeline );

		return pNewPipeline;
	}

}//namespace Render