#ifndef ZP_PLATFORM
#define ZP_PLATFORM

#	if defined( _WINDOWS )
		#include <Windows.h>
		#include <Shlwapi.h >
		typedef HWND winHandle_t;
#	else
		typedef unsigned int winHandle_t; 
#endif



#endif //ZP_PLATFORM