
// IOCP_Test.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CIOCP_TestApp:
// �� Ŭ������ ������ ���ؼ��� IOCP_Test.cpp�� �����Ͻʽÿ�.
//

class CIOCP_TestApp : public CWinAppEx
{
public:
	CIOCP_TestApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CIOCP_TestApp theApp;