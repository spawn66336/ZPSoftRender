#include "RenderPipeline.h"
#include "RenderPrimitive.h"

namespace Render
{


	RenderPipeline::RenderPipeline(void):
	m_pRenderContext( NULL )
	{
	}


	RenderPipeline::~RenderPipeline(void)
	{
		m_pRenderContext = NULL;
	}

	void RenderPipeline::DrawElements( RenderPrimitive& renderPrimitive )
	{

	}

	void RenderPipeline::RunVertexShaderStage( void )
	{

	}

	void RenderPipeline::RunRasterizationStage( void )
	{

	}

	void RenderPipeline::RunEarlyZPassStage( void )
	{

	}

	void RenderPipeline::RunFragmentShaderStage( void )
	{

	}

	void RenderPipeline::RunMergeStage( void )
	{

	}



}//namespace Render