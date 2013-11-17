#ifndef ZP_CLIPMAPLEVEL
#define ZP_CLIPMAPLEVEL
#include "ClipMapTerrainDefine.h"

namespace Terrain
{

class ClipMapLevel
{
public:
	ClipMapLevel(void);
	virtual ~ClipMapLevel(void);

	void Init( const unsigned int uiLevel , const unsigned int uiClipMapSize , const float fGridWidth );
	void Update( const ClipMapGridPos& center );
	const ClipMapGridPos& GetCenter( void ) const { return m_centerPos; }
	const ClipMapArea& GetArea( void ) const { return m_currArea; }
protected:
	unsigned int  m_uiLevel;			  //��ǰClipMap�㼶
	unsigned int  m_uiClipMapSize;  //ClipMap��С
	unsigned int  m_uiGridSize;		 //һ�����θ������ϸ��ĸ������
	float				 m_fGridWidth;		 //��ǰ����θ�����ʾ�Ŀ�ȣ�����Ϊ��λ��
	ClipMapGridPos m_centerPos;	 //��ǰ�������� (���ϸ��ĸ��������)
	ClipMapArea m_currArea;			//��ǰ����ʾ�ĸ߶�ͼ���� (���ϸ��ĸ����������)
	unsigned int m_uiFlag;				//��־
};

}//namespace Terrain
#endif