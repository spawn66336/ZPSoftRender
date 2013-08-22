#include "MeshManager.h"
#include "ColladaMeshLoader.h"
#include "Mesh.h"
#include "ZPMathDependency.h"

namespace Resource
{
	 
	MeshManager::MeshManager(void)
	{
		m_pLoader = new ColladaMeshLoader;
	}


	MeshManager::~MeshManager(void)
	{
		ZP_SAFE_DELETE( m_pLoader );
	}

	Mesh* MeshManager::CreateOrRetrieveMesh( const String& path )
	{
		Mesh* pMesh = RetrieveMesh( path );
		if( NULL != pMesh )
		{
			return pMesh;
		}
		pMesh = m_pLoader->Load( path );

		if( pMesh )
		{
			m_meshes.insert( std::make_pair( path , pMesh ) );
			m_itActiveMesh = m_meshes.begin();
		}
		return pMesh;
	}

	Mesh* MeshManager::RetrieveMesh( const String& name )
	{
		meshTable_t::iterator itFindMesh = m_meshes.find( name );
		if( itFindMesh == m_meshes.end() )
		{
			return NULL;
		}
		return itFindMesh->second;
	}

	bool MeshManager::DeleteMeshByName( const String& name )
	{
		meshTable_t::iterator itFindMesh = m_meshes.find( name );
		if( itFindMesh == m_meshes.end() )
		{
			return false;
		} 

		ZP_SAFE_DELETE( itFindMesh->second );
		m_meshes.erase( itFindMesh );

		m_itActiveMesh = m_meshes.begin();
		return true;
	}

	bool MeshManager::DeleteMesh( Mesh* mesh )
	{

	}

	void MeshManager::DeleteAllMesh( void )
	{

	}

	void MeshManager::DrawActiveMesh( Render::IRender* render, const Math::Matrix4& localToWorldMat )
	{
		if( m_itActiveMesh != m_meshes.end() )
		{
			
		}
	}

	void MeshManager::SetFirstMeshActive( void )
	{
		m_itActiveMesh = m_meshes.begin();
	}

	void MeshManager::SetNextMeshActive( void )
	{
		if( m_itActiveMesh == m_meshes.end() )
		{
			m_itActiveMesh = m_meshes.begin();
		}else{
			m_itActiveMesh++;
		}
	}


}//namespace Resource