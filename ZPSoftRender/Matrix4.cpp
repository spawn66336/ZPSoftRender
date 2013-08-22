#include "Matrix4.h"
#include "MathUtil.h"

namespace Math
{ 
	const Matrix4 Matrix4::ZERO( 0.0f,0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f ); 

	const Matrix4 Matrix4::IDENTITY( 1.0f,0.0f,0.0f,0.0f , 0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f );


	Math::Vec4 Matrix4::GetRow( const int iRow ) const
	{
		ZP_ASSERT( iRow >=0 && iRow < 4 );
		
		return Vec4( m[iRow][0] , m[iRow][1] , m[iRow][2] , m[iRow][3] );
	}

	Math::Vec4 Matrix4::GetColumn( const int iCol ) const
	{
		ZP_ASSERT( iCol >=0 && iCol < 4 );

		return Vec4( m[0][iCol] , m[1][iCol] , m[2][iCol] , m[3][iCol] );
	}

	void Matrix4::SetRow( const int iRow , const Vec4& vec )
	{
		ZP_ASSERT( iRow >=0 && iRow < 4 );

		m[iRow][0] = vec.x; m[iRow][1] = vec.y; m[iRow][2] = vec.z; m[iRow][3] = vec.w;
	}

	void Matrix4::SetColumn( const int iCol , const Vec4& vec )
	{
		ZP_ASSERT( iCol >=0 && iCol < 4 );

		m[0][iCol] = vec.x; m[1][iCol] = vec.y; m[2][iCol] = vec.z; m[3][iCol] = vec.w;
	}

	Math::Matrix3 Matrix4::GetRotatePart( void ) const
	{
		return Matrix3( 
			m[0][0] , m[0][1] , m[0][2] ,
			m[1][0] , m[1][1] , m[1][2] ,
			m[2][0] , m[2][1] , m[2][2]
			);
	}

	Math::Vec3 Matrix4::GetTranslationPart( void ) const
	{
		return Vec3( m[3][0] , m[3][1] , m[3][2] );
	}

	Matrix4 Matrix4::MakeTranslationMatrix( const Vec3& vec )
	{
		Matrix4 rs;
		rs.operator=( Matrix4::IDENTITY);
		rs.m[3][0] = vec.x; rs.m[3][1] = vec.y; rs.m[3][2] = vec.z;
		return rs;
	}

	Matrix4 Matrix4::MakeRotateWithAxisMatrix( const Vec3& axis , const Real theta )
	{
		Matrix4 rs;
		rs.operator=( Matrix4::IDENTITY );

		Real sinTheta = MathUtil::Sin( theta );
		Real cosTheta = MathUtil::Cos( theta );
		Real oneMinusCosTheta = 1.0f - cosTheta;
		Real AxAx = axis.x*axis.x;
		Real AyAy = axis.y*axis.y;
		Real AzAz = axis.z*axis.z;
		Real AxAy = axis.x*axis.y;
		Real AxAz = axis.x*axis.z;
		Real AyAz = axis.y*axis.z;

		rs.m[0][0] = AxAx*oneMinusCosTheta + cosTheta; 
		rs.m[0][1] = AxAy*oneMinusCosTheta + axis.z*sinTheta;
		rs.m[0][2] = AxAz*oneMinusCosTheta - axis.y*sinTheta;

		rs.m[1][0] = AxAy*oneMinusCosTheta - axis.z*sinTheta;
		rs.m[1][1] = AyAy*oneMinusCosTheta + cosTheta;
		rs.m[1][2] = AyAz*oneMinusCosTheta + axis.x*sinTheta;

		rs.m[2][0] = AxAz*oneMinusCosTheta + axis.y*sinTheta;
		rs.m[2][1] = AyAz*oneMinusCosTheta - axis.x*sinTheta;
		rs.m[2][2] = AzAz*oneMinusCosTheta + cosTheta;

		return rs;
	}

 

}//namespace Math