#ifndef ZP_LIGHT
#define ZP_LIGHT

#include "ZPMathDependency.h"

namespace Render
{

	class Light
	{
	public:
		Light(void);
		virtual ~Light(void);

		String Name(void) const { return m_name; } 
		void SetName( const String& name){ m_name = name; }

		const Math::Vec4& Ambient() const { return m_ambient; }
		const Math::Vec4& Diffuse() const { return m_diffuse; }
		const Math::Vec4& Specular() const { return m_specular; }
		const Math::Vec4& Position() const { return m_pos; }
		 
	    void SetAmbient( const Math::Vec4& ambient){ m_ambient = ambient; } 
		void SetDiffuse( const Math::Vec4& diffuse){ m_diffuse = diffuse;  }  
		void SetSpecular( const Math::Vec4& specular){ m_specular = specular; } 
		void SetPosition( const Math::Vec4& pos){ m_pos = pos; }

		bool IsActive(){ return m_active; } 
		void SetActive( bool active){ m_active = active;}

	protected:
		String m_name;
		bool    m_active;
		Math::Vec4   m_ambient;
		Math::Vec4	m_diffuse;
		Math::Vec4   m_specular;
		Math::Vec4   m_pos; 
	};

}//namespace Render
#endif