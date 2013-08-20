#ifndef ZP_FREEIMAGETEXTURELOADER
#define ZP_FREEIMAGETEXTURELOADER

#include "ITextureLoader.h"

namespace Resource
{

class FreeImageTextureLoader : public ITextureLoader
{
	public:
		FreeImageTextureLoader(void);
		virtual ~FreeImageTextureLoader(void);

		virtual Texture2D* Load( const String& path );
};

}//namespace Resource

#endif //ZP_FREEIMAGETEXTURELOADER