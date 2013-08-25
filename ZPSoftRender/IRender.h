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

	virtual void Init( const winHandle_t hwnd ) = 0;

	virtual void Destroy() = 0;

	virtual void Resize() = 0;

	virtual void BeginDraw( Camera* pCam ) = 0;

	virtual void EndDraw( void ) = 0;

	virtual void SwapBuffers( void ) =0;

	/** 
	 * @param color
	 */
	virtual void SetClearColor( const Math::Vec4& color) = 0;

	/** 
	 * @param flag
	 */
	virtual void ClearBuffer(unsigned int flag) = 0;

	virtual void ApplyMaterial( Resource::Material* pMaterial ) = 0;

 
	virtual  void DrawElements( RenderPrimitive& renderPrimitive ) = 0;

	/**
	 * 
	 * @param enable
	 */
	virtual void EnableTexture2D( bool enable) = 0;

	/**
	 * 
	 * @param enable
	 */
	virtual void EnableLight( bool enable ) = 0;
	 

	/**
	 * 
	 * @param enable
	 */
	virtual void EnableDepthTest( bool enable ) = 0;

	/**
	 * 
	 * @param type
	 */
	virtual void SetShadingModel( SHADE_MODEL type ) = 0;

	/**
	 * 
	 * @param name
	 */
	virtual Light* CreateLight( const String& name ) = 0;

	/**
	 * 
	 * @param name
	 */
	virtual bool DeleteLightByName( const String& name ) = 0;

	/**
	 * 
	 * @param name
	 */
	virtual Light* FindLightByName( const String& name) = 0;

	virtual void DeleteAllLights( void ) = 0;

	virtual void SetProjectionMatrix( const Math::Matrix4& mat ) = 0;

	virtual void PushMatrix() = 0;

	virtual void PopMatrix() = 0;

	/** 
	 * @param mat
	 */
	virtual void LoadMatrix( const Math::Matrix4 &mat) = 0;

	virtual void LoadIdentity() = 0;

	/** 
	* @param mat
	*/
	virtual void MultMatrix( const Math::Matrix4 &mat) = 0;

};

}//namespace Render

#endif//ZP_IRENDER