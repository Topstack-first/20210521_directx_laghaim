#ifndef __CONTROLGUILDM_H_
#define __CONTROLGUILDM_H_

#include "Config.h"

#include "Nk2dframe.h"
#include "BasicButton.h"

class CSurface;
class CTextBoard;
class CMutiListBox;

class CJoinRequestControl; // ������ ��� ���
class CJobChangeControl;   // ��å
class CGradeChangeControl; // ��޸� ����
class CNoticeControl;	   // ��������
class CMemberControl;	   // ����

class CControlGuildManage
{
private:
	typedef enum _eDrawPage
	{
		PAGE_GRADE_CHANGE = 0,	// ��� ��޸� ����
		PAGE_NOTICE,			// ��� ���� ����
		PAGE_JOB,				// ��� ��å ����
		PAGE_MEMBER,				// ��� ��� ����
		PAGE_REQUEST,			// ��� ���� ����
		PAGE_NONE,
		PAGE_NUM
	};

	_eDrawPage	m_current_page;
public:
	HWND	m_hWnd;
	RECT    m_ClientRc;
	CSurface*  m_pBack;				// ��� �κ�

	CBasicButton m_Btclass;		// ��� ��޸� ����
	CBasicButton m_BtNotice;	// ��� ���� ����
	CBasicButton m_BtJob;		// ��� ��å ����
	CBasicButton m_BtMember;	// ��� ��� ����
	CBasicButton m_BtEntry;		// ��� ���� ����
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