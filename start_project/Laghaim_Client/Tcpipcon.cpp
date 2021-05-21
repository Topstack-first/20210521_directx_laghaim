#include "stdafx.h"
#include "Tcpipcon.h"
#include "packet_header.h"
#include "RecvPacketMgr.h"
#include "SEED_256_KISA.h"

CTcpIpConnection * g_pTcpIp = NULL;

extern int g_MatrixSvr;
extern int g_nCurWorldIdx;

static DWORD pdwRoundKey[48];
static BYTE pbUserKey[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

void CTcpIpConnection::InitSeedEncrypt()
{
	SeedRoundKey(pdwRoundKey, pbUserKey);
}

CTcpIpConnection::CTcpIpConnection()
	: m_nPort(0)
	, m_chksum(0)
{
	m_sock = INVALID_SOCKET;
}

CTcpIpConnection::~CTcpIpConnection()
{
	if( m_sock != INVALID_SOCKET )
	{
		closesocket(m_sock);
	}	
}

void CTcpIpConnection::ResetConnection()
{
	CloseSocket();
}

void CTcpIpConnection::CloseSocket()
{
	if( m_sock != INVALID_SOCKET )
	{
		closesocket(m_sock);
	}

	m_chksum = 0;
	m_sock = INVALID_SOCKET;
}

BOOL CTcpIpConnection::InitConnection(SOCKET outSocket)
{
	m_chksum = 0;
	m_sock = outSocket;
	BOOL bl = TRUE;
	unsigned long blength = 256;
	unsigned long bpr = 0;

	setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&bl, sizeof(BOOL));
	setsockopt(m_sock, IPPROTO_TCP, SO_KEEPALIVE, (const char*)&bl, sizeof(BOOL));

	{
		int optval;
		int optlen = sizeof(optval);
		getsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char*)&optval, &optlen);

		optval = optval * 4;
		setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char*)&optval, sizeof(optval));	
	}

	ResetSendingBuffer();

	return TRUE;
}

void CTcpIpConnection::SetTerminate()
{
	if( m_sock!=INVALID_SOCKET )
	{
		closesocket(m_sock);
	}

	m_chksum = 0;
	m_sock = INVALID_SOCKET;
}

BOOL CTcpIpConnection::ConnectToServer(char* serverName, int _port)
{
	SOCKET ServerSock = socket(AF_INET, SOCK_STREAM, 0);

	unsigned long val = 1;
	ioctlsocket(ServerSock, FIONBIO, &val);

	sockaddr_in sai;
	ZeroMemory(&sai, sizeof(sockaddr_in));
	sai.sin_port = htons(_port);
	sai.sin_family = AF_INET;
	sai.sin_addr.s_addr = inet_addr(serverName);
	if(INADDR_NONE == sai.sin_addr.s_addr)
	{
		hostent *Hostent = gethostbyname(serverName);
		if (Hostent == NULL)
		{	
			return FALSE;
		}
		sai.sin_addr.s_addr = ((struct in_addr FAR *)(Hostent->h_addr))->s_addr;
	}

	int res = connect(ServerSock, (sockaddr*)&sai, sizeof(sockaddr_in));
	if (res < 0)
	{
		int err = WSAGetLastError();
		if( err != WSAEWOULDBLOCK && err != EINPROGRESS )
		{	
			return FALSE;
		}
	};

	if (res == 0)
		goto connect_done;

	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(ServerSock, &fdset);

	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	if( select(0, NULL, &fdset, NULL, &tv) == SOCKET_ERROR )
	{
		return FALSE;
	}
	if( !FD_ISSET(ServerSock, &fdset) )
	{
		return FALSE;
	}

connect_done:
	
	ResetConnection();
	InitConnection(ServerSock);

	if( g_MatrixSvr == 1 )
	{
		g_nCurWorldIdx = 10;
	}
	else if( g_MatrixSvr == 2 )
	{
		g_nCurWorldIdx = 19;
	}

	m_strIp = serverName;
	m_nPort = _port;	

	return TRUE;
}

int CTcpIpConnection::ReadMessage()
{
	char recv_buf[MAX_RECV_BUF_LEN];
	int l = recv(GetSocket(), recv_buf, MAX_RECV_BUF_LEN, 0);

	if( l == 0 )
		return -1;

	if( l == SOCKET_ERROR )
	{
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK)
		{
			ZeroMemory( recv_buf, MAX_RECV_BUF_LEN );
			return 0;
		}

		ZeroMemory( recv_buf, MAX_RECV_BUF_LEN );
		return -1;

	}

	if( l > 0 )
	{
		RecvPacketMgr* packMgr = RecvPacketMgr::GetInstance();
		packMgr->PushData(recv_buf, l);
	}

	return l;
}

const std::string& CTcpIpConnection::GetIp()
{
	return m_strIp;
}

const int& CTcpIpConnection::GetPort()
{
	return m_nPort;
}

void CTcpIpConnection::encrypt(char* buf, int size)
{
	int loopcount = size / 16;
	for (int i = 0; i < loopcount; ++i)
	{
		SeedEncrypt((BYTE*)buf + (i * 16), pdwRoundKey);
	}
}

void CTcpIpConnection::decrypt(char* buf, int size)
{
	int loopcount = size / 16;
	for (int i = 0; i < loopcount; ++i)
	{
		SeedDecrypt((BYTE*)buf + (i * 16), pdwRoundKey);
	}
}
