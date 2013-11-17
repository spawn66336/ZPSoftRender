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

	protected:
		String m_strMapName;										//地图路径
		FILE*   m_pDemFile;	
		ClipMapDemFileHeader_t* m_pFileHeader;	//文件头
	};

}//namespace Terrain
#endif