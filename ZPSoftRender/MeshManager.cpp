#include "MeshManager.h"
#include "ColladaMeshLoader.h"
#include "Mesh.h"
#include "ZPMathDependency.h"

namespace Resource
{
	 
	MeshManager::MeshManager(void)
	{
		m_pLoader = new ColladaMeshLoader;
		SetFirstMeshActive();
		m_strDefaultLocation = ".\\media\\models\\";
	}

	MeshManager::~MeshManager(void)
	{
		ZP_SAFE_DELETE( m_pLoader );
		this->DeleteAllMesh();
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
			SetFirstMeshActive();
		}
		return pMesh;
	}


	Mesh* MeshManager::CreateOrRetrieveMeshFromDefaultLocation( const String& name )
	{
		return CreateOrRetrieveMesh( m_strDefaultLocation + name );
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

		SetFirstMeshActive();
		return true;
	}

	bool MeshManager::DeleteMesh( Mesh* pMesh )
	{
		meshTable_t::iterator itFindMesh = m_meshes.find( pMesh->Name() );
		if( itFindMesh == m_meshes.end() )
		{
			return false;
		} 

		ZP_ASSERT( itFindMesh->second == pMesh );
		ZP_SAFE_DELETE( itFindMesh->second );
		m_meshes.erase( itFindMesh );

		SetFirstMeshActive();
		return true;
	}

	void MeshManager::DeleteAllMesh( void )
	{
		meshTable_t::iterator itMesh = m_meshes.begin();
		while( itMesh != m_meshes.end() )
		{
			ZP_SAFE_DELETE( itMesh->second );
			itMesh++;
		}
		m_meshes.clear();

		SetFirstMeshActive();
	}

	void MeshManager::DrawActiveMesh( Render::IRender* pRenderer, const Math::Matrix4& localToWorldMat )
	{
		if( m_itActiveMesh != m_meshes.end() )
		{
			Mesh* pMesh = m_itActiveMesh->second;
			ZP_ASSERT( NULL != pMesh );
			pMesh->Draw( pRenderer , localToWorldMat );
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
			if( m_itActiveMesh == m_meshes.end() )
			{
				m_itActiveMesh = m_meshes.begin();
			}
		}
	}


}//namespace Resource