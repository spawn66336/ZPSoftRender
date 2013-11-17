#include "ClipMapLevel.h"

namespace Terrain
{


ClipMapLevel::ClipMapLevel(void):
m_uiLevel(0),
m_uiClipMapSize(0),
m_uiGridSize(0),
m_fGridWidth(0.0f),
m_uiFlag(0)
{

}


ClipMapLevel::~ClipMapLevel(void)
{

}
 
void ClipMapLevel::Init( const unsigned int uiLevel ,  const unsigned int uiClipMapSize ,  const float fGridWidth )
{
	m_uiLevel = uiLevel;
	m_uiClipMapSize = uiClipMapSize;
	m_uiGridSize = 1<<uiLevel;
	m_fGridWidth = fGridWidth*( (float)m_uiGridSize );
}

void ClipMapLevel::Update( const ClipMapGridPos& center )
{
	m_centerPos = center; 
	//使用更新过的中心坐标重新计算当前区域
	m_currArea.minPos.x = m_centerPos.x - ( m_uiClipMapSize>>1 ) * m_uiGridSize ;
	m_currArea.minPos.z = m_centerPos.z - ( m_uiClipMapSize>>1 ) * m_uiGridSize ;
	m_currArea.maxPos.x = m_currArea.minPos.x + ( m_uiClipMapSize - 1 )*m_uiGridSize;
	m_currArea.maxPos.z = m_currArea.minPos.z + ( m_uiClipMapSize - 1 )*m_uiGridSize;
	
	
}


}//namespace Terrain