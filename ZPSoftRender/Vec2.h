#ifndef ZP_VEC2
#define ZP_VEC2

#include "ZPDependency.h"

namespace Math
{     
		//二维向量
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
			//二维向量的平方长度 （此函数不做耗时的平方根运算）
			Real SquaredLength(void) const { return x*x + y*y; }
			//二维向量到v的平方距离 （此函数不做耗时的平方根运算）
			Real SquaredDistance( const Vec2& v ) const ;
			//二维向量的长度（此函数含有平方根运算）
			Real Length(void) const;
			//二维向量到v的距离（此函数含有平方根运算）
			Real Distance( const Vec2& v ) const;
			//检查该二维向量是否是一个无效值
			bool IsNaN(void) const;
			//二维向量长度是否为0
			bool IsZeroLength(void) const;
			//将二维向量单位化
			Real Normalize(void);
			//将二维向量点乘
			Real DotProduct( const Vec2& v ) const;
			//求当前点到点p的中点
			Vec2 MidPoint( const Vec2& p ) const;
			//使向量的每个分量取自身与传入向量v的最小值
			void MakeFloor( const Vec2& v );
			//使向量的每个分量取自身与传入向量v的最大值
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

			//当前向量的元素都小于rhs向量中的元素时返回 true
			bool operator < ( const Vec2& rhs ) const;
			//当前向量的元素都大于rhs向量中的元素时返回true
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