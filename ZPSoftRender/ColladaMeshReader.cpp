#include "ColladaMeshReader.h"

#include "COLLADASaxFWLLoader.h"

#include "COLLADAFWRoot.h"
#include "COLLADAFWGeometry.h"
#include "COLLADAFWNode.h"
#include "COLLADAFWVisualScene.h"
#include "COLLADAFWInstanceGeometry.h"
#include "COLLADAFWMesh.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "ColladaSubMeshReader.h"
#include "TextureManager.h"
#include "IRender.h"
#include "Material.h"

namespace Resource
{ 

	ColladaMeshReader::ColladaMeshReader( const COLLADABU::URI& path  , Mesh* pMesh ):
	m_pMesh(pMesh)
	{
		m_uriMeshPath = path;
	} 

	ColladaMeshReader::~ColladaMeshReader(void)
	{
		visualSceneList_t::iterator itVisualScenePointer = m_visualScenes.begin();
		while( itVisualScenePointer != m_visualScenes.end() )
		{
			ZP_SAFE_DELETE(*itVisualScenePointer);
			itVisualScenePointer++;
		}
		m_visualScenes.clear();
	}

	bool ColladaMeshReader::Load(void)
	{
		if( NULL == m_pMesh )
		{
			return false;
		}

		COLLADASaxFWL::Loader loader;
		COLLADAFW::Root root( &loader, this );

		m_readerPass = SCENE_INFO_PASS;

		// Load scene graph 
		if ( !root.loadDocument( m_uriMeshPath.toNativePath() ) )
		{ 
			return false;
		}

		m_readerPass = GEOMETRY_PASS;
		root.loadDocument( m_uriMeshPath.toNativePath() );
		
		return true;
	}

	void ColladaMeshReader::cancel( const String& errorMessage )
	{

	}

	void ColladaMeshReader::start()
	{

	}

	void ColladaMeshReader::finish()
	{

	}

	bool ColladaMeshReader::writeGlobalAsset( const COLLADAFW::FileInfo* asset )
	{
		return true;
	}

	bool ColladaMeshReader::writeVisualScene( const COLLADAFW::VisualScene* visualScene )
	{
		if( m_readerPass != SCENE_INFO_PASS )
		{
			return true;
		}
		CreateUniqueIdNodeMap( visualScene );
		CreateUniqueIdGeoInfoMap( visualScene ); 
		return true;
	}

	bool ColladaMeshReader::writeScene( const COLLADAFW::Scene* scene )
	{ 
		if( m_readerPass != SCENE_INFO_PASS )
		{
			return true;
		}
		return true;
	}

	bool ColladaMeshReader::writeLibraryNodes( const COLLADAFW::LibraryNodes* libraryNodes )
	{
		if( m_readerPass != SCENE_INFO_PASS )
		{
			return true;
		}
		return true;
	}

	bool ColladaMeshReader::writeGeometry( const COLLADAFW::Geometry* geometry )
	{
		if( m_readerPass != GEOMETRY_PASS )
		{
			return true;
		}

		if( geometry->getType() != COLLADAFW::Geometry::GEO_TYPE_MESH )
		{
			return true;
		}

		ColladaSubMeshReader subMeshReader( (COLLADAFW::Mesh*)geometry );
		Resource::SubMesh* pNewSubMesh =  subMeshReader.Load();

		if( pNewSubMesh )
		{
			uniqueIdGeoInfoMap_t::iterator itFindGeoInfo = 
			m_uniqueIdGeoInfoMap.find( geometry->getUniqueId() ); 
			if( itFindGeoInfo != m_uniqueIdGeoInfoMap.end() )
			{
				//设置本地到世界坐标转换矩阵
				COLLADABU::Math::Matrix4 localToWorldMatrix = 
					itFindGeoInfo->second.worldMatrix;

				pNewSubMesh->SetMatrix( 
							ColladaMatrix4ToMyMatrix4(localToWorldMatrix) *
							Math::Matrix4( 
										1.0f , 0.0f , 0.0f , 0.0f ,
										0.0f , 0.0f , 1.0f , 0.0f ,
										0.0f , 1.0f , 0.0f , 0.0f ,
										0.0f , 0.0f , 0.0f , 1.0f 
										)
							);

				//获取材质信息
				COLLADAFW::InstanceGeometry* pInstGeo = itFindGeoInfo->second.fwInstanceGeometry;
				COLLADAFW::MaterialBindingArray& materialBindArray = pInstGeo->getMaterialBindings();
				if( 0 != materialBindArray.getCount() )
				{
					COLLADAFW::MaterialBinding materialBinding = materialBindArray[0];
					uniqueIdMaterialMap_t::iterator itFindMaterial = 
						m_uniqueIdMaterialMap.find( materialBinding.getReferencedMaterial() );

					if( itFindMaterial != m_uniqueIdMaterialMap.end() )
					{
						COLLADAFW::Material material = itFindMaterial->second;
						uniqueIdEffectMap_t::iterator itFindEffect = 
								m_uniqueIdEffectMap.find( material.getInstantiatedEffect() );
						if( itFindEffect != m_uniqueIdEffectMap.end() )
						{
							COLLADAFW::Effect effect = itFindEffect->second;
							
							if( effect.getCommonEffects().getCount() )
							{
								COLLADAFW::EffectCommon* pCommonEffect = 
									effect.getCommonEffects()[0];

								COLLADAFW::SamplerPointerArray& samplerPointerArray = 
									pCommonEffect->getSamplerPointerArray();

								//提取漫反射纹理
								if( pCommonEffect->getDiffuse().isTexture() )
								{ 
									COLLADAFW::Sampler* pSampler = 
										samplerPointerArray[ pCommonEffect->getDiffuse().getTexture().getSamplerId() ];
									String strImageName;
									if( FindImageNameByUniqueId( pSampler->getSourceImage() , strImageName ) )
									{
										Texture2D* pTexture2D = 
											Resource::TextureManager::GetInstance()->CreateOrRetrieveTexture2DFromDefaultLocation( strImageName );
										pNewSubMesh->GetMaterial()->SetTexture( pTexture2D , Render::DIFFUSE_CH );
									}
								}//if( pCommonEffect->getDiffuse().isTexture() )

								if( pCommonEffect->getSpecular().isTexture() )
								{
									COLLADAFW::Sampler* pSampler = 
										samplerPointerArray[ pCommonEffect->getSpecular().getTexture().getSamplerId() ];
									String strImageName;
									if( FindImageNameByUniqueId( pSampler->getSourceImage() , strImageName ) )
									{
										Texture2D* pTexture2D = 
											Resource::TextureManager::GetInstance()->CreateOrRetrieveTexture2DFromDefaultLocation( strImageName );
										pNewSubMesh->GetMaterial()->SetTexture( pTexture2D , Render::BUMPMAP_CH );
									}
								}//if( pCommonEffect->getSpecular().isTexture() )
							}
						}
					}
				} 
			} 


			if( !m_pMesh->AddSubMesh( pNewSubMesh ) )
			{
				ZP_SAFE_DELETE( pNewSubMesh );
			}
		} 
		return true;
	}

	bool ColladaMeshReader::writeMaterial( const COLLADAFW::Material* material )
	{
		if( m_readerPass != SCENE_INFO_PASS )
		{
			return true;
		}

		m_uniqueIdMaterialMap.insert( 
			std::make_pair( material->getUniqueId() , *material ) );
		
		return true;
	}

	bool ColladaMeshReader::writeEffect( const COLLADAFW::Effect* effect )
	{
		if( m_readerPass != SCENE_INFO_PASS )
		{
			return true;
		}
		m_uniqueIdEffectMap.insert( 
			std::make_pair( effect->getUniqueId() , *effect ) );
		return true;
	}

	bool ColladaMeshReader::writeCamera( const COLLADAFW::Camera* camera )
	{
		if( m_readerPass != SCENE_INFO_PASS )
		{
			return true;
		}  
		return true;
	}

	bool ColladaMeshReader::writeImage( const COLLADAFW::Image* image )
	{
		if( m_readerPass != SCENE_INFO_PASS )
		{
			return true;
		}

		m_uniqueIdImgNameMap.insert( 
				std::make_pair( image->getUniqueId() , image->getImageURI().getPathFile() ) 
			);

		return true;
	}

	bool ColladaMeshReader::writeLight( const COLLADAFW::Light* light )
	{
		if( m_readerPass != SCENE_INFO_PASS )
		{
			return true;
		}

		return true;
	}

	bool ColladaMeshReader::writeAnimation( const COLLADAFW::Animation* animation )
	{
		return true;
	}

	bool ColladaMeshReader::writeAnimationList( const COLLADAFW::AnimationList* animationList )
	{
		return true;
	}

	bool ColladaMeshReader::writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData )
	{
		return true;
	}

	bool ColladaMeshReader::writeController( const COLLADAFW::Controller* Controller )
	{
		return true;
	}

	void ColladaMeshReader::CreateUniqueIdNodeMap( const COLLADAFW::VisualScene* visualScene )
	{
		if( NULL == visualScene )
		{
			return;
		} 
		CreateUniqueIdNodeMap( visualScene->getRootNodes() );
	}

	void ColladaMeshReader::CreateUniqueIdNodeMap( const COLLADAFW::NodePointerArray& nodes )
	{ 
		for ( size_t i = 0, cout = nodes.getCount(); i < cout; ++i)
		{
			COLLADAFW::Node* node = nodes[i];
			CreateUniqueIdNodeMap( node );
		}
	}

	void ColladaMeshReader::CreateUniqueIdNodeMap( COLLADAFW::Node* node )
	{
		m_uniqueIdNodeMap[node->getUniqueId()] = node;
		CreateUniqueIdNodeMap( node->getChildNodes() );
	}

	void ColladaMeshReader::CreateUniqueIdGeoInfoMap( const COLLADAFW::VisualScene* visualScene )
	{
		NodeInfo nodeInfo;
		nodeInfo.matrix = COLLADABU::Math::Matrix4::IDENTITY;
		m_nodeInfoStack.push( nodeInfo ); 
		CreateUniqueIdGeoInfoMap( visualScene->getRootNodes() );
	}

	void ColladaMeshReader::CreateUniqueIdGeoInfoMap( const COLLADAFW::NodePointerArray& nodes )
	{ 
		for( size_t iNode = 0 ; iNode < nodes.getCount() ; iNode++ )
		{
			CreateUniqueIdGeoInfoMap( nodes[iNode] );
		}

	}

	void ColladaMeshReader::CreateUniqueIdGeoInfoMap( COLLADAFW::Node* node )
	{
		COLLADABU::Math::Matrix4 parentMatrix = m_nodeInfoStack.top().matrix;
		COLLADABU::Math::Matrix4 localToWorldMatrix = parentMatrix * node->getTransformationMatrix();
		
		NodeInfo nodeInfo;
		nodeInfo.matrix = localToWorldMatrix;
		m_nodeInfoStack.push( nodeInfo );

		CreateUniqueIdGeoInfoMap( node->getChildNodes() );

	     COLLADAFW::InstanceGeometryPointerArray& instanceGeos =
			 node->getInstanceGeometries();

		 for( size_t iGeo = 0 ; iGeo < instanceGeos.getCount() ; iGeo++ )
		 {
			 COLLADAFW::InstanceGeometry* pInstGeo = instanceGeos[iGeo];
			 InstanceGeometryInfo geoInfo( pInstGeo , localToWorldMatrix );
			 m_uniqueIdGeoInfoMap.insert( 
									 std::make_pair(
									 pInstGeo->getInstanciatedObjectId() , 
									 geoInfo )
									 );
		 }

		m_nodeInfoStack.pop();
	}

	Math::Matrix4 ColladaMeshReader::ColladaMatrix4ToMyMatrix4( 
		const COLLADABU::Math::Matrix4& mat )
	{
		Math::Matrix4 rs;
		for( int iRow = 0 ; iRow < 4 ; iRow++ )
		{
			for( int iCol = 0 ; iCol < 4 ; iCol++ )
			{
				rs.m[iCol][iRow] = (Real)mat[iRow][iCol];
			}
		}
		return rs;
	}

	bool ColladaMeshReader::FindImageNameByUniqueId( const COLLADAFW::UniqueId& id , String& name )
	{
		uniqueIdImageNameMap_t::iterator itFindImageName = 
							m_uniqueIdImgNameMap.find(id);
		if( itFindImageName != m_uniqueIdImgNameMap.end() )
		{
			name = itFindImageName->second;
			return true;
		}
		return false;
	}

}//namespace Resource