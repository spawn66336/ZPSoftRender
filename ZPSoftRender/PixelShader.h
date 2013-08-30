#ifndef ZP_PIXELSHADER
#define ZP_PIXELSHADER

#include "ZPDependency.h"
#include "ZPMathDependency.h"
#include "IRender.h"

namespace Resource
{
	class Material;
	class Texture2D;
}

namespace Render
{
	class RVertex;

	enum TEXTURE2D_SAMPLER_WRAP_TYPE
	{
		WRAP_CLAMP = 1,
		WRAP_REPEAT = 2
	};

	class Texture2DSampler
	{
	public:
		Texture2DSampler(void);
		~Texture2DSampler();

		void SetTexture2D( Resource::Texture2D* pTexture );

		inline bool IsActive( void ) const { return NULL != m_pTexture; }

		Math::Vec4 Sample( Math::Vec2& v2Texcoord );

		unsigned int GetWrapType( void ) const { return m_uiWrapType; }

		void SetWrapType( const unsigned int type ){ m_uiWrapType = type; }

	protected: 
		Resource::Texture2D* m_pTexture;
		unsigned int m_uiBytesPerPixel;
		unsigned int m_uiLineOffset;
		unsigned int m_uiWrapType;
	};

	class PixelShader
	{
	public:
		PixelShader(void);
		~PixelShader(void);
		 
		inline SHADE_MODEL GetShadeModel( void ) const { return m_shadeModel; }
		inline void SetShadeModel( const SHADE_MODEL model ){ m_shadeModel = model; }

		inline const Math::Vec4& GetFaceColor( void ) const { return m_v4FaceColor; }
		inline void SetFaceColor( const Math::Vec4& color ){ m_v4FaceColor = color; }
		
		void SetMaterial( Resource::Material* pMaterial );

		inline void AddLight( Light* pLight ){ m_lights.push_back( pLight ); }
		inline void ClearAllLights( void ){ m_lights.clear(); }
		 
		Math::Vec4 Run( const RVertex& v );
		  
		Math::Vec4 m_v4FaceColor;				///>面颜色
		Resource::Material* m_pMaterial;      ///>材质
		SHADE_MODEL m_shadeModel;       ///>着色模型 
		typedef std::vector<Light*> lightTable_t; 
		lightTable_t m_lights;
		Texture2DSampler	m_diffuseTextureSampler;
		Texture2DSampler	m_normTextureSampler;
	};

}//namespace Render;

#endif //ZP_PIXELSHADER