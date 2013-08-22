
// ModelViewerView.h : CModelViewerView 类的接口
//

#pragma once

#include "ZP3DEngine.h"

class CModelViewerView : public CView
{
protected: // 仅从序列化创建
	CModelViewerView();
	DECLARE_DYNCREATE(CModelViewerView)

// 特性
public:
	CModelViewerDoc* GetDocument() const;

// 操作
public:

	void RenderOneFrame(void);

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
};

#ifndef _DEBUG  // ModelViewerView.cpp 中的调试版本
inline CModelViewerDoc* CModelViewerView::GetDocument() const
   { return reinterpret_cast<CModelViewerDoc*>(m_pDocument); }
#endif

