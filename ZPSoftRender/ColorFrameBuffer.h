#ifndef ZP_COLORFRAMEBUFFER
#define ZP_COLORFRAMEBUFFER

#include "FrameBuffer.h"

namespace Render
{ 
	class ColorFrameBuffer : public FrameBuffer
	{
	public:
		ColorFrameBuffer(void);
		virtual ~ColorFrameBuffer(void);

	};

}//namespace Render

#endif