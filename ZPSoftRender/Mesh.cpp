#include "Mesh.h"

namespace Resource
{ 

	Mesh::Mesh(void)
	{
	}

	Mesh::~Mesh(void)
	{
	}

	int Mesh::SubMeshCount( void ) const
	{
		return m_submeshTab.size();
	}

	void Mesh::AddSubMesh( SubMesh* submesh )
	{

	}

	bool Mesh::DeleteSubMeshByName( const String& name )
	{

	}

	bool Mesh::DeleteSubMesh( SubMesh* submesh )
	{

	}

	void Mesh::Draw( Render::IRender* render, const Math::Matrix4& localToWorldMat )
	{

	}

}//namespace Resource