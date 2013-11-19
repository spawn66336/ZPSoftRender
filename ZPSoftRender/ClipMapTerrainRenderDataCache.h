#ifndef ZP_CLIPMAPTERRAINRENDERDATACACHE
#define ZP_CLIPMAPTERRAINRENDERDATACACHE

#include "ZPDependency.h" 
#include "ZPMathDependency.h"
#include "d3d9.h"

namespace Render
{
	class D3DRenderOperation;
}
namespace Terrain
{ 
	class ClipMapLevel;

	class TerrainLevelRenderData
	{
	protected:
		TerrainLevelRenderData();
	public:
		TerrainLevelRenderData( LPDIRECT3DDEVICE9 pDevice );
		~TerrainLevelRenderData();

		void Update( ClipMapLevel* pLevel );
		void GetRenderOps( std::vector<Render::D3DRenderOperation*>& opList );

		LPDIRECT3DDEVICE9	m_pDevice;
		LPDIRECT3DVERTEXDECLARATION9 m_pVBDecl;
		LPDIRECT3DVERTEXBUFFER9 m_pVB;
		LPDIRECT3DINDEXBUFFER9 m_ppTileIB[12];
		LPDIRECT3DINDEXBUFFER9 m_pGapTileIB;
		LPDIRECT3DINDEXBUFFER9 m_pCenterTileIB;		//中心区块IB
		Math::Vec3 m_v3LocalPos;
		unsigned int m_uiLevelCurrFlag;     //所对应层级的标志（用来标识某块是否用于显示）

		unsigned int m_uiVertNum;

		unsigned int m_uiIndiceNumPerTile;
		unsigned int m_uiPrimtivePerTile;

		unsigned int m_uiGapTileIndiceNum;
		unsigned int m_uiGapTilePrimitiveNum;

		unsigned int m_uiCenterTileIndiceNum;
		unsigned int m_uiCenterTilePrimitiveNum;

		Render::D3DRenderOperation* m_tileRenderOps[12];
		Render::D3DRenderOperation* m_pGapTileOp;
		Render::D3DRenderOperation* m_pCenterTileOp;
	};

	class ClipMapTerrainRenderDataCache
	{
	public:
		ClipMapTerrainRenderDataCache(void);
		virtual ~ClipMapTerrainRenderDataCache(void);

		
		void Init( LPDIRECT3DDEVICE9 pDevice );
		void Destroy(void);
		void UpdateTerrainLevelRenderData( ClipMapLevel* pLevel );
		void GetRenderOps( std::vector<Render::D3DRenderOperation*>& opList );
		void OnLostDevice( void );
		void OnResetDevice( void );

	protected:
		typedef std::map<void*,TerrainLevelRenderData*> RenderDataTab_t;
		LPDIRECT3DDEVICE9 m_pDevice;
		RenderDataTab_t m_renderDataTab;
	};

}//namespace Terrain

#endif