
// FivechServerDlg.h : 头文件
//

#pragma once

// CFivechServerDlg 对话框
#include "Network.h"
#include "Board.h"
#include "Judge.h"

class CFivechServerDlg : public CDialogEx
{
// 构造
public:
	CFivechServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FIVECHSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
