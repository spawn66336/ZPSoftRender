#include "RenderPipeline.h"
#include "RenderPrimitive.h"
#include "FrameStackAllocator.h"
#include "RenderContext.h"

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
		FrameStackAllocator::GetInstance()->Clear();
		m_renderList.Clear(); 
		m_renderList.CopyFromRenderPrimitive( renderPrimitive ); 

		this->RunVertexShaderStage();
		this->RunRasterizationStage();
		this->RunEarlyZPassStage();
		this->RunFragmentShaderStage();
		this->RunMergeStage();
	}

	void RenderPipeline::RunVertexShaderStage( void )
	{
		m_renderList.TransformFromLocalSpaceToCameraSpace( m_pRenderContext->GetCurrModelViewMatrix() );
		m_renderList.RemoveBackFaceInCameraSpace();
		m_renderList.TransformFromCameraSpaceToProjectionSpace( m_pRenderContext->GetCurrProjectionMatrix() );
		m_renderList.TransformFromProjectionSpaceToScreenSpace( m_pRenderContext->GetCurrProjectionToScreenMatrix() );
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
		m_renderList.DrawFacesWireFrameToFrameBuffer( m_pRenderContext->GetColorFrameBuffer() );
	}



}//namespace Render