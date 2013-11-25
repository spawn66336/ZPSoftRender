
// ModelViewerView.cpp : CModelViewerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ModelViewer.h"
#endif

#include "ModelViewerDoc.h"
#include "ModelViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModelViewerView

IMPLEMENT_DYNCREATE(CModelViewerView, CView)

BEGIN_MESSAGE_MAP(CModelViewerView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CModelViewerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDM_RESET_CAMERA, &CModelViewerView::OnResetCamera)
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_FRAME_SHADING, &CModelViewerView::OnFrameShading)
	ON_COMMAND(IDM_FLAT_SHADING, &CModelViewerView::OnFlatShading)
	ON_COMMAND(IDM_GOURAUD_SHADING, &CModelViewerView::OnGouraudShading)
	ON_COMMAND(IDM_PHONG_SHADING, &CModelViewerView::OnPhongShading)
	ON_COMMAND(IDM_BUMP_SHADING, &CModelViewerView::OnBumpShading)
	ON_COMMAND(IDM_RESET_MODEL, &CModelViewerView::OnResetModel)
	ON_WM_NCMOUSELEAVE()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CModelViewerView ����/����

CModelViewerView::CModelViewerView():
	m_pEngine(NULL),
	m_pCamera( NULL ),
	m_bDragFlag( FALSE )
{
	 
}

CModelViewerView::~CModelViewerView()
{ 
}

BOOL CModelViewerView::PreCreateWindow(CREATESTRUCT& cs)
{ 
	return CView::PreCreateWindow(cs);
}

// CModelViewerView ����

void CModelViewerView::OnDraw(CDC* /*pDC*/)
{
	CModelViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return; 

	if( NULL != m_pEngine )
	{
		m_pEngine->RenderOneFrame();
	}
}


// CModelViewerView ��ӡ


void CModelViewerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CModelViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CModelViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{ 
}

void CModelViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{ 
}

void CModelViewerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CModelViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
 
// CModelViewerView ���

#ifdef _DEBUG
void CModelViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CModelViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CModelViewerDoc* CModelViewerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModelViewerDoc)));
	return (CModelViewerDoc*)m_pDocument;
}
 

#endif //_DEBUG

void CModelViewerView::RenderOneFrame( void )
{
	if( m_pEngine )
	{
		m_pEngine->RenderOneFrame();
	}
	
}

// CModelViewerView ��Ϣ�������


void CModelViewerView::OnDestroy()
{
	m_pEngine = NULL;
	m_pCamera = NULL;
	CView::OnDestroy(); 
}


int CModelViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;  
	return 0;
}


void CModelViewerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if( m_pEngine )
	{
		m_pEngine->Resize();
	}
}


void CModelViewerView::OnMouseMove(UINT nFlags, CPoint point)
{  
	if( m_pCamera )
	{
		if( TRUE == m_bDragFlag )
		{
			int dx = point.x - m_mousePoint.x;
			int dy = point.y - m_mousePoint.y;

			m_pCamera->RotateWithUp( ((float)dx) / 1000.0f );
			m_pCamera->RotateWithT( ((float)dy) / 1000.0f );  
		}
	} 
	m_mousePoint = point; 
	CView::OnMouseMove(nFlags, point);
}


void CModelViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bDragFlag = FALSE;
	m_mousePoint = point;
	CView::OnLButtonUp(nFlags, point);
}


void CModelViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{ 
	m_bDragFlag = TRUE;
	m_mousePoint = point;
	CView::OnLButtonDown(nFlags, point);
}


void CModelViewerView::OnResetCamera()
{
	if( m_pCamera )
	{
		m_pCamera->Reset();
	}
}

void CModelViewerView::FrameStarted( void )
{
	if( NULL == m_pCamera )
	{
		return;
	}

	if( this != GetFocus() )
	{
		return;
	}

	Real moveSpeed = 1.0f;
	bool bMovingFlag = false;

	//ǰ��
	if( GetKeyState('W')&0x80 ||
		GetKeyState('w')&0x80)
	{
		m_pCamera->MoveAlongDir(moveSpeed);
		bMovingFlag = true;
	}

	//����
	if( GetKeyState('S')&0x80 ||
		GetKeyState('s')&0x80)
	{
		m_pCamera->MoveAlongDir(-moveSpeed);
		bMovingFlag = true;
	}

	//��ƽ��
	if( GetKeyState('A')&0x80 ||
		GetKeyState('a')&0x80)
	{
		m_pCamera->MoveAlongT(-moveSpeed);
		bMovingFlag = true;
	}

	//��ƽ��
	if( GetKeyState('D')&0x80 ||
		GetKeyState('d')&0x80)
	{
		m_pCamera->MoveAlongT(moveSpeed);
		bMovingFlag = true;
	}

	if( GetKeyState('Q')&0x80 ||
		GetKeyState('q')&0x80
		)
	{
		m_pCamera->RotateWithDir( 0.01f );
		bMovingFlag = true;
	}

	if( GetKeyState('E')&0x80 ||
		GetKeyState('e')&0x80
		)
	{
		m_pCamera->RotateWithDir( -0.01f );
		bMovingFlag = true;
	}

	 

	if( GetKeyState( VK_UP ) & 0x80 )
	{
		m_pEngine->RotateMeshWithXAxis( 0.01f );
		bMovingFlag = true;
	}

	if( GetKeyState( VK_DOWN ) & 0x80 )
	{
		m_pEngine->RotateMeshWithXAxis( -0.01f );
		bMovingFlag = true;
	}

	if( GetKeyState( VK_LEFT ) & 0x80 )
	{
		m_pEngine->RotateMeshWithYAxis( 0.01f );
		bMovingFlag = true;
	}

	if( GetKeyState( VK_RIGHT ) & 0x80 )
	{
		m_pEngine->RotateMeshWithYAxis( -0.01f );
		bMovingFlag = true;
	}

	if( TRUE == m_bDragFlag )
	{
		bMovingFlag = true;
	}

	//���õ��϶�ʱ��Ϊ�߿���Ⱦ
	bMovingFlag = false;
	if( m_pEngine )
	m_pEngine->SetMovingFlag( bMovingFlag );
}

void CModelViewerView::FrameEnded( void )
{ 
}
 
void CModelViewerView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{ 

	if( m_pEngine )
	{ 

		if( nChar == 'L' ||
			nChar == 'l'
			)
		{
			m_pEngine->SwitchShadeModel();
		}

		if( nChar == 'C' ||
			nChar == 'c'
			)
		{
			m_pEngine->SwitchMesh();
		}

	}

	if( nChar == 'R'  ||
		nChar == 'r'
		)
	{
		if( m_pCamera )
		{
			m_pCamera->Reset();
		}
	}

#ifdef ZP_CLIPMAP_TERRAIN_DEMO
	if( nChar == 'T' ||
		nChar == 't' )
	{
		m_pEngine->SwitchShowMask();
	}
#endif
	__super::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CModelViewerView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return __super::OnEraseBkgnd(pDC);
}


void CModelViewerView::OnFrameShading()
{ 
	m_pEngine->SetShadeModel( Render::WIREFRAME_MODEL );
}


void CModelViewerView::OnFlatShading()
{ 
	if( m_pEngine )
	m_pEngine->SetShadeModel( Render::FLAT_MODEL );
}


void CModelViewerView::OnGouraudShading()
{ 
	if( m_pEngine )
	m_pEngine->SetShadeModel( Render::GOURAUD_MODEL );
}


void CModelViewerView::OnPhongShading()
{ 
	if( m_pEngine )
	m_pEngine->SetShadeModel( Render::PHONG_MODEL );
}


void CModelViewerView::OnBumpShading()
{ 
	if( m_pEngine )
	m_pEngine->SetShadeModel( Render::NORMMAP_MODEL );
}


void CModelViewerView::OnResetModel()
{
	if( m_pEngine )
		m_pEngine->ResetMesh();
}


void CModelViewerView::OnNcMouseLeave()
{
	// �ù���Ҫ��ʹ�� Windows 2000 ����߰汾��
	// ���� _WIN32_WINNT �� WINVER ���� >= 0x0500��
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bDragFlag = FALSE;
	__super::OnNcMouseLeave();
}


void CModelViewerView::OnNcLButtonUp(UINT nHitTest, CPoint point)
{ 
	m_bDragFlag = FALSE;
	__super::OnNcLButtonUp(nHitTest, point);
}


void CModelViewerView::OnNcMouseMove(UINT nHitTest, CPoint point)
{ 
	m_bDragFlag = FALSE;
	__super::OnNcMouseMove(nHitTest, point);
}


LRESULT CModelViewerView::OnNcHitTest(CPoint point)
{ 
	return __super::OnNcHitTest(point);
}


void CModelViewerView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

}

void CModelViewerView::SetEngine( ZP3DEngine* pEngine , Camera* pCamera )
{
	m_pEngine = pEngine;
	m_pCamera = pCamera;
}
