#ifndef ZP_GLRENDERIMPL
#define ZP_GLRENDERIMPL


#include "IRender.h"

namespace Render
{

class GLRenderImpl : public IRender
{
public:
	GLRenderImpl(void);
	virtual ~GLRenderImpl(void);
};

}//namespace Render

#endif// ZP_GLRENDERIMPL