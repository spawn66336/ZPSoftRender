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
		* @brief Ϊ���������ö�ά����
		* @param pTexture �����õĶ�ά����
		* @remark �˺�������ݴ���Ķ�ά�����������
		*				  �����ֽڿ��
		*/
		void SetTexture2D( Resource::Texture2D* pTexture );

		/**
		* @brief ��ǰ��ɫ���Ƿ�Ϊ����
		* @return ����ǰ�����������򷵻ؿ���
		*/
		inline bool IsActive( void ) const { return NULL != m_pTexture; }

		/**
		* @brief �������������Ļ����㷨����
		*/
		unsigned int GetWrapType( void ) const { return m_uiWrapType; }

		/**
		* @brief ��������������Ļ����㷨����
		* @param type �����õ������������������
		*/
		void SetWrapType( const unsigned int type ){ m_uiWrapType = type; }

		/**
		* @brief �������������������
		* @return �������������괦����ɫ
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
		* @brief ��ȡ��ǰ������ɫ������ɫģʽ
		*/
		inline SHADE_MODEL GetShadeModel( void ) const { return m_shadeModel; }

		/**
		* @brief Ϊ��ǰ������ɫ������ɫģʽ
		*/
		inline void SetShadeModel( const SHADE_MODEL model ){ m_shadeModel = model; }

		/**
		* @brief ��ô���ɫ�������ɫ
		* @remark ��ֵֻ��Flat��ɫ������
		*/
		inline const Math::Vec4& GetFaceColor( void ) const { return m_v4FaceColor; }

		/**
		* @brief ���ô���ɫ�������ɫ
		* @remark ��ֵֻ��Flat��ɫ������
		*/
		inline void SetFaceColor( const Math::Vec4& color ){ m_v4FaceColor = color; }
		
		/**
		* @brief ���õ�ǰ����Ⱦģ�͵Ĳ���
		*/
		void SetMaterial( Resource::Material* pMaterial );

		/**
		* @brief ����Դ������ɫ��Դ�б�
		*/
		inline void AddLight( Light* pLight ){ m_lights.push_back( pLight ); }

		/**
		* @brief �����ɫ��Դ�б�
		*/
		inline void ClearAllLights( void ){ m_lights.clear(); }
		
		/**
		* @brief ִ����ɫ
		* @param v ɨ���߲�ֵ����������Ϣ
		* @return ��ǰ������ɫ
		*/
		Math::Vec4 Run( const RVertex& v );
		  
		Math::Vec4 m_v4FaceColor;									///>����ɫ
		Resource::Material* m_pMaterial;							///>����
		SHADE_MODEL m_shadeModel;							///>��ɫģ�� 
		typedef std::vector<Light*> lightTable_t; 
		lightTable_t m_lights;											///>��ɫ��Դ�б�
		Texture2DSampler	m_diffuseTextureSampler;		///>���������������
		Texture2DSampler	m_normTextureSampler;		///> �������������
	};

}//namespace Render;

#endif //ZP_PIXELSHADER