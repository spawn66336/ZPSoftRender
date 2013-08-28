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

		virtual void RunVertexShaderStage( void );

		virtual void RunMergeStage( void );
	};

}//namespace Render

#endif //ZP_GOURAUDSHADINGPIPELINE