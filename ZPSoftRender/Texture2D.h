#ifndef ZP_TEXTURE2D
#define ZP_TEXTURE2D

#include "ZPDependency.h"

namespace Resource
{

	class  Texture2D
	{
	public:
		Texture2D( void );
		virtual ~Texture2D( void );

		String Name(void) const { return m_name; }
		void SetName( const String& name ){ m_name = name; }

		int Width( void ) const { return m_width; }
		void SetWidth( const int width ){ m_width = width; }

		int Height( void ) const { return m_height; }
		void SetHeight( const int height ){ m_height = height; }

		int Bpp(void) const { return m_bpp; }
		void SetBpp( const int bpp ) { m_bpp = bpp; }

		unsigned char* Pixels(void) const { return m_pixels; }
		void SetPixels( unsigned char *const pixels );

		bool IsCommit(void) const;

		unsigned int GetTextureHandle(void) const { return m_textureHandle; }
		void SetTextureHandle( const int handle ){ m_textureHandle = handle; }

		void* GetUserPointer( void ) const { return m_pUserPointer; }
		void SetUserPointer( void* p) { m_pUserPointer = p; }
	protected:

		String m_name;			///>纹理名称
		int	  m_width;				///>纹理宽度（按像素计）
		int	  m_height;			///>纹理高度（按像素计）
		int      m_bpp;				///>bits per pixel
		unsigned char*  m_pixels;	///>像素缓冲区

		unsigned int m_textureHandle; ///>用于OpenGL
		void* m_pUserPointer;
	};

}//namespace Resource

#endif//ZP_TEXTURE2D