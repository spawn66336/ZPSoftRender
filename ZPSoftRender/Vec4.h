#ifndef ZP_VEC4
#define ZP_VEC4

#include "ZPDependency.h"

namespace Math
{ 
	class Vec3;

	class ZPEXPORT Vec4
	{
	public: 
		Real x , y , z , w; 

		Vec4(void);
		Vec4( const Real in_x , const Real in_y , const Real in_z , const Real in_w ); 
		explicit Vec4( const Vec3& v );
		explicit Vec4( Real* const r );
		explicit Vec4( const Real val );

		~Vec4();

		Real operator[]( const int i ) const;
		Real& operator[]( const int i );

		Real* Ptr(void){ return &x; }
		const Real* Ptr(void) const { return &x; }

		//赋值运算符
		Vec4& operator=( const Vec4& rhs );
		Vec4& operator=( const Vec3& rhs );
		Vec4& operator=( const Real val );

		//比较运算符
		bool operator==( const Vec4& rhs) const;
		bool operator != ( const Vec4& rhs ) const;


		Vec4 operator + ( const Vec4& rhs ) const;
		Vec4 operator - ( const Vec4& rhs ) const;
		Vec4 operator * ( const Real val ) const;
		Vec4 operator * ( const Vec4& rhs ) const;
		Vec4 operator / ( const Real val ) const;
		Vec4 operator / ( const Vec4& rhs ) const;
		Vec4 operator + () const;
		Vec4 operator - () const;

		Vec4& operator += ( const Real val );
		Vec4& operator += ( const Vec4& rhs ); 
		Vec4& operator -= ( const Real val );
		Vec4& operator -= ( const Vec4& rhs ); 
		Vec4& operator *= ( const Real val );
		Vec4& operator *= ( const Vec4& rhs );
		Vec4& operator /= ( const Real val );
		Vec4& operator /= ( const Vec4& rhs );

		void Set( const Real in_x , const Real in_y , const Real in_z , const Real in_w );
		Real DotProduct( const Vec4& v  ) const;
		bool IsNaN(void) const;

		static const Vec4 ZERO;

	};

	Vec4 operator + ( const Vec4& lhs , const Real rhs );
	Vec4 operator + ( const Real lhs , const Vec4& rhs );
	Vec4 operator * ( const Real lhs , const Vec4& rhs );
	Vec4 operator / ( const Real lhs , const Vec4& rhs ); 
	Vec4 operator - ( const Vec4& lhs , const Real rhs );
	Vec4 operator - ( const Real lhs , const Vec4& rhs );


}//namespace Math

#endif// ZP_VEC4

