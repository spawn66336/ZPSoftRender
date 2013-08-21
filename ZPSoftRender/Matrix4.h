#ifndef ZP_MATRIX4
#define ZP_MATRIX4

#include "ZPDependency.h"
#include "Matrix3.h"
#include "Vec4.h"

namespace Math
{

	class Matrix4
	{
	public:
		/**
		* @brief 默认构造函数
		*/
		inline Matrix4(void){}

		inline Matrix4(
			Real m00, Real m01, Real m02, Real m03,
			Real m10, Real m11, Real m12, Real m13,
			Real m20, Real m21, Real m22, Real m23,
			Real m30, Real m31, Real m32, Real m33 )
		{
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
		}

		inline Matrix4(const Matrix3& m3x3)
		{
			operator=(IDENTITY);
			operator=(m3x3);
		} 

		~Matrix4(void){}

		Vec4 GetRow( const int iRow ) const;

		Vec4 GetColumn( const int iCol ) const;

		void SetRow( const int iRow , const Vec4& vec  );

		void SetColumn( const int iCol , const Vec4& vec );

		inline Matrix4 Concatenate( const Matrix4& m2 ) const
		{
			Matrix4 rs;
			rs.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
			rs.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
			rs.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
			rs.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

			rs.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
			rs.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
			rs.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
			rs.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

			rs.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
			rs.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
			rs.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
			rs.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

			rs.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
			rs.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
			rs.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
			rs.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

			return rs;
		}

		inline Matrix4 operator * ( const Matrix4 &rhs ) const
		{
			return Concatenate( rhs );
		}

		inline Vec4 operator * ( const Vec4& rhs ) const
		{
			return Vec4(
				m[0][0] * rhs.x + m[0][1] * rhs.y + m[0][2] * rhs.z + m[0][3] * rhs.w, 
				m[1][0] * rhs.x + m[1][1] * rhs.y + m[1][2] * rhs.z + m[1][3] * rhs.w,
				m[2][0] * rhs.x + m[2][1] * rhs.y + m[2][2] * rhs.z + m[2][3] * rhs.w,
				m[3][0] * rhs.x + m[3][1] * rhs.y + m[3][2] * rhs.z + m[3][3] * rhs.w
				);
		}

		static const Matrix4 ZERO;
		static const Matrix4 IDENTITY;

		Real m[4][4];
	};

	/**
	* @brief 1x4行向量 * 4x4矩阵 = 1x4行向量
	*/
	inline Vec4 operator * ( const Vec4& lhs , const Matrix4& rhs )
	{
		return Vec4( 
			lhs.x * rhs.m[0][0] + lhs.y * rhs.m[1][0] + lhs.z * rhs.m[2][0] + lhs.w * rhs.m[3][0] ,
			lhs.x * rhs.m[0][1] + lhs.y * rhs.m[1][1] + lhs.z * rhs.m[2][1] + lhs.w * rhs.m[3][1] ,
			lhs.x * rhs.m[0][2] + lhs.y * rhs.m[1][2] + lhs.z * rhs.m[2][2] + lhs.w * rhs.m[3][2] ,
			lhs.x * rhs.m[0][3] + lhs.y * rhs.m[1][3] + lhs.z * rhs.m[2][3] + lhs.w * rhs.m[3][3] 
			);
	}

}//namespace Math

#endif //ZP_MATRIX4