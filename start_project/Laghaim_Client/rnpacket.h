#ifndef __RENEW_PACKET_H__
#define __RENEW_PACKET_H__

#include <vector>
#include <boost/chrono/system_clocks.hpp>
#include "MemoryPoolBase.h"
#include "rntype.h"
#include "packet_header.h"

#define APPEND_PACKET_SIZE		(16)
#define PACKET_CHECK_SUM_SIZE	(10)
#define MAX_PACKET_MESSAGE		(8300)

class rnPacket : public MemoryPoolBaseWithMutex<rnPacket>
{
public:
	friend class rnSocketIOServiceTcp;
	typedef boost::shared_ptr<rnPacket>		SP;

public:
	static void init();
	static void deinit();

public:
	explicit rnPacket();
	explicit rnPacket(int size);
	explicit rnPacket(std::string& msg);
	explicit rnPacket(const char* msg);
	explicit rnPacket(const char* msg, const int length);
	~rnPacket();

	tBYTE* header()
	{
		return buffer_;
	}
	tUINT size()
	{
		return next_store_pos_ + ALL_PACKET_HEADER_SIZE;
	}
	tUINT dataSize()
	{
		return next_store_pos_;
	}
	char* body()
	{
		return (char*)body_;
	}
	char* data()
	{
		return (char *)data_;
	}
	const std::vector<std::string>& getVector()
	{
		return vec_;
	}

public:
	bool addString(std::string& msg);
	bool addString(const tBYTE* msg, tUINT length);
	void getToken();
	bool isCrypt()
	{
		return crypt_flag_;
	}
	void encrypt();
	void decrypt();

	bool isRecalc()			{ return recalc_flag_; }
	void recalcSize();
	void setRecalc()		{ recalc_flag_ = true; }
	void setDataSize(int size)		{ next_store_pos_ = size; }
	boost::chrono::steady_clock::time_point& getReadTime()		{ return read_time_; }

private:
	void			mallocBuffer( tINT size );

private:
	tBYTE*			buffer_;				// 메모리 베이스 포인트
	tBYTE*			data_;
	tBYTE*			body_;

	std::vector<std::string>	vec_;
	tUINT			next_store_pos_;		// 다음번 저장할곳, 최종적으로 패킷의 길이를 나타냄('\0' 미포함)

	tINT			init_alloc_size_;
	tINT			pool_index_;			// 메모리 풀에서의 인덱스
	bool			crypt_flag_;			// true - 암호와됨, false - 안됨
	bool			recalc_flag_;

	boost::chrono::steady_clock::time_point		read_time_;
};

#endif

