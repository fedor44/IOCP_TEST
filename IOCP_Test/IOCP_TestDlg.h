
// IOCP_TestDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CIOCP_TestDlg ��ȭ ����
class CIOCP_TestDlg : public CDialog
{
// �����Դϴ�.
public:
	CIOCP_TestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IOCP_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CButton m_Start;
	CButton m_Stop;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
