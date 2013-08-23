#include "RenderPrimitive.h"

namespace Render
{

	RenderPrimitive::RenderPrimitive(void)
	{
	}

	RenderPrimitive::RenderPrimitive( const RenderPrimitive& primitive )
	{
		this->operator =( primitive );
	}


	RenderPrimitive::~RenderPrimitive(void)
	{
	}

	RenderPrimitive& RenderPrimitive::operator=( const RenderPrimitive& rhs )
	{
		m_primType = rhs.m_primType;
		m_vertexBuf = rhs.m_vertexBuf;
		m_indices = rhs.m_indices;
		return *this;
	}

	int RenderPrimitive::IndicesCount( void ) const
	{
		return m_indices.size();
	}

	void RenderPrimitive::Clear( void )
	{
		m_primType = TYPE_LINES;
		m_vertexBuf.Clear();
		m_indices.clear();
	}

	unsigned int* RenderPrimitive::Indices( void )
	{
		return m_indices.data();
	}

	void RenderPrimitive::AppendIndex( unsigned int index )
	{
		m_indices.push_back( index );
	}

	void RenderPrimitive::ClearIndices( void )
	{
		m_indices.clear();
	}


}//namespace Render