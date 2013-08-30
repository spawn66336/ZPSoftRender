#include "Frustum.h"
#include "ZPMathDependency.h"

namespace Math
{  
	Frustum::Frustum(void)
	{
	}

	Frustum::Frustum( const Frustum& frustum )
	{
		this->operator=( frustum );
	}


	Frustum::~Frustum(void)
	{
	}

	Frustum& Frustum::operator=( const Frustum& rhs )
	{
		m_fov = rhs.m_fov;
		m_aspect = rhs.m_aspect;
		m_near = rhs.m_near;
		m_far = rhs.m_far;
		m_nearClipPlane = rhs.m_nearClipPlane;
		m_farClipPlane = rhs.m_farClipPlane;
		m_leftClipPlane = rhs.m_leftClipPlane;
		m_rightClipPlane = rhs.m_rightClipPlane;
		m_topClipPlane = rhs.m_topClipPlane;
		m_bottomClipPlane = rhs.m_bottomClipPlane;
		return *this;
	}

	bool Frustum::IsOutSide( const Vec3& p0 , const Vec3& p1 , const Vec3& p2 ) const
	{
		if( m_nearClipPlane.IsBackSide( p0 , p1 , p2 ) ||
			m_farClipPlane.IsBackSide( p0 , p1 , p2 ) ||
			m_leftClipPlane.IsBackSide( p0 , p1 , p2 )||
			m_rightClipPlane.IsBackSide( p0 , p1 , p2 ) ||
			m_topClipPlane.IsBackSide( p0 , p1 , p2 )||
			m_bottomClipPlane.IsBackSide( p0 , p1 , p2 )
			)
		{
			return true;
		}
		return false;
	}

	void Math::Frustum::Set( const Real fov , const Real aspect ,const Real n , const Real f )
	{
		m_fov = fov;
		m_aspect = aspect;
		m_near = n;
		m_far = f;

		m_nearClipPlane.Set( Vec3( 0.0f , 0.0f , 1.0f ) , Vec3( 0.0f , 0.0f , n ) );
		m_farClipPlane.Set( Vec3( 0.0f , 0.0f , -1.0f ) , Vec3( 0.0f , 0.0f , f ) );

		Real fRadHalfFov = MathUtil::DegreesToRadians( fov * 0.5f );
		Real fSinHalfFov = MathUtil::Sin( fRadHalfFov );
		Real fCosHalfFov = MathUtil::Cos( fRadHalfFov );
		Real fFactor = 1.0f / aspect;
		Real d = 1.0f * Math::MathUtil::Tan( fRadHalfFov );
		Real fInvLongSide = 1.0f / Math::MathUtil::Sqrt( 1.0f + d * fFactor );
		Real fSin = d * fFactor * fInvLongSide;
		Real fCos = 1.0f * fInvLongSide;

		m_leftClipPlane.Set( Vec3( fCosHalfFov  , 0.0f , fSinHalfFov ) , Vec3( 0.0f , 0.0f , 0.0f ) );
		m_rightClipPlane.Set( Vec3( -fCosHalfFov , 0.0f , fSinHalfFov ) , Vec3( 0.0f , 0.0f , 0.0f ) );
		m_topClipPlane.Set( Vec3( 0.0f , -fCos , fSin ) , Vec3( 0.0f , 0.0f , 0.0f ) ); 
		m_bottomClipPlane.Set( Vec3( 0.0f , fCos , fSin ) , Vec3( 0.0f , 0.0f , 0.0f ) ); 
	}
	 
}//namespace Math