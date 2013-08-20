#ifndef ZP_DEPTHFRAMEBUFFER
#define ZP_DEPTHFRAMEBUFFER


#include "FrameBuffer.h"

namespace Render
{  
	class DepthFrameBuffer : public FrameBuffer
	{
	public:
		DepthFrameBuffer(void);
		virtual ~DepthFrameBuffer(void);
	};

}//namespace Render

#endif //ZP_DEPTHFRAMEBUFFER