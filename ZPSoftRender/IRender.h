#ifndef ZP_IRENDER
#define ZP_IRENDER

#include "ZPDependency.h"
#include "RenderPrimitive.h"

class Camera;

namespace Math
{
	class Vec2;
	class Vec3;
	class Vec4;
	class Matrix4;
}

namespace Resource
{
	class Texture2D;
	class Material;
}

namespace Render
{ 
	
    class Light;

	 
	enum TEXMAP_CHANNEL
	{
		DIFFUSE_CH = 0,
		BUMPMAP_CH = 1
	};

	enum CLEAR_BUFFER_FLAG
	{
		COLOR_BUFFER_FLAG = 1,
		DEPTH_BUFFER_FLAG = 2
	};

	enum SHADE_MODEL
	{
		WIREFRAME_MODEL = 0,
		FLAT_MODEL,
		GOURAUD_MODEL,
		PHONG_MODEL,
		NORMMAP_MODEL
	};
	 
class IRender
{
public: 

	IRender(void){}
	virtual ~IRender(void){}

	/**
	* @brief 初始化渲染器
	* @param hwnd 窗体句柄
	*/
	virtual void Init( const winHandle_t hwnd ) = 0;

	 

	/**
	* @brief 销毁渲染器对渲染器申请的资源进行清除
	* @remark 需要在窗体销毁前调用
	*/
	virtual void Destroy() = 0;

	/**
	* @brief 根据窗体大小调整后台缓冲区大小
	* @remark 此函数需要在每次窗体大小有变化时调用,
	*				本函数会自动获取窗体的大小来更新
	*/
	virtual void Resize() = 0;

	/**
	* @brief 开始绘制
	* @param pCam 相机指针
	* @remark 在每一帧绘制实际物体前都需要调用此函数，该函数会
	*				  准备好绘制上下文。
	*/
	virtual void BeginDraw( Camera* pCam ) = 0;

	/**
	* @brief 结束绘制 
	* @remark 在每一帧绘制实际物体后都需要调用此函数
	*/
	virtual void EndDraw( void ) = 0;

	/**
	* @brief 交换缓冲区
	* @remark 将后台缓冲区拷贝到前台显示缓冲区
	*/
	virtual void SwapBuffers( void ) =0;

	/** 
	 * @brief 设置颜色缓冲区清除的颜色
	 * @param color 要清除的颜色
	 */
	virtual void SetClearColor( const Math::Vec4& color) = 0;

	/** 
	 * @brief 设置要清除的缓冲区
	 * @param flag
	 */
	virtual void ClearBuffer(unsigned int flag) = 0;

	/**
	* @brief 应用材质
	* @param pMaterial 指向材质的指针
	*/
	virtual void ApplyMaterial( Resource::Material* pMaterial ) = 0;

	virtual void UnapplyMaterial( Resource::Material* pMaterial ) {};

	/**
	* @brief 当前渲染设备是否可用
	* @remark 用来应对D3D设备丢失
	*/
	virtual bool IsActive( void ) { return true; }
	
	/**
	* @brief 绘制几何图源
	* @param renderPrimitive 待渲染图元
	*/
	virtual  void DrawElements( RenderPrimitive& renderPrimitive ) = 0;

	/**
	 * @brief 设置是否启用纹理
	 * @param enable
	 */
	virtual void EnableTexture2D( bool enable) = 0;

	/**
	 * @brief 设置是否启用光照
	 * @param enable
	 */
	virtual void EnableLight( bool enable ) = 0;
	 

	/**
	 * @brief 设置是否启用深度测试
	 * @param enable
	 */
	virtual void EnableDepthTest( bool enable ) = 0;

	/**
	 * @brief 设置着色模型
	 * @param type 着色模型类型
	 */
	virtual void SetShadingModel( SHADE_MODEL type ) = 0;

	/**
	 * @brief 创建光源
	 * @param name 光源名称
	 * @return 指向创建的光源对象指针
	 * @remark 调用者不可释放返回的光源对象，光源对象由渲染器管理
	 */
	virtual Light* CreateLight( const String& name ) = 0;

	/**
	 * @brief 通过名称删除光源对象
	 * @param name	待删除的光源对象名称
	 * @return 删除是否成功
	 */
	virtual bool DeleteLightByName( const String& name ) = 0;

	/**
	 * @brief 通过名称查找光源对象
	 * @param name 待查找光源对象名称
	 * @return 若有同名光源则返回光源对象，否则返回NULL
	 */
	virtual Light* FindLightByName( const String& name) = 0;

	/**
	* @brief 删除所有光源
	*/
	virtual void DeleteAllLights( void ) = 0;

	/**
	* @brief 设置投影矩阵
	* @param mat 待设置投影矩阵
	*/
	virtual void SetProjectionMatrix( const Math::Matrix4& mat ) = 0;

	/**
	* @brief 将当前模型视图矩阵压入栈中
	* @remark 此函数只影响模型视图矩阵
	*/
	virtual void PushMatrix() = 0;

	/**
	* @brief 将矩阵栈的顶部元素出栈，
	*			  并设置为当前模型视图矩阵
	* @remark 此函数只影响模型视图矩阵
	*/
	virtual void PopMatrix() = 0;

	/** 
	 * @brief 设置当前模型视图矩阵
	 * @param mat 待设置矩阵
	 * @remark  此函数只影响模型视图矩阵
	 */
	virtual void LoadMatrix( const Math::Matrix4 &mat) = 0;

	/**
	* @brief 将当前模型视图矩阵设置为单位矩阵
	* @remark 此函数只影响模型视图矩阵
	*/
	virtual void LoadIdentity() = 0;

	/**
	* @brief 将用户指定矩阵与当前模型视图矩阵相乘
	* @param mat 待相乘的矩阵
	* @remark 此函数只影响模型视图矩阵
	*/
	virtual void MultMatrix( const Math::Matrix4 &mat) = 0;

	/**
	* @brief 用来D3D渲染器捕获用户子模型信息
	*/
	virtual void* GetUserPointer( void ) const { return NULL; }
	virtual void SetUserPointer( void* p ) { }

};

}//namespace Render

#endif//ZP_IRENDER