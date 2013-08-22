#ifndef ZP_MESHMANAGER
#define ZP_MESHMANAGER

#include "ZPDependency.h"

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
	 
	class ZPEXPORT MeshManager
	{
	public:
		MeshManager(void);
		virtual ~MeshManager(void);

		Mesh* CreateOrRetrieveMesh( const String& path );
			 
		Mesh* RetrieveMesh( const String& name );
		 
	    bool DeleteMeshByName( const String& name );
		 
		bool DeleteMesh( Mesh* mesh );

	    void DeleteAllMesh( void ); 

	    void DrawActiveMesh( Render::IRender* render, const Math::Matrix4& localToWorldMat);

	    void SetFirstMeshActive( void );

	    void SetNextMeshActive( void );

	protected:

		typedef std::map<String,Mesh*> meshTable_t;

		meshTable_t m_meshes;						///>ģ���б�
		meshTable_t::iterator m_itActiveMesh;  ///>��ǰ����ʾ��ģ�͵�����
		IMeshLoader* m_pLoader;					///>ģ�Ͷ�ȡ��

	}; 

}//namespace Resource

#endif//ZP_MESHMANAGER