#ifndef ZP_SUBMESH
#define ZP_SUBMESH

#include "ZPDependency.h"

namespace Math
{
	class Vec2;
	class Vec3;
	class Vec4;
	class Matrix4;
}

namespace Resource
{ 
	class Material;

	class ZPEXPORT SubMesh
	{ 
	
	public:

		SubMesh(void);
		virtual ~SubMesh(void);

		String Name( void ) const{ return m_name; }

		void SetName( const String& name){ m_name = name; }

		Material* Material( void );

		void SetMaterial( Material* mat );

		 Math::Vec3* Vertices( void );

		 Math::Vec2* Texcoords( void );

		 Math::Vec3* Normals( void );

		 Math::Vec3* Tangents( void );

		 Math::Vec3* Binormals( void );

		 void* Indices( void );

		 void SetVertices( Math::Vec3* vertices, int num );

		 void SetNormals( Math::Vec3* norms, int num );

		 void SetTexcoords( Math::Vec2* texcoords, int num );

		 void SetTangents( Math::Vec3* tangents, int num);

		 void SetBinormals( Math::Vec3* binormals, int num );

		 void SetIndices( int* indices, int num);
		  
		  
		int VertexCount( void ) const ;

		int NormalCount( void ) const;

		int TexcoordCount( void ) const;

		int BinormalCount( void ) const;

		int TangentCount( void ) const;

		int IndicesCount( void ) const;

		int PrimitiveType( void ) const; 

	protected:

		String m_name;			///>子模型名
		Material* m_pMaterial; ///>该子模型的材质

		 Math::Vec3* m_pVertices; ///>顶点位置列表
		 int	m_iVerticesCount;		  ///>顶点数

	};

}//namespace Resrouce

#endif//ZP_SUBMESH