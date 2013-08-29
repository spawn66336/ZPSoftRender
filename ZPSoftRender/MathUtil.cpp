#include "MathUtil.h" 
#include "Vec4.h"
 
	namespace Math
	{
		const Real MathUtil::PI	 = Real( 4.0f*atan( 1.0f ) );
		const Real MathUtil::HALF_PI = 0.5f*PI ; 
		const Real MathUtil::TWO_PI = 2.0f*PI;
		const Real MathUtil::fDeg2Rad = PI / Real(180.0);
		const Real MathUtil::fRad2Deg = Real(180.0) / PI;
		const Real MathUtil::fByte2NormReal = ( 1.0f / 255.0f ) ;
		
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

		Math::BGRA8888_t MathUtil::ColorVecToBGRA8888( const Vec4& color )
		{
			BGRA8888_t rgba;
			unsigned char* pElems = reinterpret_cast<unsigned char*>( &rgba ); 
			pElems[0] = static_cast<unsigned char>( 255.0f * Clamp( color.z , 0.0f , 1.0f ) );
			pElems[1] = static_cast<unsigned char>( 255.0f * Clamp( color.y , 0.0f , 1.0f ) );
			pElems[2] = static_cast<unsigned char>( 255.0f * Clamp( color.x , 0.0f , 1.0f ) );
			pElems[3] = static_cast<unsigned char>( 255.0f * Clamp( color.w , 0.0f , 1.0f ) );
			return rgba; 
		}

		Real MathUtil::Clamp( const Real val , const Real lower , const Real upper )
		{
			Real final_val = val;
			if( val <= lower )
			{
				final_val = lower;
			}
			if( val >= upper )
			{
				final_val = upper;
			}
			return final_val; 
		}

		int MathUtil::Abs( const int x )
		{
			return abs(x);
		}

		Real MathUtil::Pow(  const Real x , const Real y  )
		{
			return pow( x , y );
		}

		Real MathUtil::Ceil( const Real x )
		{
			return ceil(x);
		}

		Math::Vec4 MathUtil::RGBA8888ToVec4(  RGBA8888_t color )
		{
			Math::Vec4 v4Color;
			unsigned char* pColor = reinterpret_cast<unsigned char*>( &color );
			v4Color.x = static_cast<Real>( pColor[0] ) * fByte2NormReal;
			v4Color.y = static_cast<Real>( pColor[1] ) * fByte2NormReal;
			v4Color.z = static_cast<Real>( pColor[2] ) * fByte2NormReal;
			v4Color.w = static_cast<Real>( pColor[3] ) * fByte2NormReal;
			return v4Color;
		}

	

  

	}// namespace Math 