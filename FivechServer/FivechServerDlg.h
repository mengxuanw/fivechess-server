
// FivechServerDlg.h : ͷ�ļ�
//

#pragma once

// CFivechServerDlg �Ի���
#include "Network.h"
#include "Board.h"
#include "Judge.h"

class CFivechServerDlg : public CDialogEx
{
// ����
public:
	CFivechServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIVECHSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNetEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBegin();
	CBoard boards[MAX_PLAYERS];
	CJudge judges[MAX_PLAYERS];
	afx_msg void OnBnClickedQuit();
	void HandleMessage(sMessage *Msg);
	void ChoiceChess(sMessage *Msg);
	void ClientLBDown(sMessage *Msg);
};
