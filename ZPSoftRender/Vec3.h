#ifndef ZP_VEC3
#define ZP_VEC3

#include "ZPDependency.h"

namespace Math
{ 
	//��ά����
	class ZPEXPORT Vec3 
	{
	public: 
		Real x , y , z; 

		Vec3( void );  
		Vec3( const Real in_x , const Real in_y , const Real in_z );
		explicit Vec3( Real* const r );
		explicit Vec3( const Real val );
		~Vec3();

		void Set( const Real in_x , const Real in_y , const Real in_z );
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

}//namespace Math

#endif //ZP_VEC3
