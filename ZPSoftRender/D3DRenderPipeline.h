#ifndef ZP_D3DRENDERPIPELINE
#define ZP_D3DRENDERPIPELINE

#include "ZPDependency.h"
#include "d3d9.h"
#include "d3dx9effect.h"

namespace Resource
{
	class Material;
}

namespace Render
{

	class D3DRenderOperation;

	class MaterialGroup
	{
	public: 
		MaterialGroup():m_pMaterial(0){}
		Resource::Material* m_pMaterial;
		std::vector<D3DRenderOperation*>	 m_OpList;
	};

	class D3DRenderPipeline
	{
	public:
		D3DRenderPipeline(void);
		virtual ~D3DRenderPipeline(void);

		void Init( IDirect3DDevice9* pDevice , ID3DXEffectPool* pPool );
		void Destroy( void );
		void PushRenderOp( Resource::Material* pMat , D3DRenderOperation* pOp );
		void Render( void );
		void Reset( void );
		void OnLostDevice( void );
		void OnResetDevice( void );

	private:
		
		LPDIRECT3DDEVICE9 m_pDevice; 
		ID3DXEffectPool* m_pEffectPool;
		LPD3DXEFFECT m_pEffect;
		typedef std::map<Resource::Material* , MaterialGroup > renderGroup_t;
		renderGroup_t m_renderGroup;
	};

}//namespace Render

#endif