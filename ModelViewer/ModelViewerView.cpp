
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
END_MESSAGE_MAP()

// CModelViewerView ����/����

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CModelViewerView ����

void CModelViewerView::OnDraw(CDC* /*pDC*/)
{
	CModelViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pEngine->RenderOneFrame();
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
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CModelViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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
	CView::OnDestroy();
	 
	m_pEngine->Destroy();
}


int CModelViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	//��ʼ������
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
