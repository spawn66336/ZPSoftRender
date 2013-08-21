#include "Matrix4.h"

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

 

}//namespace Math