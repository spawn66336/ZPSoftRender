#ifndef ZP_D3DINCLUDECALLBACK
#define ZP_D3DINCLUDECALLBACK
#include "d3dx9shader.h"

namespace Render{

	class D3DIncludeCallback : public ID3DXInclude
	{
		STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		STDMETHOD(Close)(THIS_ LPCVOID pData);
	};

}//namespace Render

#endif