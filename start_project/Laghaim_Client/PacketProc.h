#ifndef _PACKET_PROC_H_
#define _PACKET_PROC_H_

#include "singleton.h"
#include "rnpacket.h"

class PacketProcMgr : public CSingleton<PacketProcMgr>
{
private:
	typedef void (*cmd_f)(rnPacket::SP packet);
	typedef std::map<unsigned int, cmd_f> MAP_FUNC;

public:
	PacketProcMgr();
	~PacketProcMgr();

public:
	void Init();
	void AddCmd(const unsigned int pack_index, void (*cmd_p)(rnPacket::SP packet));
	void Excute(rnPacket::SP packet);

private:
	MAP_FUNC m_mapFunc;
};

#endif // _PACKET_PROC_H_
