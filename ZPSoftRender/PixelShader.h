#ifndef ZP_PIXELSHADER
#define ZP_PIXELSHADER

#include "ZPDependency.h"
#include "ZPMathDependency.h"
#include "IRender.h"

namespace Resource
{
	class Material;
}

namespace Render
{
	class RVertex;

	class PixelShader
	{
	public:
		PixelShader(void);
		~PixelShader(void);
		 
		inline SHADE_MODEL GetShadeModel( void ) const { return m_shadeModel; }
		inline void SetShadeModel( const SHADE_MODEL model ){ m_shadeModel = model; }

		inline const Math::Vec4& GetFaceColor( void ) const { return m_v4FaceColor; }
		inline void SetFaceColor( const Math::Vec4& color ){ m_v4FaceColor = color; }
		 
		Math::Vec4 Run( const RVertex& v );
		  
		Math::Vec4 m_v4FaceColor;				///>面颜色
		Resource::Material* m_pMaterial;      ///>材质
		SHADE_MODEL m_shadeModel;       ///>着色模型

	};

}//namespace Render;

#endif //ZP_PIXELSHADER