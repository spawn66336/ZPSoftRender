#ifndef ZP_VERTEXBUFFER
#define ZP_VERTEXBUFFER

#include "Vertex.h"

namespace Render
{

	enum VERTEX_CHANNEL
	{
		POSITION_CH = 1,
		NORMAL_CH = 2,
		TEXCOORD_CH = 4,
		TANGENT_CH = 8,
		BINORMAL_CH = 16
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(void);
		VertexBuffer( const VertexBuffer& buf );
		~VertexBuffer(void);

		unsigned int& ChannelFlag(void){ return m_uiChannelFlag; }

		const unsigned int& ChannelFlag(void) const { return m_uiChannelFlag; }

		Math::Vec3* PositionPointer( void );

		Math::Vec3* NormalPointer( void );

		Math::Vec2* TexcoordPointer( void );

		Math::Vec3* TangentPointer( void );

		Math::Vec3* BinormalPointer( void );

		Vertex* Pointer(void);

		int Count(void) const; 

		void Clear(void);

		int Stride(void) const;

		void AppendVertex( const Vertex& v );

		VertexBuffer& operator=( const VertexBuffer& rhs );

	protected:
		typedef std::vector<Vertex> vertexBuffer_t;

		unsigned int m_uiChannelFlag;
		vertexBuffer_t m_vBuf; 
	};

}//namespace Render

#endif //ZP_VERTEXBUFFER