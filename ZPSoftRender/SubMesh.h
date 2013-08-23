#ifndef ZP_SUBMESH
#define ZP_SUBMESH

#include "ZPDependency.h"
#include "RenderPrimitive.h"

namespace Math
{
	class Vec2;
	class Vec3;
	class Vec4;
	class Matrix4;
}

namespace Resource
{ 
	class Material;

	class  SubMesh
	{  
	public:

		SubMesh(void);
		virtual ~SubMesh(void);

		String Name( void ) const{ return m_name; }

		void SetName( const String& name){ m_name = name; }

		Resource::Material* GetMaterial( void );

		void SetMaterial( Resource::Material* mat );

		const Math::Matrix4 GetMatrix(void) const { return m_mat; }

		void SetMatrix( const Math::Matrix4& mat ){ m_mat = mat; }

		Render::RenderPrimitive& RenderData(void){  return m_renderPrimitive; }

		const Render::RenderPrimitive& RenderData(void) const { return m_renderPrimitive; }

		void SetRenderData( const Render::RenderPrimitive& data );
		 
	protected: 
		String m_name;							///>��ģ����
		Resource::Material*  m_pMaterial; ///>����ģ�͵Ĳ��� 
		Math::Matrix4			m_mat;			///>��ģ�ͱ任����
		Render::RenderPrimitive m_renderPrimitive; ///>������Ⱦ�ļ���ͼԪ 
	};

}//namespace Resrouce

#endif//ZP_SUBMESH