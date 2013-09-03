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

		/**
		* @brief 为采样器设置二维纹理
		* @param pTexture 待设置的二维纹理
		* @remark 此函数会根据传入的二维纹理计算纹理
		*				  的行字节跨度
		*/
		void SetTexture2D( Resource::Texture2D* pTexture );

		/**
		* @brief 当前着色器是否为可用
		* @return 若当前纹理已设置则返回可用
		*/
		inline bool IsActive( void ) const { return NULL != m_pTexture; }

		/**
		* @brief 获得纹理采样器的环绕算法类型
		*/
		unsigned int GetWrapType( void ) const { return m_uiWrapType; }

		/**
		* @brief 设置纹理采样器的环绕算法类型
		* @param type 待设置的纹理采样器环绕类型
		*/
		void SetWrapType( const unsigned int type ){ m_uiWrapType = type; }

		/**
		* @brief 根据纹理坐标采样纹理
		* @return 纹理在纹理坐标处的颜色
		*/
		Math::Vec4 Sample( Math::Vec2& v2Texcoord );

	protected:

		void CalcTexcoord( Math::Vec2& v2Texcoord  , Math::Vec2& v2FinalUV , int& iu , int& iv );
		 
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
		 
		/**
		* @brief 获取当前像素着色器的着色模式
		*/
		inline SHADE_MODEL GetShadeModel( void ) const { return m_shadeModel; }

		/**
		* @brief 为当前像素着色设置着色模式
		*/
		inline void SetShadeModel( const SHADE_MODEL model ){ m_shadeModel = model; }

		/**
		* @brief 获得待着色面的面颜色
		* @remark 此值只对Flat着色有意义
		*/
		inline const Math::Vec4& GetFaceColor( void ) const { return m_v4FaceColor; }

		/**
		* @brief 设置待着色面的面颜色
		* @remark 此值只对Flat着色有意义
		*/
		inline void SetFaceColor( const Math::Vec4& color ){ m_v4FaceColor = color; }
		
		/**
		* @brief 设置当前所渲染模型的材质
		*/
		void SetMaterial( Resource::Material* pMaterial );

		/**
		* @brief 将光源加入着色光源列表
		*/
		inline void AddLight( Light* pLight ){ m_lights.push_back( pLight ); }

		/**
		* @brief 清空着色光源列表
		*/
		inline void ClearAllLights( void ){ m_lights.clear(); }
		
		/**
		* @brief 执行着色
		* @param v 扫描线插值出的像素信息
		* @return 当前像素颜色
		*/
		Math::Vec4 Run( const RVertex& v );
		  
		Math::Vec4 m_v4FaceColor;									///>面颜色
		Resource::Material* m_pMaterial;							///>材质
		SHADE_MODEL m_shadeModel;							///>着色模型 
		typedef std::vector<Light*> lightTable_t; 
		lightTable_t m_lights;											///>着色光源列表
		Texture2DSampler	m_diffuseTextureSampler;		///>漫反射纹理采样器
		Texture2DSampler	m_normTextureSampler;		///> 法线纹理采样器
	};

}//namespace Render;

#endif //ZP_PIXELSHADER