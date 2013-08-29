#ifndef ZP_MATHUTIL
#define ZP_MATHUTIL

#include "ZPDependency.h"


	namespace Math
	{   
		class Vec4;

		typedef unsigned int BGRA8888_t;
		typedef unsigned int RGBA8888_t;

		template<typename T>
		void SwapT( T& lhs , T& rhs )
		{
			T tmp = lhs;
			lhs = rhs;
			rhs = tmp;
		}

		class MathUtil
		{ 
			MathUtil(){}
		public:
			~MathUtil(){}   

			/*
			===========================
							数学库基础函数
			===========================
			*/
			static int Abs( const int x );
			static Real Fabs( const Real x );
			static Real Cos( const Real x );
			static Real Sin( const Real x );
			static Real Tan( const Real x ); 
			static Real Sqrt( const Real val ); 
			static Real Pow( const Real x , const Real y );
			static bool IsNaN( const Real val ); 
			static Real Ceil( const Real x );

			static inline Real DegreesToRadians(Real deg) { return deg * fDeg2Rad; }
			static inline Real RadiansToDegrees(Real rad) { return rad * fRad2Deg; }

			static bool RealEqual( const Real lhs , const Real rhs , const Real tolerance );

			/*
			===========================
							数学库随机数相关
			===========================
			*/
			static Real UnitRandom(void);
			static Real RangeRandom( const Real low , const Real high );
			static Real SymmetricRandom(void);

			static Real Clamp( const Real val , const Real lower , const Real upper );

			static BGRA8888_t ColorVecToBGRA8888( const Vec4& color );

			static Math::Vec4 RGBA8888ToVec4(  RGBA8888_t color );

			static inline unsigned char MapDepthToU8( const Real depth )	
			{
				return static_cast<unsigned char>( 255.0f * depth );
			}
			static inline unsigned int MapDepthToU32( const Real depth )
			{
				return static_cast<unsigned int>( static_cast<Real>( 0xffffffff ) *depth );
			}

			/*
			===========================
							矩形相关运算
			===========================
			*/
			  
			static const Real PI;
			static const Real TWO_PI;
			static const Real HALF_PI;
			static const Real fDeg2Rad;
			static const Real fRad2Deg;
			static const Real fByte2NormReal;

		};// class MathUtil
		 
	}//namespace Math
	 
#endif//ZP_MATHUTIL