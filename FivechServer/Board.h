#pragma once
#include "Network.h"
#include "resource.h"

#include <vector>

using namespace std;
typedef struct chess
{
	int x;
	int y;
	int black;
}chess;
// CBoard �Ի���

class CBoard : public CDialog
{
	DECLARE_DYNAMIC(CBoard)

public:
	CBoard(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBoard();

// �Ի�������
	enum { IDD = IDD_BOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	vector<chess> Chess;
	int turn;
	int color;
	int id;
	bool over;
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void setTurn(int t);
	void setColor(int c);
	int getColor();
	int getTurn();
	void setID(int id);
	void setOver(bool o);
	void addChess(chess c);
};