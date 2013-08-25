
// ModelViewerView.h : CModelViewerView ��Ľӿ�
//

#pragma once

#include "ZP3DEngine.h"
#include "Camera.h"

class CModelViewerView : public CView , public IFrameListener
{
protected: // �������л�����
	CModelViewerView();
	DECLARE_DYNCREATE(CModelViewerView)

// ����
public:
	CModelViewerDoc* GetDocument() const;

// ����
public:

	void RenderOneFrame(void);

	//ʵ��FrameListener�ӿ�
	virtual void FrameStarted( void ); 
	virtual void FrameEnded( void );

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CModelViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	ZP3DEngine* m_pEngine;
	Camera*		  m_pCamera;
	BOOL			  m_bDragFlag;
	CPoint			  m_mousePoint;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnResetCamera();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // ModelViewerView.cpp �еĵ��԰汾
inline CModelViewerDoc* CModelViewerView::GetDocument() const
   { return reinterpret_cast<CModelViewerDoc*>(m_pDocument); }
#endif

