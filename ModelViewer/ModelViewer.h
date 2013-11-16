
// ModelViewer.h : ModelViewer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "ZP3DEngine.h"
#include "Camera.h"

// CModelViewerApp:
// �йش����ʵ�֣������ ModelViewer.cpp
//

class CModelViewerApp : public CWinAppEx
{
public:
	CModelViewerApp(); 

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);

protected:

	ZP3DEngine* m_pEngine;
	Camera*		  m_pCamera;
};

extern CModelViewerApp theApp;
