#ifndef ZP_ITEXTURELOADER
#define ZP_ITEXTURELOADER

#include "ZPDependency.h"

namespace Resource
{
	class Texture2D;

	class ITextureLoader
	{
	public:
		ITextureLoader(){}
		virtual ~ITextureLoader(){}

		virtual Texture2D* Load( const String& path ) = 0;
	};

}//namespace Resource

#endif //ZP_ITEXTURELOADER