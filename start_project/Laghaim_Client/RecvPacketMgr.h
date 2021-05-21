#ifndef __RECV_PACKET_MGR_H__
#define __RECV_PACKET_MGR_H__

#include "singleton.h"

class RecvPacketMgr : public CSingleton<RecvPacketMgr>
{
	enum
	{
		MaxStreamBufferSize		 = 0x00300000,		///< 최대 버퍼 사이즈 : 3145728 BYTE
		MaxStreamBufferValidSize = 3000000,			///< 가용 버퍼 사이즈 : 3000000 BYTE
	};

public:
	RecvPacketMgr();
	~RecvPacketMgr();

public:
	char* PopPacket();						// 패킷 꺼내기. 패킷 시작 포인터 리턴
	void PushData(char* data, int size);	// 데이터 넣기

private:
	char* m_pStream;		// 버퍼 포인터
	long m_nPacketCount;	// 저장된 패킷의 갯수

private:
	char* m_pFront;			// 뽑아낼 주소값
	char* m_pRear;			// 완료된 패킷의 마지막 위치
	char* m_pLast;			// 신규 데이터가 붙는 곳
};

#endif // __RECV_PACKET_MGR_H__
