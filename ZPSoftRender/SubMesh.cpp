#include "SubMesh.h"
#include "Material.h"

namespace Resource
{


SubMesh::SubMesh(void):
m_pMaterial(NULL),
m_mat( Math::Matrix4::IDENTITY )
{
}


SubMesh::~SubMesh(void)
{
}

Resource::Material* SubMesh::GetMaterial( void )
{
	return m_pMaterial;
}

void SubMesh::SetMaterial( Resource::Material* mat )
{
	m_pMaterial = mat;
}

void SubMesh::SetRenderData( const Render::RenderPrimitive& data )
{
	m_renderPrimitive = data;
}

}//namespace Resource