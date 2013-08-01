#ifndef ZP_MATH
#define ZP_MATH

#define EPSLION	(1e-06)
#define EQUAL( x , y )  ( ( ( (x) - (y) ) < EPSLION ) &&  ( ( (x) - (y) ) > -EPSLION ) )
#define LESS_THAN( x , y ) ( ( (y)-(x) ) > EPSLION )
#define GREATER_THAN( x , y ) ( ( (x)-(y) ) > EPSLION )

 
	namespace Math
	{  

		typedef float Real; 

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


		//��ά����
		class Vec2
		{
		public: 

			Real x , y;

			Vec2( void ); 
			Vec2( const Real in_x , const Real in_y );
			explicit Vec2( Real* const r );
			explicit Vec2( const Real val ); 
			~Vec2();  

			//��ά������ƽ������ ���˺���������ʱ��ƽ�������㣩
			Real SquaredLength(void) const { return x*x + y*y; }
			//��ά������v��ƽ������ ���˺���������ʱ��ƽ�������㣩
			Real SquaredDistance( const Vec2& v ) const ;
			//��ά�����ĳ��ȣ��˺�������ƽ�������㣩
			Real Length(void) const;
			//��ά������v�ľ��루�˺�������ƽ�������㣩
			Real Distance( const Vec2& v ) const;
			//���ö�ά�����Ƿ���һ����Чֵ
			bool IsNaN(void) const;
			//��ά���������Ƿ�Ϊ0
			bool IsZeroLength(void) const;
			//����ά������λ��
			Real Normalize(void);
			//����ά�������
			Real DotProduct( const Vec2& v ) const;
			//��ǰ�㵽��p���е�
			Vec2 MidPoint( const Vec2& p ) const;
			//ʹ������ÿ������ȡ�����봫������v����Сֵ
			void MakeFloor( const Vec2& v );
			//ʹ������ÿ������ȡ�����봫������v�����ֵ
			void MakeCeil( const Vec2& v );
			 

			Real operator[]( const int i ) const;
			Real& operator[]( const int i );

			Real* Ptr(void);
			const Real* Ptr(void) const;

			Vec2& operator = ( const Vec2& rhs );
			Vec2& operator = ( const Real rhs );

			Vec2 operator + ( const Vec2& rhs ) const;
			Vec2 operator - ( const Vec2& rhs ) const;
			Vec2 operator * ( const Real rhs ) const;
			Vec2 operator * ( const Vec2& rhs ) const;
			Vec2 operator / ( const Real rhs ) const;
			Vec2 operator / ( const Vec2& rhs ) const; 

			Vec2& operator += ( const Vec2& rhs );
			Vec2& operator -= ( const Vec2& rhs );
			Vec2& operator *= ( const Real val );
			Vec2& operator /= ( const Real val ); 

			const Vec2& operator + () const;
			Vec2 operator - () const;

			bool operator == ( const Vec2& rhs ) const;
			bool operator != ( const Vec2& rhs ) const;

			//��ǰ������Ԫ�ض�С��rhs�����е�Ԫ��ʱ���� true
			bool operator < ( const Vec2& rhs ) const;
			//��ǰ������Ԫ�ض�����rhs�����е�Ԫ��ʱ����true
			bool operator > ( const Vec2& rhs ) const;

			 
			static const Vec2 ZERO;
			static const Vec2 UNIT_X;
			static const Vec2 UNIT_Y;
			static const Vec2 NEGATIVE_UNIT_X;
			static const Vec2 NEGATIVE_UNIT_Y;
			static const Vec2 UNIT_SCALE;
			 
		};   

		Vec2 operator + ( const Vec2& lhs , const Real rhs );
		Vec2 operator + ( const Real lhs , const Vec2& rhs );
		Vec2 operator - ( const Vec2& lhs , const Real rhs );
		Vec2 operator - ( const Real lhs , const Vec2& rhs );
		Vec2 operator * ( const Real lhs , const Vec2& rhs );
		Vec2 operator / ( const Real lhs , const Vec2& rhs );


		//��ά����
		class Vec3 
		{
		public: 
			Real x , y , z; 

			Vec3( void );  
			Vec3( const Real in_x , const Real in_y , const Real in_z );
			explicit Vec3( Real* const r );
			explicit Vec3( const Real val );
			~Vec3();

			//��ά������ƽ������ ���˺���������ʱ��ƽ�������㣩
			Real SquaredLength(void) const { return x*x + y*y + z*z ; }
			//��ά������v��ƽ������ ���˺���������ʱ��ƽ�������㣩
			Real SquaredDistance( const Vec3& v ) const ;
			//��ά�����ĳ��ȣ��˺�������ƽ�������㣩
			Real Length(void) const;
			//��ά������v�ľ��루�˺�������ƽ�������㣩
			Real Distance( const Vec3& v ) const;
			//������ά�����Ƿ���һ����Чֵ
			bool IsNaN(void) const;
			//��ά���������Ƿ�Ϊ0
			bool IsZeroLength(void) const;
			//����ά������λ��
			Real Normalize(void);
			//���������ᱻ���ģ�ֻ���ر�������һ����һ���Ŀ���
			Vec3 NormalisedCopy(void) const;
			//����һ����������ƽ���һ����������
			Vec3 Reflect( const Vec3& normal ) const;
			//����ά�������
			Real DotProduct( const Vec3& v ) const;
			//��ǰ�㵽��p���е�
			Vec3 MidPoint( const Vec3& p ) const;
			//ʹ������ÿ������ȡ�����봫������v����Сֵ
			void MakeFloor( const Vec3& v );
			//ʹ������ÿ������ȡ�����봫������v�����ֵ
			void MakeCeil( const Vec3& v );
			//�������
			Vec3 CrossProduct( const Vec3& v ) const;
			//����һ����ֱ�ڴ�����������
			Vec3 Prependicular(void) const;
			//�����Ƿ����������һ��������������̶������
			bool PositionEquals( const Vec3& rhs , Real tolerance = EPSLION ) const;

			Real operator[]( const int i ) const;
			Real& operator[]( const int i );

			Real* Ptr(void);
			const Real* Ptr(void) const;

			Vec3& operator = ( const Vec3& rhs );
			Vec3& operator += ( const Vec3& rhs );
			Vec3& operator -= ( const Vec3& rhs );
			Vec3& operator *= ( const Real val );
			Vec3& operator /= ( const Real val ); 

			Vec3 operator + ( const Vec3& rhs ) const;
			Vec3 operator - ( const Vec3& rhs ) const;
			Vec3 operator * ( const Real val ) const;
			Vec3 operator * ( const Vec3& rhs ) const;
			Vec3 operator / ( const Real val ) const;
			Vec3 operator / ( const Vec3& rhs ) const;

			const Vec3& operator + () const;
			Vec3 operator - () const;

			bool operator == ( const Vec3& rhs ) const;
			bool operator != ( const Vec3& rhs ) const;

			//��ǰ������Ԫ�ض�С��rhs�����е�Ԫ��ʱ���� true
			bool operator < ( const Vec3& rhs ) const;
			//��ǰ������Ԫ�ض�����rhs�����е�Ԫ��ʱ���� true
			bool operator > ( const Vec3& rhs ) const;

			static const Vec3 ZERO;
			static const Vec3 UNIT_X;
			static const Vec3 UNIT_Y;
			static const Vec3 UNIT_Z;
			static const Vec3 NEGATIVE_UNIT_X;
			static const Vec3 NEGATIVE_UNIT_Y;
			static const Vec3 NEGATIVE_UNIT_Z;
			static const Vec3 UNIT_SCALE;  
		};

		Vec3 operator + ( const Vec3& lhs , const Real rhs );
		Vec3 operator + ( const Real lhs , const Vec3& rhs );
		Vec3 operator - ( const Vec3& lhs , const Real rhs );
		Vec3 operator - ( const Real lhs , const Vec3& rhs );
		Vec3 operator * ( const Real lhs , const Vec3& rhs );
		Vec3 operator / ( const Real lhs , const Vec3& rhs );


		class Vec4
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

			//��ֵ�����
			Vec4& operator=( const Vec4& rhs );
			Vec4& operator=( const Vec3& rhs );
			Vec4& operator=( const Real val );

			//�Ƚ������
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


		template<typename T>
		class TRect
		{
		public: 
			TRect(void):left(0),right(0),top(0),bottom(0){}
			TRect( const T l , const T r , const T t , const T b ):left(l),right(r),top(t),bottom(b){}
			TRect( const TRect<T>& rect ):left(rect.left),right(rect.right),top(rect.top),bottom(rect.bottom){}
			~TRect(){}

			TRect& operator=( const TRect& rhs )
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

			TRect& Merge( const TRect& rect )
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

			TRect Intersect( const TRect& rect ) const
			{
				TRect ret;
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

			bool IsIntersect( const TRect& rect ) const
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

		typedef TRect<Real> realRect_t;
		typedef TRect<int>	  rect_t; 
	}


#endif