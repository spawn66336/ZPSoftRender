
#ifndef ZP_FRAMESTACKALLOCATOR
#define ZP_FRAMESTACKALLOCATOR

#include "ZPDependency.h"
#include "Singleton.h"

namespace Render
{ 

	const unsigned int MAX_FRAMESTACK_ALLOCATOR_SIZE = 1024*1024*10;

	class FrameStackAllocator : public Util::SingletonT<FrameStackAllocator>
	{
	public:
		FrameStackAllocator(void);
		virtual ~FrameStackAllocator(void);

		void* Alloc( const unsigned int uiSize );

		inline unsigned int Capacity( void ) const { return m_uiCapacity; }

		inline unsigned int Watermark( void ) const { return m_uiWatermark; }

		inline unsigned int BytesUsed( void ) const { return m_uiBytesUsed; }

		inline void Clear( void ){ m_uiBytesUsed = 0; }

	protected:

		unsigned int     m_uiWatermark;		
		unsigned int     m_uiCapacity;
		unsigned int     m_uiBytesUsed; 
		unsigned char* m_pBuf;
	};

}//namespace Render

#endif //ZP_FRAMESTACKALLOCATOR