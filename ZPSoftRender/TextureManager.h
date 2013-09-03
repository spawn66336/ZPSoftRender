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

		/**
		* @brief 创建或获取纹理
		* @param name 纹理名称（一般为纹理路径名）
		* @remark 此函数会先在纹理管理器中查找同名纹理，若存在则
		*				 返回，否则会尝试从所给路径中读取纹理，若读取成功
		*				 则返回创建的纹理对象。返回的对象不要删除由纹理管理
		*				器管理生命周期
		*/
		Texture2D* CreateOrRetrieveTexture2D( const String& name );

		/**
		* @brief 创建或获取纹理
		* @param name 纹理名称
		* @remark  此函数会先在纹理管理器中查找同名纹理，若存在则
		*				   返回，否则会尝试从默认路径中读取纹理，若读取成功
		*				   则返回创建的纹理对象。返回的对象不要删除由纹理管理器
		*					管理生命周期。
		*/
		Texture2D* CreateOrRetrieveTexture2DFromDefaultLocation( const String& name );

		/**
		* @brief 获取纹理
		* @param name 纹理名称
		* @remark  此函数会在纹理管理器中查找同名纹理，若存在则
		*				   返回，否则返回空。返回的对象不要删除由纹理管理器
		*					管理生命周期。
		*/
		Texture2D* RetrieveTexture2D( const String& name );

		/**
		* @brief 删除指定纹理
		* @param name 纹理名称
		* @remark  此函数会在纹理管理器中查找同名纹理，若存在则
		*				   删除并返回true，否则返回false。 
		*/
		bool DeleteTexture2DByName( const String& name );

		/**
		* @brief 删除所有二维纹理
		*/
		void DeleteAllTexture2D( void ); 

	protected:

		typedef std::map<String,Texture2D*> textureTable_t;

		String m_strDefaultLocation;
		ITextureLoader* m_pTextureLoader;
		textureTable_t m_textures;
	};

}//namespace Resource

#endif//ZP_TEXTUREMANAGER
