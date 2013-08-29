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

		virtual void RunLightingStage( void );

		virtual void RunRasterizationStage( void );
	};

}//namespace Render

#endif //ZP_FLATSHADINGPIPELINE