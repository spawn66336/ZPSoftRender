#ifndef ZP_VEC3
#define ZP_VEC3

#include "ZPDependency.h"

namespace Math
{ 
	//三维向量
	class Vec3 
	{
	public: 
		Real x , y , z; 

		Vec3( void );  
		Vec3( const Real in_x , const Real in_y , const Real in_z );
		explicit Vec3( Real* const r );
		explicit Vec3( const Real val );
		~Vec3();

		//三维向量的平方长度 （此函数不做耗时的平方根运算）
		Real SquaredLength(void) const { return x*x + y*y + z*z ; }
		//三维向量到v的平方距离 （此函数不做耗时的平方根运算）
		Real SquaredDistance( const Vec3& v ) const ;
		//三维向量的长度（此函数含有平方根运算）
		Real Length(void) const;
		//三维向量到v的距离（此函数含有平方根运算）
		Real Distance( const Vec3& v ) const;
		//检查该三维向量是否是一个无效值
		bool IsNaN(void) const;
		//三维向量长度是否为0
		bool IsZeroLength(void) const;
		//将三维向量单位化
		Real Normalize(void);
		//此向量不会被更改，只返回本向量的一个归一化的拷贝
		Vec3 NormalisedCopy(void) const;
		//计算一个给定法线平面的一个反射向量
		Vec3 Reflect( const Vec3& normal ) const;
		//将三维向量点乘
		Real DotProduct( const Vec3& v ) const;
		//求当前点到点p的中点
		Vec3 MidPoint( const Vec3& p ) const;
		//使向量的每个分量取自身与传入向量v的最小值
		void MakeFloor( const Vec3& v );
		//使向量的每个分量取自身与传入向量v的最大值
		void MakeCeil( const Vec3& v );
		//叉乘运算
		Vec3 CrossProduct( const Vec3& v ) const;
		//生成一个垂直于此向量的向量
		Vec3 Prependicular(void) const;
		//返回是否此向量与另一个向量在误差容忍度内相等
		bool PositionEquals( const Vec3& rhs , Real tolerance = EPSLION ) const;

		Real operator[]( const int i ) const;
		Real& operator[]( const int i );

		Real* Ptr(void);
		const Real* Ptr(void) const;

		Vec3& operator = ( const Vec3& rhs );
		Vec3& operator += ( const Vec3& rhs );
		Vec3& operator -= ( const Vec3& rhs );
		Vec3& operator *= ( const Real val );
		Vec3& operator /= ( const Real val ); 

		Vec3 operator + ( const Vec3& rhs ) const;
		Vec3 operator - ( const Vec3& rhs ) const;
		Vec3 operator * ( const Real val ) const;
		Vec3 operator * ( const Vec3& rhs ) const;
		Vec3 operator / ( const Real val ) const;
		Vec3 operator / ( const Vec3& rhs ) const;

		const Vec3& operator + () const;
		Vec3 operator - () const;

		bool operator == ( const Vec3& rhs ) const;
		bool operator != ( const Vec3& rhs ) const;

		//当前向量的元素都小于rhs向量中的元素时返回 true
		bool operator < ( const Vec3& rhs ) const;
		//当前向量的元素都大于rhs向量中的元素时返回 true
		bool operator > ( const Vec3& rhs ) const;

		static const Vec3 ZERO;
		static const Vec3 UNIT_X;
		static const Vec3 UNIT_Y;
		static const Vec3 UNIT_Z;
		static const Vec3 NEGATIVE_UNIT_X;
		static const Vec3 NEGATIVE_UNIT_Y;
		static const Vec3 NEGATIVE_UNIT_Z;
		static const Vec3 UNIT_SCALE;  
	};

	Vec3 operator + ( const Vec3& lhs , const Real rhs );
	Vec3 operator + ( const Real lhs , const Vec3& rhs );
	Vec3 operator - ( const Vec3& lhs , const Real rhs );
	Vec3 operator - ( const Real lhs , const Vec3& rhs );
	Vec3 operator * ( const Real lhs , const Vec3& rhs );
	Vec3 operator / ( const Real lhs , const Vec3& rhs );

}//namespace Math

#endif //ZP_VEC3
