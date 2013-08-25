#include "ColladaMeshLoader.h"
#include "Mesh.h"
#include "ColladaMeshReader.h"

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
		
		ColladaMeshReader colladaMeshReader( path , pMesh );
		if( false == colladaMeshReader.Load() )
		{ 
			ZP_SAFE_DELETE( pMesh );
		} 
		return pMesh;
	}


}//namespace Resource