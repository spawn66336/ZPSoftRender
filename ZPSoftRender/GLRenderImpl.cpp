#include "GLRenderImpl.h"
#include <gl\GL.h>
#include <gl\GLU.h>
#include "Light.h"


namespace Render
{

GLRenderImpl::GLRenderImpl(void):
m_hWnd(NULL),
m_hDC(NULL),
m_hRC(NULL),
m_clearColor(), 
m_enableTexture2D(false),  
m_enableDepthTest(false),
m_enableLighting(false),
m_shadeModel(PHONG_MODEL)
{

}


GLRenderImpl::~GLRenderImpl(void)
{
	DeleteAllLights();
}

void GLRenderImpl::Init( const winHandle_t hwnd )
{
	ZP_ASSERT( NULL != hwnd );
	m_hWnd = hwnd;
	m_hDC = ::GetDC( m_hWnd );
	ZP_ASSERT( NULL != m_hDC );

	//�������ظ�ʽ
	_SetupPixelFormat(); 
	m_hRC = wglCreateContext( m_hDC ); 
	wglMakeCurrent( m_hDC , m_hRC );
	::GetClientRect( m_hWnd , &m_wndRect );
}

void GLRenderImpl::Destroy()
{
	ZP_ASSERT( NULL != m_hWnd && NULL != m_hRC && NULL != m_hDC );

	wglMakeCurrent( m_hDC , NULL );
	wglDeleteContext( m_hRC );
	::ReleaseDC( m_hWnd , m_hDC );
	m_hRC = NULL;
	m_hDC = NULL;
	m_hWnd = NULL;
}

void GLRenderImpl::Resize()
{
	ZP_ASSERT( NULL != m_hWnd );
	::GetClientRect( m_hWnd , &m_wndRect );
}

void GLRenderImpl::SetClearColor( const Math::Vec4& color )
{
	m_clearColor = color; 
}

void GLRenderImpl::ClearBuffer( unsigned int flag ){ }

void GLRenderImpl::VertexPointer( const Math::Vec3* vertices )
{
	glVertexPointer( 3 , GL_FLOAT , 0 , vertices );
}

void GLRenderImpl::NormalPointer( const Math::Vec3* normals )
{
	glNormalPointer( GL_FLOAT , 0 , normals );
}

void GLRenderImpl::TexcoordPointer( const Math::Vec2* texcoords )
{
	glTexCoordPointer( 2 , GL_FLOAT , 0 , texcoords );
}

void GLRenderImpl::DrawElements( int* indices, int count, PRIMITIVE_TYPE primitiveType )
{
	GLenum primitiveTypeGL = GL_LINES;

	switch( primitiveType )
	{
	case TYPE_LINES:
		primitiveTypeGL = GL_LINES;
		break;
	case TYPE_TRIANGLES:
		primitiveTypeGL = GL_TRIANGLES;
		break;
	default:
		return;
	}

	glDrawElements( primitiveTypeGL , count , GL_INT , indices );
}

void GLRenderImpl::EnableTexture2D( bool enable )
{
	m_enableTexture2D = enable;
}

void GLRenderImpl::EnableLight( bool enable )
{
	m_enableLighting = enable;

}

void GLRenderImpl::SetTexture2D( int channel, Resource::Texture2D* tex )
{

}

void GLRenderImpl::EnableDepthTest( bool enable )
{
	m_enableDepthTest = enable;
}

void GLRenderImpl::SetShadingModel( SHADE_MODEL type )
{
	m_shadeModel = type;


}

Light* GLRenderImpl::CreateLight( const String& name )
{
	Light* pLight = FindLightByName( name );

	if( pLight )
	{
		return pLight;
	}

	pLight = new Light;
	pLight->SetName( name ); 
	m_lights.insert( std::make_pair( name , pLight ) );
	return pLight;
}

bool GLRenderImpl::DeleteLightByName( const String& name )
{
	lightTable_t::iterator itFindLight = m_lights.find( name );

	//δ�ҵ������ƵĹ�Դ
	if( itFindLight == m_lights.end() )
	{
		return false;
	}

	ZP_SAFE_DELETE( itFindLight->second );
	m_lights.erase( itFindLight );
	return true;
}

Light* GLRenderImpl::FindLightByName( const String& name )
{
	lightTable_t::iterator itFindLight = m_lights.find( name );

	//δ�ҵ������ƵĹ�Դ
	if( itFindLight == m_lights.end() )
	{
		return NULL;
	}
	return itFindLight->second;
}

void GLRenderImpl::DeleteAllLights( void )
{
	lightTable_t::iterator it = m_lights.begin();
	while( it != m_lights.end() )
	{
		ZP_SAFE_DELETE( it->second );
		it++;
	}
	m_lights.clear();
}
 

void GLRenderImpl::PushMatrix()
{
	glPushMatrix();
}

void GLRenderImpl::PopMatrix()
{
	glPopMatrix();
}

void GLRenderImpl::LoadMatrix( const Math::Matrix4 &mat )
{
	glLoadMatrixf( (GLfloat*)mat.m );
}

void GLRenderImpl::LoadIdentity()
{
	glLoadIdentity();
}

void GLRenderImpl::MultMatrix( const Math::Matrix4 &mat )
{
	glMultMatrixf( (GLfloat*)mat.m );
}

void GLRenderImpl::_SetupPixelFormat( void )
{
	ZP_ASSERT( NULL != m_hDC );

	BOOL bResult = FALSE;

	PIXELFORMATDESCRIPTOR pfd = //�������ظ�ʽ 
	{    
		sizeof(PIXELFORMATDESCRIPTOR), // ������ʽ�������Ĵ�С    
			1,  
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER , PFD_TYPE_RGBA,     
			24,      
			0,         // ��Alpha����    
			0,         // ����Shift Bit    
			0,         // ���ۼӻ���     
			0, 0, 0, 0,       // ���Ծۼ�λ     
			32,         // 32λ Z-���� (��Ȼ���) 
			0,         // ���ɰ建��    
			0,         // �޸�������     
			PFD_MAIN_PLANE,      // ����ͼ��    
			0,         // Reserved     
			0, 0, 0        // ���Բ����� 
	}; 
	int nIndex = ChoosePixelFormat( m_hDC, &pfd); //ѡ��ոն�������ظ�ʽ 
	ZP_ASSERT( 0 != nIndex ); 
	 
	bResult = SetPixelFormat( m_hDC , nIndex, &pfd);   //�������ظ�ʽ

	ZP_ASSERT( TRUE == bResult );
}

void GLRenderImpl::BeginDraw( void )
{ 
	GLsizei wndWidth =  ( m_wndRect.right - m_wndRect.left );
	GLsizei wndHeight =  ( m_wndRect.bottom - m_wndRect.top );
	Real aspect = (Real)wndWidth / (Real)wndHeight;

	glViewport( 0 , 0 , wndWidth , wndHeight );
	glClearColor( m_clearColor.x , m_clearColor.y , m_clearColor.z , m_clearColor.w );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	
	glMatrixMode( GL_PROJECTION );
	gluPerspective( 90.0f ,  aspect , 1.0 , 1000.0f );
	//glLoadMatrixf( (GLfloat*)m_projectionMat.m );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	_ApplyShadeModel();

	if( m_enableTexture2D )
	{
		glEnable( GL_TEXTURE_2D );
	}else{
		glDisable( GL_TEXTURE_2D );
	} 

	if( m_enableDepthTest )
	{
		glEnable( GL_DEPTH_TEST );
	}else{
		glDisable( GL_DEPTH_TEST );
	}

	if( m_enableLighting )
	{
		glEnable( GL_LIGHTING );
	}else{
		glDisable( GL_LIGHTING );
	} 

	//׼����Դ
	if( m_enableLighting )
	{
		int iLight = 0;
		lightTable_t::iterator itLight = m_lights.begin();
		while( itLight != m_lights.end() )
		{
			//OpenGL���֧�ֵ�8����Դ
			if( iLight >= 8 )
			{
				break;
			}

			Light* pLight = itLight->second;
			
			if( pLight->IsActive() )
			{
				GLenum lightIndxGL = GL_LIGHT0 + iLight;
				glEnable( lightIndxGL );
				glLightfv( lightIndxGL , GL_AMBIENT , &( pLight->Ambient().x ) );
				glLightfv( lightIndxGL , GL_DIFFUSE  ,  &( pLight->Diffuse().x ) );
				glLightfv( lightIndxGL , GL_SPECULAR , &( pLight->Specular().x ) );
				glLightfv( lightIndxGL , GL_POSITION , &( pLight->Position().x ) );

				iLight++;
			}
		}
	}//if( m_enableLighting )

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	 
}

void GLRenderImpl::EndDraw( void )
{
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY ); 

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	//�ر�����Ӳ����Դ
	for( int iLight = 0 ; iLight < 8 ; iLight++ )
	{
		glDisable( GL_LIGHT0 + iLight );
	}
}

void GLRenderImpl::SwapBuffers( void )
{
	ZP_ASSERT( NULL != m_hDC );
	::SwapBuffers( m_hDC );
}

void GLRenderImpl::SetProjectionMatrix( const Math::Matrix4& mat )
{
	m_projectionMat = mat;
}

void GLRenderImpl::_ApplyShadeModel( void )
{
	GLenum polyMode = GL_FILL;
	GLenum shadeModelGL = GL_FLAT;	

	switch( m_shadeModel )
	{
	case WIREFRAME_MODEL:
		polyMode = GL_LINE;
		shadeModelGL = GL_SMOOTH;
		break;
	case FLAT_MODEL:
		shadeModelGL = GL_FLAT;
		break;
	case GOURAUD_MODEL:
	case PHONG_MODEL:
	case NORMMAP_MODEL:
		shadeModelGL = GL_SMOOTH;
		break;
	default:
		return;
	} 

	glPolygonMode( GL_FRONT_AND_BACK , polyMode );
	glShadeModel( shadeModelGL );
}

}//namespace Render