#include "ClipMapHeightMapBlock.h"
#include "ClipMapReader.h"
namespace Terrain
{

	ClipMapHeightMapBlock::ClipMapHeightMapBlock(void):
	m_uiLevel(0),		//��ǰ���ڼ���
	m_uiClipMapSize(0),		//����ͼ��С
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
		//��ʼ���߳�ͼ����
		m_pHeightMap = new float[uiSize];
		memset( m_pHeightMap , 0 , sizeof(float)*uiSize );  
	}

	void ClipMapHeightMapBlock::Destroy( void )
	{
		ZP_SAFE_DELETE_BUFFER( m_pHeightMap );
	}

	bool ClipMapHeightMapBlock::Update( const ClipMapArea& newArea )
	{
		//���θ�����ȫ������
		if( m_uiFlag&AREA_UNINIT )
		{
			_UpdateArea( newArea );
			m_currArea = newArea;
			m_uiFlag &= ~AREA_IS_DIRTY;
			m_uiFlag &= ~AREA_UNINIT;
			return true;
		} 
		ZP_ASSERT( !( m_uiFlag&AREA_IS_DIRTY ) );
		
		//����������û�б仯���������
		if( m_currArea.Equal(newArea) )
		{
			return false;
		}
		m_uiFlag |= AREA_IS_DIRTY; 
		 
		//����ȫ�����£�������ʵ�ֲ��ָ���
		_UpdateArea( newArea );
		m_currArea = newArea; 
		m_uiFlag &= ~AREA_IS_DIRTY;
		return true;
	}

	void ClipMapHeightMapBlock::_UpdateArea( const ClipMapArea& updateArea )
	{ 
		int stride = 1<<m_uiLevel;

		/*
		for( int z = updateArea.minPos.z ; z <= updateArea.maxPos.z ; z+=stride )
		{
			for( int x = updateArea.minPos.x ; x <= updateArea.maxPos.x ; x+= stride )
			{ 
				float h =  ClipMapReader::GetInstance()->Sample( x , z ); 
				int localX = x>>m_uiLevel;
				int localZ = z>>m_uiLevel;
				_SetHeight( localX , localZ , h*0.01f );
			}
		} 
		*/

		int z = updateArea.minPos.z; 
		for( unsigned int localz = 0 ; localz < m_uiClipMapSize ; localz++ )
		{
			int x = updateArea.minPos.x;
			for( unsigned int localx = 0 ; localx < m_uiClipMapSize ; localx++ )
			{
				float h =  ClipMapReader::GetInstance()->Sample( x , z ); 
				m_pHeightMap[ localz*m_uiClipMapSize + localx ] = h*0.1f;
				x += stride;
			}
			z += stride;
		}
	}


	void ClipMapHeightMapBlock::_SetHeight( const int localX , const int localZ , const float h )
	{ 
		int iWrapX = localX%m_uiClipMapSize;
		int iWrapZ = localZ%m_uiClipMapSize; 
		m_pHeightMap[iWrapZ*m_uiClipMapSize + iWrapX] = h;
	}

	float ClipMapHeightMapBlock::Sample( const int localX , const int localZ )
	{ 
		/*int iWrapX = localX%m_uiClipMapSize;
		int iWrapZ = localZ%m_uiClipMapSize; 
		return m_pHeightMap[iWrapZ*m_uiClipMapSize + iWrapX];*/
		return m_pHeightMap[localZ*m_uiClipMapSize + localX];
	}

}//namespace Terrain