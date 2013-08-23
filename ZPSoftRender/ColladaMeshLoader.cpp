#include "ColladaMeshLoader.h"
#include "Mesh.h"

namespace Resource
{ 

	ColladaMeshLoader::ColladaMeshLoader(void)
	{
	}

	ColladaMeshLoader::~ColladaMeshLoader(void)
	{
	}

	Mesh* ColladaMeshLoader::Load( const String& path )
	{
		Mesh* pMesh = new Mesh;
		pMesh->SetName( path );
		return pMesh;
	}


}//namespace Resource