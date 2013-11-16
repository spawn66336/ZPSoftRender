#include "ZPDependency.h"
#include "D3DIncludeCallback.h"

namespace Render{
	HRESULT STDMETHODCALLTYPE  D3DIncludeCallback::Open(THIS_ D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
	{
		String strPath = ".\\shader\\";
		strPath.append( pFileName );
		FILE* pFile = NULL;
		if( 0 != fopen_s( &pFile , strPath.c_str() , "rb" ) )
		{
			return S_FALSE;
		}
		ZP_ASSERT( pFile != NULL );

		fseek( pFile , 0 , SEEK_END );
		int iFileLen = ftell( pFile );


		void* pBuf = new char[iFileLen+1];
		ZP_ASSERT( NULL != pBuf );
		memset( pBuf , 0 , iFileLen+1);

		fseek( pFile , 0 , SEEK_SET );
		*pBytes = fread_s( pBuf , iFileLen+1 , 1 , iFileLen , pFile );
		*ppData = pBuf;

		fclose( pFile );
		pFile = NULL;
		return S_OK;
	}
	HRESULT STDMETHODCALLTYPE  D3DIncludeCallback::Close(THIS_ LPCVOID pData)
	{
		ZP_ASSERT( pData );

		char* pBuf = (char*)pData;
		delete[] pBuf;
		pBuf = NULL;

		return S_OK;
	}

}//namespace Render