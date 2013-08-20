#ifndef ZP_COLLADAMESHLOADER
#define ZP_COLLADAMESHLOADER

#include "IMeshLoader.h"

namespace Resource
{ 

	class ColladaMeshLoader : public IMeshLoader
	{
	public:
		ColladaMeshLoader(void);
		virtual ~ColladaMeshLoader(void);

		virtual Mesh* Load( const String& path );
	};
 
}//namespace Resource

#endif//ZP_COLLADAMESHLOADER