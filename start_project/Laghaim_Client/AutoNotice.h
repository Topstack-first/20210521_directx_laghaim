#ifndef __auto_notice_screen_print_class_200708231459__
#define __auto_notice_screen_print_class_200708231459__
// 브라질 요청 : 10분 마다 공지 메시지 출력


#define AUTONOTICE_MAX_COUNT	6

class CAutoNotice
{
public:
	char	m_strNotice[AUTONOTICE_MAX_COUNT][512];

	BOOL	m_bActive;

	DWORD	m_dwBeforeTime;
	DWORD	m_DelayTime;
	int		m_Count;

	CAutoNotice();
	~CAutoNotice();

	void SetTime(DWORD minute)
	{
		m_DelayTime = minute *(1000*60);
	}
	void SetActive(BOOL active)
	{
		m_bActive	 = active;
	}
	HRESULT	MsgProc();
};

#endif