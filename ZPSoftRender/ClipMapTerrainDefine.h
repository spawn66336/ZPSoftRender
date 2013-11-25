#ifndef ZP_CLIPMAP_TERRAIN_DEFINE
#define ZP_CLIPMAP_TERRAIN_DEFINE

#include "ZPDependency.h"
#include "ZPMathDependency.h"

namespace Terrain
{
	enum CLIPMAPLEVEL_FLAG
	{
		VERTS_CHANGE= 1<<0 ,
		HEIGHTMAP_DIRTY = 1<<1,
		SHOW_OUTER_TILES = 1<<2,
		SHOW_CENTER_TILE = 1<<3,
		SHOW_GAP_TILES = 1<<4,
		SHOW_LEFT_TOP_L_TILE = 1<<5,
		SHOW_LEFT_BOTTOM_L_TILE = 1<<6,
		SHOW_RIGHT_TOP_L_TILE = 1<<7,
		SHOW_RIGHT_BOTTOM_L_TILE = 1<<8,
		SHOW_FIXED_UP_RING = 1<<9
	};


	typedef struct{
		int size; //结构体大小
		int img_width; //顶层图片的宽度
		int img_height; //顶层图片的高度
		int pixel_size;		//像素的字节大小
		int component; //像素的分量数

		int is_dem; //是否是高程图
		int max_height;	//当前高程图最高的高度（对于高程图有意义）
		int min_height;//当前高程图最低的高度（对于高程图有意义）
		int span_hori; //横向跨度（单位米）
		int span_vert; //垂直跨度（单位米）

		int mipmap_level; //mipmap的层数
		unsigned int mipmap_level_offset[1]; 
	}ClipMapDemFileHeader_t;

	class TerrainVertex
	{
	public:
		Math::Vec3 m_pos;
		Math::Vec3 m_norm;
		DWORD	  m_color;
	};

	class ClipMapGridPos
	{
	public:
		ClipMapGridPos():x(0),z(0){}
		ClipMapGridPos( const int in_x , const int in_z ):x(in_x),z(in_z){}
		ClipMapGridPos( const ClipMapGridPos& pos ):x(pos.x),z(pos.z){}
		~ClipMapGridPos(){}
		int x;
		int z;
	};

	class ClipMapArea
	{ 
	public:
		//区域的横向采样点数
		int Width( void ) const { return maxPos.x - minPos.x; }
		//区域的纵向采样点数
		int Height( void ) const { return maxPos.z - minPos.z; }
		//两个区域是否相等
		bool Equal( const ClipMapArea& other ) const
		{
			if( 
				minPos.x == other.minPos.x &&
				minPos.z == other.minPos.z &&
				Width() ==  other.Width() &&
				Height() == other.Height()
				)
			{
				return true;
			}
			return false;
		}

		ClipMapGridPos maxPos;
		ClipMapGridPos minPos;
	};

	//将传入数映射到奇数
	extern int Map2Odd( int x );

	//求余数
	extern int Mod( int x , int y );
}//namespace Terrain

#endif