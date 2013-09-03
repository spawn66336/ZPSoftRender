#ifndef ZP_MATERIALMANAGER
#define ZP_MATERIALMANAGER

#include "ZPDependency.h"
#include "Singleton.h"

namespace Resource
{
	class Material;

	class  MaterialManager : public Util::SingletonT<MaterialManager>
	{
	public:
		MaterialManager(void);
		virtual ~MaterialManager(void); 

		/**
		* @brief 创建或获取材质
		* @param name 材质名称
		* @remark 此函数会先在材质管理器中查找同名材质
		*				  若存在则返回，否则会创建材质
		*/
		Resource::Material* CreateOrRetrieveMaterial( const String& name );

		/**
		* @brief 获取材质
		* @param name 材质名称
		* @remark 此函数会先在材质管理器中查找同名材质
		*				  若存在则返回，否则返回空
		*/
		Resource::Material* RetrieveMaterial( const String& name );

		/**
		* @brief 删除同名材质
		* @param name 材质名称
		* @remark 此函数会先在材质管理器中查找同名材质
		*				  若存在则删除并返回true，否则删除失败
		*				  返回false
		*/
		bool DeleteMaterialByName( const String& name );
		  
		/**
		* @brief 删除所有材质  
		*/
		void DeleteAllMaterial( void ); 

	protected: 
		typedef std::map<String,Resource::Material*> materialTable_t;

		materialTable_t m_materials;		///>材质列表
	};

}//namespace Resource

#endif//ZP_MATERIALMANAGER