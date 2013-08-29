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

}//namespace Render