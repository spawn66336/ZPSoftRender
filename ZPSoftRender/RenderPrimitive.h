#ifndef ZP_RENDERPRIMITIVE
#define ZP_RENDERPRIMITIVE

#include "VertexBuffer.h"

namespace Render
{
	enum PRIMITIVE_TYPE
	{
		TYPE_LINES,
		TYPE_TRIANGLES
	};

	class RenderPrimitive
	{
	public:

		typedef std::vector<unsigned int> indicesBuf_t;

	public:

		RenderPrimitive(void);
		RenderPrimitive( const RenderPrimitive& primitive );
		~RenderPrimitive(void);

		VertexBuffer& VertexBuf(void){ return m_vertexBuf; }

		const VertexBuffer& VertexBuf(void) const { return m_vertexBuf; }

		PRIMITIVE_TYPE PrimitiveType(void) const { return m_primType; }

		void PrimitiveType( const PRIMITIVE_TYPE type ){ m_primType = type; }
		 
		void Clear(void); 

		unsigned int* Indices(void);

		int IndicesCount(void) const;

		void ClearIndices(void);

		void AppendIndex( unsigned int index );

		RenderPrimitive& operator=( const RenderPrimitive& rhs );

	protected: 
		PRIMITIVE_TYPE m_primType;   ///>图元类型
		VertexBuffer m_vertexBuf;		///>顶点缓冲区 
		indicesBuf_t m_indices;				///>索引缓冲区
	};

}//namespace Render

#endif //ZP_RENDERPRIMITIVE