#include "FreeImageTextureLoader.h"
#include "Texture2D.h"
#include "FreeImage.h"

namespace Resource
{

	FreeImageTextureLoader::FreeImageTextureLoader(void)
	{
	}


	FreeImageTextureLoader::~FreeImageTextureLoader(void)
	{
	}

	Texture2D* FreeImageTextureLoader::Load( const String& path )
	{
		Texture2D* pNewTexture2D = NULL;
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

		fif = FreeImage_GetFileType( path.c_str() );

		if(fif == FIF_UNKNOWN)
			fif  =  FreeImage_GetFIFFromFilename( path.c_str() );

		if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
		{

			FIBITMAP *dib = FreeImage_Load(fif, path.c_str(), 0); 
			ZP_ASSERT( NULL != dib );

			pNewTexture2D = new Texture2D;
			ZP_ASSERT( NULL != pNewTexture2D );

			pNewTexture2D->SetWidth( FreeImage_GetWidth( dib ) );
			pNewTexture2D->SetHeight( FreeImage_GetHeight( dib ) );
			pNewTexture2D->SetBpp( 32 );
			
			unsigned char* pPixels = new unsigned char[ 4*pNewTexture2D->Width()*pNewTexture2D->Height() ];
			ZP_ASSERT( NULL != pPixels );
			
			for( int y = 0 ; y < pNewTexture2D->Height() ; y++)
			{
				for( int x = 0 ; x < pNewTexture2D->Width() ; x++ )
				{
					BOOL bResult = FALSE;
					RGBQUAD rgbQuad;
					bResult = FreeImage_GetPixelColor( dib , x , y , &rgbQuad );
					int pixelOffset = y*4*pNewTexture2D->Width()+x*4;
					 
					pPixels[pixelOffset] = rgbQuad.rgbRed;
					pPixels[pixelOffset+1] = rgbQuad.rgbGreen;
					pPixels[pixelOffset+2] = rgbQuad.rgbBlue;
					pPixels[pixelOffset+3] = rgbQuad.rgbReserved;
				}
			}

			pNewTexture2D->SetPixels( pPixels );  
			FreeImage_Unload( dib );
			dib = NULL;

		}//if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))

		return pNewTexture2D;
	}

	void FreeImageTextureLoader::InitFreeImageLib( void )
	{
		FreeImage_Initialise();
	}

	void FreeImageTextureLoader::DestroyFreeImageLib( void )
	{
		FreeImage_DeInitialise();
	}
	 

}//namespace Resource