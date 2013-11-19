#include "ClipMapReader.h"

namespace Terrain
{


ClipMapReader::ClipMapReader(void):
m_pDemFile(NULL),
m_pFileHeader(NULL),
m_pHeightMapData(NULL)
{
}


ClipMapReader::~ClipMapReader(void)
{
	ZP_ASSERT( NULL == m_pDemFile );
}

void ClipMapReader::Init( const String& strMapName )
{
	if( m_pDemFile )
	{
		return;
	}

	m_strMapName = strMapName; 
	if( 0 != fopen_s( &m_pDemFile , strMapName.c_str() , "rb" ) )
	{
		ZP_ASSERT(0);
		return;
	}
	ZP_ASSERT( NULL != m_pDemFile ); 

	int iHeadSize = 0;
	fread( &iHeadSize , sizeof(iHeadSize) , 1 , m_pDemFile );
	m_pFileHeader = (ClipMapDemFileHeader_t*)malloc( iHeadSize );
	ZP_ASSERT( NULL != m_pFileHeader );
	fseek( m_pDemFile , 0 , SEEK_SET);
	fread( (void*)m_pFileHeader , 1 , iHeadSize , m_pDemFile );
	
	//计算高度图数据大小
	unsigned int uiBufSize = 
		m_pFileHeader->img_width*
		m_pFileHeader->img_height ;

	m_pHeightMapData = new float[uiBufSize];
	memset( m_pHeightMapData , 0 , sizeof(float)*uiBufSize );
	ZP_ASSERT( NULL != m_pHeightMapData );
	//定位到第0层
	fseek( m_pDemFile , m_pFileHeader->mipmap_level_offset[0] , SEEK_SET);
	//读取高程图
	fread( m_pHeightMapData , sizeof(unsigned int) , uiBufSize , m_pDemFile );
}

void ClipMapReader::Destroy( void )
{
	//销毁高程图
	ZP_SAFE_DELETE_BUFFER( m_pHeightMapData );

	if( m_pFileHeader )
	{
		free((void*)m_pFileHeader);
		m_pFileHeader = NULL;
	}
	if( m_pDemFile )
	{
		fclose( m_pDemFile );
		m_pDemFile = NULL;
	}
}

int ClipMapReader::GetHeightMapWidth( void ) const
{
	return m_pFileHeader->img_width;
}

int ClipMapReader::GetHeightMapHeight( void ) const
{
	return m_pFileHeader->img_height;
}

int ClipMapReader::GetHeightMapMaxHeight( void ) const
{
	return m_pFileHeader->max_height;
}

int ClipMapReader::GetHeightMapMinHeight( void ) const
{
	return m_pFileHeader->min_height;
}

float ClipMapReader::Sample( const int x , const int z )
{
	//若超出范围返回0高程
	if( x < 0 || x >= GetHeightMapWidth() ||
		z < 0 || z >= GetHeightMapHeight() )
		return 0;

	return m_pHeightMapData[ z * GetHeightMapWidth() + x ] - (float)GetHeightMapMinHeight();
}

}