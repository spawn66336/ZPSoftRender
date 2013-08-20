#include "MathUtil.h" 
 
	namespace Math
	{
		const Real MathUtil::PI	 = Real( 4.0f*atan( 1.0f ) );
		const Real MathUtil::HALF_PI = 0.5f*PI ; 
		const Real MathUtil::TWO_PI = 2.0f*PI;

		 
		
		Real MathUtil::Fabs( const Real x )
		{
			return fabs( x );
		}

		Real MathUtil::Cos( const Real x )
		{
			return cos( x );
		}

		Real MathUtil::Sin( const Real x )
		{
			return sin( x );
		}

		Real MathUtil::Tan( const Real x )
		{
			return tan( x );
		}
		  

		Real MathUtil::Sqrt( const Real val )
		{
			return sqrt( val );
		} 

		bool MathUtil::IsNaN( const Real val )
		{
			return val != val;
		}

		Real MathUtil::UnitRandom( void )
		{
			return Real( rand() ) / Real( RAND_MAX );
		}

		Real MathUtil::RangeRandom( const Real low , const Real high )
		{
			return ( high - low ) * UnitRandom() + low ;
		}

		Real MathUtil::SymmetricRandom( void )
		{
			return 2.0f * UnitRandom() - 1.0f;
		} 

		bool MathUtil::RealEqual( const Real lhs , const Real rhs , const Real tolerance )
		{
			if (fabs(rhs-lhs) <= tolerance)
				return true;
			else
				return false;
		}

	



	}// namespace Math 