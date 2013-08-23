#include "VertexBuffer.h"

namespace Render
{ 

	VertexBuffer::VertexBuffer(void):
	m_uiChannelFlag(0)
	{
	}

	VertexBuffer::VertexBuffer( const VertexBuffer& buf ):
	m_uiChannelFlag(0)
	{
		 this->operator=( buf );
	} 

	VertexBuffer::~VertexBuffer(void)
	{
	}

	Vertex* VertexBuffer::Pointer( void ) 
	{
		return m_vBuf.data();
	}

	int VertexBuffer::Count( void ) const
	{
		return m_vBuf.size();
	}

	VertexBuffer& VertexBuffer::operator=( const VertexBuffer& rhs )
	{
		m_uiChannelFlag = rhs.ChannelFlag();
		m_vBuf = rhs.m_vBuf;
		return *this;
	}

	void VertexBuffer::Clear( void )
	{
		m_uiChannelFlag = 0;
		m_vBuf.clear();
	}

	int VertexBuffer::Stride( void ) const
	{
		return sizeof(Vertex);
	}

	Math::Vec3* VertexBuffer::PositionPointer( void )
	{
		return (&m_vBuf.data()[0].m_pos);
	}

	Math::Vec3* VertexBuffer::NormalPointer( void )
	{
		return (&m_vBuf.data()[0].m_norm);
	}

	Math::Vec2* VertexBuffer::TexcoordPointer( void )
	{
		return (&m_vBuf.data()[0].m_texcoord);
	}

	Math::Vec3* VertexBuffer::TangentPointer( void )
	{
		return (&m_vBuf.data()[0].m_tangent);
	}

	Math::Vec3* VertexBuffer::BinormalPointer( void )
	{
		return (&m_vBuf.data()[0].m_binormal);
	}

	void VertexBuffer::AppendVertex( const Vertex& v )
	{
		m_vBuf.push_back( v );
	}

}//namespace Render