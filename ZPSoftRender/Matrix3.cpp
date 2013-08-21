#include "Matrix3.h"

namespace Math
{
	const Matrix3 Matrix3::IDENTITY( 1.0f , 0.0f , 0.0f , 0.0f ,1.0f , 0.0f , 0.0f , 0.0f , 1.0f );

	const Matrix3 Matrix3::ZERO( 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f );

	Math::Vec3 Matrix3::GetRow( const int iRow ) const
	{
		return Vec3( m[iRow][0] , m[iRow][1] , m[iRow][2] );
	}

	Math::Vec3 Matrix3::GetColumn( const int iCol ) const
	{
		return Vec3( m[0][iCol] , m[1][iCol] , m[2][iCol] );
	}

	void Matrix3::SetRow( const int iRow , const Vec3& vec )
	{
		m[iRow][0] = vec.x; m[iRow][1] = vec.y; m[iRow][2] = vec.z;
	}

	void Matrix3::SetColumn( const int iCol , const Vec3& vec )
	{
		m[0][iCol] = vec.x; m[1][iCol] = vec.y; m[2][iCol] = vec.z; 
	}

	void Matrix3::FromAxes( const Vec3& xAxis , const Vec3& yAxis , const Vec3& zAxis )
	{
		SetColumn( 0 , xAxis );
		SetColumn( 1 , yAxis );
		SetColumn( 2 , zAxis );
	}

	bool Matrix3::operator==( const Matrix3& rhs ) const
	{
		for (int iRow = 0; iRow < 3; iRow++)
		{
			for (int iCol = 0; iCol < 3; iCol++)
			{
				if ( m[iRow][iCol] != rhs.m[iRow][iCol] )
					return false;
			}
		} 
		return true;
	}

	Math::Matrix3 Matrix3::operator+( const Matrix3& rhs ) const
	{
		return Matrix3(
			m[0][0] + rhs.m[0][0] , m[0][1] + rhs.m[0][1] , m[0][2] + rhs.m[0][2] ,
			m[1][0] + rhs.m[1][0] , m[1][1] + rhs.m[1][1] , m[1][2] + rhs.m[1][2] ,
			m[2][0] + rhs.m[2][0] , m[2][1] + rhs.m[2][1] , m[2][2] + rhs.m[2][2]
			);
	}

	Math::Matrix3 Matrix3::operator-( const Matrix3& rhs ) const
	{
		return Matrix3(
			m[0][0] - rhs.m[0][0] , m[0][1] - rhs.m[0][1] , m[0][2] - rhs.m[0][2] ,
			m[1][0] - rhs.m[1][0] , m[1][1] - rhs.m[1][1] , m[1][2] - rhs.m[1][2] ,
			m[2][0] - rhs.m[2][0] , m[2][1] - rhs.m[2][1] , m[2][2] - rhs.m[2][2]
		);
	}

	Math::Matrix3 Matrix3::operator*( const Matrix3& rhs ) const
	{
		Matrix3 rs;

		for( int iRow = 0 ; iRow < 3 ; iRow++ )
		{
			for( int iCol = 0 ; iCol < 3 ; iCol++ )
			{
				rs.m[iRow][iCol] = 
					m[iRow][0]*rhs.m[0][iCol] +
					m[iRow][1]*rhs.m[1][iCol] +
					m[iRow][2]*rhs.m[2][iCol];
			}
		}

		return rs;
	}

	Math::Vec3 Matrix3::operator*( const Vec3& rhs ) const
	{
		Vec3 rs;
		rs.x = rhs.DotProduct( this->GetRow(0) );
		rs.y = rhs.DotProduct( this->GetRow(1) );
		rs.z = rhs.DotProduct( this->GetRow(2) );
		return rs;
	}

	Math::Matrix3 Matrix3::operator*( Real rhs ) const
	{
		Matrix3 rs;

		for( int iRow = 0 ; iRow < 3 ; iRow++ )
		{
			for( int iCol = 0 ; iCol < 3 ; iCol++ )
			{
				rs.m[iRow][iCol] = m[iRow][iCol] * rhs;
			}
		}

		return rs;
	}

	Math::Matrix3 Matrix3::operator-() const
	{
		Matrix3 rs;
		for (int iRow = 0; iRow < 3; iRow++)
		{
			for (int iCol = 0; iCol < 3; iCol++)
				rs.m[iRow][iCol] = -m[iRow][iCol];
		}
		return rs;
	}

	Math::Matrix3 Matrix3::Transpose() const
	{
		Matrix3 rs;
		for ( int iRow = 0; iRow < 3; iRow++ )
		{
			for ( int iCol = 0; iCol < 3; iCol++ )
			{
				rs.m[iRow][iCol] = m[iCol][iRow];
			}
		}
		return rs;
	}



	Math::Vec3 operator*( const Vec3& lhs, const Matrix3& rhs )
	{
		Vec3 rs;
		rs.x = lhs.DotProduct( rhs.GetColumn(0) );
		rs.y = lhs.DotProduct( rhs.GetColumn(1) );
		rs.z = lhs.DotProduct( rhs.GetColumn(2) );
		return rs;
	}

	Math::Matrix3 operator*( const Real lhs, const Matrix3& rhs )
	{
		Matrix3 rs;

		for( int iRow = 0 ; iRow < 3 ; iRow++ )
		{
			for( int iCol = 0 ; iCol < 3 ; iCol++ )
			{
				rs.m[iRow][iCol] = lhs * rhs.m[iRow][iCol];
			}
		}

		return rs;
	}

}//namespace Math