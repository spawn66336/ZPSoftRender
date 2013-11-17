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
	unsigned int  m_uiLevel;			  //当前ClipMap层级
	unsigned int  m_uiClipMapSize;  //ClipMap大小
	unsigned int  m_uiGridSize;		 //一个地形格所用最精细层的格点数量
	float				 m_fGridWidth;		 //当前层地形格所表示的跨度（以米为单位）
	ClipMapGridPos m_centerPos;	 //当前层格点中心 (以最精细层的格点数量计)
	ClipMapArea m_currArea;			//当前所表示的高度图区域 (以最精细层的格点数量计算)
	unsigned int m_uiFlag;				//标志
};

}//namespace Terrain
#endif