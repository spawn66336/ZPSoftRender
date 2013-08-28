#include "PixelShader.h"
#include "RenderList.h"

namespace Render
{
	 
	PixelShader::PixelShader(void):
	m_shadeModel( FLAT_MODEL)
	{
	}


	PixelShader::~PixelShader(void)
	{
	}

	Math::Vec4 PixelShader::Run( const RVertex& v  )
	{
		if( GetShadeModel() == FLAT_MODEL )
		{
			return m_v4FaceColor;
		}

		if( GetShadeModel() == GOURAUD_MODEL )
		{
			return v.m_v4Color;
		}

		if( GetShadeModel() == PHONG_MODEL )
		{
			return v.m_v4Color;
		}

		if( GetShadeModel() == NORMMAP_MODEL )
		{
			return v.m_v4Color;
		}
		return Math::Vec4();
	}


}//namespace Render