#ifndef ZP_MATRIX3
#define ZP_MATRIX3

#include "ZPDependency.h"
#include "Vec3.h"

namespace Math
{ 

	class Matrix3
	{
	public:
		/**
		* @brief 3x3矩阵默认构造函数
		*/
		inline Matrix3(void){}

		inline explicit Matrix3( const Real a[3][3] )
		{
			memcpy( m , a , sizeof(Real)*9 );
		}

		/**
		* @brief 拷贝构造函数
		*/
		inline Matrix3( const Matrix3& otherMatrix )
		{
			memcpy( m , otherMatrix.m , sizeof(Real)*9 );
		}

		inline Matrix3( 
			Real m00 , Real m01 , Real m02 ,
			Real m10 , Real m11 , Real m12 ,
			Real m20 , Real m21 , Real m22 
			)
		{
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; 
		}

		
		/**
		* @brief 析构函数
		*/
		~Matrix3(void){}

		/**
		* @brief 获得行向量
		* @param iRow 要获得的行号
		*/
		Vec3 GetRow( const int iRow ) const;

		/**
		* @brief 获得列向量
		* @param iCol 要获得的列号
		*/
		Vec3 GetColumn( const int iCol ) const;

		/**
		* @brief 设置行向量
		* @param iRow 要设置的行号
		* @param vec   要设置的向量
		*/
		void SetRow( const int iRow , const Vec3& vec );

		/**
		* @brief 设置列向量
		* @param iCol 要设置的列号
		* @param vec 要设置的向量
		*/
		void SetColumn( const int iCol , const Vec3& vec );

		/**
		* @brief 将矩阵的列向量分别设置为x , y , z轴
		* @param xAxis 坐标架X轴
		* @param yAxis 坐标架Y轴
		* @param zAxis 坐标架Z轴
		*/
		void FromAxes( const Vec3& xAxis , const Vec3& yAxis , const Vec3& zAxis );

		inline Matrix3& operator=( const Matrix3& rhs )
		{
			memcpy( m,rhs.m, 9*sizeof(Real) );
			return *this;
		}

		bool operator== (const Matrix3& rhs) const;

		inline bool operator!= (const Matrix3& rhs ) const
		{
			return !operator==(rhs);
		}

		Matrix3 operator+ (const Matrix3& rhs) const;
		Matrix3 operator- (const Matrix3& rhs) const;
		Matrix3 operator* (const Matrix3& rhs) const;
		Matrix3 operator- () const;
		 
		/**
		* @brief 3x3矩阵 * 3x1列向量 = 3x1列向量
		*/
		Vec3 operator* (const Vec3& rhs) const;
		 
		/**
		* @brief 3x3矩阵 * 标量
		*/
		Matrix3 operator* ( Real rhs ) const;

		/**
		* @brief 矩阵转置
		*/
		Matrix3 Transpose () const;

		/**
		* @brief 获得围绕任意轴旋转的矩阵
		* @param axis 是旋转轴
		* @param theta 是旋转角度（弧度角）
		*/
		static Matrix3 MakeRotateWithAxisMatrix( const Vec3& axis , const Real theta );

		static const Matrix3 ZERO;
		static const Matrix3 IDENTITY;

		Real m[3][3]; 
	};

	/**
	* @brief 1x3行向量 * 3x3矩阵 = 1x3 行向量
	*/
	Vec3 operator* (const Vec3& lhs, const Matrix3& rhs);

	/**
	* @brief 标量 * 3x3矩阵 = 3x3矩阵
	*/
	Matrix3 operator* (const Real lhs, const Matrix3& rhs);

}//namespace Math

#endif //ZP_MATRIX3