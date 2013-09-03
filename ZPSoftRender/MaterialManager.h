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
		* @brief �������ȡ����
		* @param name ��������
		* @remark �˺��������ڲ��ʹ������в���ͬ������
		*				  �������򷵻أ�����ᴴ������
		*/
		Resource::Material* CreateOrRetrieveMaterial( const String& name );

		/**
		* @brief ��ȡ����
		* @param name ��������
		* @remark �˺��������ڲ��ʹ������в���ͬ������
		*				  �������򷵻أ����򷵻ؿ�
		*/
		Resource::Material* RetrieveMaterial( const String& name );

		/**
		* @brief ɾ��ͬ������
		* @param name ��������
		* @remark �˺��������ڲ��ʹ������в���ͬ������
		*				  ��������ɾ��������true������ɾ��ʧ��
		*				  ����false
		*/
		bool DeleteMaterialByName( const String& name );
		  
		/**
		* @brief ɾ�����в���  
		*/
		void DeleteAllMaterial( void ); 

	protected: 
		typedef std::map<String,Resource::Material*> materialTable_t;

		materialTable_t m_materials;		///>�����б�
	};

}//namespace Resource

#endif//ZP_MATERIALMANAGER