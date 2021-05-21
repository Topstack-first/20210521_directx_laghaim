#ifndef _CONTROL_LETTER_H_
#define _CONTROL_LETTER_H_

#include "Config.h"

#include "LetterList.h"
#include "NoticeList.h"
#include "LetterSend.h"
#include "LetterFriend.h"
#include "LetterReject.h"
#include "LetterView.h"
#include "NoticeView.h"

class CSurface;
class CBasicButton;
class CTcpIpConnection;
class CLetterList;
class CNoticeList;
class CLetterSend;
class CLetterFriend;
class CLetterReject;
class CLetterView;
class CNoticeView;

class CControlLetter
{
public:
	CControlLetter();
	virtual ~CControlLetter();

	void	LoadRes();
	void    DeleteRes();
	void	Draw();
	LRESULT	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);

public:

	CSurface * m_pBack;
	CSurface * m_pSecret;

	CBasicButton * m_pBtn_List;
	CBasicButton * m_pBtn_Notice;
	CBasicButton * m_pBtn_Send;
	CBasicButton * m_pBtn_Friend;
	CBasicButton * m_pBtn_Refuse;
	CBasicButton * m_pBtn_Close;



//-- Contents Xlass
	CLetterList *m_LetterList ;
	CNoticeList *m_NoticeList ;
	CLetterSend *m_LetterSend ;
	CLetterFriend *m_LetterFriend ;
	CLetterReject *m_LetterReject ;
	CLetterView *m_LetterView ;
	CNoticeView *m_NoticeView ;

//-- flag
	bool m_IsLetterList;
	bool m_IsNoticeList;
	bool m_IsLetterSend;
	bool m_IsLetterFriend;
	bool m_IsLetterReject;
	bool m_IsLetterView;
	bool m_IsNoticeView;

	CTextOutBox  * m_pTextOut;

	int		   m_nPageNum, m_nCurRoomNum;
	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;
	BOOL		m_IsAllView;

	//--------------------------------------------------------------------------
	// ���� �� �޽���
	BOOL LetterListMessage(int severNum,char *charName,int userIndex,int page);//���� ����Ʈ
	BOOL LetterViewMessage(int severNum,char* letterIndex,char *charName,int userIndex);//���� ��
	BOOL LetterCrimeMessage(int severNum,char* letterIndex);//�Ǽ����� �Ű�
	BOOL LetterSendMessage(int severNum,int userIndex,char *userId,char *charName,char *toChar,char *contents);//��������
	BOOL LetterFriendListMessage(int severNum,char *userId,int userIndex);//ģ������Ʈ
	BOOL LetterBlockListMessage(int severNum,char *charName);//���Űźθ���Ʈ
	BOOL LetterBagFriendMessage(int severNum,char *userId,int userIndex,char *charName,char* mode,char* friendName);//ģ���߰�
	BOOL LetterBagBlockMessage(int severNum,int userIndex,char *charName,char* mode,char* blockName);//����߰�
	BOOL LetterKeepDelMessage(int severNum,char* letterIndex,int userIndex,char *mode,char *charName);//����/�����ϱ�
	BOOL LetterEventListMessage(int pageNum);//��������Ʈ
	BOOL LetterEventViewMessage(char* noticeNo);//��������

	char*	m_strUpInfo;
	char*	m_strLineInfo;
	char*	m_strLetterMessage;
	char*	m_strFriendList;

	void PutList();
	void PutNoticeList();
	void PutLetterView();
	void PutFriendList();
	void ResetWebData();
	void PutNoticeView();
	void PutBlockList();
	void ViewBlockList(int startIndex,int lineNum);
	void ViewFriendList(int startIndex,int lineNum);
	void ViewSendFriendList(int startIndex,int lineNum);

	void CharChange(char IN *strIn, char OUT *strOut);

};

#endif // _CONTROL_LETTER_H_
