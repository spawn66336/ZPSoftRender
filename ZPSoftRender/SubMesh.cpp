#include "SubMesh.h"
#include "Material.h"

namespace Resource
{


SubMesh::SubMesh(void)
{
}


SubMesh::~SubMesh(void)
{
}

Material* SubMesh::Material( void )
{
	return m_pMaterial;
}

void SubMesh::SetMaterial( Material* mat )
{
	m_pMaterial = mat;
}

}//namespace Resource