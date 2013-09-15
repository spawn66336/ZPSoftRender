#include "Vertex.h"

namespace Render
{


Vertex::Vertex( const Vertex& v )
{ 
	this->operator=( v );
}

Vertex::Vertex( void )
{

}


Vertex::~Vertex(void)
{
}

Vertex& Vertex::operator=( const Vertex& rhs )
{
	m_pos = rhs.m_pos;
	m_norm = rhs.m_norm;
	m_texcoord = rhs.m_texcoord;
	m_tangent = rhs.m_tangent;
	m_binormal = rhs.m_binormal;
	return *this;
}


}//namespace Render