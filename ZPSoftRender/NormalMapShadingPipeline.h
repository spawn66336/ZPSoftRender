#ifndef ZP_NORMALMAPSHADINGPIPELINE
#define ZP_NORMALMAPSHADINGPIPELINE

#include "RenderPipeline.h"

namespace Render
{ 

	class NormalMapShadingPipeline : public RenderPipeline
	{
	public:
		NormalMapShadingPipeline(void);
		virtual ~NormalMapShadingPipeline(void);
	};

}//namespace Render

#endif //ZP_NORMALMAPSHADINGPIPELINE