#ifndef ZP_D3DRENDERIMPL
#define ZP_D3DRENDERIMPL

#include "IRender.h"
#include <d3d9.h>
#include "d3dx9shader.h"

class Camera;


namespace Resource
{ 
	class Texture2D;
}

namespace Render
{ 

	typedef struct{
		Math::Vec3 pos;
		Math::Vec3 norm; 
		Math::Vec2 tex;
		Math::Vec3 binorm;
		Math::Vec3 tangent;
	}d3dRenderVert_t;

	typedef struct{
		Math::Vec3 pos;
		unsigned int diffuse;
	}d3dRenderVert2_t;

	class D3DRenderImpl : public IRender
	{
	public:
		D3DRenderImpl(void);
		virtual ~D3DRenderImpl(void);

		virtual void Init( const winHandle_t hwnd );  
		virtual void Destroy();
		virtual void Resize(); 
		virtual void BeginDraw( Camera* pCam ); 
		virtual void EndDraw( void ); 
		virtual void SwapBuffers( void ); 
		virtual void SetClearColor( const Math::Vec4& color); 
		virtual void ClearBuffer(unsigned int flag) ; 
		virtual void ApplyMaterial( Resource::Material* pMaterial ); 
		virtual  void DrawElements( RenderPrimitive& renderPrimitive );
		virtual void EnableTexture2D( bool enable); 
		virtual void EnableLight( bool enable );
		virtual void EnableDepthTest( bool enable ); 
		virtual void SetShadingModel( SHADE_MODEL type ); 
		virtual Light* CreateLight( const String& name ); 
		virtual bool DeleteLightByName( const String& name ); 
		virtual Light* FindLightByName( const String& name); 
		virtual void DeleteAllLights( void ); 
		virtual void SetProjectionMatrix( const Math::Matrix4& mat ); 
		virtual void PushMatrix(); 
		virtual void PopMatrix();
		virtual void LoadMatrix( const Math::Matrix4 &mat);
		virtual void LoadIdentity();
		virtual void MultMatrix( const Math::Matrix4 &mat);

	protected: 

		void _SetupMatrices( Camera* pCam );
		void _ApplyShaders( void );
		void _UnapplyShaders( void );
		void _CommitTexture( Resource::Texture2D* pTexture , const int tex_ch );
		void _InitVB( void );
		void _DestroyVB( void ); 
		void _InitHelperVB( void );
		void _DestroyHelperVB( void );
		void _DrawHelper( void );
		void _PrepareRender( RenderPrimitive& renderPrimitive );
		void _OnDeviceLost( void );
		void _OnDeviceReset( void );
		void _ApplyRenderState( void );
		void _ApplyShadeModel( void );
		void _ApplyAllLights( void );
		void _DestroyTextureCache( void );

		void _DrawText( const int x , const int y , const String& str );
		void _InitFont( void );
		void _DestroyFont( void );
		void _InitEffect( void ); 
		void _DestroyEffect( void );
		bool _LoadVertexShader( const String& filename , IDirect3DVertexShader9 ** ppVS );
		bool _LoadPixelShader( const String& filename , IDirect3DPixelShader9 ** ppPS );

	protected: 
		D3DPRESENT_PARAMETERS m_d3dParams;
		IDirect3D9* m_pD3D9;
		LPDIRECT3DDEVICE9 m_pD3D9Device;
		LPDIRECT3DVERTEXBUFFER9 m_pD3DVB;
		LPDIRECT3DINDEXBUFFER9 m_pD3DIB;


		LPDIRECT3DVERTEXBUFFER9 m_pD3DHelperVB;
		LPDIRECT3DINDEXBUFFER9 m_pD3DHelperIB;

		IDirect3DVertexShader9* m_pVS;		///>顶点着色器  
		LPD3DXCONSTANTTABLE  m_pD3DVSConstantTab; 
		IDirect3DPixelShader9* m_pPS;		///>像素着色器
		LPD3DXCONSTANTTABLE  m_pD3DPSConstantTab; 
		D3DLIGHT9 m_currLight;
		D3DMATERIAL9 m_material;
		ID3DXEffectPool* m_pEffectPool;	 ///>用于共享全局变量
		LPD3DXEFFECT m_pGlobalConstEffect; ///>用于变更全局变量的Effect
		LPD3DXEFFECT m_pEffect;
		ID3DXFont* m_pFont;

		D3DVIEWPORT9 m_viewPort;
		unsigned int m_uiFVF;
		HWND m_hWnd; 
		Real		m_fAspect;
		unsigned int m_uiClearFlag;
		Math::Vec4 m_v4ClearColor;
		bool	m_bEnableLighting;
		bool m_bEnableTexture;
		bool m_bEnableDepthTest;
		SHADE_MODEL m_shadeModel;

		Math::Matrix4 m_m4ProjMat;		///>投影矩阵
		Math::Matrix4 m_m4ViewMat;     ///>世界到相机矩阵
		Math::Matrix4 m_m4WorldMat;   ///>世界矩阵
		typedef std::stack<Math::Matrix4> matrixStack_t; 
		matrixStack_t m_worldMatrixStack;	///>世界矩阵栈
		typedef std::map< unsigned int , IDirect3DTexture9*> textureList_t;
		textureList_t m_textureCache;
		typedef std::map<String,Light*> lightTable_t; 
		lightTable_t m_lights;						///>光源列表
	};

}//namespace Render


#endif //ZP_D3DRENDERIMPL