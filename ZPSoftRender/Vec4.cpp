#include "Vec4.h"
#include "Vec3.h"
#include "MathUtil.h"

namespace Math
{

Vec4::Vec4( void ):x(0.0f),y(0.0f),z(0.0f),w(0.0f){ }

Vec4::Vec4( const Vec3& v )
	:x( v.x ),y( v.y ),z( v.z ),w( 1.0f )
{ 
}

Vec4::Vec4( const Real in_x , const Real in_y , const Real in_z , const Real in_w )
	:x(in_x),y(in_y),z(in_z),w(in_w)
{ 
}

Vec4::Vec4( const Real val )
	:x(val),y(val),z(val),w(val)
{ 
}

Vec4::Vec4( Real* const r )
	:x( r[0] ), y( r[1] ), z( r[2] ), w( r[3] )
{ 
}


Vec4::~Vec4()
{
}

Real Vec4::operator[]( const int i ) const
{
	assert( ( i >= 0 && i < 4 ) );
	return *( &x + i );
}

Real& Vec4::operator[]( const int i ) 
{
	assert( ( i >= 0 && i < 4 ) );
	return *( &x + i );
}

Vec4& Vec4::operator=( const Vec4& rhs )
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

Vec4& Vec4::operator=( const Real val )
{
	x = val;
	y = val;
	z = val;
	w = val;
	return *this;
}

Vec4& Vec4::operator=( const Vec3& rhs )
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = 1.0f;
	return *this;
}

bool Vec4::operator==( const Vec4& rhs ) const
{
	return ( x == rhs.x &&
		y == rhs.y &&
		z == rhs.z &&
		w == rhs.w );
}

bool Vec4::operator!=( const Vec4& rhs ) const
{
	return ( x != rhs.x ||
		y != rhs.y ||
		z != rhs.z ||
		w != rhs.w );
}

Vec4 Vec4::operator+( const Vec4& rhs ) const
{
	return Vec4( x + rhs.x , y + rhs.y , z + rhs.z , w + rhs.w );
} 

Vec4 Vec4::operator-( const Vec4& rhs ) const
{
	return Vec4( x - rhs.x , y - rhs.y , z - rhs.z , w - rhs.w );
}

Vec4 Vec4::operator*( const Real val ) const
{
	return Vec4( x * val , y * val , z * val , w * val );
}

Vec4 Vec4::operator*( const Vec4& rhs ) const
{
	return Vec4( x * rhs.x , y * rhs.y , z * rhs.z , w * rhs.w );
}

Vec4 Vec4::operator/( const Real val ) const
{
	assert( val != 0.0 );

	Real valInv = 1.0f / val;

	return Vec4( x * valInv , y * valInv , z * valInv , w * valInv );
}

Vec4 Vec4::operator/( const Vec4& rhs ) const
{
	return Vec4( x / rhs.x , y / rhs.y , z / rhs.z , w / rhs.w );
}

Vec4 Vec4::operator+() const
{
	return *this;
}

Vec4 Vec4::operator-() const
{
	return Vec4( -x , - y , -z , -w );
}

Vec4& Vec4::operator+=( const Real val )
{
	x += val;
	y += val;
	z += val;
	w += val;
	return *this;
}

Vec4& Vec4::operator+=( const Vec4& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

Vec4& Vec4::operator-=( const Real val )
{
	x -= val;
	y -= val;
	z -= val;
	w -= val;
	return *this;
}

Vec4& Vec4::operator-=( const Vec4& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

Vec4& Vec4::operator*=( const Real val )
{
	x *= val;
	y *= val;
	z *= val;
	w *= val;
	return *this;
}

Vec4& Vec4::operator*=( const Vec4& rhs )
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;
	return *this;
}

Vec4& Vec4::operator/=( const Real val )
{
	assert( val != 0.0 );

	Real valInv = 1.0f / val;

	x *= valInv;
	y *= valInv;
	z *= valInv;
	w *= valInv;

	return *this;
}

Vec4& Vec4::operator/=( const Vec4& rhs )
{
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	w /= rhs.w;

	return *this;
}

Real Vec4::DotProduct( const Vec4& v ) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

bool Vec4::IsNaN( void ) const
{
	return MathUtil::IsNaN(x) || MathUtil::IsNaN(y) || MathUtil::IsNaN(z) || MathUtil::IsNaN(w);
}

void Vec4::Set( const Real in_x , const Real in_y , const Real in_z , const Real in_w )
{
	x = in_x;
	y = in_y;
	z = in_z;
	w = in_w;
}



Vec4 operator+( const Vec4& lhs , const Real rhs )
{
	return Vec4( lhs.x + rhs , 
		lhs.y + rhs ,
		lhs.z + rhs ,
		lhs.w + rhs 
		);
}

Vec4 operator+( const Real lhs , const Vec4& rhs )
{
	return Vec4(
		lhs + rhs.x ,
		lhs + rhs.y ,
		lhs + rhs.z ,
		lhs + rhs.w
		);
}




Vec4 operator-( const Vec4& lhs , const Real rhs )
{
	return Vec4(
		lhs.x - rhs ,
		lhs.y - rhs ,
		lhs.z - rhs ,
		lhs.w - rhs 
		);
}

Vec4 operator-( const Real lhs , const Vec4& rhs )
{
	return Vec4(
		lhs - rhs.x ,
		lhs - rhs.y ,
		lhs - rhs.z ,
		lhs - rhs.w 
		);
}

Vec4 operator*( const Real lhs , const Vec4& rhs )
{
	return Vec4(
		lhs * rhs.x ,
		lhs * rhs.y ,
		lhs * rhs.z ,
		lhs * rhs.w 
		);
} 


Vec4 operator/( const Real lhs , const Vec4& rhs )
{
	return Vec4(
		lhs / rhs.x ,
		lhs / rhs.y ,
		lhs / rhs.z ,
		lhs / rhs.w 
		);
}

const Vec4 Vec4::ZERO( 0 , 0 , 0 , 0 );

}//namespace Math