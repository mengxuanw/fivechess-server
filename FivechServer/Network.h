#pragma once

//#include "Board.h"


#include "define.h"

class CNetwork
{
private:
	CNetwork(void);
	~CNetwork(void);
public:
	static CNetwork* GetInstance(){return &s_Network;};
	BOOL OnClose(SOCKET CurSock);
	BOOL OnSend(sMessage *Msg);
	sMessage* OnReceive(SOCKET CurSock);
	BOOL OnAccept(SOCKET CurSock, HWND hwnd);
	BOOL InitNetwork(HWND m_hWnd);
	BOOL QueueMessage(void * Msg);
	//int ChoiceChess(sMessage *Msg);
	//int ClientLBDown(sMessage *Msg);
	BOOL Quit();
	int FirstEmpty();
	sPlayer Players[MAX_PLAYERS];
private:
	SOCKET ServerSock;	
	SOCKET ClientSock[MAX_PLAYERS];
	sockaddr PlayerAddr[MAX_PLAYERS];
	sMessage *m_messages;
	int m_MsgHead;						//消息队列头指针
	int m_MsgTail;						//消息队列尾指针
	CRITICAL_SECTION m_MessageCS;
	int Num_Players;
	static CNetwork s_Network;
};

