#include "MaterialManager.h"
#include "Material.h"

namespace Resource
{


MaterialManager::MaterialManager(void)
{
}


MaterialManager::~MaterialManager(void)
{
	this->DeleteAllMaterial();
}

Resource::Material* MaterialManager::CreateOrRetrieveMaterial( const String& name )
{
	Resource::Material* pMaterial = RetrieveMaterial( name );
	if( NULL != pMaterial )
	{
		return pMaterial;
	}

	pMaterial = new Resource::Material;
	ZP_ASSERT( NULL != pMaterial ); 
	m_materials.insert( std::make_pair( name , pMaterial ) ); 
	return pMaterial;
}

Resource::Material* MaterialManager::RetrieveMaterial( const String& name )
{
	materialTable_t::iterator itFindMaterial = m_materials.find( name );
	if( itFindMaterial == m_materials.end() )
	{
		return NULL;
	}

	ZP_ASSERT( NULL != itFindMaterial->second );
	return itFindMaterial->second;
}

bool MaterialManager::DeleteMaterialByName( const String& name )
{
	materialTable_t::iterator itFindMaterial = m_materials.find( name );
	if( itFindMaterial == m_materials.end() )
	{
		return false;
	}

	ZP_ASSERT( NULL != itFindMaterial->second );
	ZP_SAFE_DELETE( itFindMaterial->second );
	m_materials.erase( itFindMaterial );
	return true;
}

void MaterialManager::DeleteAllMaterial( void )
{
	materialTable_t::iterator itMaterial = m_materials.begin();
	while( itMaterial != m_materials.end() )
	{
		ZP_SAFE_DELETE( itMaterial->second );
		itMaterial++;
	}
	m_materials.clear();
}

}//namespace Resource