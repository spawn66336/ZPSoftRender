#include "TextureManager.h"
#include "ITextureLoader.h"
#include "FreeImageTextureLoader.h"
#include "Texture2D.h"

namespace Resource
{


TextureManager::TextureManager(void):
m_pTextureLoader(NULL)
{
	FreeImageTextureLoader::InitFreeImageLib();
	m_pTextureLoader = new FreeImageTextureLoader;
}


TextureManager::~TextureManager(void)
{
	this->DeleteAllTexture2D(); 
	ZP_SAFE_DELETE( m_pTextureLoader );
	FreeImageTextureLoader::DestroyFreeImageLib();
}

Texture2D* TextureManager::CreateOrRetrieveTexture2D( const String& name )
{
	Texture2D* pTex2D = RetrieveTexture2D( name );

	if( NULL != pTex2D )
	{
		return pTex2D;
	}

	pTex2D = m_pTextureLoader->Load( name );

	if( NULL != pTex2D )
	{
		m_textures.insert( std::make_pair( name , pTex2D ) );
	}
	return pTex2D;
}

Texture2D* TextureManager::RetrieveTexture2D( const String& name )
{
	textureTable_t::iterator itFindTex2D = m_textures.find( name );

	if( itFindTex2D == m_textures.end() )
	{
		return NULL;
	} 

	return itFindTex2D->second;
}

bool TextureManager::DeleteTexture2DByName( const String& name )
{
	textureTable_t::iterator itDelTex2D = m_textures.find( name );

	if( itDelTex2D == m_textures.end() )
	{
		return false;
	} 

	ZP_ASSERT( NULL != itDelTex2D->second );
	ZP_SAFE_DELETE( itDelTex2D->second );
	m_textures.erase( itDelTex2D );
	return true;
}

void TextureManager::DeleteAllTexture2D( void )
{
	textureTable_t::iterator itDelTex2D = m_textures.begin();

	while( itDelTex2D != m_textures.end() )
	{
		ZP_SAFE_DELETE( itDelTex2D->second );
		itDelTex2D++;
	}
	m_textures.clear();
}


}//namespace Resource