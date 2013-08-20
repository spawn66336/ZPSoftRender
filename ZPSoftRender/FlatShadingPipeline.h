#ifndef ZP_FLATSHADINGPIPELINE
#define ZP_FLATSHADINGPIPELINE

#include "RenderPipeline.h"

namespace Render
{ 

	class FlatShadingPipeline : public RenderPipeline
	{
	public:
		FlatShadingPipeline(void);
		virtual ~FlatShadingPipeline(void);
	};

}//namespace Render

#endif //ZP_FLATSHADINGPIPELINE