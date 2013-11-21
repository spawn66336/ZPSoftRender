#ifndef ZP_CLIPMAP_READER
#define ZP_CLIPMAP_READER

#include "ClipMapTerrainDefine.h"
#include "Singleton.h"

namespace Terrain
{ 

	class ClipMapReader : public Util::SingletonT<ClipMapReader>
	{
	public:
		ClipMapReader(void);
		virtual ~ClipMapReader(void);

		void Init( const String& strMapName );
		void Destroy( void );

		int GetHeightMapWidth( void ) const;
		int GetHeightMapHeight( void ) const;
		int GetHeightMapMaxHeight( void ) const;
		int GetHeightMapMinHeight( void ) const;

		bool Sample( const int x , const int z , float& sample );
		bool SampleNormal( const int x , const int z , Math::Vec3& norm);

	protected:
		void _CalcNormalMap( void );

	protected:
		String m_strMapName;										//地图路径
		FILE*   m_pDemFile;	
		ClipMapDemFileHeader_t* m_pFileHeader;	//文件头
		float* m_pHeightMapData;			//高程图
		Math::Vec3* m_pNormalMap;		//由高程求出的法线图
	};

}//namespace Terrain
#endif