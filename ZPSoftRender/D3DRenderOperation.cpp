#include "D3DRenderOperation.h"

namespace Render
{

	RenderOperationCache::RenderOperationCache()
	{

	}

	RenderOperationCache::~RenderOperationCache()
	{
		Clear();
	}

	D3DRenderOperation* RenderOperationCache::CreateRenderOperation( void* pMark )
	{
		auto findOp = m_renderOpTab.find( pMark );
		if( findOp != m_renderOpTab.end() )
		{
			return findOp->second;
		}
		
		D3DRenderOperation* pNewOp = new D3DRenderOperation;
		m_renderOpTab.insert( std::make_pair(pMark , pNewOp ) );
		return pNewOp;
	}

	void RenderOperationCache::Clear( void )
	{
		auto itOp = m_renderOpTab.begin();
		while( itOp != m_renderOpTab.end() )
		{
			ZP_SAFE_DELETE( itOp->second  );
			++itOp;
		}
		m_renderOpTab.clear();
	}

}