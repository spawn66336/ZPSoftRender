#ifndef ZP_PHONGSHADINGPIPELINE
#define ZP_PHONGSHADINGPIPELINE


#include "RenderPipeline.h"

namespace Render
{
	 
	class PhongShadingPipeline : public RenderPipeline
	{
	public:
		PhongShadingPipeline(void);
		virtual ~PhongShadingPipeline(void);
	};

}// namespace Render

#endif// ZP_PHONGSHADINGPIPELINE