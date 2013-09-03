
#ifndef ZP_FRAMESTACKALLOCATOR
#define ZP_FRAMESTACKALLOCATOR

#include "ZPDependency.h"
#include "Singleton.h"

namespace Render
{ 

	const unsigned int MAX_FRAMESTACK_ALLOCATOR_SIZE = 1024*1024*10;


	/**
	* @brief 帧栈分配器
	*/
	class FrameStackAllocator : public Util::SingletonT<FrameStackAllocator>
	{
	public:
		FrameStackAllocator(void);
		virtual ~FrameStackAllocator(void);

		/**
		* @brief 从帧栈分配器中分配空间
		* @param uiSize 在帧栈分配器中要申请的空间大小（按字节计）
		* @return 返回分配到空间的头指针
		*/
		void* Alloc( const unsigned int uiSize );

		/**
		* @brief 帧栈分配器的总容量
		*/
		inline unsigned int Capacity( void ) const { return m_uiCapacity; }

		/**
		* @brief 帧栈分配器最大使用量
		*/
		inline unsigned int Watermark( void ) const { return m_uiWatermark; }

		/**
		* @brief 当前帧栈分配器的空间用量
		*/
		inline unsigned int BytesUsed( void ) const { return m_uiBytesUsed; }

		/**
		* @brief 清空帧栈分配器
		* @remark 此函数并没有真正释放栈帧分配器中的内存
		*				 而是将用量清零。
		*/
		inline void Clear( void ){ m_uiBytesUsed = 0; }

	protected:

		unsigned int     m_uiWatermark;	///>帧栈分配器水位线
		unsigned int     m_uiCapacity;		///>帧栈分配器总容量
		unsigned int     m_uiBytesUsed;	///>帧栈分配器的当前用量
		unsigned char* m_pBuf;				///>帧栈分配器缓冲区头指针
	};

}//namespace Render

#endif //ZP_FRAMESTACKALLOCATOR