#ifndef ZP_RENDERPIPELINE
#define ZP_RENDERPIPELINE
#include "RenderList.h"

namespace Render
{
	class RenderContext;
	class RenderPrimitive;

	class RenderPipeline
	{
	public:
		RenderPipeline(void);
		virtual ~RenderPipeline(void); 

		virtual void SetRenderContext( RenderContext* pRenderContext ){ m_pRenderContext = pRenderContext; }

		virtual RenderContext* GetRenderContext( void ) const { return m_pRenderContext; }

		virtual void DrawElements( RenderPrimitive& renderPrimitive );

	protected:

		virtual void RunVertexShaderStage( void );

		virtual void RunRasterizationStage( void );

		virtual void RunEarlyZPassStage( void );

		virtual void RunFragmentShaderStage( void );

		virtual void RunMergeStage( void );

	private:

		RenderContext* m_pRenderContext;
		RenderList		  m_renderList;
	};

}//namespace Render

#endif// ZP_RENDERPIPELINE