#include "ColladaSubMeshReader.h"
#include "SubMesh.h"
#include "MaterialManager.h"

namespace Resource
{

ColladaSubMeshReader::ColladaSubMeshReader( COLLADAFW::Mesh* pMesh ):
m_pMesh( pMesh ),
m_positions( pMesh->getPositions() ),
m_normals( pMesh->getNormals() ),
m_uvs( pMesh->getUVCoords() ),
m_tangents( pMesh->getTangents() ),
m_binormals( pMesh->getBinormals() ),
m_bHasNormals(false),
m_bHasUVs(false),
m_bHasTangents(false),
m_bHasBinormals(false)
{
}


ColladaSubMeshReader::~ColladaSubMeshReader(void)
{
}

SubMesh* ColladaSubMeshReader::Load( void )
{  
	COLLADAFW::MeshPrimitiveArray& primitives = m_pMesh->getMeshPrimitives();

	//若没有几何图元信息
	if( 0 == primitives.getCount() )
	{
		return NULL;
	}

	COLLADAFW::MeshPrimitive* pPrimitive = primitives[0]; 
 
	if(  COLLADAFW::MeshPrimitive::TRIANGLES != pPrimitive->getPrimitiveType()  )
	{
		return NULL;
	}

	COLLADAFW::UIntValuesArray& positionIndices = pPrimitive->getPositionIndices(); 
	//若没有位置索引 则直接退出
	if( 0 == positionIndices.getCount() )
	{
		return NULL;
	} 

	COLLADAFW::UIntValuesArray& normalIndices = pPrimitive->getNormalIndices(); 
	m_bHasNormals = ( normalIndices.getCount() != 0 );  
	COLLADAFW::UIntValuesArray& tangentIndices = pPrimitive->getTangentIndices(); 
	m_bHasTangents = ( tangentIndices.getCount() != 0 ); 
	COLLADAFW::UIntValuesArray& binormalIndices = pPrimitive->getBinormalIndices(); 
	m_bHasBinormals = ( binormalIndices.getCount() != 0 );
	COLLADAFW::IndexList* pUVs = NULL;
	if( pPrimitive->getUVCoordIndicesArray().getCount() )
	{
		pUVs = pPrimitive->getUVCoordIndicesArray()[0]; 
		m_bHasUVs = ( pUVs->getIndices().getCount() != 0 );
	} 

	SubMesh* pSubMesh = new SubMesh;
	pSubMesh->SetName( m_pMesh->getName() );  
	Resource::Material* pMaterial = Resource::MaterialManager::GetInstance()->CreateOrRetrieveMaterial( m_pMesh->getName() );
	pSubMesh->SetMaterial( pMaterial );

	Render::VertexBuffer& vertexBuf =  pSubMesh->RenderData().VertexBuf();
	pSubMesh->RenderData().PrimitiveType( Render::TYPE_TRIANGLES );

	vertexBuf.ChannelFlag() = Render::POSITION_CH;
	if( m_bHasNormals )
	{
		vertexBuf.ChannelFlag() |= Render::NORMAL_CH;
	}
	if( m_bHasUVs )
	{
		vertexBuf.ChannelFlag() |= Render::TEXCOORD_CH;
	}
	if( m_bHasTangents )
	{
		vertexBuf.ChannelFlag() |= Render::TANGENT_CH;
	}
	if( m_bHasBinormals )
	{
		vertexBuf.ChannelFlag() |= Render::BINORMAL_CH;
	}

	unsigned int uiCurrIndex = 0;
	unsigned int uiIndicesCount = positionIndices.getCount();

	for( unsigned int uiIndex = 0 ; uiIndex < uiIndicesCount ; uiIndex++ )
	{
		Tuple tuple;
		tuple.m_posIndex = positionIndices[uiIndex];
		if( m_bHasNormals )
		{
			tuple.m_normIndex = normalIndices[uiIndex];
		}
		if( m_bHasUVs )
		{
			tuple.m_uvIndex = pUVs->getIndices()[uiIndex];
		}
		if( m_bHasTangents )
		{
			tuple.m_tangentIndex = tangentIndices[uiIndex];
		}
		if( m_bHasBinormals )
		{
			tuple.m_binormalIndex = binormalIndices[uiIndex];
		}

		indicesTable_t::iterator itIndex = m_indicesMap.find(tuple); 
		if( itIndex == m_indicesMap.end() )
		{
			Render::Vertex newVert;
			LoadVertexByTuple( tuple , newVert );
			vertexBuf.AppendVertex( newVert );
			m_indicesMap.insert( std::make_pair( tuple , uiCurrIndex ) );
			pSubMesh->RenderData().AppendIndex( uiCurrIndex );
			uiCurrIndex++;
		}else{
			pSubMesh->RenderData().AppendIndex( itIndex->second );
		}

	}//for( unsigned int uiIndex = 0 ; uiIndex < uiIndicesCount ; uiIndex++ )

	return pSubMesh;
}

void ColladaSubMeshReader::LoadVertexByTuple( const Tuple& tuple , Render::Vertex& v )
{
	if( m_positions.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_FLOAT)
	{
		const float* pPositions =  m_positions.getFloatValues()->getData();
		pPositions += tuple.m_posIndex*3;
		v.m_pos.Set( pPositions[0] , pPositions[1] , pPositions[2] );

	}else{
		const double* pPositions = m_positions.getDoubleValues()->getData();
		pPositions += tuple.m_posIndex*3;
		v.m_pos.Set( (float)pPositions[0] , (float)pPositions[1] , (float)pPositions[2] );
	}

	if( m_bHasNormals )
	{ 
		if( m_normals.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_FLOAT )
		{
			const float* pNormals =  m_normals.getFloatValues()->getData();
			pNormals += tuple.m_normIndex*3;
			v.m_norm.Set( pNormals[0] , pNormals[1] , pNormals[2] );
		}else{
			const double* pNormals = m_normals.getDoubleValues()->getData();
			pNormals += tuple.m_normIndex*3;
			v.m_norm.Set( (float)pNormals[0] , (float)pNormals[1] , (float)pNormals[2] );
		} 
		v.m_norm.Normalize();
	}//if( bHasNormals )

	if( m_bHasUVs )
	{ 
		const int stride = m_uvs.getStride(0); 
		if( m_uvs.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_FLOAT )
		{
			const float* pUVs =  m_uvs.getFloatValues()->getData();
			pUVs += tuple.m_uvIndex*stride;
			v.m_texcoord.Set( pUVs[0] , pUVs[1]  );
		}else{
			const double* pUVs = m_uvs.getDoubleValues()->getData();
			pUVs += tuple.m_uvIndex*stride;
			v.m_texcoord.Set( (float)pUVs[0] , (float)pUVs[1] );
		} 
	}//if( bHasUVs )

	if( m_bHasTangents )
	{ 
		if( m_tangents.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_FLOAT )
		{
			const float* pTangents =  m_tangents.getFloatValues()->getData();
			pTangents += tuple.m_tangentIndex*3;
			v.m_tangent.Set( pTangents[0] , pTangents[1] , pTangents[2] );
		}else{
			const double* pTangents = m_tangents.getDoubleValues()->getData();
			pTangents += tuple.m_tangentIndex*3;
			v.m_tangent.Set( (float)pTangents[0] , (float)pTangents[1] , (float)pTangents[2] );
		} 
	}//if( bHasTangents )

	if( m_bHasBinormals )
	{ 
		if( m_binormals.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_FLOAT )
		{
			const float* pBinormals =  m_binormals.getFloatValues()->getData();
			pBinormals += tuple.m_binormalIndex*3;
			v.m_binormal.Set( pBinormals[0] , pBinormals[1] , pBinormals[2] );
		}else{
			const double* pBinormals = m_tangents.getDoubleValues()->getData();
			pBinormals += tuple.m_binormalIndex*3;
			v.m_binormal.Set( (float)pBinormals[0] , (float)pBinormals[1] , (float)pBinormals[2] );
		} 
	}//if( bHasBinormals )
}


bool Tuple::operator<( const Tuple& rhs ) const
{
	if ( m_posIndex < rhs.m_posIndex )
		return true;

	if ( m_posIndex > rhs.m_posIndex )
		return false;

	if ( m_normIndex < rhs.m_normIndex )
		return true;

	if ( m_normIndex > rhs.m_normIndex )
		return false;

	if ( m_uvIndex < rhs.m_uvIndex )
		return true;

	if ( m_uvIndex > rhs.m_uvIndex )
		return false;

	if ( m_tangentIndex < rhs.m_tangentIndex )
		return true;

	if ( m_tangentIndex > rhs.m_tangentIndex )
		return false;

	if ( m_binormalIndex < rhs.m_binormalIndex )
		return true;

	if ( m_binormalIndex > rhs.m_binormalIndex )
		return false;

	return false;
}

}//namespace Resource