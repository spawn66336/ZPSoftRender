#ifndef ZP_MATERIAL
#define ZP_MATERIAL

#include "ZPDependency.h"
#include "ZPMathDependency.h"

namespace Render
{
	class IRender;
}

namespace Resource
{
	class Texture2D;

	const int MAX_TEXTURE_CHANNEL = 2;

	class Material
	{
	public:
		Material(void);
		virtual ~Material(void);

		String Name( void ) const { return m_name; }  
		void SetName( const String& name ){ m_name = name; }

		const Math::Vec4& GetAmbient(void) const { return m_ambient; }
		void SetAmbient( const Math::Vec4& ambient ){ m_ambient = ambient; }

		const Math::Vec4& GetDiffuse(void) const { return m_diffuse; }
		void SetDiffuse( const Math::Vec4& diffuse ){ m_diffuse = diffuse; }

		const Math::Vec4& GetSpecular(void) const { return m_specular; }
		void SetSpecular( const Math::Vec4& specular ){ m_specular = specular; }

		const Real& GetShininess( void ) const { return m_shininess; }
		void SetShininess( const Real shininess ){ m_shininess = shininess; } 
 
		Texture2D* GetTexture( int channel );
		void SetTexture( Texture2D* pTexture, int channel );  

		//用户自定义指针
		void* GetUserPointer( void ) const { return m_pUserPointer; }
		void SetUserPointer( void* p ){ m_pUserPointer = p; }

	protected:

		String m_name;
		Math::Vec4 m_ambient;
		Math::Vec4 m_diffuse;
		Math::Vec4 m_specular;
		Real			   m_shininess;
		Texture2D* m_texChannels[MAX_TEXTURE_CHANNEL];
		void* m_pUserPointer; 
	};

}//namespace Resource

#endif//ZP_MATERIAL