#ifndef _AUTO_MOUSE_H_
#define _AUTO_MOUSE_H_

#include "Config.h"


#include "ddutil.h"

class CNk2DFrame;
class CSurface;
class CBasicButton;
class CTextBox;
class CTextOutBox;
class CTcpIpConnection;
class CAlphaPopUp;
class CMultiText;

#define NUM_OF_CHECKNUMBER 10 // 클라이언트 체크 넘버의 갯수. ///숫자입력시스템보완
#define NUM_OF_CONFIRMNUMBER 10 // 클라이언트에서 오토마우스 체크 성공시 서버로 보내는 인증넘버의 갯수. ///숫자입력시스템보완

#define AUTOMOUSE_TYPE_NUMBER 0 // 문자 입력 시스템
#define AUTOMOUSE_TYPE_QnA 1    // 질답 확인 시스템
#define AUTOMOUSE_TYPE_QUIZ 2    // 퀴즈 퀴즈 이벤트 시스템

// nate 2004 - 3
// 숫자 확인 시스템
class C3d_PosPoint;

class CAutoMouse
{
public:
	void SetTextInFocus();
	void AddWord(char *w1,char *w2,int flag, BOOL bAct);
	CAutoMouse();
	virtual ~CAutoMouse();

	//==================================================================
	// nate 2004 - 3
	// 숫자 확인 시스템
	C3d_PosPoint		*m_pBoard;
	CSurface	 		*m_pSurfaceNum;
	CSurface	 		*m_pSurfaceBack;
	CSurface	 		*m_pQuestionmark;
	CSurface	 		*m_pQuestionmark2;
	CTextBox	 		*m_pTextIn; // 문자 확인 박스
	CTextOutBox  		*m_pMunjaText; // 서버에서 온 문자 출력 박스
	CBasicButton 		*m_pBtn1;//확인버튼
	CAlphaPopUp	 		*m_pHelpInfo; // 도움말
	CBasicButton 		*m_pCloseBtn; // 퀴즈용 닫기 버튼 추가.


	BYTE				m_byCheckCount;

	int					m_nNum[ 4 ];
	float				m_fRandom[ 4 ];
	void SetRandomNum();
	//==================================================================

	////////////////////////////////////////////////////////////////////////////////////////
	// 수정자 : 이원석
	// 수정일 : 05-04-07
	// 수정내용 : 숫자입력시스템 보완 처리. ///숫자입력시스템보완 // UpdateLog 171번 참조.
	int m_nDecodeBit; // 체크넘버의 암호화 비트 게임 실행시 랜덤하게 구해진다.
	int m_nRealCheckNumber[NUM_OF_CHECKNUMBER]; // 게임 실행시 랜덤하게 구해지는 4자리 체크 넘버.( 위 m_nDecodeBit값으로 부터 암호화 되어 저장된다. )
	void MakeCheckNumber(); // 게임 시작시 실행. 체크넘버와 암호화 비트를 랜덤값으로 구해준다.


	BOOL m_bOneFailed;
	BYTE m_nConfirmNumber[NUM_OF_CONFIRMNUMBER]; // 인증번호. 5개는 서버로 부터...5개는 앞 5개 넘버와 시간값으로부터 클라이언트에서 구한다.(구하는 타이밍은 인증코드를 생성하는 타이밍에...)
	void SetConfirmNumber( int Arg1, int Arg2 ); // 서버로부터 받은 번호로부터 인증번호 5개를 산출한다.
	void SendConfirm( char *InputStr ); // 인증번호를 계산해서 보낸다.
	////////////////////////////////////////////////////////////////////////////////////////



	char m_dic_word[10];
	char m_strhelpinfo[100];

	bool m_bOnHelp;
	BOOL m_bCurAct;
	bool m_bActive;

	float m_ftime;
	float m_felapsedtime;


	int m_CurTextView;// 챗팅창 위치,크기 flag

	int m_Type; // 문자입력인지 QnA 인지.

	void Init();
	BOOL IsInside(int x, int y);
	void LoadRes(int Type);

	void DeleteRes();
	void Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT QnAMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};


#endif // _AUTO_MOUSE_H_
