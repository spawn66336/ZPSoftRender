#ifndef ZP_ZP3DENGINE
#define ZP_ZP3DENGINE
 
#include "ZPDependency.h"

class Camera;

namespace Render
{
	class IRender;
}

namespace Resource
{
	class MeshManager;
	class TextureManager;
	class MaterialManager;

}

class ZPEXPORT ZP3DEngine
{
public:
	ZP3DEngine(void);
	virtual ~ZP3DEngine(void);

	virtual void Init( const winHandle_t hwnd ); 
	virtual void Destroy( void ); 
	virtual void Resize( void ); 

	virtual Camera* GetCamera( void );
	virtual Resource::MeshManager* GetMeshManager( void );
	virtual Resource::MaterialManager* GetMaterialManager( void );
	virtual Resource::TextureManager* GetTextureManager( void );

	virtual void RenderOneFrame( void );

protected:
	Render::IRender* m_pRenderer;
	Camera* m_pCamera;
};

#endif// ZP_ZP3DENGINE
