#ifndef ZP_IRENDER
#define ZP_IRENDER

#include "ZPDependency.h"

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
}

namespace Render
{ 
	
    class Light;

enum PRIMITIVE_TYPE
{
	TYPE_LINES,
	TYPE_TRIANGLES
};

enum TEXMAP_CHANNEL
{
	DIFFUSE_CH = 0,
	BUMPMAP_CH = 1
};

enum SHADE_MODEL
{
	WIREFRAME_MODEL,
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

	virtual void BeginDraw( void ) = 0;

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

	/**
	 * 
	 * @param vertices
	 */
	virtual void VertexPointer( const Math::Vec3* vertices) = 0;

	/**
	 * 
	 * @param normals
	 */
	virtual void NormalPointer( const Math::Vec3* normals) = 0;

	/**
	 * 
	 * @param texcoords
	 */
	virtual void TexcoordPointer( const Math::Vec2* texcoords) = 0;

	/**
	 * 
	 * @param tangents
	 */
	virtual void TangentPointer( const Math::Vec3* tangents) = 0;

	/**
	 * 
	 * @param binormals
	 */
	virtual void BinormalPointer( const Math::Vec3* binormals) = 0;

	/**
	 * 
	 * @param indices
	 * @param count
	 * @param primitiveType
	 */
	virtual  void DrawElements( int* indices, int count, PRIMITIVE_TYPE primitiveType ) = 0;

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
	 * @param channel
	 * @param tex
	 */
	virtual void SetTexture2D( int channel, Resource::Texture2D* tex ) = 0;

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