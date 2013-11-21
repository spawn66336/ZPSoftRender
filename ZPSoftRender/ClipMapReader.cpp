#include "ClipMapReader.h"

namespace Terrain
{


ClipMapReader::ClipMapReader(void):
m_pDemFile(NULL),
m_pFileHeader(NULL),
m_pHeightMapData(NULL),
m_pNormalMap(NULL)
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

	m_pNormalMap = new Math::Vec3[uiBufSize];
	//从高程图中计算法线图
	_CalcNormalMap();
}

void ClipMapReader::Destroy( void )
{
	//销毁高程图
	ZP_SAFE_DELETE_BUFFER( m_pHeightMapData );
	//销毁法线图
	ZP_SAFE_DELETE_BUFFER( m_pNormalMap );

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

bool ClipMapReader::Sample( const int x , const int z , float& sample )
{
	//若超出范围返回0高程
	if( x < 0 || x >= GetHeightMapWidth() ||
		z < 0 || z >= GetHeightMapHeight() )
	{
		sample = 0.0f;
		return false;
	}
	sample =  ( m_pHeightMapData[ z * GetHeightMapWidth() + x ] - (float)GetHeightMapMinHeight() )*0.1f;
	return true;
}

bool ClipMapReader::SampleNormal( const int x , const int z , Math::Vec3& norm )
{
	//若超出范围返回0高程
	if( x < 0 || x >= GetHeightMapWidth() ||
		z < 0 || z >= GetHeightMapHeight() )
	{ 
		norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		return false;
	}
	norm = m_pNormalMap[z * GetHeightMapWidth() + x ];
	return true;
}

void ClipMapReader::_CalcNormalMap( void )
{
	unsigned int uiWidth = m_pFileHeader->img_width;
	unsigned int uiHeight = m_pFileHeader->img_height;

	for( unsigned int z = 0 ; z < uiHeight ; z++ )
	{
		for( unsigned int x = 0 ; x < uiWidth ; x++ )
		{
			unsigned int uiOffset = z*uiWidth+x;
			
			float fCurrHeight = 0.0f;
			float fHeight0 = 0.0f; // x+1
			float fHeight1 = 0.0f; // x-1
			float fHeight2 = 0.0f; // z+1
			float fHeight3 = 0.0f; // z-1

			Sample( x , z , fCurrHeight );
			if(!Sample( x+1 , z , fHeight0 ) )
			{
				fHeight0 = fCurrHeight;
			}
			if(!Sample(x-1,z,fHeight1) )
			{
				fHeight1 = fCurrHeight;
			}
			if(!Sample(x,z+1,fHeight2))
			{
				fHeight2 = fCurrHeight;
			}
			if(!Sample(x,z-1,fHeight3))
			{
				fHeight3 = fCurrHeight;
			}

			Math::Vec3 v3V0(  4.0f , fHeight0 - fCurrHeight , 0.0f );
			Math::Vec3 v3V1(  -4.0f , fHeight1 - fCurrHeight , 0.0f );
			Math::Vec3 v3V2(  0.0f , fHeight2 - fCurrHeight , 4.0f );
			Math::Vec3 v3V3( 0.0f , fHeight3 - fCurrHeight , -4.0f );

			Math::Vec3 v3Norm0 = v3V0.CrossProduct( v3V2 );
			Math::Vec3 v3Norm1 = v3V2.CrossProduct( v3V1 );
			Math::Vec3 v3Norm2 = v3V1.CrossProduct( v3V3 );
			Math::Vec3 v3Norm3 = v3V3.CrossProduct( v3V0 );
		 
			m_pNormalMap[uiOffset] = ( v3Norm0 + v3Norm1 + v3Norm2 + v3Norm3 )*-0.25f;
			m_pNormalMap[uiOffset].Normalize();
		}
	}
}



}