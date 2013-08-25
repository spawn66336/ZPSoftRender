#include "Vec3.h"
#include "MathUtil.h"

namespace Math
{

	const Vec3 Vec3::ZERO(0.0f,0.0f,0.0f);
	const Vec3 Vec3::UNIT_X(1.0f,0.0f,0.0f);
	const Vec3 Vec3::UNIT_Y(0.0f,1.0f,0.0f);
	const Vec3 Vec3::UNIT_Z(0.0f,0.0f,1.0f);
	const Vec3 Vec3::NEGATIVE_UNIT_X(-1.0f,0.0f,0.0f);
	const Vec3 Vec3::NEGATIVE_UNIT_Y(0.0f,-1.0f,0.0f);
	const Vec3 Vec3::NEGATIVE_UNIT_Z(0.0f,0.0f,-1.0f);
	const Vec3 Vec3::UNIT_SCALE(1.0f,1.0f,1.0f); 

	Vec3::Vec3( void ):x(0.0f),y(0.0f),z(0.0f){}  
	Vec3::Vec3( const Real val ):x(val),y(val),z(val){} 
	Vec3::Vec3( const Real in_x , const Real in_y , const Real in_z ):x(in_x),y(in_y),z(in_z){}  
	Vec3::Vec3( Real* const r )
		: x( r[0] ), y( r[1] ), z( r[2] )
	{ 
	}

	Vec3::~Vec3(){}

	Real Vec3::SquaredDistance( const Vec3& v ) const
	{
		return ( (*this) - v ).SquaredLength(); 
	}

	Real Vec3::Length( void ) const
	{
		return MathUtil::Sqrt( x*x + y*y +z*z );
	}

	Real Vec3::Distance( const Vec3& v ) const
	{
		return ( (*this) - v ).Length();
	}

	bool Vec3::IsNaN( void ) const
	{
		return MathUtil::IsNaN( x ) || MathUtil::IsNaN( y ) || MathUtil::IsNaN( z );
	}

	bool Vec3::IsZeroLength( void ) const
	{
		Real sqlen = (x * x) + (y * y) + ( z * z );
		return (sqlen < (EPSLION * EPSLION));
	}

	Real Vec3::Normalize( void )
	{
		Real len = MathUtil::Sqrt( ( x*x )+( y*y )+( z*z ) );
		if( len > 1e-08 )
		{
			Real invLen = 1.0f / len;
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}
		return len;
	}

	Real Vec3::DotProduct( const Vec3& v ) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3 Vec3::MidPoint( const Vec3& p ) const
	{
		return Vec3(
			( x + p.x )*0.5f ,
			( y + p.y )*0.5f ,
			( z + p.z )*0.5f
			);
	}

	void Vec3::MakeFloor( const Vec3& v )
	{
		if( v.x < x ) x = v.x;
		if( v.y < y ) y = v.y;
		if( v.z < z )  z = v.z;
	}

	void Vec3::MakeCeil( const Vec3& v )
	{
		if( v.x > x ) x = v.x;
		if( v.y > y ) y = v.y;
		if( v.z > z )  z = v.z;
	}

	Vec3 Vec3::CrossProduct( const Vec3& v ) const
	{
		return Vec3(
			y * v.z - z * v.y ,
			z * v.x - x * v.z ,
			x * v.y - y * v.x
			);
	}

	Vec3 Vec3::Prependicular( void ) const
	{
		static const Real squareZero = (Real)( 1e-06 * 1e-06 );

		Vec3 perp = this->CrossProduct( Vec3::UNIT_X );

		if( perp.SquaredLength() < squareZero )
		{
			//此向量是 Y 轴的倍数，所以我们需要用另一个轴
			perp = this->CrossProduct( Vec3::UNIT_Y );
		}

		perp.Normalize();
		return perp;
	}

	Vec3 Vec3::NormalisedCopy( void ) const
	{
		Vec3 ret = *this;
		ret.Normalize();
		return ret;
	}

	Vec3 Vec3::Reflect( const Vec3& normal ) const
	{
		return Vec3( *this - ( 2 * this->DotProduct(normal) * normal ) );
	}

	Real Vec3::operator[]( const int i ) const
	{
		assert( ( i >= 0 && i < 3 ) );
		return *( &x + i );
	}

	Real& Vec3::operator[]( const int i )
	{
		assert( ( i >= 0 && i < 3 ) );
		return *( &x + i );
	}

	Real* Vec3::Ptr( void )
	{
		return &x;
	}

	const Real* Vec3::Ptr( void ) const
	{
		return &x;
	}

	Vec3& Vec3::operator=( const Vec3& rhs )
	{
		x = rhs.x; y = rhs.y; z = rhs.z;
		return *this;
	}

	Vec3& Vec3::operator+=( const Vec3& rhs )
	{
		x += rhs.x; y += rhs.y; z += rhs.z;
		return *this;
	}

	Vec3& Vec3::operator-=( const Vec3& rhs )
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
		return *this;
	}

	Vec3& Vec3::operator*=( const Real val )
	{
		x *= val; y *= val; z *= val;
		return *this;
	}

	Vec3& Vec3::operator/=( const Real val )
	{
		x /= val; y /= val; z /= val;
		return *this;
	}

	Vec3 Vec3::operator+( const Vec3& rhs ) const
	{
		return Vec3(
			x + rhs.x ,
			y + rhs.y ,
			z + rhs.z 
			);
	}

	Vec3 Vec3::operator-( const Vec3& rhs ) const
	{
		return Vec3(
			x - rhs.x ,
			y - rhs.y ,
			z - rhs.z 
			);
	}

	Vec3 Vec3::operator*( const Real val ) const
	{
		return Vec3(
			x * val ,
			y * val ,
			z * val 
			);
	}

	Vec3 Vec3::operator*( const Vec3& rhs ) const
	{
		return Vec3(
			x * rhs.x ,
			y * rhs.y ,
			z * rhs.z 
			);
	}

	Vec3 Vec3::operator/( const Real val ) const
	{
		assert( val != 0.0 );

		Real valInv = 1.0f / val;

		return Vec3(
			x * valInv,
			y * valInv,
			z * valInv);
	}

	Vec3 Vec3::operator/( const Vec3& rhs ) const
	{
		return Vec3(
			x / rhs.x,
			y / rhs.y,
			z / rhs.z);
	}

	const Vec3& Vec3::operator+() const
	{
		return *this;
	}

	Vec3 Vec3::operator-() const
	{
		return Vec3( -x , -y , -z );
	}

	bool Vec3::operator==( const Vec3& rhs ) const
	{
		return ( x == rhs.x ) && ( y == rhs.y ) && ( z == rhs.z );
	}

	bool Vec3::operator!=( const Vec3& rhs ) const
	{
		return ( x != rhs.x ) || ( y != rhs.y ) || ( z != rhs.z );
	}

	bool Vec3::operator<( const Vec3& rhs ) const
	{
		if( x < rhs.x && y < rhs.y && z < rhs.z )
			return true;
		return false;
	}

	bool Vec3::operator>( const Vec3& rhs ) const
	{
		if( x > rhs.x && y > rhs.y && z > rhs.z )
			return true;
		return false;
	}

	bool Vec3::PositionEquals( const Vec3& rhs , Real tolerance /*= EPSLION */ ) const
	{
		return MathUtil::RealEqual( x , rhs.x , tolerance ) &&
			MathUtil::RealEqual( y , rhs.y , tolerance ) &&
			MathUtil::RealEqual( z , rhs.z , tolerance );
	}

	void Vec3::Set( const Real in_x , const Real in_y , const Real in_z )
	{
		x = in_x;
		y = in_y;
		z = in_z;
	}





	Vec3 operator+( const Vec3& lhs , const Real rhs )
	{
		return Vec3(
			lhs.x + rhs,
			lhs.y + rhs,
			lhs.z + rhs
			);
	}

	Vec3 operator+( const Real lhs , const Vec3& rhs )
	{
		return Vec3(
			lhs + rhs.x,
			lhs + rhs.y,
			lhs + rhs.z 
			);
	}

	Vec3 operator-( const Vec3& lhs , const Real rhs )
	{
		return Vec3( 
			lhs.x - rhs , 
			lhs.y - rhs , 
			lhs.z - rhs 
			);
	}

	Vec3 operator-( const Real lhs , const Vec3& rhs )
	{
		return Vec3(
			lhs - rhs.x ,
			lhs - rhs.y ,
			lhs - rhs.z
			);
	}

	Vec3 operator*( const Real lhs , const Vec3& rhs )
	{
		return Vec3(
			lhs * rhs.x ,
			lhs * rhs.y ,
			lhs * rhs.z 
			);
	}

	Vec3 operator/( const Real lhs , const Vec3& rhs )
	{
		return Vec3(
			lhs / rhs.x ,
			lhs / rhs.y ,
			lhs / rhs.z 
			);
	}
 

}//namespace Math