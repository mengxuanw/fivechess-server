#include "StdAfx.h"
#include "Network.h"

CNetwork CNetwork::s_Network;

CNetwork::CNetwork(void)
{
}


CNetwork::~CNetwork(void)
{
}

BOOL CNetwork::InitNetwork(HWND m_hWnd)
{
	WSADATA wsaData;
	BOOL ret = WSAStartup(MAKEWORD(2,2),&wsaData);
	if (ret != 0)
	{
		MessageBox(NULL,L"初始化网络协议失败",L"",0);
		return FALSE;
	}

	ServerSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (ServerSock == INVALID_SOCKET)
	{
		MessageBox(NULL,L"创建套接字失败",L"",0);
		WSACleanup();
		return FALSE;
	}
	sockaddr_in localaddr;
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(8888);
	localaddr.sin_addr.s_addr = 0;
	if (bind(ServerSock,(const struct sockaddr*)&localaddr,sizeof(sockaddr)) == SOCKET_ERROR)
	{
		MessageBox(NULL,L"绑定地址失败",L"",0);
		closesocket(ServerSock);
		WSACleanup();
		return FALSE;
	}
	WSAAsyncSelect(ServerSock,m_hWnd,NETWORK_EVENT,FD_READ | FD_ACCEPT | FD_CLOSE);
	listen(ServerSock,MAX_PLAYERS);
	return TRUE;
}

BOOL CNetwork::OnAccept(SOCKET CurSock, HWND hwnd)
{
	Num_Players = FirstEmpty();
	if (Num_Players == -1)
	{
		MessageBox(NULL,L"已经没有位置了",L"",0);
		return FALSE;
	}
	int AddrLen = sizeof(PlayerAddr[Num_Players]);
	ClientSock[Num_Players] = accept(ServerSock,&PlayerAddr[Num_Players],&AddrLen);
	if (ClientSock[Num_Players] == INVALID_SOCKET)
	{
		MessageBox(NULL,L"Unable to accept connection",L"",0);
		WSACleanup();
		return FALSE;
	}
	else
	{
		WSAAsyncSelect(ClientSock[Num_Players],hwnd,NETWORK_EVENT,FD_READ | FD_CLOSE);
		Players[Num_Players].Connected = true;
		sMessage uMsg;
		uMsg.Header.Type = MSG_ASSIGN_PLAYERID;
		uMsg.Header.Size = sizeof(sMessage);

		//只是将用户的ID发送到客户端，唯一的作用是分配ID
		Players[Num_Players].PlayerID = uMsg.Header.PlayerID = Num_Players;
		//将服务器分配的ID发送到自己
		if(send(ClientSock[Num_Players],(char *)&uMsg,sizeof(uMsg),0) == SOCKET_ERROR)
		{
			MessageBox(NULL,L"createPlayer Can't send smessage",L"",0);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CNetwork::OnClose(SOCKET CurSock)
{
	closesocket(CurSock);
	return TRUE;
}

/*int CNetwork::ChoiceChess(sMessage *Msg)
{
	int id = Msg->Header.PlayerID;
	if(Players[id].Connected == false)
		return -1;
	Players[id].black = Msg->black;
	boards[id].DoModal();
	boards[id].setID(id);
	boards[id].setTurn(!Msg->black);
	if(Msg->black)
		boards[id].setColor(0);
	else
		boards[id].setColor(1);
	return 0;
}

int CNetwork::ClientLBDown(sMessage *Msg)
{
	int id = Msg->Header.PlayerID;
	if(Players[id].Connected == false)
		return -1;
	chess tmp;
	tmp.x = Msg->x;
	tmp.y = Msg->y;
	tmp.black = Msg->black;
	boards[id].addChess(tmp);
	return 0;
}*/

sMessage* CNetwork::OnReceive(SOCKET CurSock)
{
	sMessage *uMsg = new sMessage;
    if(recv(CurSock,(char *)uMsg,sizeof(*uMsg),0) == SOCKET_ERROR)
	{
		MessageBox(NULL,L" OnReceive: Can't recv",L"",0);
		return NULL;
	}
	return uMsg;
}

BOOL CNetwork::OnSend(sMessage *Msg)
{
	sMessage msg;
	switch(Msg->Header.Type)
	{
		case MSG_LBUTTON_DOWN:
		{
			msg.Header.Type = MSG_LBUTTON_DOWN;
			msg.x = Msg->x;
			msg.y = Msg->y;
			msg.black = Msg->black;
			msg.Header.Size = sizeof(sMessage);
			msg.Header.PlayerID = Msg->Header.PlayerID;
		}
		case MSG_WIN:
		case MSG_LOSE:
			{
				msg.Header.Type = Msg->Header.Type;
				msg.Header.Size = Msg->Header.Size;
				msg.Header.PlayerID = Msg->Header.PlayerID;
			}
		break;
		default:break;
	}
	if(send(ClientSock[msg.Header.PlayerID],(char*)&msg, sizeof(msg), 0) == SOCKET_ERROR)
	{
		MessageBox(NULL,L" OnSend: Can't send",L"",0);
		return FALSE;
	}
	return TRUE;
}

int CNetwork::FirstEmpty()
{
	for(int i = 0; i < MAX_PLAYERS ; i++)
	{
		if(Players[i].Connected == false)
			return i;
	}
	return -1;
}

BOOL CNetwork::Quit()
{
	closesocket(ServerSock);
	WSACleanup();
	return TRUE;
}

BOOL CNetwork::QueueMessage(void * Msg)
{
	sMessageHeader * mh = (sMessageHeader *)Msg;
	if (m_messages == NULL)
	{
		return FALSE;
	}
	if (((m_MsgHead + 1) % NUM_MESSAGES) == m_MsgTail)
	{
		return FALSE;
	}
	if (mh->Size <= sizeof(sMessage))
	{
		EnterCriticalSection(&m_MessageCS);
		memcpy(&m_messages[m_MsgHead],Msg,mh->Size);
		m_MsgHead ++;
		if (m_MsgHead >= NUM_MESSAGES)
		{
			m_MsgHead = 0;
		}
		LeaveCriticalSection(&m_MessageCS);
	}
	return TRUE;
}