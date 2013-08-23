#include "Mesh.h"
#include "SubMesh.h"
#include "IRender.h"

namespace Resource
{ 

	Mesh::Mesh(void)
	{
	}

	Mesh::~Mesh(void)
	{
		this->DeleteAllSubMeshes();
	}

	int Mesh::SubMeshCount( void ) const
	{
		return m_submeshTab.size();
	}

	bool Mesh::AddSubMesh( SubMesh* pSubMesh )
	{
		SubMesh* pFindSubMesh = FindSubMeshByName( pSubMesh->Name() );

		//���ʧ�� �Ѿ�����ͬ��������
		if( NULL != pFindSubMesh )
		{
			return false;
		}

		m_submeshTab.insert( std::make_pair( pSubMesh->Name() , pSubMesh ) );
		return true;
	} 

	SubMesh* Mesh::FindSubMeshByName( const String& name )
	{
		subMeshTable_t::iterator itFindSubMesh = m_submeshTab.find( name );
		if( itFindSubMesh == m_submeshTab.end() )
		{
			return NULL;
		}
		return itFindSubMesh->second;
	}

	bool Mesh::DeleteSubMeshByName( const String& name )
	{
		subMeshTable_t::iterator itDelSubMesh = m_submeshTab.find( name );

		//δ�ҵ���ɾ��������
		if( itDelSubMesh == m_submeshTab.end() )
		{
			return false;
		}

		ZP_SAFE_DELETE( itDelSubMesh->second );
		m_submeshTab.erase( itDelSubMesh );
		return true;
	}

	void Mesh::DeleteAllSubMeshes( void )
	{
		subMeshTable_t::iterator itSubMesh = m_submeshTab.begin();
		while( itSubMesh != m_submeshTab.end() )
		{
			ZP_SAFE_DELETE( itSubMesh->second );
			itSubMesh++;
		}
		m_submeshTab.clear();
	}

	bool Mesh::DeleteSubMesh( SubMesh* pSubMesh )
	{
		subMeshTable_t::iterator itFindSubMesh = m_submeshTab.find( pSubMesh->Name() );
		if( itFindSubMesh == m_submeshTab.end() )
		{
			return NULL;
		}

		ZP_ASSERT( itFindSubMesh->second == pSubMesh ); 
		ZP_SAFE_DELETE( itFindSubMesh->second );
		m_submeshTab.erase( itFindSubMesh );
		return true;
	}

	void Mesh::Draw( Render::IRender* pRenderer, const Math::Matrix4& localToWorldMat )
	{
		ZP_ASSERT( NULL != pRenderer ); 

		pRenderer->PushMatrix();
		pRenderer->MultMatrix( localToWorldMat );

		subMeshTable_t::iterator itSubMesh = m_submeshTab.begin();
		while( itSubMesh != m_submeshTab.end() )
		{
			SubMesh* pSubMesh = itSubMesh->second;
			ZP_ASSERT( NULL != pSubMesh );

			Resource::Material* pMaterial = pSubMesh->GetMaterial();

			if( NULL != pMaterial )
			{
				pRenderer->ApplyMaterial( pMaterial );	
			}
			
			pRenderer->PushMatrix(); 
			pRenderer->MultMatrix( pSubMesh->GetMatrix() ); 

				pRenderer->DrawElements( pSubMesh->RenderData() );

			pRenderer->PopMatrix();
			itSubMesh++;
		}

		pRenderer->PopMatrix();
	}




}//namespace Resource