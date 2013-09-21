#ifndef ZP_PREDEFINE
#define  ZP_PREDEFINE

#include "ZPStdDependency.h"

#	if defined( _DEBUG )
#		define ZP_DEBUG
#	endif

#	if defined( _USRDLL )
#			define ZP_NONCLIENT
#	endif

#  if defined( UNICODE )
#			define ZP_UNICODE
#  endif

#	if	defined( ZP_NONCLIENT )
#			define ZPEXPORT	__declspec( dllexport )
#	else	
#			define ZPEXPORT	__declspec( dllimport )
#	endif

#	if defined( ZP_DEBUG )
#			define ZP_ASSERT(x) assert(x)
#	else
#			define ZP_ASSERT(x) 
#	endif

#define ZP_SAFE_NEW( x , t ) do{ if( NULL != (x) ){ delete (x); (x) = NULL; } (x) = new t; ZP_ASSERT(x); }while(0)
#define ZP_SAFE_DELETE( x ) do{ if( NULL != (x) ){ delete (x); (x) = NULL; } }while(0)
#define ZP_SAFE_NEW_BUFFER( x , t , size ) do{ if( NULL != (x) ){ delete[] (x); (x) = NULL; } if( 0 != size ){ (x) = new t[size]; ZP_ASSERT(x); } }while(0)
#define ZP_SAFE_DELETE_BUFFER( x ) do{ if( NULL != (x) ){ delete[] (x); (x) = NULL; } }while(0)

#define ZP_SAFE_RELEASE( x ) do{ if( NULL != (x) ){ (x)->Release(); (x) = NULL; } }while(0)

typedef std::string String;

typedef float Real; 

#define EPSLION	0.0001f
#define REAL_EQUAL( x , y )  ( ( ( (x) - (y) ) < EPSLION ) &&  ( ( (x) - (y) ) > -EPSLION ) )
#define REAL_LESS_THAN( x , y ) ( ( (y)-(x) ) > EPSLION )
#define REAL_GREATER_THAN( x , y ) ( ( (x)-(y) ) > EPSLION )

#endif//ZP_PREDEFINE