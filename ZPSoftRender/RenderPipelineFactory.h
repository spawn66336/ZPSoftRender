#ifndef ZP_RENDERPIPELINEFACTORY
#define ZP_RENDERPIPELINEFACTORY

#include "IRender.h"
#include "Singleton.h"

namespace Render
{  
	class RenderPipeline;

	class RenderPipelineFactory : public Util::SingletonT<RenderPipelineFactory>
	{
	public:
		RenderPipelineFactory( void );
		virtual ~RenderPipelineFactory( void );

		RenderPipeline* Create( const SHADE_MODEL model );
	};

}//namespace Render

#endif //ZP_RENDERPIPELINEFACTORY