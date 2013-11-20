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

	protected:
		String m_strMapName;										//��ͼ·��
		FILE*   m_pDemFile;	
		ClipMapDemFileHeader_t* m_pFileHeader;	//�ļ�ͷ
		float* m_pHeightMapData;
	};

}//namespace Terrain
#endif