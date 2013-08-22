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
		* @brief 3x3����Ĭ�Ϲ��캯��
		*/
		inline Matrix3(void){}

		inline explicit Matrix3( const Real a[3][3] )
		{
			memcpy( m , a , sizeof(Real)*9 );
		}

		/**
		* @brief �������캯��
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
		* @brief ��������
		*/
		~Matrix3(void){}

		/**
		* @brief ���������
		* @param iRow Ҫ��õ��к�
		*/
		Vec3 GetRow( const int iRow ) const;

		/**
		* @brief ���������
		* @param iCol Ҫ��õ��к�
		*/
		Vec3 GetColumn( const int iCol ) const;

		/**
		* @brief ����������
		* @param iRow Ҫ���õ��к�
		* @param vec   Ҫ���õ�����
		*/
		void SetRow( const int iRow , const Vec3& vec );

		/**
		* @brief ����������
		* @param iCol Ҫ���õ��к�
		* @param vec Ҫ���õ�����
		*/
		void SetColumn( const int iCol , const Vec3& vec );

		/**
		* @brief ��������������ֱ�����Ϊx , y , z��
		* @param xAxis �����X��
		* @param yAxis �����Y��
		* @param zAxis �����Z��
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
		* @brief 3x3���� * 3x1������ = 3x1������
		*/
		Vec3 operator* (const Vec3& rhs) const;
		 
		/**
		* @brief 3x3���� * ����
		*/
		Matrix3 operator* ( Real rhs ) const;

		/**
		* @brief ����ת��
		*/
		Matrix3 Transpose () const;

		/**
		* @brief ���Χ����������ת�ľ���
		* @param axis ����ת��
		* @param theta ����ת�Ƕȣ����Ƚǣ�
		*/
		static Matrix3 MakeRotateWithAxisMatrix( const Vec3& axis , const Real theta );

		static const Matrix3 ZERO;
		static const Matrix3 IDENTITY;

		Real m[3][3]; 
	};

	/**
	* @brief 1x3������ * 3x3���� = 1x3 ������
	*/
	Vec3 operator* (const Vec3& lhs, const Matrix3& rhs);

	/**
	* @brief ���� * 3x3���� = 3x3����
	*/
	Matrix3 operator* (const Real lhs, const Matrix3& rhs);

}//namespace Math

#endif //ZP_MATRIX3