#ifndef ZP_MATHUTIL
#define ZP_MATHUTIL

#include "ZPDependency.h"
 
	namespace Math
	{    
		class MathUtil
		{ 
			MathUtil(){}
		public:
			~MathUtil(){}   

			/*
			===========================
							��ѧ���������
			===========================
			*/
			static Real Fabs( const Real x );
			static Real Cos( const Real x );
			static Real Sin( const Real x );
			static Real Tan( const Real x ); 
			static Real Sqrt( const Real val ); 
			static bool IsNaN( const Real val ); 

			static bool RealEqual( const Real lhs , const Real rhs , const Real tolerance );

			/*
			===========================
							��ѧ����������
			===========================
			*/
			static Real UnitRandom(void);
			static Real RangeRandom( const Real low , const Real high );
			static Real SymmetricRandom(void);

			/*
			===========================
							�����������
			===========================
			*/
			  
			static const Real PI;
			static const Real TWO_PI;
			static const Real HALF_PI;

		};// class MathUtil
		 
	}//namespace Math
	 
#endif//ZP_MATHUTIL