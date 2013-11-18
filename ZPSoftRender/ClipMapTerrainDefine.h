#ifndef ZP_CLIPMAP_TERRAIN_DEFINE
#define ZP_CLIPMAP_TERRAIN_DEFINE

#include "ZPDependency.h"
#include "ZPMathDependency.h"

namespace Terrain
{

	typedef struct{
		int size; //�ṹ���С
		int img_width; //����ͼƬ�Ŀ��
		int img_height; //����ͼƬ�ĸ߶�
		int pixel_size;		//���ص��ֽڴ�С
		int component; //���صķ�����

		int is_dem; //�Ƿ��Ǹ߳�ͼ
		int max_height;	//��ǰ�߳�ͼ��ߵĸ߶ȣ����ڸ߳�ͼ�����壩
		int min_height;//��ǰ�߳�ͼ��͵ĸ߶ȣ����ڸ߳�ͼ�����壩
		int span_hori; //�����ȣ���λ�ף�
		int span_vert; //��ֱ��ȣ���λ�ף�

		int mipmap_level; //mipmap�Ĳ���
		unsigned int mipmap_level_offset[1]; 
	}ClipMapDemFileHeader_t;

	class TerrainVertex
	{
	public:
		Math::Vec3 m_pos;
		Math::Vec3 m_norm;
		Math::Vec4 m_color;
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
		//����ĺ����������
		int Width( void ) const { return maxPos.x - minPos.x; }
		//����������������
		int Height( void ) const { return maxPos.z - minPos.z; }
		//���������Ƿ����
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

	//��������ӳ�䵽����
	extern int Map2Odd( int x );

	//������
	extern int Mod( int x , int y );
}//namespace Terrain

#endif