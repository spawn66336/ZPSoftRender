#ifndef ZP_FRUSTUM
#define ZP_FRUSTUM

#include "Plane.h"

namespace Math
{
	 /**
	 * @brief 视椎体
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

		Real m_fov;									///>视椎体张角（以角度计）
		Real m_aspect;								///>视口宽高比
		Real m_near;									///>近平面距离
		Real m_far;									///>远平面距离
		Math::Plane m_nearClipPlane;		///>近剪裁平面
		Math::Plane m_farClipPlane;			///>远剪裁平面
		Math::Plane m_leftClipPlane;			///>左剪裁平面
		Math::Plane m_rightClipPlane;		///>右剪裁平面
		Math::Plane m_topClipPlane;			///>顶剪裁平面
		Math::Plane m_bottomClipPlane;	///>底剪裁平面
	};

}//namespace Math

#endif //ZP_FRUSTUM