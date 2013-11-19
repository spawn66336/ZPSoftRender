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

	//顶点数
	unsigned int GetVertsNum( void ) const;

	
	unsigned int GetTileNum( void ) const { return 12; }
	//每个地形子块的索引数
	unsigned int GetIndicesNumPerTile( void ) const;
	//每个地形子块的几何体数
	unsigned int GetPrimtiveNumPerTile( void ) const;
	unsigned int GetGapTileIndicesNum( void ) const;
	unsigned int GetGapTilePrimitiveNum( void ) const;

	unsigned int GetCenterTileIndicesNum( void ) const;
	unsigned int GetCenterTilePrimitiveNum( void ) const;

	Math::Vec3 GetLocalPos( void ) const;

protected:
	void _InitVerts( void );
	//构建12个子地形块的索引
	void _InitTilesIndices( void );
	void _InitGapTilesIndices( void );
	void _InitCenterTileIndices( void );
	void _UpdateVerts( void );
protected:
	unsigned int  m_uiLevel;			  //当前ClipMap层级
	unsigned int  m_uiClipMapSize;  //ClipMap大小
	unsigned int  m_uiGridSize;		 //一个地形格所用最精细层的格点数量
	float				 m_fGridWidth;		 //当前层地形格所表示的跨度（以米为单位）
	ClipMapGridPos m_centerPos;	 //当前层格点中心 (以最精细层的格点数量计)
	ClipMapArea m_currArea;			//当前所表示的高度图区域 (以最精细层的格点数量计算)
	ClipMapHeightMapBlock m_heightMapBlock;	//当前层高度图块
	TerrainVertex* m_pVerts;			//当前层的顶点
	unsigned short** m_ppTilesIndices; //12个小地形块的索引
	unsigned short*   m_pGapTileIndices; //上下左右各四个修补条
	unsigned short*	m_pCenterTileIndices;		//中心块（只对于顶层层可见）
	unsigned int m_uiFlag;				//标志
};

}//namespace Terrain
#endif