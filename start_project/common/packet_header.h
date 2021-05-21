#ifndef _PACKET_HEADER_H_
#define _PACKET_HEADER_H_

#pragma pack(push, 1)
struct stPacketHeader
{
	unsigned short size_;	
};
struct stPacketSubHeader
{
	struct  
	{
		unsigned short  index_ : 14;
		unsigned short  type_ : 2;			
	} msg;

	int dummy_;

	unsigned short	seq_;
};

struct stPacketSniffingHeader
{
	unsigned int  now_time;
	unsigned char channel_num;
	unsigned char world_num;
	unsigned int  user_index;
	unsigned int  char_index;
};

#pragma pack(pop)

#define PACKET_HEADER_SIZE	(sizeof(stPacketHeader))
#define PACKET_SUB_HEADER_SIZE	(sizeof(stPacketSubHeader))
#define ALL_PACKET_HEADER_SIZE	(PACKET_HEADER_SIZE + PACKET_SUB_HEADER_SIZE)
#define SNIFFING_HEADER_SIZE (sizeof(stPacketSniffingHeader))

#endif // _PACKET_HEADER_H_
