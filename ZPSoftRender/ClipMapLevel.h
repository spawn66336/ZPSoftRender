#ifndef ZP_CLIPMAPLEVEL
#define ZP_CLIPMAPLEVEL

#include "ClipMapTerrainDefine.h"
#include "ClipMapHeightMapBlock.h"

namespace Terrain
{

	enum CLIPMAPLEVEL_FLAG
	{
		VERTS_CHANGE= 1
	};

class ClipMapLevel
{
public:
	ClipMapLevel(void);
	virtual ~ClipMapLevel(void);

	void Init( const unsigned int uiLevel , const unsigned int uiClipMapSize , const float fGridWidth );
	void Update( const ClipMapGridPos& center );
	const ClipMapGridPos& GetCenter( void ) const { return m_centerPos; }
	const ClipMapArea& GetArea( void ) const { return m_currArea; }

	bool TestFlag( CLIPMAPLEVEL_FLAG bit  );
	void SetFlag( CLIPMAPLEVEL_FLAG bit , bool b );
	
	TerrainVertex* GetVerts( void ){ return m_pVerts; } 
	unsigned short** GetTilesIndices( void ){ return m_ppTilesIndices; }

	//������
	unsigned int GetVertsNum( void ) const;
	//ÿ�������ӿ��������
	unsigned int GetIndicesNumPerTile( void ) const;
	//ÿ�������ӿ�ļ�������
	unsigned int GetPrimtiveNumPerTile( void ) const;
	unsigned int GetTileNum( void ) const { return 12; }
	Math::Vec3 GetLocalPos( void ) const;

protected:
	void _InitVerts( void );
	//����12���ӵ��ο������
	void _InitTilesIndices( void );
	void _UpdateVerts( void );
protected:
	unsigned int  m_uiLevel;			  //��ǰClipMap�㼶
	unsigned int  m_uiClipMapSize;  //ClipMap��С
	unsigned int  m_uiGridSize;		 //һ�����θ������ϸ��ĸ������
	float				 m_fGridWidth;		 //��ǰ����θ�����ʾ�Ŀ�ȣ�����Ϊ��λ��
	ClipMapGridPos m_centerPos;	 //��ǰ�������� (���ϸ��ĸ��������)
	ClipMapArea m_currArea;			//��ǰ����ʾ�ĸ߶�ͼ���� (���ϸ��ĸ����������)
	ClipMapHeightMapBlock m_heightMapBlock;	//��ǰ��߶�ͼ��
	TerrainVertex* m_pVerts;			//��ǰ��Ķ���
	unsigned short** m_ppTilesIndices; //12��С���ο������
	unsigned int m_uiFlag;				//��־
};

}//namespace Terrain
#endif