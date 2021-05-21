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
		// 헤더 사이즈보다 작을때는 데이터 수집 진행
		if( (m_pLast - m_pRear) < PACKET_HEADER_SIZE )
		{
			return;
		}

		// 헤더에 정의된 크기를 얻는다
		int nLength = ((stPacketHeader*)m_pRear)->size_ + PACKET_HEADER_SIZE;

		if( nLength > (m_pLast - m_pRear) )
		{
			// 데이타를 더 수집해야한다.
			return;
		}

		// 가용 버퍼를 넘어설때 버퍼 시작점으로 포인터 변경
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
				// 저장된 패킷이 있다면 저장되었던 데이터 모두를 버퍼 앞으로 이동
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