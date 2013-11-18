#ifndef ZP_CLIPMAPTERRAIN
#define ZP_CLIPMAPTERRAIN

#include "ClipMapTerrainDefine.h"
#include "IRender.h"
#include "Singleton.h"
#include "ClipMapTerrainRenderDataCache.h"


namespace Terrain
{
	class ClipMapLevel;


	 
	class ClipMapTerrain : public Util::SingletonT<ClipMapTerrain>
	{
	public:
		ClipMapTerrain(void);
		virtual ~ClipMapTerrain(void);

		/**
		* @brief ��ʼ������ͼ����
		* @param strMapName ����ͼ�ļ�·��
		* @param uiClipMapPow ����ͼ���ݣ���ʼ���������ֵ�������ͼ��С(����)
		* @param v3TerrainOrigin ����ԭ�㣨���Ͻǣ�
		* @param fGridWidth		���θ���ϸ��ĸ��ȣ�����Ϊ��λ��
		*/
		void Init( LPDIRECT3DDEVICE9 pDevice , const String& strMapName , const unsigned int uiClipMapPow, const Math::Vec3& v3TerrainOrigin , const float fGridWidth ); 
		void Destroy( void );
		void Update(  const Math::Vec3 v3CamPos  );  
		void GetRenderOps( std::vector<Render::D3DRenderOperation*>& opList );
		void OnLostDevice( void );
		void OnResetDevice( void );

	protected:

		ClipMapGridPos _WorldPos2GridPos( const Math::Vec3& v3Pos );

	protected:
		LPDIRECT3DDEVICE9 m_pDevice;
		String m_strMapName;					//�߶�ͼ��
		unsigned int m_uiClipMapSize;		//����ͼ��С
		unsigned int m_uiClipMapPow;		//����ͼ��С����
		Math::Vec3 m_v3CamPos;			//�����λ��
		Math::Vec3 m_v3TerrainOrigin;     //����ԭ��
		float			   m_fGridWidth;			//���� 
		ClipMapGridPos m_CamPos;			//���ӳ�䵽���β����������
		
		unsigned int	   m_uiLevelNum;		//������
		ClipMapLevel* m_pLevels;				//����ͼ��

		std::vector<Render::D3DRenderOperation*> m_tmpOpList;
		ClipMapTerrainRenderDataCache m_renderDataCache;
	};

}//namespace Terrain

#endif