
// FivechServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FivechServer.h"
#include "FivechServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFivechServerDlg 对话框




CFivechServerDlg::CFivechServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFivechServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFivechServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFivechServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(NETWORK_EVENT, OnNetEvent)
	ON_BN_CLICKED(ID_BEGIN, &CFivechServerDlg::OnBnClickedBegin)
	ON_BN_CLICKED(ID_QUIT, &CFivechServerDlg::OnBnClickedQuit)
END_MESSAGE_MAP()


// CFivechServerDlg 消息处理程序

BOOL CFivechServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//net = CNetwork::GetInstance();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFivechServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFivechServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFivechServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CFivechServerDlg::OnNetEvent(WPARAM wParam, LPARAM lParam)
{
	int iEvent = WSAGETSELECTEVENT(lParam);
	SOCKET CurSock= (SOCKET)wParam;
    switch(iEvent)
    {
		case FD_ACCEPT:
			CNetwork::GetInstance()->OnAccept(CurSock,m_hWnd);
			break;
		case FD_CLOSE:
			CNetwork::GetInstance()->OnClose(CurSock);
			break;
		case FD_READ:
			{
				sMessage* msg = CNetwork::GetInstance()->OnReceive(CurSock);//接收到一个消息
				//处理这个消息
				HandleMessage(msg);
			}
			break;
		default:break;
	}
	return 0;
}

void CFivechServerDlg::OnBnClickedBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	CNetwork::GetInstance()->InitNetwork(m_hWnd);
}

void CFivechServerDlg::OnBnClickedQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	CNetwork::GetInstance()->Quit();
}
void CFivechServerDlg::ChoiceChess(sMessage *Msg)
{
	int id = Msg->Header.PlayerID;
	if(CNetwork::GetInstance()->Players[id].Connected == false)
		return;
	CNetwork::GetInstance()->Players[id].black = Msg->black;
	boards[id].setID(id);
	boards[id].setTurn(!Msg->black);
	if(Msg->black)
		boards[id].setColor(0);
	else
		boards[id].setColor(1);
	boards[id].DoModal();
}

void CFivechServerDlg::ClientLBDown(sMessage *Msg)
{
	int id = Msg->Header.PlayerID;
	if(CNetwork::GetInstance()->Players[id].Connected == false)
		return;
	chess tmp;
	tmp.x = Msg->x;
	tmp.y = Msg->y;
	tmp.black = Msg->black;
	boards[id].addChess(tmp);
	judges[id].changeFlag(Msg->x,Msg->y,Msg->black);
	//已分胜负
	int type = judges[id].is_Win();
	if(type != -1)
	{
		sMessage uMsg;
		uMsg.Header.PlayerID = id;
		uMsg.Header.Size = sizeof(sMessage);
		if(type = boards[id].getColor())
			uMsg.Header.Type = MSG_LOSE;
		else
			uMsg.Header.Type = MSG_WIN;
		CNetwork::GetInstance()->OnSend(&uMsg);
		if(type = boards[id].getColor())
			MessageBox(NULL,L"You win");
		else
			MessageBox(NULL,L"You lose");
		boards[id].setOver(true);
	}
	else
		boards[id].setTurn(1);
}

void CFivechServerDlg::HandleMessage(sMessage *Msg)
{
	switch(Msg->Header.Type)
	{
	case MSG_CHOICE_CHESS:
		ChoiceChess(Msg);
		break;
	case MSG_LBUTTON_DOWN:
		ClientLBDown(Msg);
		break;
	default:break;
	}
}
