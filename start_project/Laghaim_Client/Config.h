#ifndef _CONFIG_H_
#define	_CONFIG_H_

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)
#pragma warning(disable : 4244) // 형변환
#pragma warning(disable : 4018) // 비교구문 형불일치

//////////////////////////////////////////////////////////////////////////
// 매트릭스 서버 확인 값
#define MATRIXSVR_NO		0
#define MATRIXSVR_OLD		1
#define MATRIXSVR_2008		2
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 클라 서버간 패킷 버전
#define PACKET_VER	854754  //Version change PABLO
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Generic Global Macro Function
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#endif	//_CONFIG_H_