
// FivechServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFivechServerApp:
// �йش����ʵ�֣������ FivechServer.cpp
//

class CFivechServerApp : public CWinApp
{
public:
	CFivechServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFivechServerApp theApp;