#ifndef ZP_GOURAUDSHADINGPIPELINE
#define ZP_GOURAUDSHADINGPIPELINE

#include "RenderPipeline.h"

namespace Render
{

	class GouraudShadingPipeline : public RenderPipeline
	{
	public:
		GouraudShadingPipeline(void);
		virtual ~GouraudShadingPipeline(void);

		virtual void RunLightingStage( void );

		virtual void RunRasterizationStage( void );
	};

}//namespace Render

#endif //ZP_GOURAUDSHADINGPIPELINE