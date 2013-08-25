#include "Vec2.h"
#include "MathUtil.h"

namespace Math
{ 

	const Vec2 Vec2::ZERO( 0 , 0 );
	const Vec2 Vec2::UNIT_X( 1 , 0 );
	const Vec2 Vec2::UNIT_Y( 0 , 1 );
	const Vec2 Vec2::NEGATIVE_UNIT_X( -1 , 0 );
	const Vec2 Vec2::NEGATIVE_UNIT_Y( 0 , -1 );
	const Vec2 Vec2::UNIT_SCALE( 1 , 1 );

	Vec2::Vec2( void ):x(0.0f),y(0.0f){} 
	Vec2::Vec2( const Real val ):x( val ),y( val ){} 
	Vec2::Vec2( const Real in_x , const Real in_y ):x( in_x ),y( in_y ){}  
	Vec2::Vec2( Real* const r )
		:x( r[0] ), y( r[1] )
	{ 
	}

	Vec2::~Vec2(){}

	Vec2& Vec2::operator = ( const Vec2& rhs )
	{
		x = rhs.x; y = rhs.y;
		return *this;
	}

	Vec2& Vec2::operator=( const Real rhs )
	{
		x = rhs; y = rhs;
		return *this;
	}

	Vec2& Vec2::operator += ( const Vec2& rhs )
	{
		x += rhs.x; y += rhs.y;
		return *this;
	}

	Vec2& Vec2::operator -= ( const Vec2& rhs )
	{
		x -= rhs.x; y -= rhs.y;
		return *this;
	}

	Vec2& Vec2::operator *= ( const Real val )
	{
		x *= val; y*= val;
		return *this;
	}

	Vec2& Vec2::operator /= ( const Real val )
	{
		x /= val; y /= val;
		return *this;
	}

	bool Vec2::operator==( const Vec2& rhs ) const
	{
		return ( x == rhs.x ) && ( y == rhs.y );
	} 

	bool Vec2::operator!=( const Vec2& rhs ) const
	{
		return ( x != rhs.x ) || ( y != rhs.y );
	} 

	bool Vec2::operator<( const Vec2& rhs ) const
	{
		if( x < rhs.x && y < rhs.y )
			return true;
		return false;
	}

	bool Vec2::operator>( const Vec2& rhs ) const
	{
		if( x > rhs.x && y > rhs.y )
			return true;
		return false;
	}


	void Vec2::Set( const Real in_x , const Real in_y )
	{
		x = in_x;
		y = in_y;
	}


	Real Vec2::SquaredDistance( const Vec2& v ) const
	{
		return ( (*this) - v ).SquaredLength(); 
	}

	Real Vec2::Length( void ) const
	{
		return MathUtil::Sqrt( x*x + y*y );
	}

	Real Vec2::Distance( const Vec2& v ) const
	{
		return ((*this) - v).Length();
	}

	bool Vec2::IsNaN( void ) const
	{
		return MathUtil::IsNaN( x ) || MathUtil::IsNaN( y );
	}

	bool Vec2::IsZeroLength( void ) const
	{
		Real sqlen = (x * x) + (y * y);
		return (sqlen < (EPSLION * EPSLION));
	}

	Real Vec2::Normalize( void )
	{
		Real len = MathUtil::Sqrt( ( x*x )+( y*y ) );
		if( len > 1e-08 )
		{
			Real invLen = 1.0f / len;
			x *= invLen;
			y *= invLen;
		}
		return len;
	}

	Real Vec2::DotProduct( const Vec2& v ) const
	{
		return x * v.x + y * v.y;
	}


	Vec2 Vec2::MidPoint( const Vec2& p ) const
	{
		return Vec2(
			( x + p.x )*0.5f ,
			( y + p.y )*0.5f
			);
	}

	Real Vec2::operator[]( const int i ) const
	{
		assert( ( i >= 0 && i < 2 ) );
		return *( &x + i );
	}

	Real& Vec2::operator[]( const int i )
	{
		assert( ( i >= 0 && i < 2 ) );
		return *( &x + i );
	}

	Real* Vec2::Ptr( void )
	{
		return &x;
	}

	const Real* Vec2::Ptr( void ) const
	{
		return &x;
	}



	Vec2 Vec2::operator+( const Vec2& rhs ) const
	{
		return Vec2( 
			x + rhs.x ,
			y + rhs.y 
			);
	}

	Vec2 Vec2::operator-( const Vec2& rhs ) const
	{
		return Vec2(
			x - rhs.x ,
			y - rhs.y 
			);
	}

	Vec2 Vec2::operator*( const Real rhs ) const
	{
		return Vec2(
			x * rhs ,
			y * rhs 
			);
	}

	Vec2 Vec2::operator*( const Vec2& rhs ) const
	{
		return Vec2(
			x * rhs.x ,
			y * rhs.y
			);
	}

	Vec2 Vec2::operator/( const Real rhs ) const
	{
		assert( rhs != 0.0 );

		Real valInv = 1.0f / rhs;

		return Vec2(
			x * valInv,
			y * valInv
			);
	}

	Vec2 Vec2::operator/( const Vec2& rhs ) const
	{
		return Vec2(
			x / rhs.x,
			y / rhs.y
			);
	}

	const Vec2& Vec2::operator + () const
	{
		return *this;
	}

	Vec2 Vec2::operator - () const
	{
		return Vec2( -x , -y );
	}

	void Vec2::MakeFloor( const Vec2& v )
	{
		if( v.x < x ) x = v.x;
		if( v.y < y ) y = v.y;
	}

	void Vec2::MakeCeil( const Vec2& v )
	{
		if( v.x > x ) x = v.x;
		if( v.y > y ) y = v.y;
	}






	Vec2 operator+( const Real lhs , const Vec2& rhs )
	{
		return Vec2(
			lhs + rhs.x ,
			lhs + rhs.y 
			);
	}

	Vec2 operator+( const Vec2& lhs , const Real rhs )
	{
		return Vec2(
			lhs.x + rhs ,
			lhs.y + rhs 
			);
	} 

	Vec2 operator - ( const Vec2& lhs , const Real rhs )
	{
		return Vec2(
			lhs.x - rhs ,
			lhs.y - rhs 
			);
	}

	Vec2 operator - ( const Real lhs , const Vec2& rhs )
	{
		return Vec2(
			lhs - rhs.x ,
			lhs - rhs.y 
			);
	}

	Vec2 operator * ( const Real lhs , const Vec2& rhs )
	{
		return Vec2( 
			lhs * rhs.x ,
			lhs * rhs.y 
			);
	}

	Vec2 operator / ( const Real lhs , const Vec2& rhs )
	{
		return Vec2(
			lhs / rhs.x ,
			lhs / rhs.y
			);
	}

}//namespace Math