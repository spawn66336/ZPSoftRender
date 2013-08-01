#include "ZPMath.h"
#include <math.h>
#include <assert.h>
#include <stdlib.h>

 
	namespace Math
	{
		const Real MathUtil::PI	 = Real( 4.0f*atan( 1.0f ) );
		const Real MathUtil::HALF_PI = 0.5f*PI ; 
		const Real MathUtil::TWO_PI = 2.0f*PI;

		const Vec2 Vec2::ZERO( 0 , 0 );
		const Vec2 Vec2::UNIT_X( 1 , 0 );
		const Vec2 Vec2::UNIT_Y( 0 , 1 );
		const Vec2 Vec2::NEGATIVE_UNIT_X( -1 , 0 );
		const Vec2 Vec2::NEGATIVE_UNIT_Y( 0 , -1 );
		const Vec2 Vec2::UNIT_SCALE( 1 , 1 );
		
		
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
		 

		const Vec3 Vec3::ZERO(0.0f,0.0f,0.0f);
		const Vec3 Vec3::UNIT_X(1.0f,0.0f,0.0f);
		const Vec3 Vec3::UNIT_Y(0.0f,1.0f,0.0f);
		const Vec3 Vec3::UNIT_Z(0.0f,0.0f,1.0f);
		const Vec3 Vec3::NEGATIVE_UNIT_X(-1.0f,0.0f,0.0f);
		const Vec3 Vec3::NEGATIVE_UNIT_Y(0.0f,-1.0f,0.0f);
		const Vec3 Vec3::NEGATIVE_UNIT_Z(0.0f,0.0f,-1.0f);
		const Vec3 Vec3::UNIT_SCALE(1.0f,1.0f,1.0f); 
		  

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

	}// namespace Math 