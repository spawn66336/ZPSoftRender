#include "FrameStackAllocator.h"

namespace Render
{ 

		FrameStackAllocator::FrameStackAllocator(void):
			m_uiBytesUsed(0),
			m_uiWatermark(0),
			m_uiCapacity( MAX_FRAMESTACK_ALLOCATOR_SIZE ),
			m_pBuf(NULL)
		{
			m_pBuf = new unsigned char[m_uiCapacity];
			ZP_ASSERT( NULL != m_pBuf );
		}


	FrameStackAllocator::~FrameStackAllocator(void)
	{
		m_uiBytesUsed = 0;
		m_uiWatermark = 0;
		m_uiCapacity = 0;
		ZP_SAFE_DELETE_BUFFER( m_pBuf );
	}

	void* FrameStackAllocator::Alloc( const unsigned int uiSize )
	{
		ZP_ASSERT( ( uiSize + m_uiBytesUsed ) <= m_uiCapacity );

		//容量超过所需
		if( uiSize + m_uiBytesUsed > m_uiCapacity )
		{
			return NULL;
		}

		void* pAllocBuf = &m_pBuf[m_uiBytesUsed];
		m_uiBytesUsed += uiSize; 
		m_uiWatermark = m_uiWatermark > m_uiBytesUsed ? m_uiWatermark : m_uiBytesUsed;
		return pAllocBuf;
	}

}//namespace Render