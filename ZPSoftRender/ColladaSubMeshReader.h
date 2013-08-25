#ifndef ZP_COLLADASUBMESHREADER
#define ZP_COLLADASUBMESHREADER

#include "ZPDependency.h"

#include "COLLADAFWIWriter.h"
#include "COLLADAFWUniqueId.h"
#include "COLLADAFWVisualScene.h"
#include "COLLADAFWLibraryNodes.h"
#include "COLLADAFWMaterial.h"
#include "COLLADAFWEffect.h"
#include "COLLADAFWImage.h"

#include "COLLADABUURI.h"
#include "Math/COLLADABUMathMatrix4.h"
#include "COLLADAFWMeshVertexData.h"
#include "COLLADAFWMesh.h"

namespace Render
{
	class Vertex;
}

namespace Resource
{
	class SubMesh;

	class Tuple
	{
	public: 
		Tuple():m_posIndex(0),m_normIndex(0),m_uvIndex(0),m_tangentIndex(0),m_binormalIndex(0){}
		~Tuple(){}
		bool operator<(const Tuple& rhs) const;
		unsigned int m_posIndex;
		unsigned int m_normIndex;
		unsigned int m_uvIndex;
		unsigned int m_tangentIndex;
		unsigned int m_binormalIndex;
	};

	class ColladaSubMeshReader
	{
	public:
		ColladaSubMeshReader( COLLADAFW::Mesh* pMesh );
		virtual ~ColladaSubMeshReader(void);

		SubMesh* Load( void );

	protected:
			
		void LoadVertexByTuple( const Tuple& tuple , Render::Vertex& v );

	protected:
		COLLADAFW::Mesh*					m_pMesh;
		COLLADAFW::MeshVertexData& m_positions;
		COLLADAFW::MeshVertexData& m_normals;
		COLLADAFW::MeshVertexData& m_uvs;
		COLLADAFW::MeshVertexData& m_tangents;
		COLLADAFW::MeshVertexData& m_binormals;

		bool m_bHasNormals;
		bool m_bHasUVs;
		bool m_bHasTangents;
		bool m_bHasBinormals;

		typedef std::map<Tuple,unsigned int> indicesTable_t;
		indicesTable_t m_indicesMap;
	};

}//namespace Resource

#endif //ZP_COLLADASUBMESHREADER