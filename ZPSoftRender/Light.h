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

		inline String Name(void) const { return m_name; } 
		inline void SetName( const String& name){ m_name = name; }

		inline const Math::Vec4& Ambient() const { return m_v4Ambient; }
		inline const Math::Vec4& Diffuse() const { return m_v4Diffuse; }
		inline const Math::Vec4& Specular() const { return m_v4Specular; }
		inline const Math::Vec4& Position() const { return m_v4Pos; }
		 
	    inline void SetAmbient( const Math::Vec4& ambient){ m_v4Ambient = ambient; } 
		inline void SetDiffuse( const Math::Vec4& diffuse){ m_v4Diffuse = diffuse;  }  
		inline void SetSpecular( const Math::Vec4& specular){ m_v4Specular = specular; } 
		inline void SetPosition( const Math::Vec4& pos){ m_v4Pos = pos; }

		inline bool IsActive(){ return m_active; } 
		inline void SetActive( bool active){ m_active = active;}

		inline const Math::Vec4& GetPosInCamera( void ) const { return m_v4PosInCam; }
		inline void SetPosInCamera( const Math::Vec4& pos ){ m_v4PosInCam = pos; }

	protected:
		String m_name;						///>光源名称
		bool    m_active;						///>当前光源是否为可用
		Math::Vec4   m_v4Ambient;		///>光源的环境光项
		Math::Vec4	m_v4Diffuse;		///>光源的漫反射光项
		Math::Vec4   m_v4Specular;		///>光源的镜面高光项
		Math::Vec4   m_v4Pos;				///>光源的位置
		Math::Vec4   m_v4PosInCam;	///>光源在相机空间中的位置
	};

}//namespace Render
#endif