#ifndef ZP_MESHMANAGER
#define ZP_MESHMANAGER

#include "ZPDependency.h"
#include "Singleton.h"

namespace Render
{
	class IRender;
}

namespace Math
{
	class Matrix4;
}

namespace Resource
{
	class Mesh;
	class IMeshLoader;
	 
	class  MeshManager : public Util::SingletonT<MeshManager>
	{
	public:
		MeshManager(void);
		virtual ~MeshManager(void);

		/**
		* @brief �������ȡģ��
		* @param name ģ�����ƣ�һ��Ϊ·������
		* @remark �˺���������ģ�͹������в���ͬ��ģ��
		*				  �������򷵻أ�����᳢�ԴӴ����·����
		*				  ��ȡ����ȡ�ɹ��򷵻ط��򷵻�Ϊ��
		*/
		Mesh* CreateOrRetrieveMesh( const String& name );

		/**
		* @brief �������ȡģ��
		* @param name ģ������
		* @remark �˺���������ģ�͹������в���ͬ��ģ��
		*				  �������򷵻أ�����᳢�Դ�Ĭ��·����
		*				  ������Ϊname��ģ�ͣ�������ɹ��򷵻�ģ�Ͷ���
		*				  ���򷵻�Ϊ��
		*/
		Mesh* CreateOrRetrieveMeshFromDefaultLocation( const String& name );
		
		/**
		* @brief ����ģ��
		* @param name ģ������
		* @remark �˺�������ģ�͹������в���ͬ��ģ��
		*				  �������򷵻أ����򷵻ؿ�
		*/
		Mesh* RetrieveMesh( const String& name );
		 
		/**
		* @brief ɾ��ģ��
		* @param name ģ������
		* @remark �˺�������ģ�͹������в���ͬ��ģ��
		*				  ��������ɾ��������true,���򷵻�false
		*/
	    bool DeleteMeshByName( const String& name );
		 
		/**
		* @brief ɾ��ģ��
		* @param pMesh ��ɾ����ģ�Ͷ���
		* @remark �˺�������ģ�͹������в���ͬ��ģ��
		*				  ��������ɾ��������true,���򷵻�false
		*/
		bool DeleteMesh( Mesh* pMesh );
		
		/**
		* @brief ɾ������ģ��
		*/
	    void DeleteAllMesh( void ); 

	public: //ģ�ͻ��ƺ���

		/**
		* @brief ���Ƶ�ǰģ��
		* @param pRenderer ��Ⱦ������Ⱦ��
		* @param localToWorldMat ��������ϵ����������ϵ�任����
		*/
	    void DrawActiveMesh( Render::IRender* pRenderer, const Math::Matrix4& localToWorldMat);

		/**
		* @brief ���ù������е�һ��ģ��Ϊ��ǰģ�� 
		*/
	    void SetFirstMeshActive( void );

		/**
		* @brief �л�����һ��ģ��
		*/
	    void SetNextMeshActive( void );

	protected:

		typedef std::map<String,Mesh*> meshTable_t;

		String m_strDefaultLocation;				///>ģ�͵�Ĭ�϶�ȡλ��
		meshTable_t m_meshes;						///>ģ���б�
		meshTable_t::iterator m_itActiveMesh;  ///>��ǰ����ʾ��ģ�͵�����
		IMeshLoader* m_pLoader;					///>ģ�Ͷ�ȡ��

	}; 

}//namespace Resource

#endif//ZP_MESHMANAGER