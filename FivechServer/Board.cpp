// Board.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FivechServer.h"
#include "Board.h"
#include "afxdialogex.h"


// CBoard �Ի���

IMPLEMENT_DYNAMIC(CBoard, CDialog)

CBoard::CBoard(CWnd* pParent /*=NULL*/)
	: CDialog(CBoard::IDD, pParent)
{
	over = false;
}

CBoard::~CBoard()
{
}

void CBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBoard, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CBoard ��Ϣ�������

void CBoard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	for(int i = 0; i < 17; i ++)
	{
		dc.MoveTo(25, 25 + i*30);
		dc.LineTo(505, 25 + i*30);
		dc.MoveTo(25 + i*30, 25);
		dc.LineTo(25 + i*30, 505);
	}
	for(int i = 0; i < Chess.size(); i ++)
	{
		int color = !Chess[i].black;
		CPoint tmppoint;
		CPen pNewPen;
		CPen *pOldPen;
		pNewPen.CreatePen(PS_SOLID,1,RGB(255*color,255*color,255*color));
		pOldPen = dc.SelectObject(&pNewPen);

		CBrush fillbrush;
		CBrush *oldbrush;
		fillbrush.CreateSolidBrush(RGB(255*color,255*color,255*color));
		oldbrush = dc.SelectObject(&fillbrush);
		dc.Ellipse(10+30*Chess[i].x, 10+30*Chess[i].y, 10+30*(Chess[i].x+1), 10+30*(Chess[i].y+1));
		dc.SelectObject(oldbrush);
		dc.SelectObject(pOldPen);
		fillbrush.DeleteObject();
		pNewPen.DeleteObject();
	}
}

void CBoard::setTurn(int t)
{
	turn = t;
}
void CBoard::setColor(int c)
{
	color = c;
}
int CBoard::getColor()
{
	return color;
}
int CBoard::getTurn()
{
	return turn;
}
void CBoard::setID(int id)
{
	this->id = id;
}

void CBoard::addChess(chess c)
{
	Chess.push_back(c);
	Invalidate();
}
void CBoard::setOver(bool o)
{
	over = o;
}
void CBoard::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!over)
	{
		if(turn)
		{
			//��ӵ���������
			if(point.x > 10 && point.x < 490 && point.y > 10 && point.y < 490)
			{
				chess tmp;
				tmp.x = (point.x - 10)/30;
				tmp.y = (point.y - 10)/30;
				tmp.black = color;
				Chess.push_back(tmp);
				//���͸���Ӧ�Ŀͻ��ˣ�������ӵ���Ϣ
			
				sMessage *Msg = new sMessage;
				Msg->Header.Type = MSG_LBUTTON_DOWN;
				Msg->Header.PlayerID = id;
				Msg->black = color;
				Msg->x = tmp.x;
				Msg->y = tmp.y;
				CNetwork::GetInstance()->OnSend(Msg);
				delete Msg;
				turn = 0;
				Invalidate();
			}
		}
		else
		{
			MessageBox(NULL,L" �ȴ��Է�����");
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}