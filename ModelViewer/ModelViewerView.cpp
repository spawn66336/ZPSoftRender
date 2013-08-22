
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
END_MESSAGE_MAP()

// CModelViewerView 构造/析构

CModelViewerView::CModelViewerView():
	m_pEngine(NULL)
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
