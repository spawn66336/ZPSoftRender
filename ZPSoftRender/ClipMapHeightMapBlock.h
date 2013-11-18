#ifndef ZP_CLIPMAPHEIGHTMAPBLOCK
#define ZP_CLIPMAPHEIGHTMAPBLOCK

#include "ClipMapTerrainDefine.h"

namespace Terrain
{
	enum AREA_UPDATE_FLAG
	{
		AREA_UNINIT = 1<<0, 
		AREA_IS_DIRTY = 1<<1
	};
	 
class ClipMapHeightMapBlock
{
public:
	ClipMapHeightMapBlock(void);
	virtual ~ClipMapHeightMapBlock(void);

	void Init( const unsigned int uiLevel , const unsigned int uiClipMapSize );
	void Destroy( void );
	/**
	* @brief 用新区域更新当前层高度图
	* @newArea 待更新区域
	* @return 是否进行了真正的更新
	*/
	bool Update( const ClipMapArea& newArea );
	unsigned int Sample( const int localX , const int localZ );
protected:
	void _UpdateArea( const ClipMapArea& updateArea );

	/**
	* @brief 设置高程图某点高度
	* @param x 横坐标（当前层坐标）
	* @param z 纵坐标（当前层坐标）
	* @param h 待设置高度
	*/
	void _SetHeight( const int localX , const int localZ , const unsigned int h );
protected:
	ClipMapArea m_currArea;
	unsigned int  m_uiLevel;				//当前所在级别
	unsigned int  m_uiClipMapSize;		//剪切图大小
	unsigned int  m_uiFlag;
	unsigned int* m_pHeightMap;		//当前层高程图
};

}//namespace Terrain
#endif