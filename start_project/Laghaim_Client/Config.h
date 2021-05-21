#ifndef _CONFIG_H_
#define	_CONFIG_H_

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)
#pragma warning(disable : 4244) // ����ȯ
#pragma warning(disable : 4018) // �񱳱��� ������ġ

//////////////////////////////////////////////////////////////////////////
// ��Ʈ���� ���� Ȯ�� ��
#define MATRIXSVR_NO		0
#define MATRIXSVR_OLD		1
#define MATRIXSVR_2008		2
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Ŭ�� ������ ��Ŷ ����
#define PACKET_VER	854754  //Version change PABLO
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Generic Global Macro Function
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#endif	//_CONFIG_H_