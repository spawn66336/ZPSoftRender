#ifndef ZP_PLANE
#define ZP_PLANE

#include "ZPDependency.h"
#include "Vec3.h"

namespace Math
{
	enum CLASSIFY_POINT
	{
		FRONT_SIDE = 1,
		ON_THE_PLANE = 0,
		BACK_SIDE = -1
	};

	class Plane
	{
	public:
		Plane(void);
		Plane( const Vec3& norm , const Vec3& p0 );
		Plane( const Plane& plane );
		~Plane(void);

		inline void Set( const Vec3& norm , const Vec3& p0 ){ m_v3Normal = norm; m_v3P0 = p0; }

		Plane& operator=( const Plane& rhs );

		 int ClassifyPoint( const Vec3& pos ) const;

		bool IsFrontSide( const Vec3& pos ) const;
		 
		bool IsOnThePlane( const Vec3& pos ) const;

		bool IsBackSide( const Vec3& pos ) const; 

		bool IsFrontSide( const Vec3& p0 , const Vec3& p1 , const Vec3& p2 ) const;

		bool IsOnThePlane( const Vec3& p0 , const Vec3& p1 , const Vec3& p2 ) const;

		bool IsBackSide( const Vec3& p0 , const Vec3& p1 , const Vec3& p2 ) const;

		bool IsThroughThePlane( const Vec3& p0 , const Vec3& p1  ) const;

		bool ClipLineSegment( const Vec3& p0 , const Vec3& p1 , Real& t ) const; 

	protected:
		Vec3 m_v3Normal;
		Vec3 m_v3P0;
	};

}

#endif //ZP_PLANE