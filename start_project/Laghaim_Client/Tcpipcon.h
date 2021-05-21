#ifndef _TCPIP_CON_H_
#define _TCPIP_CON_H_

#include "Config.h"
#include "rnpacket.h"

#define MAX_RECV_BUF_LEN	8192
#define DEFAULT_FRAME  0x76453201

class CPacketControl
{
	static BOOL m_bSendEnabled;

public:
	static void EnableSend(BOOL flag);
	static BOOL SendEnabled();
};

class CTcpIpConnection
{
public:
	static void InitSeedEncrypt();

public:
	CTcpIpConnection();
	~CTcpIpConnection();

	BOOL InitConnection(SOCKET outSocket);
	void ResetConnection();
	void SetTerminate();

	SOCKET GetSocket()
	{
		return m_sock;
	}

	void CloseSocket();

	BOOL ConnectToServer(char* serverName, int port);
		
	BOOL SendNetMessage(void* msg, BOOL bForce = FALSE, BOOL bSecure = TRUE); //TCP/IP wrapper
	BOOL SendPacket(rnPacket::SP packet, BOOL bForce = FALSE);
	BOOL FlushSendingBuffer();
	void ResetSendingBuffer();
	
	int ReadMessage();

	const std::string& GetIp();
	const int& GetPort();

	void encrypt(char* buf, int size);
	void decrypt(char* buf, int size);	

private:
	SOCKET m_sock;

	std::string m_strIp;
	int m_nPort;

	unsigned short	m_chksum;
};

extern CTcpIpConnection * g_pTcpIp;

#endif // _TCPIP_CON_H_