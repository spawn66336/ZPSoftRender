#ifndef ZP_FRUSTUM
#define ZP_FRUSTUM

#include "Plane.h"

namespace Math
{
	 
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

		Real m_fov;
		Real m_aspect;
		Real m_near;
		Real m_far;
		Math::Plane m_nearClipPlane;
		Math::Plane m_farClipPlane;
		Math::Plane m_leftClipPlane;
		Math::Plane m_rightClipPlane;
		Math::Plane m_topClipPlane;
		Math::Plane m_bottomClipPlane;
	};

}//namespace Math

#endif //ZP_FRUSTUM