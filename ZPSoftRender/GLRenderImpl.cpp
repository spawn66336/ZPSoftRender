#include "GLRenderImpl.h"
#include <gl\GL.h>
#include <gl\GLU.h>
#include "Light.h"
#include "Material.h"
#include "Texture2D.h"
#include "Camera.h"


namespace Render
{

GLRenderImpl::GLRenderImpl(void):
m_hWnd(NULL),
m_hDC(NULL),
m_hRC(NULL),
m_clearColor(), 
m_enableTexture2D(true),  
m_enableDepthTest(true),
m_enableLighting(true),
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

	//设置像素格式
	_SetupPixelFormat(); 
	m_hRC = wglCreateContext( m_hDC ); 
	wglMakeCurrent( m_hDC , m_hRC );
	::GetClientRect( m_hWnd , &m_wndRect );
}

void GLRenderImpl::Destroy()
{
	ZP_ASSERT( NULL != m_hWnd && NULL != m_hRC && NULL != m_hDC );

	//清除所有已提交的纹理
	commitTextureList_t::iterator itCommitTexHandle = m_commitTexturesList.begin();
	while( itCommitTexHandle != m_commitTexturesList.end() )
	{
		glDeleteTextures( 1 , &(*itCommitTexHandle) );
		itCommitTexHandle++;
	}
	m_commitTexturesList.clear();

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
 

void GLRenderImpl::ApplyMaterial( Resource::Material* pMaterial )
{
	ZP_ASSERT( NULL != pMaterial );
	
	glMaterialfv( GL_FRONT , GL_AMBIENT , &( pMaterial->GetAmbient().x ) );
	glMaterialfv( GL_FRONT , GL_DIFFUSE , &( pMaterial->GetDiffuse().x ) );
	glMaterialfv( GL_FRONT , GL_SPECULAR , &( pMaterial->GetSpecular().x ) );
	glMaterialfv( GL_FRONT , GL_SHININESS , &( pMaterial->GetShininess() ) );
	glColor4fv( &( pMaterial->GetDiffuse().x ) );

	if( m_enableTexture2D )
	{ 
		Resource::Texture2D* pDiffuseTexture = pMaterial->GetTexture( DIFFUSE_CH );
		if( pDiffuseTexture && NULL != pDiffuseTexture->Pixels() )
		{
			if( pDiffuseTexture->IsCommit() )
			{
				glBindTexture( GL_TEXTURE_2D , pDiffuseTexture->GetTextureHandle() );
			}else{
				GLuint newTextureHandle = 0;
				glGenTextures( 1 , &newTextureHandle );
				m_commitTexturesList.push_back( newTextureHandle );
				glBindTexture( GL_TEXTURE_2D , newTextureHandle );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST );
				glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , 
					pDiffuseTexture->Width() , pDiffuseTexture->Height() , 0 , GL_RGBA , GL_UNSIGNED_BYTE , 
					pDiffuseTexture->Pixels() );

				pDiffuseTexture->SetTextureHandle( newTextureHandle );
			}
		}else{ //若没有漫反射纹理则将纹理绑定为空
			glBindTexture( GL_TEXTURE_2D , 0 );
		}
	} 
}


void GLRenderImpl::DrawElements( RenderPrimitive& renderPrimitive )
{

	GLenum primitiveTypeGL = GL_LINES;

	switch( renderPrimitive.PrimitiveType() )
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

	VertexBuffer& vexBufRef = renderPrimitive.VertexBuf();

	if( 0 == vexBufRef.Count() )
	{
		return;
	}

	bool hasPosition = false;
	bool hasNormal = false;
	bool hasTexcoord = false;

	if( vexBufRef.ChannelFlag() & POSITION_CH )
	{
		hasPosition = true;
	} 
	if( vexBufRef.ChannelFlag() & NORMAL_CH )
	{
		hasNormal = true;
	} 
	if( vexBufRef.ChannelFlag() & TEXCOORD_CH )
	{
		hasTexcoord = true;
	}

	if( hasPosition )
	{
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3 , GL_FLOAT , vexBufRef.Stride() , vexBufRef.PositionPointer() );
	}else{
		return;
	} 
	if( hasNormal )
	{
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer( GL_FLOAT , vexBufRef.Stride() , vexBufRef.NormalPointer() );
	} 
	if( hasTexcoord )
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer( 2 , GL_FLOAT , vexBufRef.Stride() , vexBufRef.TexcoordPointer() );
	}

	glDrawElements( primitiveTypeGL , renderPrimitive.IndicesCount() , GL_UNSIGNED_INT , renderPrimitive.Indices() );

	glDisableClientState( GL_VERTEX_ARRAY ); 
	if( hasNormal )
	{
		glDisableClientState( GL_NORMAL_ARRAY );
	} 
	if( hasTexcoord )
	{
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
}

void GLRenderImpl::EnableTexture2D( bool enable )
{
	m_enableTexture2D = enable;
}

void GLRenderImpl::EnableLight( bool enable )
{
	m_enableLighting = enable; 
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

	//未找到此名称的光源
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

	//未找到此名称的光源
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
	glMultMatrixf( (GLfloat*)(mat.m) );
}

void GLRenderImpl::_SetupPixelFormat( void )
{
	ZP_ASSERT( NULL != m_hDC );

	BOOL bResult = FALSE;

	PIXELFORMATDESCRIPTOR pfd = //定义像素格式 
	{    
		sizeof(PIXELFORMATDESCRIPTOR), // 上述格式描述符的大小    
			1,  
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER , PFD_TYPE_RGBA,     
			24,      
			0,         // 无Alpha缓存    
			0,         // 忽略Shift Bit    
			0,         // 无累加缓存     
			0, 0, 0, 0,       // 忽略聚集位     
			32,         // 32位 Z-缓存 (深度缓存) 
			0,         // 无蒙板缓存    
			0,         // 无辅助缓存     
			PFD_MAIN_PLANE,      // 主绘图层    
			0,         // Reserved     
			0, 0, 0        // 忽略层遮罩 
	}; 
	int nIndex = ChoosePixelFormat( m_hDC, &pfd); //选择刚刚定义的像素格式 
	ZP_ASSERT( 0 != nIndex ); 
	 
	bResult = SetPixelFormat( m_hDC , nIndex, &pfd);   //设置像素格式

	ZP_ASSERT( TRUE == bResult );
}

void GLRenderImpl::BeginDraw(  Camera* pCam  )
{ 
	GLsizei wndWidth =  ( m_wndRect.right - m_wndRect.left );
	GLsizei wndHeight =  ( m_wndRect.bottom - m_wndRect.top );
	Real aspect = (Real)wndWidth / (Real)wndHeight;

	glViewport( 0 , 0 , wndWidth , wndHeight );
	glClearColor( m_clearColor.x , m_clearColor.y , m_clearColor.z , m_clearColor.w );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90.0f ,  aspect , 1.0 , 1000.0f );
	//glLoadMatrixf( (GLfloat*)m_projectionMat.m );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();  
	this->LoadMatrix( pCam->GetGLCameraMatrix() );

	_DrawFrameOfAxes();

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
	  
	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT ,  GL_DIFFUSE );
	
	_ApplyAllActiveLights();

}

void GLRenderImpl::EndDraw( void )
{  

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	//关闭所有硬件光源
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

void GLRenderImpl::_DrawFrameOfAxes( void )
{
	glDisable( GL_LIGHTING );
	glEnable( GL_DEPTH_TEST );

	glLineWidth( 2.0f );

	glColor3f( 1.0f , 0.0f , 0.0f );
	glBegin( GL_LINES );
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(100.0f,0.0f,0.0f);
	glEnd();

	glColor3f( 0.0f , 1.0f , 0.0f );
	glBegin( GL_LINES );
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,100.0f,0.0f);
	glEnd();

	glColor3f( 0.0f , 0.0f , 1.0f );
	glBegin( GL_LINES );
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,100.0f);
	glEnd();

}

void GLRenderImpl::_ApplyAllActiveLights( void )
{
	//准备光源
	if( m_enableLighting )
	{
		int iLight = 0;
		lightTable_t::iterator itLight = m_lights.begin();
		while( itLight != m_lights.end() )
		{
			//OpenGL最大支持到8个光源
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
			itLight++;
		}//while( itLight != m_lights.end() )

		glEnable( GL_LIGHTING );
	}else{
		glDisable( GL_LIGHTING );
	}
}

}//namespace Render