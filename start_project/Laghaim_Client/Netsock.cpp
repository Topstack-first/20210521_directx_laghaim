#include "stdafx.h"
#include <assert.h>
#include "tcpipcon.h"
#include "main.h"
#include "g_devlogbox.h"
#include "packet_header.h"
#include "NkCharacter.h"

#define MAX_SENDING_BUF_LEN		4096

char sending_buf[MAX_SENDING_BUF_LEN+200] = "";
int  sending_len = 0;
char msg_buf[1024] = "";

int secure_method = -1;
int secure_num = -1;

extern bool g_bWndNetMessage;

BOOL CPacketControl::m_bSendEnabled = TRUE;
void CPacketControl::EnableSend(BOOL flag)
{
	m_bSendEnabled = flag;
}
BOOL CPacketControl::SendEnabled()
{
	return m_bSendEnabled;
}



BOOL CTcpIpConnection::SendNetMessage(void* msg, BOOL bForce, BOOL bSecure)
{
	if (CPacketControl::SendEnabled() == FALSE)
	{
		return false;
	}

	if(g_bWndNetMessage)
	{
		G_DevAddLogNetwork("--",(char*)msg);	
	}	

	rnPacket::SP packet(new rnPacket((char*)msg));	
	memset(packet->body(), 0, PACKET_SUB_HEADER_SIZE); 
	SendPacket(packet, bForce);
	return true;
}

BOOL CTcpIpConnection::SendPacket(rnPacket::SP packet, BOOL bForce)
{
	if (CPacketControl::SendEnabled() == FALSE)
	{
		return false;  //Arnold
	}

	packet->recalcSize();
	int msg_size = packet->size();
	int l;
	int TickCount = 0;
	char* tempbuf = (char*)packet->header();		
	
	stPacketHeader* hd = (stPacketHeader*)packet->header();
	stPacketSubHeader* shd = (stPacketSubHeader*)packet->body();
	shd->seq_ = m_chksum++;

	if( shd->msg.type_ == 1 )
	{
		msg_size = hd->size_ + PACKET_HEADER_SIZE;
	}
		
	if( msg_size > 1024 )
	{
		return FALSE;
	}	

	packet->encrypt();	

	memcpy(sending_buf + sending_len, tempbuf, msg_size);
	sending_len += (msg_size);

	int sentl = 0;
	if (sending_len > MAX_SENDING_BUF_LEN || bForce )
	{
		do
		{
			tempbuf = sending_buf;
			l = send(GetSocket(), tempbuf, sending_len-sentl, 0);
			if (l == SOCKET_ERROR)
			{
				sending_len = 0;
				return FALSE;
			}
			sentl+=l;
			tempbuf+=l;
			if (sentl<sending_len)
			{
				Sleep(5);
				TickCount+=1;
			}
		}
		while (sentl < sending_len  && TickCount<1000);

		if( sentl!=sending_len )
		{
			if (sending_len - sentl < MAX_SENDING_BUF_LEN)
			{
				memcpy(sending_buf, sending_buf+sentl, sending_len-sentl);
				sending_len -= sentl;
			}
			else
			{
				sending_len = 0;
				return FALSE;
			}
		}
		else
		{
			sending_len = 0;
		}
	}
	else
	{
		l = send(GetSocket(), sending_buf, sending_len, 0);

		if (l == SOCKET_ERROR)
		{
			return FALSE;
		}

		if (l < sending_len)
		{
			memcpy(sending_buf, sending_buf+l, sending_len-l);
			sending_len -= l;
		}
		else
		{
			sending_len = 0;
		}
	}

	return TRUE;
}

BOOL CTcpIpConnection::FlushSendingBuffer()
{
	if (sending_len <= 0)
		return TRUE;

	int l = send(GetSocket(), sending_buf, sending_len, 0);

	if (l == SOCKET_ERROR)
	{
		return FALSE;
	}

	if (l < sending_len)
	{
		memcpy(sending_buf, sending_buf+l, sending_len-l);
		sending_len -= l;
	}
	else
	{
		sending_len = 0;
	}

	return TRUE;
}

void CTcpIpConnection::ResetSendingBuffer()
{
	sending_buf[0] = '\0';
	sending_len = 0;
}
