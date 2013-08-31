#ifndef ZP_ZP3DENGINE
#define ZP_ZP3DENGINE
 
#include "ZPDependency.h"
#include "IRender.h"

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

	virtual void ResetMesh( void );
	virtual void RotateMeshWithXAxis( const Real theta );
	virtual void RotateMeshWithYAxis( const Real theta );
	virtual void RotateMeshWithZAxis( const Real theta );

	virtual void SwitchMesh( void );
	virtual void SwitchShadeModel( void );
	virtual void SetShadeModel( const Render::SHADE_MODEL model );
	virtual void RegisterFrameListener( IFrameListener* listener );

	virtual void SetMovingFlag( const bool flag = true );

protected:

	void LoadModels(void); 
	void LoadImages(void);

protected:
	Render::IRender* m_pRenderer;
	Camera* m_pCamera;
	IFrameListener* m_pFrameListener;
	unsigned int m_uiShadeModel;
	Math::Matrix4 m_meshMatrix; 
	bool					 m_bIsMoving;				///>当前用户是否对场景进行移动
};

#endif// ZP_ZP3DENGINE
