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
		* @brief 创建或获取模型
		* @param name 模型名称（一般为路径名）
		* @remark 此函数会先在模型管理器中查找同名模型
		*				  若存在则返回，否则会尝试从传入的路径中
		*				  读取若读取成功则返回否则返回为空
		*/
		Mesh* CreateOrRetrieveMesh( const String& name );

		/**
		* @brief 创建或获取模型
		* @param name 模型名称
		* @remark 此函数会先在模型管理器中查找同名模型
		*				  若存在则返回，否则会尝试从默认路径载
		*				  入名称为name的模型，若载入成功则返回模型对象
		*				  否则返回为空
		*/
		Mesh* CreateOrRetrieveMeshFromDefaultLocation( const String& name );
		
		/**
		* @brief 查找模型
		* @param name 模型名称
		* @remark 此函数会在模型管理器中查找同名模型
		*				  若存在则返回，否则返回空
		*/
		Mesh* RetrieveMesh( const String& name );
		 
		/**
		* @brief 删除模型
		* @param name 模型名称
		* @remark 此函数会在模型管理器中查找同名模型
		*				  若存在则删除并返回true,否则返回false
		*/
	    bool DeleteMeshByName( const String& name );
		 
		/**
		* @brief 删除模型
		* @param pMesh 待删除的模型对象
		* @remark 此函数会在模型管理器中查找同名模型
		*				  若存在则删除并返回true,否则返回false
		*/
		bool DeleteMesh( Mesh* pMesh );
		
		/**
		* @brief 删除所有模型
		*/
	    void DeleteAllMesh( void ); 

	public: //模型绘制函数

		/**
		* @brief 绘制当前模型
		* @param pRenderer 渲染所用渲染器
		* @param localToWorldMat 本地坐标系到世界坐标系变换矩阵
		*/
	    void DrawActiveMesh( Render::IRender* pRenderer, const Math::Matrix4& localToWorldMat);

		/**
		* @brief 设置管理器中第一个模型为当前模型 
		*/
	    void SetFirstMeshActive( void );

		/**
		* @brief 切换到下一个模型
		*/
	    void SetNextMeshActive( void );

	protected:

		typedef std::map<String,Mesh*> meshTable_t;

		String m_strDefaultLocation;				///>模型的默认读取位置
		meshTable_t m_meshes;						///>模型列表
		meshTable_t::iterator m_itActiveMesh;  ///>当前所显示的模型迭代器
		IMeshLoader* m_pLoader;					///>模型读取器

	}; 

}//namespace Resource

#endif//ZP_MESHMANAGER