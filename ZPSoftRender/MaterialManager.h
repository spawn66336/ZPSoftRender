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

		Resource::Material* CreateOrRetrieveMaterial( const String& name );

		Resource::Material* RetrieveMaterial( const String& name );

		bool DeleteMaterialByName( const String& name );
		  
		void DeleteAllMaterial( void ); 

	protected: 
		typedef std::map<String,Resource::Material*> materialTable_t;

		materialTable_t m_materials; 
	};

}//namespace Resource

#endif//ZP_MATERIALMANAGER