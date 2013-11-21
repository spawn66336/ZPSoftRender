#include "ClipMapHeightMapBlock.h"
#include "ClipMapReader.h"
namespace Terrain
{

	ClipMapHeightMapBlock::ClipMapHeightMapBlock(void):
	m_uiLevel(0),		//当前所在级别
	m_uiClipMapSize(0),		//剪切图大小
	m_uiFlag(AREA_UNINIT),
	m_pHeightMap(0),
	m_pNormalMap(0)
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
		m_pHeightMap = new float[uiSize];
		memset( m_pHeightMap , 0 , sizeof(float)*uiSize );  

		//初始化法线图
		m_pNormalMap = new Math::Vec3[uiSize]; 
	}

	void ClipMapHeightMapBlock::Destroy( void )
	{
		ZP_SAFE_DELETE_BUFFER( m_pHeightMap );
		ZP_SAFE_DELETE_BUFFER( m_pNormalMap );
	}

	bool ClipMapHeightMapBlock::Update( const ClipMapArea& newArea )
	{
		//初次更新则全部更新
		if( m_uiFlag&AREA_UNINIT )
		{ 
			m_currArea = newArea;
			_UpdateArea( newArea );
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
		 
		//暂行全部更新，后面再实现部分更新
		m_currArea = newArea; 
		_UpdateArea( m_currArea ); 
		m_uiFlag &= ~AREA_IS_DIRTY;
		return true;
	}

	void ClipMapHeightMapBlock::_UpdateArea( const ClipMapArea& updateArea )
	{ 
		int stride = 1<<m_uiLevel; 
		for( int z = updateArea.minPos.z ; z <= updateArea.maxPos.z ; )
		{
			for( int x = updateArea.minPos.x ; x <= updateArea.maxPos.x ;  )
			{ 
				float h = 0.0f; 
				Math::Vec3 norm;
				ClipMapReader::GetInstance()->Sample( x , z , h );
				ClipMapReader::GetInstance()->SampleNormal( x , z , norm );
				int localX = x>>m_uiLevel;
				int localZ = z>>m_uiLevel;
				_SetHeight( localX , localZ , h );
				_SetNormal( localX , localZ , norm );
				x+= stride;
			}
			z+=stride;
		}  
	} 

	void ClipMapHeightMapBlock::_SetHeight( const int localX , const int localZ , const float h )
	{   
		int iWrapX = _WrapAddress( localX );
		int iWrapZ = _WrapAddress( localZ );
		m_pHeightMap[iWrapZ*m_uiClipMapSize + iWrapX] = h;
	}

	void ClipMapHeightMapBlock::_SetNormal( const int localX , const int localZ , const Math::Vec3& norm )
	{
		int iWrapX = _WrapAddress( localX );
		int iWrapZ = _WrapAddress( localZ );
		m_pNormalMap[iWrapZ*m_uiClipMapSize+iWrapX] = norm;
	}

	float ClipMapHeightMapBlock::Sample( const int localX , const int localZ )
	{ 
		int iWrapX = _WrapAddress( localX );
		int iWrapZ = _WrapAddress( localZ );
		return m_pHeightMap[iWrapZ*m_uiClipMapSize+iWrapX]; 
	}


	Math::Vec3 ClipMapHeightMapBlock::SampleNormal( const int localX , const int localZ )
	{
		int iWrapX = _WrapAddress( localX );
		int iWrapZ = _WrapAddress( localZ );
		return m_pNormalMap[iWrapZ*m_uiClipMapSize+iWrapX];
	}


	int ClipMapHeightMapBlock::_WrapAddress( int x )
	{
		int tmp = x%((int)m_uiClipMapSize);
		if( tmp < 0 )
		{
			tmp += ((int)m_uiClipMapSize);
		}
		return tmp;
	}



}//namespace Terrain