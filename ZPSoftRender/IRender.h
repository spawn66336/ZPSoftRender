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
	* @brief ��ʼ����Ⱦ��
	* @param hwnd ������
	*/
	virtual void Init( const winHandle_t hwnd ) = 0;

	 

	/**
	* @brief ������Ⱦ������Ⱦ���������Դ�������
	* @remark ��Ҫ�ڴ�������ǰ����
	*/
	virtual void Destroy() = 0;

	/**
	* @brief ���ݴ����С������̨��������С
	* @remark �˺�����Ҫ��ÿ�δ����С�б仯ʱ����,
	*				���������Զ���ȡ����Ĵ�С������
	*/
	virtual void Resize() = 0;

	/**
	* @brief ��ʼ����
	* @param pCam ���ָ��
	* @remark ��ÿһ֡����ʵ������ǰ����Ҫ���ô˺������ú�����
	*				  ׼���û��������ġ�
	*/
	virtual void BeginDraw( Camera* pCam ) = 0;

	/**
	* @brief �������� 
	* @remark ��ÿһ֡����ʵ���������Ҫ���ô˺���
	*/
	virtual void EndDraw( void ) = 0;

	/**
	* @brief ����������
	* @remark ����̨������������ǰ̨��ʾ������
	*/
	virtual void SwapBuffers( void ) =0;

	/** 
	 * @brief ������ɫ�������������ɫ
	 * @param color Ҫ�������ɫ
	 */
	virtual void SetClearColor( const Math::Vec4& color) = 0;

	/** 
	 * @brief ����Ҫ����Ļ�����
	 * @param flag
	 */
	virtual void ClearBuffer(unsigned int flag) = 0;

	/**
	* @brief Ӧ�ò���
	* @param pMaterial ָ����ʵ�ָ��
	*/
	virtual void ApplyMaterial( Resource::Material* pMaterial ) = 0;

	
	/**
	* @brief ���Ƽ���ͼԴ
	* @param renderPrimitive ����ȾͼԪ
	*/
	virtual  void DrawElements( RenderPrimitive& renderPrimitive ) = 0;

	/**
	 * @brief �����Ƿ���������
	 * @param enable
	 */
	virtual void EnableTexture2D( bool enable) = 0;

	/**
	 * @brief �����Ƿ����ù���
	 * @param enable
	 */
	virtual void EnableLight( bool enable ) = 0;
	 

	/**
	 * @brief �����Ƿ�������Ȳ���
	 * @param enable
	 */
	virtual void EnableDepthTest( bool enable ) = 0;

	/**
	 * @brief ������ɫģ��
	 * @param type ��ɫģ������
	 */
	virtual void SetShadingModel( SHADE_MODEL type ) = 0;

	/**
	 * @brief ������Դ
	 * @param name ��Դ����
	 * @return ָ�򴴽��Ĺ�Դ����ָ��
	 * @remark �����߲����ͷŷ��صĹ�Դ���󣬹�Դ��������Ⱦ������
	 */
	virtual Light* CreateLight( const String& name ) = 0;

	/**
	 * @brief ͨ������ɾ����Դ����
	 * @param name	��ɾ���Ĺ�Դ��������
	 * @return ɾ���Ƿ�ɹ�
	 */
	virtual bool DeleteLightByName( const String& name ) = 0;

	/**
	 * @brief ͨ�����Ʋ��ҹ�Դ����
	 * @param name �����ҹ�Դ��������
	 * @return ����ͬ����Դ�򷵻ع�Դ���󣬷��򷵻�NULL
	 */
	virtual Light* FindLightByName( const String& name) = 0;

	/**
	* @brief ɾ�����й�Դ
	*/
	virtual void DeleteAllLights( void ) = 0;

	/**
	* @brief ����ͶӰ����
	* @param mat ������ͶӰ����
	*/
	virtual void SetProjectionMatrix( const Math::Matrix4& mat ) = 0;

	/**
	* @brief ����ǰģ����ͼ����ѹ��ջ��
	* @remark �˺���ֻӰ��ģ����ͼ����
	*/
	virtual void PushMatrix() = 0;

	/**
	* @brief ������ջ�Ķ���Ԫ�س�ջ��
	*			  ������Ϊ��ǰģ����ͼ����
	* @remark �˺���ֻӰ��ģ����ͼ����
	*/
	virtual void PopMatrix() = 0;

	/** 
	 * @brief ���õ�ǰģ����ͼ����
	 * @param mat �����þ���
	 * @remark  �˺���ֻӰ��ģ����ͼ����
	 */
	virtual void LoadMatrix( const Math::Matrix4 &mat) = 0;

	/**
	* @brief ����ǰģ����ͼ��������Ϊ��λ����
	* @remark �˺���ֻӰ��ģ����ͼ����
	*/
	virtual void LoadIdentity() = 0;

	/**
	* @brief ���û�ָ�������뵱ǰģ����ͼ�������
	* @param mat ����˵ľ���
	* @remark �˺���ֻӰ��ģ����ͼ����
	*/
	virtual void MultMatrix( const Math::Matrix4 &mat) = 0;

};

}//namespace Render

#endif//ZP_IRENDER