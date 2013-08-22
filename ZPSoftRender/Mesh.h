#ifndef ZP_MESH
#define ZP_MESH

#include "ZPDependency.h"

namespace Render
{
	class IRender;
}

namespace Math
{
	class Matrix4;
}

namespace Resource
{
	class SubMesh;
	 
	class ZPEXPORT Mesh
	{
	public:
		Mesh(void);
		virtual ~Mesh(void);

		String Name( void ) const{ return m_name; }

		void SetName( const String& name ){ m_name = name; }
		  
		 void Draw( Render::IRender* render, const Math::Matrix4& localToWorldMat );

		 int SubMeshCount( void ) const;

		 void AddSubMesh( SubMesh* submesh );
		  
		 bool DeleteSubMeshByName( const String& name );
		  
		 bool DeleteSubMesh( SubMesh* submesh );

	protected:

		typedef std::map<String,SubMesh*> subMeshTable_t;

		String m_name;							///>模型名称
		subMeshTable_t m_submeshTab; ///>子模型列表
	};

}//namespace Resource

#endif//ZP_MESH