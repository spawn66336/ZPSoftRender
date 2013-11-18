#include "ClipMapLevel.h"

namespace Terrain
{


	ClipMapLevel::ClipMapLevel(void):
	m_uiLevel(0),
	m_uiClipMapSize(0),
	m_uiGridSize(0),
	m_fGridWidth(0.0f),
	m_pVerts(NULL),
	m_uiFlag(0)
{

}
 
ClipMapLevel::~ClipMapLevel(void)
{
	//�ͷŶ���
	ZP_SAFE_DELETE_BUFFER( m_pVerts );

	//�ͷ�����
	for( int i = 0 ; i < 12 ; i++ )
	{
		ZP_SAFE_DELETE_BUFFER( m_ppTilesIndices[i] );
	}
	ZP_SAFE_DELETE_BUFFER( m_ppTilesIndices );
}
 
void ClipMapLevel::Init( const unsigned int uiLevel ,  const unsigned int uiClipMapSize ,  const float fGridWidth )
{
	m_uiLevel = uiLevel;
	m_uiClipMapSize = uiClipMapSize;
	m_uiGridSize = 1<<uiLevel;
	m_fGridWidth = fGridWidth*( (float)m_uiGridSize );
    m_heightMapBlock.Init( uiLevel , uiClipMapSize );
	//��ʼ������
	_InitVerts();
	//��ʼ��12���ӿ�����
	_InitTilesIndices();
}

void ClipMapLevel::_InitVerts( void )
{
	m_pVerts = new TerrainVertex[m_uiClipMapSize*m_uiClipMapSize];
	//ʹ��ǰ�鶥�������λ�ö�׼ԭ��
	Math::Vec3 v3Origin( -m_fGridWidth*(m_uiClipMapSize>>1) , 0.0f , m_fGridWidth*(m_uiClipMapSize>>1) );
	Math::Vec3 v3Dx(m_fGridWidth,0.0f,0.0f);
	Math::Vec3 v3Dz( 0.0f,0.0f,-m_fGridWidth ); 
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
		{
			unsigned int offset = z*m_uiClipMapSize+x;
			m_pVerts[offset].m_pos = v3Origin + v3Dx*(float)x + v3Dz*(float)z;
			m_pVerts[offset].m_norm = Math::Vec3( 0.0f,1.0f,0.0f);
			m_pVerts[offset].m_color = Math::Vec4(1.0f,0.0f,0.0f,1.0f);
		}
	}
}

void ClipMapLevel::Update( const ClipMapGridPos& center )
{
	m_centerPos = center; 
	//ʹ�ø��¹��������������¼��㵱ǰ����
	m_currArea.minPos.x = m_centerPos.x - ( m_uiClipMapSize>>1 ) * m_uiGridSize ;
	m_currArea.minPos.z = m_centerPos.z - ( m_uiClipMapSize>>1 ) * m_uiGridSize ;
	m_currArea.maxPos.x = m_currArea.minPos.x + ( m_uiClipMapSize - 1 )*m_uiGridSize;
	m_currArea.maxPos.z = m_currArea.minPos.z + ( m_uiClipMapSize - 1 )*m_uiGridSize;
	
	bool bChange = 
		m_heightMapBlock.Update( m_currArea );

	//���б仯���¶���
	if( bChange )
	{
		_UpdateVerts();
		SetFlag( VERTS_CHANGE , true );
	}else{
		SetFlag( VERTS_CHANGE , false );
	}
}

void ClipMapLevel::_UpdateVerts( void )
{
	//ʹ�ø̸߳��������yֵ
	int stride = 1<<m_uiLevel;
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		int localz = m_currArea.minPos.z + stride*z;
		for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
		{
			unsigned int offset = z*m_uiClipMapSize+x;
			int localx = m_currArea.minPos.x + stride*x;		
			m_pVerts[offset].m_pos.y = m_heightMapBlock.Sample( localx , localz );
		}
	}
}

void ClipMapLevel::_InitTilesIndices( void )
{
	m_ppTilesIndices = new unsigned short*[12];
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	//��m-1��������ÿ��������2m+2������
	unsigned int indicesNum = (m-1)*(2*m+2);
	
	//Ϊ12���ӿ����ռ�
	for( int i = 0 ; i < 12 ; i++ )
	{
		m_ppTilesIndices[i] = new unsigned short[indicesNum];
	}

	unsigned int usTileStartIndices[12] = {};
	usTileStartIndices[0] = 0;
	usTileStartIndices[1] = m-1;
	usTileStartIndices[2] = 2*(m-1)+2;
	usTileStartIndices[3] = 3*(m-1)+2;

	usTileStartIndices[4] = (m-1)*m_uiClipMapSize;
	usTileStartIndices[5] = (m-1)*(m_uiClipMapSize)+3*(m-1)+2;

	usTileStartIndices[6] = (2*(m-1)+2)*(m_uiClipMapSize);
	usTileStartIndices[7] = (2*(m-1)+2)*(m_uiClipMapSize)+3*(m-1)+2;

	usTileStartIndices[8] = (3*(m-1)+2)*m_uiClipMapSize;
	usTileStartIndices[9] = (3*(m-1)+2)*m_uiClipMapSize+m-1;
	usTileStartIndices[10] = (3*(m-1)+2)*m_uiClipMapSize+2*(m-1)+2;
	usTileStartIndices[11] = (3*(m-1)+2)*m_uiClipMapSize+3*(m-1)+2;

	//����12���ӿ�����
	for( int i = 0 ; i < 12 ; i++ )
	{
		unsigned int j = 0;
		unsigned int uiCurrTileStartIndx = usTileStartIndices[i];
		unsigned short* pCurrTileIndices = m_ppTilesIndices[i];

		for( unsigned int iStrip = 0 ; iStrip < m-1 ; iStrip++ )
		{  
			pCurrTileIndices[j++] = uiCurrTileStartIndx+(iStrip+1)*m_uiClipMapSize;
			for( unsigned int iVert = 0 ; iVert <= m-1 ; iVert++ )
			{
				pCurrTileIndices[j++] = uiCurrTileStartIndx+(iStrip+1)*m_uiClipMapSize+iVert;
				pCurrTileIndices[j++] = uiCurrTileStartIndx+(iStrip)*m_uiClipMapSize+iVert;
			}
			pCurrTileIndices[j++] = uiCurrTileStartIndx+(iStrip)*m_uiClipMapSize+m-1;
		} 

		ZP_ASSERT( j == indicesNum );
	}//for( int i = 0 ; i < 12 ; i++ )
}

Math::Vec3 ClipMapLevel::GetLocalPos( void ) const
{
	return Math::Vec3( ((float)m_centerPos.x) * m_fGridWidth , 0.0f , ((float)m_centerPos.z)*m_fGridWidth );
}

bool ClipMapLevel::TestFlag( CLIPMAPLEVEL_FLAG bit )
{
	return m_uiFlag&bit;
}

void ClipMapLevel::SetFlag( CLIPMAPLEVEL_FLAG bit , bool b )
{
	if( b )
	{
		m_uiFlag |= bit;
	}else{
		m_uiFlag &= ~bit;
	}
}


unsigned int ClipMapLevel::GetVertsNum( void ) const
{
	return m_uiClipMapSize*m_uiClipMapSize;
}

unsigned int ClipMapLevel::GetIndicesNumPerTile( void ) const
{
		unsigned int m = ( m_uiClipMapSize+1 )>>2;
		return (m-1)*(2*m+2);
}

unsigned int ClipMapLevel::GetPrimtiveNumPerTile( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )>>2;
	return (m-2)*4 //�������˻�������
			   +2			//��β��һ���˻�������
			   +(m-1)*(m-1)*2; 
}





}//namespace Terrain