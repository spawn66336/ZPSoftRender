#ifndef ZP_IMESHLOADER
#define ZP_IMESHLOADER

#include "ZPDependency.h"

namespace Resource
{
	class Mesh;

	class IMeshLoader
	{
	public:
		IMeshLoader(){}
		virtual ~IMeshLoader(){}
		virtual Mesh* Load( const String& path ) = 0;	 
	};
}//namespace Resouce

#endif //ZP_IMESHLOADER