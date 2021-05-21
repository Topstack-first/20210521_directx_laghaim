#ifndef __RECV_PACKET_MGR_H__
#define __RECV_PACKET_MGR_H__

#include "singleton.h"

class RecvPacketMgr : public CSingleton<RecvPacketMgr>
{
	enum
	{
		MaxStreamBufferSize		 = 0x00300000,		///< �ִ� ���� ������ : 3145728 BYTE
		MaxStreamBufferValidSize = 3000000,			///< ���� ���� ������ : 3000000 BYTE
	};

public:
	RecvPacketMgr();
	~RecvPacketMgr();

public:
	char* PopPacket();						// ��Ŷ ������. ��Ŷ ���� ������ ����
	void PushData(char* data, int size);	// ������ �ֱ�

private:
	char* m_pStream;		// ���� ������
	long m_nPacketCount;	// ����� ��Ŷ�� ����

private:
	char* m_pFront;			// �̾Ƴ� �ּҰ�
	char* m_pRear;			// �Ϸ�� ��Ŷ�� ������ ��ġ
	char* m_pLast;			// �ű� �����Ͱ� �ٴ� ��
};

#endif // __RECV_PACKET_MGR_H__
