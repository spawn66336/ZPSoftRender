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
		Math::Vec3 m_v3LocalPos;
		unsigned int m_uiVertNum;
		unsigned int m_uiIndiceNumPerTile;
		unsigned int m_uiPrimtivePerTile;
		Render::D3DRenderOperation* m_tileRenderOps[12];
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