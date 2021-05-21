#ifndef __CONTROLGUILDM_H_
#define __CONTROLGUILDM_H_

#include "Config.h"

#include "Nk2dframe.h"
#include "BasicButton.h"

class CSurface;
class CTextBoard;
class CMutiListBox;

class CJoinRequestControl; // 가입자 대기 명단
class CJobChangeControl;   // 직책
class CGradeChangeControl; // 계급명 변경
class CNoticeControl;	   // 공지사항
class CMemberControl;	   // 길드원

class CControlGuildManage
{
private:
	typedef enum _eDrawPage
	{
		PAGE_GRADE_CHANGE = 0,	// 길드 계급명 변경
		PAGE_NOTICE,			// 길드 공지 관리
		PAGE_JOB,				// 길드 직책 관리
		PAGE_MEMBER,				// 길드 멤버 관리
		PAGE_REQUEST,			// 길드 가입 승인
		PAGE_NONE,
		PAGE_NUM
	};

	_eDrawPage	m_current_page;
public:
	HWND	m_hWnd;
	RECT    m_ClientRc;
	CSurface*  m_pBack;				// 배경 인벤

	CBasicButton m_Btclass;		// 길드 계급명 변경
	CBasicButton m_BtNotice;	// 길드 공지 관리
	CBasicButton m_BtJob;		// 길드 직책 관리
	CBasicButton m_BtMember;	// 길드 멤버 관리
	CBasicButton m_BtEntry;		// 길드 가입 승인
	CBasicButton m_BtClose;

	CJoinRequestControl *m_pJoinWait;
	CGradeChangeControl *m_pGradeName;
	CJobChangeControl   *m_pJob;
	CNoticeControl		*m_pNoticeInfo;
	CMemberControl		*m_pMemberlist;

public:
	CControlGuildManage();
	~CControlGuildManage();

	void Init();
	void LoadRes();
	void Draw();
	void DeleteRes();
	BOOL IsInside(int, int);

	void SetPosition();
	void Restore();

	BOOL IsCheckBtnStatus();

	void ResetNotice(char *szNotice);
	void ResetGrade(char *szgrade, int nLevel);
	void ResetJoinRequest(int charindex);
	void ResetMember();

	void set_btn_status(BOOL bEnable);

	void all_data_Reset();
	void HookClose();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


};
#endif // __CONTROLGUILDM_H_