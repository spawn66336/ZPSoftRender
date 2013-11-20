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
			m_currArea = newArea;
			_UpdateArea( newArea );
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
				ClipMapReader::GetInstance()->Sample( x , z , h );
				int localX = x>>m_uiLevel;
				int localZ = z>>m_uiLevel;
				_SetHeight( localX , localZ , h*0.1f );
				x+= stride;
			}
			z+=stride;
		}  
	} 

	void ClipMapHeightMapBlock::_SetHeight( const int localX , const int localZ , const float h )
	{   
		int iWrapX = localX%((int)m_uiClipMapSize);
		if( iWrapX < 0 )
		{
			iWrapX += ((int)m_uiClipMapSize);
		}
		int iWrapZ = localZ%((int)m_uiClipMapSize);
		if( iWrapZ < 0 )
		{
			iWrapZ += ((int)m_uiClipMapSize);
		}
		m_pHeightMap[iWrapZ*m_uiClipMapSize + iWrapX] = h;
	}

	float ClipMapHeightMapBlock::Sample( const int localX , const int localZ )
	{ 
		int iWrapX = localX%((int)m_uiClipMapSize);
		if( iWrapX < 0 )
		{
			iWrapX += ((int)m_uiClipMapSize);
		}
		int iWrapZ = localZ%((int)m_uiClipMapSize);
		if( iWrapZ < 0 )
		{
			iWrapZ += ((int)m_uiClipMapSize);
		}
		return m_pHeightMap[iWrapZ*m_uiClipMapSize + iWrapX]; 
	}

}//namespace Terrain