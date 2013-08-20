#ifndef ZP_RECT
#define ZP_RECT

#include "ZPDependency.h"

namespace Math
{
	 
	template<typename T>
	class RectT
	{
	public: 
		RectT(void):left(0),right(0),top(0),bottom(0){}
		RectT( const T l , const T r , const T t , const T b ):left(l),right(r),top(t),bottom(b){}
		RectT( const RectT<T>& rect ):left(rect.left),right(rect.right),top(rect.top),bottom(rect.bottom){}
		~RectT(){}

		RectT& operator=( const RectT& rhs )
		{
			left = rhs.left;
			right = rhs.right;
			top = rhs.top;
			bottom = rhs.bottom;
			return *this;
		}

		T Width( void ) const
		{
			return right - left;
		}

		T Height( void ) const
		{
			return bottom - top;
		}

		bool IsNull( void ) const
		{
			return Width() == 0 || Height() == 0;
		}

		void SetNull( void )
		{
			left = right = bottom = top = 0;
		}

		Real GetAspect(void) const
		{
			return Real( Width() ) / Real( Height() );
		}

		RectT& Merge( const RectT& rect )
		{
			if( IsNull() )
			{
				*this = rect;
			}else if( !rect.IsNull() ){
				left = std::min( left , rect.left );
				right = std::max( right , rect.right );
				top = std::min( top , rect.top );
				bottom = std::max( bottom , rect.bottom );
			}
			return *this;
		}

		RectT Intersect( const RectT& rect ) const
		{
			RectT ret;
			if( IsNull() || rect.IsNull() )
			{
				return ret;
			}else{ 
				ret.left = std::max( left , rect.left );
				ret.right = std::min( right , rect.right );
				ret.top = std::max( top , rect.top );
				ret.bottom = std::min( bottom , rect.bottom ); 
			}

			if( ret.left > ret.right || ret.top > ret.bottom )
			{
				ret.SetNull();
				return ret;
			}
			return ret;
		}

		bool IsIntersect( const RectT& rect ) const
		{
			return ( !Intersect( rect ).IsNull() );
		}

		bool IsPointInRect( const Vec2& p  ) const
		{
			if( 
				T(p.x) >= left && T(p.x) <= right &&
				T(p.y) >= top && T(p.y) <= bottom 
				) 
				return true; 

			return false;
		} 

		T left;
		T right;
		T top;
		T bottom;
	};

	typedef RectT<Real> realRect_t;
	typedef RectT<int>	  rect_t; 

}

#endif