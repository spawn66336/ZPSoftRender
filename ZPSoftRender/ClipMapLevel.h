#ifndef ZP_CLIPMAPLEVEL
#define ZP_CLIPMAPLEVEL

#include "ClipMapTerrainDefine.h"
#include "ClipMapHeightMapBlock.h"

namespace Terrain
{


class ClipMapLevel
{
public:
	ClipMapLevel(void);
	virtual ~ClipMapLevel(void);

	void Init( const unsigned int uiLevel , const unsigned int uiClipMapSize , const float fGridWidth );
	void SetNextLevel( ClipMapLevel* pLevel );
	void UpdateHeightMap( const ClipMapGridPos& center );
	//���¶�����Ҫ�ڸ��������в�֮�����
	void UpdateVerts( void );
	const ClipMapGridPos& GetCenter( void ) const { return m_centerPos; }
	const ClipMapArea& GetArea( void ) const { return m_currArea; }

	float GetCenterHeight( void ) const { return m_fCenterHeight; }

	bool TestFlag( unsigned int bit  );
	void SetFlag( unsigned int bit , bool b );
	unsigned int GetFlag(void) const { return m_uiFlag; }
	void SetShowLTileFlag( unsigned int bit );
	void ClearShowFlags( void );
	

	TerrainVertex* GetVerts( void ) { return m_pVerts; } 
	TerrainVertex* GetFixedRingVerts( void ){ return m_pFixedRingVerts; }

	unsigned short** GetTilesIndices( void ) { return m_ppTilesIndices; }
	unsigned short*  GetGapTileIndices( void ) { return m_pGapTileIndices; }
	unsigned short* GetCenterTileIndices( void ){ return m_pCenterTileIndices; }

	unsigned short* GetLeftTopLTileIndices( void ){ return m_pLeftTopLTileIndices; }
	unsigned short* GetLeftBottomLTileIndices( void ){ return m_pLeftBottomLTileIndices; }
	unsigned short* GetRightTopLTileIndices( void ){ return m_pRightTopTileIndices; }
	unsigned short* GetRightBottomLTileIndices( void ){ return m_pRightBottomTileIndices; }

	unsigned short* GetFixedRingIndices( void ){ return m_pFixedRingIndices; }

	//������
	unsigned int GetVertsNum( void ) const;
	//�ⲿ�޲���������
	unsigned int GetFixedRingVertNum( void ) const;
	unsigned int GetFixedRingIndicesNum( void ) const;
	unsigned int GetFixedRingPrimitiveNum( void ) const;

	//�����Χ����Ƭ��
	unsigned int GetTileNum( void ) const { return 12; }
	//ÿ�������ӿ��������
	unsigned int GetIndicesNumPerTile( void ) const;
	//ÿ�������ӿ�ļ�������
	unsigned int GetPrimtiveNumPerTile( void ) const;
	//����������ҵ��޲���Ƭ������������
	unsigned int GetGapTileIndicesNum( void ) const;
	//����������ҵ��޲���Ƭ�����ܻ�������
	unsigned int GetGapTilePrimitiveNum( void ) const; 

	unsigned int GetCenterTileIndicesNum( void ) const;
	unsigned int GetCenterTilePrimitiveNum( void ) const;

	unsigned int GetIndicesNumPerLTile( void ) const;
	unsigned int GetPrimitiveNumPerLTile( void ) const;
	 
	Math::Vec3 GetLocalPos( void ) const;

	ClipMapHeightMapBlock& GetHeightMapBlock( void ){ return m_heightMapBlock; }

protected:
	void _InitVerts( void );
	void _InitFixedRingVerts( void );
	//����12���ӵ��ο������
	void _InitTilesIndices( void );
	void _InitGapTilesIndices( void );
	void _InitCenterTileIndices( void );
	void _InitLFixedTileIndices( void );
	void _InitFixedRingIndices( void );
	//���ݸ߳�ͼ���¶���ĸ߶�ֵ
	void _UpdateVerts( void );
	void _UpdateFixedRingVerts( void );
protected:
	unsigned int  m_uiLevel;			  //��ǰClipMap�㼶
	unsigned int  m_uiClipMapSize;  //ClipMap��С
	unsigned int  m_uiGridSize;		 //һ�����θ������ϸ��ĸ������
	float				 m_fGridWidth;		 //��ǰ����θ�����ʾ�Ŀ�ȣ�����Ϊ��λ��
	float				 m_fCenterHeight; //��ǰ�����ĵ�߶ȣ�����Ϊ��λ��
	ClipMapGridPos m_centerPos;	 //��ǰ�������� (���ϸ��ĸ��������)
	ClipMapArea m_currArea;			//��ǰ����ʾ�ĸ߶�ͼ���� (���ϸ��ĸ����������)
	ClipMapHeightMapBlock m_heightMapBlock;	//��ǰ��߶�ͼ��
	TerrainVertex* m_pVerts;			//��ǰ��Ķ���
	TerrainVertex* m_pFixedRingVerts; //�����޲�T���ѷ�Ķ��㻺����

	unsigned short** m_ppTilesIndices; //12��С���ο������
	unsigned short*   m_pGapTileIndices; //�������Ҹ��ĸ��޲���
	unsigned short*	m_pCenterTileIndices;		//���Ŀ飨ֻ���ڶ����ɼ���

	unsigned short*	m_pLeftTopLTileIndices;
	unsigned short*   m_pLeftBottomLTileIndices;
	unsigned short*   m_pRightTopTileIndices;
	unsigned short*   m_pRightBottomTileIndices;

	unsigned short*   m_pFixedRingIndices; //�����޲�T���ѷ������������

	ClipMapLevel* m_pNextLevel;   //��һ���ֲڼ���
	unsigned int m_uiFlag;				//��־
};

}//namespace Terrain
#endif