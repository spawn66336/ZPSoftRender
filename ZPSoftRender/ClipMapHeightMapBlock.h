#ifndef ZP_CLIPMAPHEIGHTMAPBLOCK
#define ZP_CLIPMAPHEIGHTMAPBLOCK

#include "ClipMapTerrainDefine.h"

namespace Terrain
{
	enum AREA_UPDATE_FLAG
	{
		AREA_UNINIT = 1<<0, 
		AREA_IS_DIRTY = 1<<1
	};
	 
class ClipMapHeightMapBlock
{
public:
	ClipMapHeightMapBlock(void);
	virtual ~ClipMapHeightMapBlock(void);

	void Init( const unsigned int uiLevel , const unsigned int uiClipMapSize );
	void Destroy( void );
	/**
	* @brief ����������µ�ǰ��߶�ͼ
	* @newArea ����������
	* @return �Ƿ�����������ĸ���
	*/
	bool Update( const ClipMapArea& newArea );
	float Sample( const int localX , const int localZ );
	Math::Vec3 SampleNormal( const int localX , const int localZ );
protected:
	void _UpdateArea( const ClipMapArea& updateArea );
	 
	int _WrapAddress( int x );
	/**
	* @brief ���ø߳�ͼĳ��߶�
	* @param x �����꣨��ǰ�����꣩
	* @param z �����꣨��ǰ�����꣩
	* @param h �����ø߶�
	*/
	void _SetHeight( const int localX , const int localZ , const float h );
	void _SetNormal( const int localX , const int localZ , const Math::Vec3& norm );
protected:
	ClipMapArea m_currArea;
	unsigned int  m_uiLevel;				//��ǰ���ڼ���
	unsigned int  m_uiClipMapSize;		//����ͼ��С
	unsigned int  m_uiFlag;
	float* m_pHeightMap;		//��ǰ��߳�ͼ
	Math::Vec3* m_pNormalMap; //��ǰ�㷨��ͼ
};

}//namespace Terrain
#endif