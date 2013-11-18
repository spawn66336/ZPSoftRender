#include "ClipMapHeightMapBlock.h"
#include "ClipMapReader.h"
namespace Terrain
{

	ClipMapHeightMapBlock::ClipMapHeightMapBlock(void):
	m_uiLevel(0),		//当前所在级别
	m_uiClipMapSize(0),		//剪切图大小
	m_uiFlag(AREA_UNINIT),
	m_pHeightMap(0)
	{
	}
	 
	ClipMapHeightMapBlock::~ClipMapHeightMapBlock(void)
	{
		Destroy();
	}

	void ClipMapHeightMapBlock::Init( const unsigned int uiLevel , const unsigned int uiClipMapSize )
	{
		m_uiLevel = uiLevel;
		m_uiClipMapSize = uiClipMapSize;

		unsigned int uiSize = m_uiClipMapSize*m_uiClipMapSize;
		//初始化高程图区域
		m_pHeightMap = new unsigned int[uiSize];
		memset( m_pHeightMap , 0 , sizeof(unsigned int)*uiSize );  
	}

	void ClipMapHeightMapBlock::Destroy( void )
	{
		ZP_SAFE_DELETE_BUFFER( m_pHeightMap );
	}

	bool ClipMapHeightMapBlock::Update( const ClipMapArea& newArea )
	{
		//初次更新则全部更新
		if( m_uiFlag&AREA_UNINIT )
		{
			_UpdateArea( m_currArea );
			m_currArea = newArea;
			m_uiFlag &= ~AREA_IS_DIRTY;
			m_uiFlag &= ~AREA_UNINIT;
			return true;
		} 
		ZP_ASSERT( !( m_uiFlag&AREA_IS_DIRTY ) );
		
		//若两个区域没有变化则无需更新
		if( m_currArea.Equal(newArea) )
		{
			return false;
		}
		m_uiFlag |= AREA_IS_DIRTY; 

		//int diffx = newArea.minPos.x - m_currArea.minPos.x;
		//int diffz = newArea.minPos.z - m_currArea.minPos.z;
		//暂行全部更新，后面再实现部分更新
		_UpdateArea( newArea );
		m_currArea = newArea; 
		m_uiFlag &= ~AREA_IS_DIRTY;
		return true;
	}

	void ClipMapHeightMapBlock::_UpdateArea( const ClipMapArea& updateArea )
	{
		int x = updateArea.minPos.x;
		int z = updateArea.minPos.z;
		int stride = 1<<m_uiLevel;

		for( ; z <= updateArea.maxPos.z ; z+=stride )
		{
			for( ; x <= updateArea.maxPos.x ; x+= stride )
			{ 
				unsigned int h =  ClipMapReader::GetInstance()->Sample( x , z ); 
				int localX = x>>m_uiLevel;
				int localZ = z>>m_uiLevel;
				_SetHeight( localX , localZ , h );
			}
		} 
	}


	void ClipMapHeightMapBlock::_SetHeight( const int localX , const int localZ , const unsigned int h )
	{ 
		int iWrapX = localX%m_uiClipMapSize;
		int iWrapZ = localZ%m_uiClipMapSize; 
		m_pHeightMap[iWrapZ*m_uiClipMapSize + iWrapX] = h;
	}

	unsigned int ClipMapHeightMapBlock::Sample( const int localX , const int localZ )
	{
		int iWrapX = localX%m_uiClipMapSize;
		int iWrapZ = localZ%m_uiClipMapSize; 
		return m_pHeightMap[iWrapZ*m_uiClipMapSize + iWrapX];
	}

}//namespace Terrain