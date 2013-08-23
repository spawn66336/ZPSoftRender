#include "Texture2D.h"

namespace Resource
{



Texture2D::Texture2D(void):
m_width(0),
m_height(0),
m_bpp(0),
m_pixels(NULL),
m_textureHandle(0)
{
}


Texture2D::~Texture2D(void)
{
	 ZP_SAFE_DELETE( m_pixels );
}

void Texture2D::SetPixels( unsigned char *const pixels )
{
	ZP_SAFE_DELETE( m_pixels );
	m_pixels = pixels;
}

bool Texture2D::IsCommit( void ) const
{
	return 0 != m_textureHandle;
}

}//namespace Resource
