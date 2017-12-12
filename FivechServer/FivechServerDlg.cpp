
// FivechServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FivechServer.h"
#include "FivechServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFivechServerDlg �Ի���




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


// CFivechServerDlg ��Ϣ�������

BOOL CFivechServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//net = CNetwork::GetInstance();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFivechServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
				sMessage* msg = CNetwork::GetInstance()->OnReceive(CurSock);//���յ�һ����Ϣ
				//���������Ϣ
				HandleMessage(msg);
			}
			break;
		default:break;
	}
	return 0;
}

void CFivechServerDlg::OnBnClickedBegin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CNetwork::GetInstance()->InitNetwork(m_hWnd);
}

void CFivechServerDlg::OnBnClickedQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//�ѷ�ʤ��
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
