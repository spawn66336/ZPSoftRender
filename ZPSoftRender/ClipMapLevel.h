#ifndef ZP_CLIPMAPLEVEL
#define ZP_CLIPMAPLEVEL

#include "ClipMapTerrainDefine.h"
#include "ClipMapHeightMapBlock.h"

namespace Terrain
{

	enum CLIPMAPLEVEL_FLAG
	{
		VERTS_CHANGE= 1<<0 ,
		SHOW_CENTER_TILE = 1<<1 ,
		SHOW_GAP_TILES = 1<<2,
		SHOW_LEFT_TOP_L_TILE = 1<<3,
		SHOW_LEFT_BOTTOM_L_TILE = 1<<4,
		SHOW_RIGHT_TOP_L_TILE = 1<<5,
		SHOW_RIGHT_BOTTOM_L_TILE = 1<<6,
		SHOW_FIXED_UP_RING = 1<<7
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
	unsigned int GetFlag(void) const { return m_uiFlag; }
	
	TerrainVertex* GetVerts( void ) { return m_pVerts; } 
	unsigned short** GetTilesIndices( void ) { return m_ppTilesIndices; }
	unsigned short*  GetGapTileIndices( void ) { return m_pGapTileIndices; }
	unsigned short* GetCenterTileIndices( void ){ return m_pCenterTileIndices; }

	//������
	unsigned int GetVertsNum( void ) const;

	
	unsigned int GetTileNum( void ) const { return 12; }
	//ÿ�������ӿ��������
	unsigned int GetIndicesNumPerTile( void ) const;
	//ÿ�������ӿ�ļ�������
	unsigned int GetPrimtiveNumPerTile( void ) const;
	unsigned int GetGapTileIndicesNum( void ) const;
	unsigned int GetGapTilePrimitiveNum( void ) const;

	unsigned int GetCenterTileIndicesNum( void ) const;
	unsigned int GetCenterTilePrimitiveNum( void ) const;

	Math::Vec3 GetLocalPos( void ) const;

protected:
	void _InitVerts( void );
	//����12���ӵ��ο������
	void _InitTilesIndices( void );
	void _InitGapTilesIndices( void );
	void _InitCenterTileIndices( void );
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
	unsigned short*   m_pGapTileIndices; //�������Ҹ��ĸ��޲���
	unsigned short*	m_pCenterTileIndices;		//���Ŀ飨ֻ���ڶ����ɼ���
	unsigned int m_uiFlag;				//��־
};

}//namespace Terrain
#endif