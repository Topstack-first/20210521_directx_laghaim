#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>

#include "rnpacket.h"
#include "SEED_256_KISA.h"

#include <boost/pool/singleton_pool.hpp>
#include <boost/bind.hpp>
#include <boost/tokenizer.hpp>

typedef boost::tokenizer<boost::char_separator<char> > stokenizer;
static boost::char_separator<char> sep(" \n", NULL, boost::drop_empty_tokens);

//////////////////////////////////////////////////////////////////////
// SEED 암호화를 위한 초기화
static DWORD pdwRoundKey[48];
static BYTE pbUserKey[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

//////////////////////////////////////////////////////////////////////////
#define DEFINE_POOL_OBJECT(x)		struct _tag_##x {}; typedef boost::singleton_pool<_tag_##x, x> g_buffer_pool_##x;


DEFINE_POOL_OBJECT(128);
DEFINE_POOL_OBJECT(192);
DEFINE_POOL_OBJECT(256);
DEFINE_POOL_OBJECT(384);
DEFINE_POOL_OBJECT(512);
DEFINE_POOL_OBJECT(786);
DEFINE_POOL_OBJECT(1024);
DEFINE_POOL_OBJECT(1536);
DEFINE_POOL_OBJECT(2048);
DEFINE_POOL_OBJECT(3072);
DEFINE_POOL_OBJECT(4096);
DEFINE_POOL_OBJECT(6144);
DEFINE_POOL_OBJECT(MAX_PACKET_MESSAGE);


#define MALLOC_BUFFER( x )	do { \
	buffer_ = (tBYTE*)g_buffer_pool_##x::malloc(); \
} while(0)

#define FREE_BUFFER( buffer, x )	g_buffer_pool_##x::free( buffer );

static tINT *packet_size_index = NULL;

void rnPacket::init(void)
{
	tINT base_index[] =
	{
		128,
		192,
		256,
		384,
		512,
		786,
		1024,
		1536,
		2048,
		3072,
		4096,
		6144,
		MAX_PACKET_MESSAGE,
	};

	packet_size_index = new tINT[MAX_PACKET_MESSAGE + 1];
	tINT index = 0;
	for (int i = 0; i <= MAX_PACKET_MESSAGE; ++i)
	{
		packet_size_index[i] = index;

		if (i >= base_index[index])
		{
			++index;
		}
	}

	SeedRoundKey(pdwRoundKey, pbUserKey);
}

void rnPacket::deinit()
{
	delete [] packet_size_index;
}

void rnPacket::mallocBuffer( tINT size )
{
	buffer_ = NULL;
	body_ = NULL;
	data_ = NULL;

	size += PACKET_CHECK_SUM_SIZE;
	init_alloc_size_ = size;

	if( size <= MAX_PACKET_MESSAGE )
	{
		tINT index = packet_size_index[size];
		switch( index )
		{
		case 0:
			MALLOC_BUFFER(128);
			break;
		case 1:
			MALLOC_BUFFER(192);
			break;
		case 2:
			MALLOC_BUFFER(256);
			break;
		case 3:
			MALLOC_BUFFER(384);
			break;
		case 4:
			MALLOC_BUFFER(512);
			break;
		case 5:
			MALLOC_BUFFER(786);
			break;
		case 6:
			MALLOC_BUFFER(1024);
			break;
		case 7:
			MALLOC_BUFFER(1536);
			break;
		case 8:
			MALLOC_BUFFER(2048);
			break;
		case 9:
			MALLOC_BUFFER(3072);
			break;
		case 10:
			MALLOC_BUFFER(4096);
			break;
		case 11:
			MALLOC_BUFFER(6144);
			break;
		default:
			MALLOC_BUFFER(MAX_PACKET_MESSAGE);
			break;
		}

		pool_index_ = index;
	}
	else
	{
		//LOG_ERROR("invalid packet size[%d]", size);
		return;
	}

	if(buffer_ == NULL)
	{
		//LOG_ERROR("malloc rnpacket size[%d] fail", size);
		return;
	}

	tINT test_offset = init_alloc_size_ - PACKET_CHECK_SUM_SIZE;

	buffer_[test_offset] = 0xff;
	buffer_[test_offset + 1] = 0xff;
	buffer_[test_offset + 2] = 0xff;

	body_ = buffer_ + PACKET_HEADER_SIZE;
	data_ = buffer_ + ALL_PACKET_HEADER_SIZE;
}

rnPacket::rnPacket()
{
	tUINT size = MAX_PACKET_MESSAGE - PACKET_CHECK_SUM_SIZE;
	mallocBuffer(size);
	memset(body_, 0, PACKET_SUB_HEADER_SIZE);
	next_store_pos_ = 0;
	crypt_flag_ = false;
	recalc_flag_ = false;
}

rnPacket::rnPacket(int size)
{
	mallocBuffer(size + ALL_PACKET_HEADER_SIZE);
	memset(body_, 0, PACKET_SUB_HEADER_SIZE);
	next_store_pos_ = size - PACKET_SUB_HEADER_SIZE;
	crypt_flag_ = false;
	recalc_flag_ = false;
}

rnPacket::rnPacket( std::string& msg )
{
	tUINT size = msg.length();
	mallocBuffer(size + ALL_PACKET_HEADER_SIZE);
	memset(body_, 0, PACKET_SUB_HEADER_SIZE);
	memcpy(data_, msg.c_str(), msg.length());
	next_store_pos_ = size;
	crypt_flag_ = false;
	recalc_flag_ = false;
}

rnPacket::rnPacket( const char* msg )
{
	tUINT size = ::strlen(msg);
	mallocBuffer(size + ALL_PACKET_HEADER_SIZE);
	memset(body_, 0, PACKET_SUB_HEADER_SIZE);
	memcpy(data_, msg, size);	
	next_store_pos_ = size;
	crypt_flag_ = false;
	recalc_flag_ = false;
}

rnPacket::rnPacket( const char* msg, const int length )
{
	tUINT size = length;
	mallocBuffer(size + ALL_PACKET_HEADER_SIZE);
	memset(body_, 0, PACKET_SUB_HEADER_SIZE);
	memcpy(data_, msg, size);
	next_store_pos_ = size;
	crypt_flag_ = false;
	recalc_flag_ = false;
}

rnPacket::~rnPacket()
{
	tINT test_offset = init_alloc_size_ - PACKET_CHECK_SUM_SIZE;

	if( buffer_[test_offset] != 0xff ||
			buffer_[test_offset + 1] != 0xff ||
			buffer_[test_offset + 2] != 0xff )
	{
		//LOG_ERROR( "rnPacket::check error" );
		return;
	}

	switch( pool_index_ )
	{
	case 0:
		FREE_BUFFER(buffer_, 128);
		break;
	case 1:
		FREE_BUFFER(buffer_, 192);
		break;
	case 2:
		FREE_BUFFER(buffer_, 256);
		break;
	case 3:
		FREE_BUFFER(buffer_, 384);
		break;
	case 4:
		FREE_BUFFER(buffer_, 512);
		break;
	case 5:
		FREE_BUFFER(buffer_, 786);
		break;
	case 6:
		FREE_BUFFER(buffer_, 1024);
		break;
	case 7:
		FREE_BUFFER(buffer_, 1536);
		break;
	case 8:
		FREE_BUFFER(buffer_, 2048);
		break;
	case 9:
		FREE_BUFFER(buffer_, 3072);
		break;
	case 10:
		FREE_BUFFER(buffer_, 4096);
		break;
	case 11:
		FREE_BUFFER(buffer_, 6144);
		break;
	default:
		FREE_BUFFER(buffer_, MAX_PACKET_MESSAGE);
		break;
	}
}

bool rnPacket::addString( std::string& msg )
{
	if (next_store_pos_ + msg.length() >= MAX_PACKET_MESSAGE)
		return false;

	tBYTE* p = data_ + next_store_pos_;
	memcpy(p, msg.c_str(), msg.length());
	next_store_pos_ += msg.length();
	data_[next_store_pos_] = '\0';

	return true;
}

bool rnPacket::addString( const tBYTE* msg, tUINT length )
{
	if (next_store_pos_ + length >= MAX_PACKET_MESSAGE)
		return false;

	tBYTE* p = data_ + next_store_pos_;
	memcpy(p, msg, length);
	next_store_pos_ += length;
	data_[next_store_pos_] = '\0';

	return true;
}

void rnPacket::getToken()
{
	std::string str((char *)data_);
	stokenizer token(str, sep);

	vec_.assign(token.begin(), token.end());
}

void rnPacket::encrypt()
{
	stPacketHeader* pHeader = reinterpret_cast<stPacketHeader*>(buffer_);
	int loopcount = pHeader->size_ / 16;
	for (int i = 0; i < loopcount; ++i)
	{
		SeedEncrypt(body_ + (i * 16), pdwRoundKey);
	}

	crypt_flag_ = true;
}

void rnPacket::decrypt()
{
	stPacketHeader* pHeader = reinterpret_cast<stPacketHeader*>(buffer_);
	int loopcount = pHeader->size_ / 16;
	for (int i = 0; i < loopcount; ++i)
	{
		SeedDecrypt(body_ + (i * 16), pdwRoundKey);
	}

	crypt_flag_ = false;
}

void rnPacket::recalcSize()
{
	stPacketHeader* pHeader = reinterpret_cast<stPacketHeader*>(buffer_);
	pHeader->size_ = next_store_pos_ + PACKET_SUB_HEADER_SIZE;
	recalc_flag_ = true;
}
