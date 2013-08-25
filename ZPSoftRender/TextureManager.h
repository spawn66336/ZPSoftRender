#ifndef ZP_TEXTUREMANAGER
#define ZP_TEXTUREMANAGER

#include "ZPDependency.h"
#include "Singleton.h"

namespace Resource
{ 
	class ITextureLoader;
	class Texture2D;

	class  TextureManager : public Util::SingletonT<TextureManager>
	{
	public:
		TextureManager(void);
		virtual ~TextureManager(void);

		Texture2D* CreateOrRetrieveTexture2D( const String& name );

		Texture2D* CreateOrRetrieveTexture2DFromDefaultLocation( const String& name );

		Texture2D* RetrieveTexture2D( const String& name );

		bool DeleteTexture2DByName( const String& name );

		void DeleteAllTexture2D( void ); 

	protected:

		typedef std::map<String,Texture2D*> textureTable_t;

		String m_strDefaultLocation;
		ITextureLoader* m_pTextureLoader;
		textureTable_t m_textures;
	};

}//namespace Resource

#endif//ZP_TEXTUREMANAGER
