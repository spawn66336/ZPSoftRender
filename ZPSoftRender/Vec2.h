#ifndef ZP_VEC2
#define ZP_VEC2

#include "ZPDependency.h"

namespace Math
{     
		//��ά����
		class ZPEXPORT Vec2
		{
		public: 

			Real x , y;

			Vec2( void ); 
			Vec2( const Real in_x , const Real in_y );
			explicit Vec2( Real* const r );
			explicit Vec2( const Real val ); 
			~Vec2();  

			void Set( const Real in_x , const Real in_y );
			//��ά������ƽ������ ���˺���������ʱ��ƽ�������㣩
			Real SquaredLength(void) const { return x*x + y*y; }
			//��ά������v��ƽ������ ���˺���������ʱ��ƽ�������㣩
			Real SquaredDistance( const Vec2& v ) const ;
			//��ά�����ĳ��ȣ��˺�������ƽ�������㣩
			Real Length(void) const;
			//��ά������v�ľ��루�˺�������ƽ�������㣩
			Real Distance( const Vec2& v ) const;
			//���ö�ά�����Ƿ���һ����Чֵ
			bool IsNaN(void) const;
			//��ά���������Ƿ�Ϊ0
			bool IsZeroLength(void) const;
			//����ά������λ��
			Real Normalize(void);
			//����ά�������
			Real DotProduct( const Vec2& v ) const;
			//��ǰ�㵽��p���е�
			Vec2 MidPoint( const Vec2& p ) const;
			//ʹ������ÿ������ȡ�����봫������v����Сֵ
			void MakeFloor( const Vec2& v );
			//ʹ������ÿ������ȡ�����봫������v�����ֵ
			void MakeCeil( const Vec2& v );


			Real operator[]( const int i ) const;
			Real& operator[]( const int i );

			Real* Ptr(void);
			const Real* Ptr(void) const;

			Vec2& operator = ( const Vec2& rhs );
			Vec2& operator = ( const Real rhs );

			Vec2 operator + ( const Vec2& rhs ) const;
			Vec2 operator - ( const Vec2& rhs ) const;
			Vec2 operator * ( const Real rhs ) const;
			Vec2 operator * ( const Vec2& rhs ) const;
			Vec2 operator / ( const Real rhs ) const;
			Vec2 operator / ( const Vec2& rhs ) const; 

			Vec2& operator += ( const Vec2& rhs );
			Vec2& operator -= ( const Vec2& rhs );
			Vec2& operator *= ( const Real val );
			Vec2& operator /= ( const Real val ); 

			const Vec2& operator + () const;
			Vec2 operator - () const;

			bool operator == ( const Vec2& rhs ) const;
			bool operator != ( const Vec2& rhs ) const;

			//��ǰ������Ԫ�ض�С��rhs�����е�Ԫ��ʱ���� true
			bool operator < ( const Vec2& rhs ) const;
			//��ǰ������Ԫ�ض�����rhs�����е�Ԫ��ʱ����true
			bool operator > ( const Vec2& rhs ) const;


			static const Vec2 ZERO;
			static const Vec2 UNIT_X;
			static const Vec2 UNIT_Y;
			static const Vec2 NEGATIVE_UNIT_X;
			static const Vec2 NEGATIVE_UNIT_Y;
			static const Vec2 UNIT_SCALE;

	 };   

	Vec2 operator + ( const Vec2& lhs , const Real rhs );
	Vec2 operator + ( const Real lhs , const Vec2& rhs );
	Vec2 operator - ( const Vec2& lhs , const Real rhs );
	Vec2 operator - ( const Real lhs , const Vec2& rhs );
	Vec2 operator * ( const Real lhs , const Vec2& rhs );
	Vec2 operator / ( const Real lhs , const Vec2& rhs );

}//namespace Math

#endif //ZP_VEC2