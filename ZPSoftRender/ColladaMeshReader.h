#ifndef ZP_COLLADAMESHREADER
#define ZP_COLLADAMESHREADER

#include "ZPDependency.h"
#include "ZPMathDependency.h"
#include "COLLADAFWIWriter.h"
#include "COLLADAFWUniqueId.h"
#include "COLLADAFWVisualScene.h"
#include "COLLADAFWLibraryNodes.h"
#include "COLLADAFWMaterial.h"
#include "COLLADAFWEffect.h"
#include "COLLADAFWImage.h"

#include "COLLADABUURI.h"
#include "Math/COLLADABUMathMatrix4.h"

namespace Resource
{ 
	class Mesh; 

	class ColladaMeshReader : public COLLADAFW::IWriter
	{
	public: 

		enum MESHREADER_PASS
		{
			SCENE_INFO_PASS = 1,
			GEOMETRY_PASS
		};

		struct NodeInfo
		{
		public:
			COLLADABU::Math::Matrix4 matrix;
		};

		struct InstanceGeometryInfo
		{
			InstanceGeometryInfo( COLLADAFW::InstanceGeometry* _fwInstanceGeometry, const COLLADABU::Math::Matrix4& _worldMatrix)
				:  fwInstanceGeometry(_fwInstanceGeometry), worldMatrix(_worldMatrix) {}
			COLLADAFW::InstanceGeometry* fwInstanceGeometry;
			COLLADABU::Math::Matrix4 worldMatrix;
		};
		 
		typedef std::vector<COLLADAFW::VisualScene*> visualSceneList_t; 
		typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Node*> uniqueIdNodeMap; 
		typedef std::map<COLLADAFW::UniqueId,InstanceGeometryInfo> uniqueIdGeoInfoMap_t; 
		typedef std::stack<NodeInfo> nodeInfoStack_t;
		typedef std::map<COLLADAFW::UniqueId,String> uniqueIdImageNameMap_t;
		typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Material> uniqueIdMaterialMap_t;
		typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Effect> uniqueIdEffectMap_t;
	public:
		ColladaMeshReader( const COLLADABU::URI& path , Mesh* pMesh );
		virtual ~ColladaMeshReader(void);

	public:

		bool Load();
		void cancel(const String& errorMessage);  
		void start();
		void finish(); 
		virtual bool writeGlobalAsset ( const COLLADAFW::FileInfo* asset ); 
		virtual bool writeVisualScene ( const COLLADAFW::VisualScene* visualScene ); 
		virtual bool writeScene ( const COLLADAFW::Scene* scene ); 
		virtual bool writeLibraryNodes( const COLLADAFW::LibraryNodes* libraryNodes ); 
		virtual bool writeGeometry ( const COLLADAFW::Geometry* geometry ); 
		virtual bool writeMaterial( const COLLADAFW::Material* material ); 
		virtual bool writeEffect( const COLLADAFW::Effect* effect ); 
		virtual bool writeCamera( const COLLADAFW::Camera* camera ); 
		virtual bool writeImage( const COLLADAFW::Image* image ); 
		virtual bool writeLight( const COLLADAFW::Light* light ); 
		virtual bool writeAnimation( const COLLADAFW::Animation* animation ); 
		virtual bool writeAnimationList( const COLLADAFW::AnimationList* animationList ); 
		virtual bool writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData ); 
		virtual bool writeController( const COLLADAFW::Controller* Controller ); 
		virtual bool writeFormulas( const COLLADAFW::Formulas* formulas ){return true;} 
		virtual bool writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene ){return true;}

	protected:

		void CreateUniqueIdNodeMap( const COLLADAFW::VisualScene* visualScene ); 
		void CreateUniqueIdNodeMap( const COLLADAFW::NodePointerArray& nodes ); 
		void CreateUniqueIdNodeMap( COLLADAFW::Node* node );
		void CreateUniqueIdGeoInfoMap( const COLLADAFW::VisualScene* visualScene );
		void CreateUniqueIdGeoInfoMap( const COLLADAFW::NodePointerArray& nodes ); 
		void CreateUniqueIdGeoInfoMap( COLLADAFW::Node* node );
		bool FindImageNameByUniqueId( const COLLADAFW::UniqueId& id , String& name );

	protected: //工具函数

		Math::Matrix4 ColladaMatrix4ToMyMatrix4( const COLLADABU::Math::Matrix4& mat );

	protected:

		Mesh*					m_pMesh;    ///>待载入模型实例
		MESHREADER_PASS m_readerPass; 
		COLLADABU::URI m_uriMeshPath;
		visualSceneList_t  m_visualScenes;	
		uniqueIdNodeMap m_uniqueIdNodeMap;
		uniqueIdGeoInfoMap_t m_uniqueIdGeoInfoMap;
		nodeInfoStack_t    m_nodeInfoStack;
		uniqueIdImageNameMap_t m_uniqueIdImgNameMap;
		uniqueIdMaterialMap_t m_uniqueIdMaterialMap;
		uniqueIdEffectMap_t m_uniqueIdEffectMap;
	};




}//namespace Resource

#endif //ZP_COLLADAMESHREADER