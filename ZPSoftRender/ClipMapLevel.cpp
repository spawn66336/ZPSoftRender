#include "ClipMapLevel.h"

namespace Terrain
{


	ClipMapLevel::ClipMapLevel(void):
	m_uiLevel(0),
	m_uiClipMapSize(0),
	m_uiGridSize(0),
	m_fCenterHeight(0),
	m_fGridWidth(0.0f),
	m_pVerts(NULL),
	m_pFixedRingVerts(NULL),
	m_ppTilesIndices(NULL),
	m_pGapTileIndices(NULL),
	m_pCenterTileIndices(NULL),
	m_pLeftTopLTileIndices(NULL),
	m_pLeftBottomLTileIndices(NULL),
	m_pRightTopTileIndices(NULL),
	m_pRightBottomTileIndices(NULL),
	m_pFixedRingIndices(NULL),
	m_pNextLevel(NULL),
	m_uiFlag( SHOW_OUTER_TILES|
					SHOW_GAP_TILES| 
				    SHOW_FIXED_UP_RING )
{

}
 
ClipMapLevel::~ClipMapLevel(void)
{
	//�ͷŶ���
	ZP_SAFE_DELETE_BUFFER( m_pVerts );
	//�ͷ��޲�������
	ZP_SAFE_DELETE_BUFFER( m_pFixedRingVerts );

	if( m_ppTilesIndices )
	{
		//�ͷ�����
		for( int i = 0 ; i < 12 ; i++ )
		{
			ZP_SAFE_DELETE_BUFFER( m_ppTilesIndices[i] );
		}
		ZP_SAFE_DELETE_BUFFER( m_ppTilesIndices );
	}

	ZP_SAFE_DELETE_BUFFER( m_pGapTileIndices );
	ZP_SAFE_DELETE_BUFFER( m_pCenterTileIndices );

	ZP_SAFE_DELETE_BUFFER( m_pLeftTopLTileIndices );
	ZP_SAFE_DELETE_BUFFER( m_pLeftBottomLTileIndices );
	ZP_SAFE_DELETE_BUFFER( m_pRightTopTileIndices );
	ZP_SAFE_DELETE_BUFFER( m_pRightBottomTileIndices );

	ZP_SAFE_DELETE_BUFFER( m_pFixedRingIndices );
}
 
void ClipMapLevel::Init( const unsigned int uiLevel , const unsigned int uiClipMapSize ,  const float fGridWidth )
{
 

	m_uiLevel = uiLevel;
	m_uiClipMapSize = uiClipMapSize;
	m_uiGridSize = 1<<uiLevel;
	m_fGridWidth = fGridWidth*( (float)m_uiGridSize );
    m_heightMapBlock.Init( uiLevel , uiClipMapSize );
	//��ʼ������
	_InitVerts();
	//��ʼ���޲�������
	_InitFixedRingVerts();
	//��ʼ��12���ӿ�����
	_InitTilesIndices();
	//��ʼ����������4���޲�������
	_InitGapTilesIndices();
	//�������Ŀ������
	_InitCenterTileIndices();
	//��ʼ��L���޲�������
	_InitLFixedTileIndices();

	_InitFixedRingIndices();

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
			m_pVerts[offset].m_color = 0xffffffff;
		}
	}
}


void ClipMapLevel::_InitFixedRingVerts( void )
{
	unsigned int uiVertNum = m_uiClipMapSize*4 //��Ȧ
											  +( ((m_uiClipMapSize-1)>>1) + 1)*4; //��Ȧ�ֲڲ�
	m_pFixedRingVerts = new TerrainVertex[uiVertNum];

	Math::Vec3 v3Origin( -m_fGridWidth*(m_uiClipMapSize>>1) , 0.0f , m_fGridWidth*(m_uiClipMapSize>>1) );
	Math::Vec3 v3Dx(m_fGridWidth,0.0f,0.0f);
	Math::Vec3 v3Dz( 0.0f,0.0f,-m_fGridWidth ); 

	unsigned int j = 0;
	//�ڻ����� �������ң�
	for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)x);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//�ڻ��Ҳ� (���ϵ���)
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)(m_uiClipMapSize-1)) + v3Dz*((float)z);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//�ڻ����棨�����ң�
	for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)x) + v3Dz*((float)(m_uiClipMapSize-1));
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//�ڻ���ࣨ���ϵ��£�
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dz*((float)z);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//�⻷���棨�����ң�
	for( unsigned int x = 0 ; x <  ((m_uiClipMapSize-1)>>1)+1 ; x++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)x*2);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//�⻷���棨���ϵ��£�
	for( unsigned int z = 0 ; z <  ((m_uiClipMapSize-1)>>1)+1 ; z++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)(m_uiClipMapSize-1)) + v3Dz*((float)z*2);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//�⻷���棨�����ң�
	for( unsigned int x = 0 ; x < ((m_uiClipMapSize-1)>>1)+1 ; x++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)x*2) + v3Dz*((float)(m_uiClipMapSize-1));
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//�⻷��ࣨ���ϵ��£�
	for( unsigned int z = 0 ; z <  ((m_uiClipMapSize-1)>>1)+1 ; z++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin +  v3Dz*((float)z*2);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	ZP_ASSERT( j == uiVertNum );
}


void ClipMapLevel::UpdateHeightMap( const ClipMapGridPos& center )
{
	m_centerPos = center; 
	//ʹ�ø��¹��������������¼��㵱ǰ����
	m_currArea.minPos.x = m_centerPos.x - ( m_uiClipMapSize>>1 ) * m_uiGridSize ;
	m_currArea.minPos.z = m_centerPos.z - ( m_uiClipMapSize>>1 ) * m_uiGridSize ;
	m_currArea.maxPos.x = m_currArea.minPos.x + ( m_uiClipMapSize - 1 )*m_uiGridSize;
	m_currArea.maxPos.z = m_currArea.minPos.z + ( m_uiClipMapSize - 1 )*m_uiGridSize;
	
	//���µ�ǰ��ĸ߳�ͼ
	bool bChange = 
		m_heightMapBlock.Update( m_currArea );

	if( bChange )
	{
		SetFlag( HEIGHTMAP_DIRTY , true );
	}
 
}


void ClipMapLevel::UpdateVerts( void )
{
	//���߶�ͼ�б���¶���
	if( TestFlag(HEIGHTMAP_DIRTY) )
	{
		_UpdateVerts();
		_UpdateFixedRingVerts();
		SetFlag( VERTS_CHANGE , true );
		SetFlag( HEIGHTMAP_DIRTY , false );
	}else{
		SetFlag( VERTS_CHANGE , false );
	}
}



void ClipMapLevel::_UpdateFixedRingVerts( void )
{
	//������Ȧ����߶�
	int stride = 1<<m_uiLevel;

	//������Ȧ�Ϸ�����߶� 
	for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
	{
		int localz = m_currArea.maxPos.z;
		int localx = m_currArea.minPos.x + stride*x;
		m_pFixedRingVerts[x].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel );
		m_pFixedRingVerts[x].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
	}
	//������Ȧ�ҷ�����߶�
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		int localx = m_currArea.minPos.x + stride*(m_uiClipMapSize-1);
		int localz = m_currArea.maxPos.z - stride*z;
		m_pFixedRingVerts[m_uiClipMapSize+z].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel );
		m_pFixedRingVerts[m_uiClipMapSize+z].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
	}
	//������Ȧ�·�����߶�
	for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
	{
		int localx = m_currArea.minPos.x + stride*x;
		int localz = m_currArea.minPos.z;
		m_pFixedRingVerts[2*m_uiClipMapSize+x].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel );
		m_pFixedRingVerts[2*m_uiClipMapSize+x].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
	}
	//������Ȧ��ඥ��߶�
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		int localx = m_currArea.minPos.x;
		int localz = m_currArea.maxPos.z - stride*z;
		m_pFixedRingVerts[3*m_uiClipMapSize+z].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel );
		m_pFixedRingVerts[3*m_uiClipMapSize+z].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
	}

	//��û����һ�ֲڼ����򲻸�����Ȧ
	if( !m_pNextLevel )
	{
		return;
	}

	unsigned int uiOutterVertsPerLine = ((m_uiClipMapSize-1)>>1) + 1;
	unsigned int uiOffset2OuterRing = m_uiClipMapSize*4; 
	unsigned int uiNextLevelStride = stride<<1;
	ClipMapHeightMapBlock& nextLevelHMBlockRef = m_pNextLevel->GetHeightMapBlock();
	
	//�����⻷����
	for( unsigned int x = 0 ; x < uiOutterVertsPerLine ; x++ )
	{
		int localx = m_currArea.minPos.x + uiNextLevelStride*x;
		int localz = m_currArea.maxPos.z;
		m_pFixedRingVerts[uiOffset2OuterRing+x].m_pos.y = 
			nextLevelHMBlockRef.Sample( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
		m_pFixedRingVerts[uiOffset2OuterRing+x].m_norm = m_heightMapBlock.SampleNormal( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
	}

	//�����⻷�Ҳ�
	for( unsigned int z = 0 ; z < uiOutterVertsPerLine ; z++ )
	{
		int localx = m_currArea.maxPos.x;
		int localz = m_currArea.maxPos.z - uiNextLevelStride*z;
		m_pFixedRingVerts[uiOffset2OuterRing+uiOutterVertsPerLine+z].m_pos.y = 
			nextLevelHMBlockRef.Sample( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
		m_pFixedRingVerts[uiOffset2OuterRing+uiOutterVertsPerLine+z].m_norm = 
			m_heightMapBlock.SampleNormal( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
	}

	//�����⻷����
	for( unsigned int x = 0 ; x < uiOutterVertsPerLine ; x++ )
	{
		int localx = m_currArea.minPos.x + uiNextLevelStride*x;
		int localz = m_currArea.minPos.z;
		m_pFixedRingVerts[uiOffset2OuterRing+2*uiOutterVertsPerLine+x].m_pos.y = 
			nextLevelHMBlockRef.Sample( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
		m_pFixedRingVerts[uiOffset2OuterRing+2*uiOutterVertsPerLine+x].m_norm = 
			m_heightMapBlock.SampleNormal( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
	}

	//�����⻷���
	for( unsigned int z = 0 ; z < uiOutterVertsPerLine ; z++ )
	{
		int localx = m_currArea.minPos.x;
		int localz = m_currArea.maxPos.z - uiNextLevelStride*z;
		m_pFixedRingVerts[uiOffset2OuterRing+3*uiOutterVertsPerLine+z].m_pos.y = 
			nextLevelHMBlockRef.Sample( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
		m_pFixedRingVerts[uiOffset2OuterRing+3*uiOutterVertsPerLine+z].m_norm = 
			m_heightMapBlock.SampleNormal( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
	}
}


void ClipMapLevel::_UpdateVerts( void )
{
	//ʹ�ø̸߳��������yֵ
	int stride = 1<<m_uiLevel;
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		int localz = m_currArea.maxPos.z - stride*z;
		for(  unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
		{
			unsigned int offset = z*m_uiClipMapSize+x;

			int localx = m_currArea.minPos.x + stride*x;		
			m_pVerts[offset].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel ); 
			m_pVerts[offset].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
		}
	}

	//������������߶�
	m_fCenterHeight = m_heightMapBlock.Sample( m_centerPos.x>>m_uiLevel , m_centerPos.z>>m_uiLevel );
 
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


void ClipMapLevel::_InitGapTilesIndices( void )
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	unsigned int indicesNum = ( ( m*2 ) + 2 )*2 * 4;
	m_pGapTileIndices = new unsigned short[indicesNum ];
	int j = 0;

	//�Ϸ����޲���
	unsigned int iStartIndex = 2*( m-1 );
	for( unsigned int iStrip = 0 ; iStrip < 2  ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip;
		m_pGapTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < m ; i++ )
		{
			m_pGapTileIndices[j++] = iStripStartIndex + i * m_uiClipMapSize;
			m_pGapTileIndices[j++] = iStripStartIndex + i * m_uiClipMapSize + 1;
		}
		m_pGapTileIndices[j++] = iStripStartIndex + ( m - 1 ) * m_uiClipMapSize + 1;
	}


	//�·����޲���
	iStartIndex = 2*( m-1 ) + ( (m-1)*3+2 )*m_uiClipMapSize;
	for( unsigned int iStrip = 0 ; iStrip < 2 ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip;
		m_pGapTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < m ; i++ )
		{
			m_pGapTileIndices[j++] = iStripStartIndex + i * m_uiClipMapSize;
			m_pGapTileIndices[j++] = iStripStartIndex + i * m_uiClipMapSize + 1;
		}
		m_pGapTileIndices[j++] = iStripStartIndex + ( m - 1 ) * m_uiClipMapSize + 1;
	}

	//�󷽵��޲���
	iStartIndex = ( (m-1)*2+1 )*m_uiClipMapSize;
	for( unsigned int iStrip = 0 ; iStrip < 2 ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip*m_uiClipMapSize;
		m_pGapTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < m ; i++ )
		{
			m_pGapTileIndices[j++] = iStripStartIndex + i;
			m_pGapTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
		}
		m_pGapTileIndices[j++] = iStripStartIndex + m - 1 - m_uiClipMapSize;
	}

	//�ҷ����޲���
	iStartIndex = ( (m-1)*2+1 )*m_uiClipMapSize + 3*(m-1)+2;
	for( unsigned int iStrip = 0 ; iStrip < 2 ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip*m_uiClipMapSize;
		m_pGapTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < m ; i++ )
		{
			m_pGapTileIndices[j++] = iStripStartIndex + i;
			m_pGapTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
		}
		m_pGapTileIndices[j++] = iStripStartIndex + m - 1 - m_uiClipMapSize;
	}

	ZP_ASSERT( indicesNum == j );
}


void ClipMapLevel::_InitCenterTileIndices( void )
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	unsigned int indicesNum = ( ( 2*m+1 )*2+2 ) * //ÿ�������������� 
												( ( m - 1) *2 + 2); //������

	m_pCenterTileIndices = new unsigned short[indicesNum];

	int j = 0;
	unsigned int iStartIndex = m*m_uiClipMapSize + m-1;

	for( unsigned int iStrip = 0 ; iStrip < ( ( m - 1) *2 + 2 ) ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip*m_uiClipMapSize;

		m_pCenterTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
		{
			m_pCenterTileIndices[j++] = iStripStartIndex + i;
			m_pCenterTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
		}
		m_pCenterTileIndices[j++] = iStripStartIndex + 2*(m-1) + 2 - m_uiClipMapSize;
	}
	ZP_ASSERT( j == indicesNum );
}

void ClipMapLevel::_InitLFixedTileIndices( void )
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	unsigned int indicesPerLTile = (2*m+1)*2+2 + 2*m*2+2;

	m_pLeftTopLTileIndices = new unsigned short[indicesPerLTile];
	m_pLeftBottomLTileIndices = new unsigned short[indicesPerLTile];
	m_pRightTopTileIndices = new unsigned short[indicesPerLTile];
	m_pRightBottomTileIndices = new unsigned short[indicesPerLTile];

	//�������� 
	unsigned int j = 0;
	unsigned int iStripStartIndex = m*m_uiClipMapSize+m-1;
	m_pLeftTopLTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
	{
		m_pLeftTopLTileIndices[j++] = iStripStartIndex + i;
		m_pLeftTopLTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
	}
	m_pLeftTopLTileIndices[j++] = iStripStartIndex + 2*m - m_uiClipMapSize;

	iStripStartIndex = m*m_uiClipMapSize+m-1;
	m_pLeftTopLTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m ; i++ )
	{
		m_pLeftTopLTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize;
		m_pLeftTopLTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize+1;
	}
	m_pLeftTopLTileIndices[j++] = iStripStartIndex+(2*m-1)*m_uiClipMapSize+1;
	ZP_ASSERT( j == indicesPerLTile );

	//��������
	j = 0;
	iStripStartIndex = (3*(m-1)+2)*m_uiClipMapSize+m-1;
	m_pLeftBottomLTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
	{
		m_pLeftBottomLTileIndices[j++] = iStripStartIndex+i;
		m_pLeftBottomLTileIndices[j++] = iStripStartIndex+i-m_uiClipMapSize;
	}
	m_pLeftBottomLTileIndices[j++] = iStripStartIndex+2*m-m_uiClipMapSize;

	iStripStartIndex = (m-1)*m_uiClipMapSize+m-1;
	m_pLeftBottomLTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m ; i++ )
	{
		m_pLeftBottomLTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize;
		m_pLeftBottomLTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize+1;
	}
	m_pLeftBottomLTileIndices[j++] = iStripStartIndex+(2*m-1)*m_uiClipMapSize+1;
	ZP_ASSERT( j == indicesPerLTile );

	//��������
	j = 0;
	iStripStartIndex = m*m_uiClipMapSize+m-1;
	m_pRightTopTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
	{
		m_pRightTopTileIndices[j++] = iStripStartIndex + i;
		m_pRightTopTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
	}
	m_pRightTopTileIndices[j++] = iStripStartIndex + 2*m - m_uiClipMapSize;

	iStripStartIndex = m*m_uiClipMapSize+3*m-2; 
	m_pRightTopTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m ; i++ )
	{
		m_pRightTopTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize;
		m_pRightTopTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize+1;
	}
	m_pRightTopTileIndices[j++] = iStripStartIndex+(2*m-1)*m_uiClipMapSize+1;
	ZP_ASSERT( j == indicesPerLTile );

	//��������
	j = 0;
	iStripStartIndex = (3*(m-1)+2)*m_uiClipMapSize+m-1;
	m_pRightBottomTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
	{
		m_pRightBottomTileIndices[j++] = iStripStartIndex+i;
		m_pRightBottomTileIndices[j++] = iStripStartIndex+i-m_uiClipMapSize;
	}
	m_pRightBottomTileIndices[j++] = iStripStartIndex+2*m-m_uiClipMapSize;

	iStripStartIndex = (m-1)*m_uiClipMapSize+3*m-2;
	m_pRightBottomTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m ; i++ )
	{
		m_pRightBottomTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize;
		m_pRightBottomTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize+1;
	}
	m_pRightBottomTileIndices[j++] =  iStripStartIndex+(2*m-1)*m_uiClipMapSize+1;
	ZP_ASSERT( j == indicesPerLTile );
}
 
void ClipMapLevel::_InitFixedRingIndices( void )
{
	 unsigned int uiBlockNum = (m_uiClipMapSize-1)>>1;

	 unsigned int uiOffset2OuterRing = m_uiClipMapSize*4; 
	 unsigned int uiInnerRingOffset = m_uiClipMapSize;
	 unsigned int uiOutterRingOffset = ((m_uiClipMapSize-1)>>1) + 1; 

	 unsigned int uiIndicesNum = 4*uiBlockNum*9;
	 m_pFixedRingIndices = new unsigned short[uiIndicesNum];
	 memset( m_pFixedRingIndices , 0 , sizeof(unsigned short)*uiIndicesNum );
	 unsigned int j = 0 ;

	 //����װǰ�������Ǵ�ʱ��Ҫ��ת������Ŀǰ�����Ϊʲô
	for( unsigned int iStrip = 0 ; iStrip < 2 ; iStrip++ )
	{
		 for( unsigned int i = 0 ; i < uiBlockNum ; i++ )
		 { 
			 m_pFixedRingIndices[j++] = uiOffset2OuterRing + iStrip*uiOutterRingOffset + i;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+1;


			 m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset + i+1;
			 m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset + i;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset + 2*i+1;


			 m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset +i+1;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+1;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+2; 
		 }
	}

	//��װ���������Ǵ�
	for( unsigned int iStrip = 2 ; iStrip < 4 ; iStrip++ )
	{
		for( unsigned int i = 0 ; i < uiBlockNum ; i++ )
		{
			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i;
			m_pFixedRingIndices[j++] = uiOffset2OuterRing + iStrip*uiOutterRingOffset + i;
			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+1;

			m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset + i;
			m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset + i+1;
			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset + 2*i+1;

			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+1;
			m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset +i+1;
			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+2; 
		}
	}
 
	 //ZP_ASSERT( j == uiIndicesNum );
}

Math::Vec3 ClipMapLevel::GetLocalPos( void ) const
{
	return Math::Vec3( ((float)(m_centerPos.x>>m_uiLevel)) * m_fGridWidth , 0.0f , 
									((float)(m_centerPos.z>>m_uiLevel)) * m_fGridWidth );
}

bool ClipMapLevel::TestFlag( unsigned int bit )
{
	return (bool)( m_uiFlag&bit );
}

void ClipMapLevel::SetFlag( unsigned int bit , bool b )
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

unsigned int ClipMapLevel::GetGapTileIndicesNum( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return  ( ( m*2 ) + 2 )*2 * 4;
}

unsigned int ClipMapLevel::GetGapTilePrimitiveNum( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return ( m - 1 )*4*4 + 2 + 4*7;
}

unsigned int ClipMapLevel::GetCenterTileIndicesNum( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return  ( ( 2*m+1 )*2+2 ) * //ÿ�������������� 
		( ( m - 1) *2 + 2); //������
}

unsigned int ClipMapLevel::GetCenterTilePrimitiveNum( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return (2*(m-1)+2)*2*(2*(m-1)+2)+ //������������
				(2*(m-1)+1)*4 + 2;  //�˻���������
}

unsigned int ClipMapLevel::GetIndicesNumPerLTile( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return  (2*m+1)*2+2 + 2*m*2+2;
}

unsigned int ClipMapLevel::GetPrimitiveNumPerLTile( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return 4+2+(2*(m-1)+2)*2+(2*m-1)*2;
}

unsigned int ClipMapLevel::GetFixedRingIndicesNum( void ) const
{
	unsigned int uiBlockNum = (m_uiClipMapSize-1)>>1; 
	return 4*uiBlockNum*9;
}

unsigned int ClipMapLevel::GetFixedRingPrimitiveNum( void ) const
{
	unsigned int uiBlockNum = (m_uiClipMapSize-1)>>1; 
	return 4*uiBlockNum*3;
}


void ClipMapLevel::SetShowLTileFlag( unsigned int bit )
{ 
	unsigned int m = 
		SHOW_LEFT_TOP_L_TILE|
		SHOW_LEFT_BOTTOM_L_TILE|
		SHOW_RIGHT_TOP_L_TILE|
		SHOW_RIGHT_BOTTOM_L_TILE;
	m_uiFlag &= ~m;
	m_uiFlag |= bit;
}

unsigned int ClipMapLevel::GetFixedRingVertNum( void ) const
{
		return m_uiClipMapSize*4 //��Ȧ
		+( ((m_uiClipMapSize-1)>>1) + 1)*4; //��Ȧ�ֲڲ�
}

void ClipMapLevel::SetNextLevel( ClipMapLevel* pLevel )
{
	m_pNextLevel = pLevel;
}

void ClipMapLevel::ClearShowFlags( void )
{
	unsigned int m = 
		SHOW_OUTER_TILES|
		SHOW_CENTER_TILE|
		SHOW_GAP_TILES|
		SHOW_LEFT_TOP_L_TILE|
		SHOW_LEFT_BOTTOM_L_TILE|
		SHOW_RIGHT_TOP_L_TILE|
		SHOW_RIGHT_BOTTOM_L_TILE|
		SHOW_FIXED_UP_RING; 
	m_uiFlag &= ~m;
}






}//namespace Terrain