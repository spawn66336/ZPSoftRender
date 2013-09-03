#ifndef ZP_FRUSTUM
#define ZP_FRUSTUM

#include "Plane.h"

namespace Math
{
	 /**
	 * @brief ��׵��
	 */
	class Frustum
	{
	public:
		Frustum(void);
		Frustum( const Frustum& frustum );
		~Frustum(void);

		void Set( const Real fov , const Real aspect  ,const Real n , const Real f );

		inline const Math::Plane& GetNearClipPlane( void ) const { return m_nearClipPlane; }
		inline const Math::Plane& GetFarClipPlane( void ) const { return m_farClipPlane; }
		inline const Math::Plane& GetLeftClipPlane( void ) const { return m_leftClipPlane; }
		inline const Math::Plane& GetRightClipPlane( void ) const { return m_rightClipPlane; }
		inline const Math::Plane& GetTopClipPlane( void ) const { return m_topClipPlane; }
		inline const Math::Plane& GetBottomClipPlane( void ) const { return m_bottomClipPlane; }
		bool IsOutSide( const Vec3& p0 , const Vec3& p1 , const Vec3& p2 ) const;

		Frustum& operator=( const Frustum& rhs );

	protected:

		Real m_fov;									///>��׵���Žǣ��ԽǶȼƣ�
		Real m_aspect;								///>�ӿڿ�߱�
		Real m_near;									///>��ƽ�����
		Real m_far;									///>Զƽ�����
		Math::Plane m_nearClipPlane;		///>������ƽ��
		Math::Plane m_farClipPlane;			///>Զ����ƽ��
		Math::Plane m_leftClipPlane;			///>�����ƽ��
		Math::Plane m_rightClipPlane;		///>�Ҽ���ƽ��
		Math::Plane m_topClipPlane;			///>������ƽ��
		Math::Plane m_bottomClipPlane;	///>�׼���ƽ��
	};

}//namespace Math

#endif //ZP_FRUSTUM