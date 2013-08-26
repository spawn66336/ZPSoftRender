
// ModelViewerView.cpp : CModelViewerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
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
END_MESSAGE_MAP()

// CModelViewerView 构造/析构

CModelViewerView::CModelViewerView():
	m_pEngine(NULL),
	m_pCamera( NULL ),
	m_bDragFlag( FALSE )
{
	 
}

CModelViewerView::~CModelViewerView()
{
	delete m_pEngine;
	m_pEngine = NULL;
}

BOOL CModelViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CModelViewerView 绘制

void CModelViewerView::OnDraw(CDC* /*pDC*/)
{
	CModelViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pEngine->RenderOneFrame();
}


// CModelViewerView 打印


void CModelViewerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CModelViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CModelViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CModelViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CModelViewerView 诊断

#ifdef _DEBUG
void CModelViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CModelViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CModelViewerDoc* CModelViewerView::GetDocument() const // 非调试版本是内联的
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

// CModelViewerView 消息处理程序


void CModelViewerView::OnDestroy()
{
	CView::OnDestroy();
	 
	m_pEngine->Destroy();
}


int CModelViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//初始化引擎
	if( NULL == m_pEngine )
	{
		m_pEngine = new ZP3DEngine;
		m_pEngine->Init( this->GetSafeHwnd() );
		m_pEngine->LoadResources();
		m_pCamera = m_pEngine->GetCamera();
		m_pEngine->RegisterFrameListener( this );
	}

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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	Real moveSpeed = 1.0f;

	//前进
	if( GetKeyState('W')&0x80 ||
		GetKeyState('w')&0x80)
	{
		m_pCamera->MoveAlongDir(moveSpeed);
	}

	//后退
	if( GetKeyState('S')&0x80 ||
		GetKeyState('s')&0x80)
	{
		m_pCamera->MoveAlongDir(-moveSpeed);
	}

	//左平移
	if( GetKeyState('A')&0x80 ||
		GetKeyState('a')&0x80)
	{
		m_pCamera->MoveAlongT(-moveSpeed);
	}

	//右平移
	if( GetKeyState('D')&0x80 ||
		GetKeyState('d')&0x80)
	{
		m_pCamera->MoveAlongT(moveSpeed);
	}

	if( GetKeyState('Q')&0x80 ||
		GetKeyState('q')&0x80
		)
	{
		m_pCamera->RotateWithDir( 0.01f );
	}

	if( GetKeyState('E')&0x80 ||
		GetKeyState('e')&0x80
		)
	{
		m_pCamera->RotateWithDir( -0.01f );
	}
}

void CModelViewerView::FrameEnded( void )
{

}


void CModelViewerView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{ 


	if( nChar == 'L' ||
		nChar == 'l'
		)
	{
		m_pEngine->SwitchShadeMode();
	}

	if( nChar == 'C' ||
		nChar == 'c'
		)
	{
		m_pEngine->SwitchMesh();
	}
	__super::OnChar(nChar, nRepCnt, nFlags);
}
