#include "Plane.h"

namespace Math
{

	Plane::Plane(void):
	m_v3Normal(),
	m_v3P0()
	{
	}

	Plane::Plane( const Vec3& norm , const Vec3& p0 ):
	m_v3Normal( norm ),
	m_v3P0( p0 )
	{

	}

	Plane::Plane( const Plane& plane )
	{
		this->operator=( plane );
	}


	Plane::~Plane(void)
	{
	}

	Plane& Plane::operator=( const Plane& rhs )
	{
		m_v3Normal = rhs.m_v3Normal;
		m_v3P0 = rhs.m_v3P0;
		return *this;
	}


	 int Plane::ClassifyPoint( const Vec3& pos ) const
	{
		Real prod = m_v3Normal.DotProduct( pos - m_v3P0 );
		if( REAL_GREATER_THAN( prod , 0.0f ) )
			return FRONT_SIDE;
		else if( REAL_LESS_THAN( prod , 0.0f ) )
			return BACK_SIDE;
		return ON_THE_PLANE;
	}


	bool Plane::IsFrontSide( const Vec3& pos ) const
	{ 
		return ClassifyPoint( pos ) == FRONT_SIDE;
	}

	bool Plane::IsBackSide( const Vec3& pos ) const
	{
		return ClassifyPoint( pos ) == BACK_SIDE;
	}

	bool Plane::IsOnThePlane( const Vec3& pos ) const
	{
		return ClassifyPoint( pos ) == ON_THE_PLANE;
	}

	bool Plane::IsThroughThePlane( const Vec3& p0 , const Vec3& p1 ) const
	{
		 int iP0Classify = ClassifyPoint( p0 );
		 int iP1Classify = ClassifyPoint( p1 ); 
		//线段没有穿过平面
		if( iP0Classify == iP1Classify )
		{
			return false;
		}
		return true;
	}


	bool Plane::IsFrontSide( const Vec3& p0 , const Vec3& p1 , const Vec3& p2 ) const
	{
		 int iP0Classify = ClassifyPoint( p0 );
		 int iP1Classify = ClassifyPoint( p1 ); 
		 int iP2Classify = ClassifyPoint( p2 );

		if( iP0Classify >= 0 && 
			iP1Classify >= 0 && 
			iP2Classify >= 0 )
		{
			if( iP0Classify == ON_THE_PLANE &&
				iP1Classify == ON_THE_PLANE && 
				iP2Classify == ON_THE_PLANE )
			{
				return false;
			}

			return true;
		}
		return false;
	}


	bool Plane::IsBackSide( const Vec3& p0 , const Vec3& p1 , const Vec3& p2 ) const
	{
		 int iP0Classify = ClassifyPoint( p0 );
		 int iP1Classify = ClassifyPoint( p1 ); 
		 int iP2Classify = ClassifyPoint( p2 );

		if( iP0Classify <= 0 && 
			iP1Classify <= 0 && 
			iP2Classify <= 0 )
		{
			if( iP0Classify == ON_THE_PLANE &&
				iP1Classify == ON_THE_PLANE && 
				iP2Classify == ON_THE_PLANE )
			{
				return false;
			}

			return true;
		}
		return false;
	}


	bool Plane::IsOnThePlane( const Vec3& p0 , const Vec3& p1 , const Vec3& p2 ) const
	{
		 int iP0Classify = ClassifyPoint( p0 );
		 int iP1Classify = ClassifyPoint( p1 ); 
		 int iP2Classify = ClassifyPoint( p2 );

		if( iP0Classify == ON_THE_PLANE &&
			iP1Classify == ON_THE_PLANE &&
			iP2Classify == ON_THE_PLANE )
		{
			return true;
		}
		return false;
	}



	bool Plane::ClipLineSegment( const Vec3& p0 , const Vec3& p1 , Real& t ) const
	{
		if( IsThroughThePlane( p0 , p1 ) )
		{
			t = m_v3Normal.DotProduct( m_v3P0 - p0 ) / m_v3Normal.DotProduct( p1 - p0 );
			return true;
		}
		return false;
	}

}//namespace Math