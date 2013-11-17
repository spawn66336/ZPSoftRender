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
		Math::Matrix4						   m_worldMat;		//�������
		unsigned int							   m_streamIndex; //������
		IDirect3DVertexDeclaration9* m_pVertexDecl; //���㶨��
		LPDIRECT3DVERTEXBUFFER9  m_pVB;				//�������ö��㻺����
		LPDIRECT3DINDEXBUFFER9   m_pIB;				//������������������
		D3DPRIMITIVETYPE				  m_primitiveType; //�������ü���������
		unsigned int							  m_stride;				//����Ŀ��
		unsigned int							  m_vertexCount;	//��������
		unsigned int							  m_primCount;		//ͼԪ��
	public:
		D3DRenderOperation():
		  m_streamIndex(0),
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