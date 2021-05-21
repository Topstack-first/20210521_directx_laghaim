#include "stdafx.h"
#include "RecvPacketMgr.h"
#include "packet_header.h"

RecvPacketMgr::RecvPacketMgr()
{
	m_pStream = new char[MaxStreamBufferSize];
	m_nPacketCount = 0;

	m_pFront = m_pStream;
	m_pRear = m_pStream;
	m_pLast = m_pStream;
}

RecvPacketMgr::~RecvPacketMgr()
{
	delete [] m_pStream;
}

char* RecvPacketMgr::PopPacket()
{
	if( m_nPacketCount == 0 )
	{
		return NULL;
	}

	--m_nPacketCount;

	char* pPos = m_pFront;
	unsigned int usLen = ((stPacketHeader*)pPos)->size_ + PACKET_HEADER_SIZE;
	m_pFront += usLen;

	if( m_pFront - m_pStream > MaxStreamBufferValidSize )
	{
		return NULL;
	}

	return pPos;
}

void RecvPacketMgr::PushData(char* data, int size)
{
	if( size <= 0 )
		return;

	memcpy(m_pLast, data, size);
	m_pLast += size;

	while( m_pLast - m_pRear > 0 )
	{
		// ��� ������� �������� ������ ���� ����
		if( (m_pLast - m_pRear) < PACKET_HEADER_SIZE )
		{
			return;
		}

		// ����� ���ǵ� ũ�⸦ ��´�
		int nLength = ((stPacketHeader*)m_pRear)->size_ + PACKET_HEADER_SIZE;

		if( nLength > (m_pLast - m_pRear) )
		{
			// ����Ÿ�� �� �����ؾ��Ѵ�.
			return;
		}

		// ���� ���۸� �Ѿ�� ���� ���������� ������ ����
		if( m_pLast - m_pStream > MaxStreamBufferValidSize )
		{
			if( m_nPacketCount == 0 )
			{
				memcpy(m_pStream, m_pRear, m_pLast-m_pRear);
				m_pLast = m_pStream + (m_pLast - m_pRear);
				m_pFront = m_pStream;
				m_pRear = m_pStream;
			}
			else
			{
				// ����� ��Ŷ�� �ִٸ� ����Ǿ��� ������ ��θ� ���� ������ �̵�
				int nGap = m_pLast - m_pRear;
				memcpy(m_pStream, m_pFront, m_pLast-m_pFront);
				m_pLast = m_pStream + (m_pLast - m_pFront);
				m_pRear = m_pLast - nGap;
				m_pFront = m_pStream;
			}
		}
		m_pRear += nLength;

		++m_nPacketCount;
	}
}