
// ModelViewerView.h : CModelViewerView 类的接口
//

#pragma once

#include "ZP3DEngine.h"
#include "Camera.h"

class CModelViewerView : public CView , public IFrameListener
{
protected: // 仅从序列化创建
	CModelViewerView();
	DECLARE_DYNCREATE(CModelViewerView)

// 特性
public:
	CModelViewerDoc* GetDocument() const;

// 操作
public:

	void SetEngine( ZP3DEngine* pEngine ,  Camera* pCamera );
	void RenderOneFrame(void);

	//实现FrameListener接口
	virtual void FrameStarted( void ); 
	virtual void FrameEnded( void );

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
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
// 生成的消息映射函数
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFrameShading();
	afx_msg void OnFlatShading();
	afx_msg void OnGouraudShading();
	afx_msg void OnPhongShading();
	afx_msg void OnBumpShading();
	afx_msg void OnResetModel();
	afx_msg void OnNcMouseLeave();
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

#ifndef _DEBUG  // ModelViewerView.cpp 中的调试版本
inline CModelViewerDoc* CModelViewerView::GetDocument() const
   { return reinterpret_cast<CModelViewerDoc*>(m_pDocument); }
#endif

