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

		virtual void RunLightingStage( void );

		virtual void RunRasterizationStage( void ); 
	};

}//namespace Render

#endif //ZP_NORMALMAPSHADINGPIPELINE