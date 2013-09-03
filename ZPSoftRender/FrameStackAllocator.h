
#ifndef ZP_FRAMESTACKALLOCATOR
#define ZP_FRAMESTACKALLOCATOR

#include "ZPDependency.h"
#include "Singleton.h"

namespace Render
{ 

	const unsigned int MAX_FRAMESTACK_ALLOCATOR_SIZE = 1024*1024*10;


	/**
	* @brief ֡ջ������
	*/
	class FrameStackAllocator : public Util::SingletonT<FrameStackAllocator>
	{
	public:
		FrameStackAllocator(void);
		virtual ~FrameStackAllocator(void);

		/**
		* @brief ��֡ջ�������з���ռ�
		* @param uiSize ��֡ջ��������Ҫ����Ŀռ��С�����ֽڼƣ�
		* @return ���ط��䵽�ռ��ͷָ��
		*/
		void* Alloc( const unsigned int uiSize );

		/**
		* @brief ֡ջ��������������
		*/
		inline unsigned int Capacity( void ) const { return m_uiCapacity; }

		/**
		* @brief ֡ջ���������ʹ����
		*/
		inline unsigned int Watermark( void ) const { return m_uiWatermark; }

		/**
		* @brief ��ǰ֡ջ�������Ŀռ�����
		*/
		inline unsigned int BytesUsed( void ) const { return m_uiBytesUsed; }

		/**
		* @brief ���֡ջ������
		* @remark �˺�����û�������ͷ�ջ֡�������е��ڴ�
		*				 ���ǽ��������㡣
		*/
		inline void Clear( void ){ m_uiBytesUsed = 0; }

	protected:

		unsigned int     m_uiWatermark;	///>֡ջ������ˮλ��
		unsigned int     m_uiCapacity;		///>֡ջ������������
		unsigned int     m_uiBytesUsed;	///>֡ջ�������ĵ�ǰ����
		unsigned char* m_pBuf;				///>֡ջ������������ͷָ��
	};

}//namespace Render

#endif //ZP_FRAMESTACKALLOCATOR