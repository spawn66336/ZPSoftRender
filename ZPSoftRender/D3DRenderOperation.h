#ifndef ZP_D3DRENDEROPERATION
#define ZP_D3DRENDEROPERATION

#include "IRender.h"
#include <d3d9.h>
#include "Matrix4.h"

namespace Render
{
	class D3DRenderOperation
	{
	public:
		Math::Matrix4						   m_worldMat;		//世界矩阵
		IDirect3DVertexDeclaration9* m_pVertexDecl; //顶点定义
		LPDIRECT3DVERTEXBUFFER9  m_pVB;				//绘制所用顶点缓冲区
		LPDIRECT3DINDEXBUFFER9   m_pIB;				//绘制所用索引缓冲区
		D3DPRIMITIVETYPE				  m_primitiveType; //绘制所用几何体类型
		unsigned int							  m_stride;				//顶点的跨度
		unsigned int							  m_vertexCount;	//顶点数量
		unsigned int							  m_primCount;		//图元数
	public:
		D3DRenderOperation():
		  m_pVertexDecl(NULL),
		  m_pVB(NULL),
		  m_pIB(NULL),
		  m_primitiveType(D3DPT_POINTLIST),
		  m_stride(0),
		  m_vertexCount(0),
		  m_primCount(0)
		{}
		~D3DRenderOperation()
		{
			ZP_SAFE_RELEASE( m_pVertexDecl );
			ZP_SAFE_RELEASE( m_pVB );
			ZP_SAFE_RELEASE( m_pIB );
		}
	};

	
	class RenderOperationCache
	{
	public:
		RenderOperationCache();
		virtual ~RenderOperationCache();
		
		D3DRenderOperation* CreateRenderOperation( void* pMark );
		void Clear( void );

	protected:
		typedef std::map<void*,D3DRenderOperation*> renderOpTable_t;
		renderOpTable_t m_renderOpTab;
	};

 

}//namespace Render

#endif //ZP_D3DRENDEROPERATION