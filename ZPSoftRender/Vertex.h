#ifndef ZP_VERTEX
#define ZP_VERTEX

#include "ZPDependency.h"
#include "ZPMathDependency.h"

namespace Render
{

		class Vertex
		{
		public:
			Vertex(void);
			Vertex( const Vertex& v );
			~Vertex(void);

			Vertex& operator=( const Vertex& rhs );

			Math::Vec3 m_pos;
			Math::Vec3 m_norm;
			Math::Vec2 m_texcoord;
			Math::Vec3 m_tangent;
			Math::Vec3 m_binormal;
		};
		 
}//namespace Render

#endif // ZP_VERTEX