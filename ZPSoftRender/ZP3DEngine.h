#ifndef ZP_ZP3DENGINE
#define ZP_ZP3DENGINE
 
#include "ZPDependency.h"

class Camera;

namespace Render
{
	class IRender;
}

class IFrameListener
{
public:
	IFrameListener(){}
	virtual ~IFrameListener(){}

	virtual void FrameStarted( void ) = 0;

	virtual void FrameEnded( void ) = 0;
};

class ZPEXPORT ZP3DEngine
{
public:
	ZP3DEngine(void);
	virtual ~ZP3DEngine(void);

	virtual void Init( const winHandle_t hwnd ); 
	virtual void Destroy( void ); 
	virtual void Resize( void ); 

	virtual void LoadResources( void );

	virtual Camera* GetCamera( void );
	virtual void RenderOneFrame( void );

	virtual void SwitchMesh( void );
	virtual void SwitchShadeMode( void );
	virtual void RegisterFrameListener( IFrameListener* listener );

protected:

	void LoadModels(void); 
	void LoadImages(void);

protected:
	Render::IRender* m_pRenderer;
	Camera* m_pCamera;
	IFrameListener* m_pFrameListener;
	unsigned int m_uiShadeMode;
};

#endif// ZP_ZP3DENGINE
