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
		LPDIRECT3DVERTEXBUFFER9 m_pFixedRingVB;
		LPDIRECT3DINDEXBUFFER9 m_ppTileIB[12];
		LPDIRECT3DINDEXBUFFER9 m_pGapTileIB;
		LPDIRECT3DINDEXBUFFER9 m_pCenterTileIB;		//��������IB

		LPDIRECT3DINDEXBUFFER9 m_pLeftTopLTileIB;		 
		LPDIRECT3DINDEXBUFFER9 m_pLeftBottomLTileIB;		 
		LPDIRECT3DINDEXBUFFER9 m_pRightTopLTileIB;		 
		LPDIRECT3DINDEXBUFFER9 m_pRightBottomLTileIB;		 

		LPDIRECT3DINDEXBUFFER9 m_pFixedRingIB;

		Math::Vec3 m_v3LocalPos;
		unsigned int m_uiLevelCurrFlag;     //����Ӧ�㼶�ı�־��������ʶĳ���Ƿ�������ʾ��

		unsigned int m_uiVertNum;

		unsigned int m_uiIndiceNumPerTile;
		unsigned int m_uiPrimtivePerTile;

		unsigned int m_uiGapTileIndiceNum;
		unsigned int m_uiGapTilePrimitiveNum;

		unsigned int m_uiCenterTileIndiceNum;
		unsigned int m_uiCenterTilePrimitiveNum;

		unsigned int m_uiIndiceNumPerLTile;
		unsigned int m_uiPrimitivePerLTile;

		unsigned int m_uiFixedRingVertNum;
		unsigned int m_uiFixedRingIndicesNum;
		unsigned int m_uiFixedRingPrimitiveNum;

		Render::D3DRenderOperation* m_tileRenderOps[12];
		Render::D3DRenderOperation* m_pGapTileOp;
		Render::D3DRenderOperation* m_pCenterTileOp;

		Render::D3DRenderOperation* m_pLeftTopLTileOp;
		Render::D3DRenderOperation* m_pLeftBottomLTileOp;
		Render::D3DRenderOperation* m_pRightTopLTileOp;
		Render::D3DRenderOperation* m_pRightBottomLTileOp;

		Render::D3DRenderOperation* m_pFixedRingOp;
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