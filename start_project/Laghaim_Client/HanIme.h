#ifndef _HAN_IME_H_
#define _HAN_IME_H_

#include "Config.h"


#include <imm.h>
#include <string>

#define IME_PROCESS  128
#define IME_END  129
#define IME_MODIFIED 130
#define IME_MAX_STRING	256
#define LETTER_MAX_X	50
#define LETTER_MAX_Y	9

// 전각, 반각 구분하기
#define isSJIS(a) ((BYTE)a >= 0x81 && (BYTE)a <= 0x9f || (BYTE)a >= 0xe0 && (BYTE)a<=0xfc)


class CHanIme
{

public:
	CHanIme();
	virtual ~CHanIme();

	void Clear();
	void Destroy();

	void SetMaxLen(int maxLen);
	void SetLenght(int len);
	virtual void DrawText() {};

	void AddEngChar(WPARAM wParam, LPARAM lParam);
	void DeleteChar();

	// Data Acess
	char *GetString()
	{
		return m_pszStr;
	};				// 현재의 입력라인 버퍼를 리턴
	char *GetChar()
	{
		return m_sKeyBuf;
	};				// 현재 입력중인 글짜를 리턴

	// IME 메시지 처리 함수
	void	OnImeStartComposition();
	void	OnImeEndComposition();
	void	OnImeComposition( DWORD wParam, DWORD lParam );

	// 메시지 처리 루틴
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
					 LPARAM lParam );

	void OnKeyDownKorea(UINT nChar);
	bool delCharKorea();
	INT moveCountKorea(INT iCount);
	INT getStringCountKorea(INT iStringPoint);
	INT getStringPointKorea(INT iStringCount);


	int		m_iWhatChar[LETTER_MAX_Y][LETTER_MAX_X];	//문자의 종류//한글구별 버퍼  한글초성:1,종성:2숫자,영어:3엔터:4
	int		m_iCharIndex[LETTER_MAX_Y][LETTER_MAX_X];	//문자의 위치
	char	m_LetterTable[LETTER_MAX_Y][LETTER_MAX_X];	//문자테이블
	int		m_iLetterCursor_x;		//문자테이블 커서위치
	int		m_iLetterCursor_y;

protected:
//////// 채팅에디트  ///////////////////////////////////////////
	BOOL	m_bIsComp;		//한글 조합중인가..
	BOOL	m_bIsBlock;		//블럭이 잡혔는지..

	INT				m_iCursPoint;	//커서위치
	INT				m_iBlockSP;		//블럭시작위치
	INT				m_iBlockLP;		//블럭끝위치
	INT				m_InputViewSP;	  //채팅입력창 보이는 시작위치

	char	*m_CopyIsHan;
	char	*m_CopyBuff;		//복사저장진짜버퍼
	char	*m_strInputView;		//채팅입력창버퍼
	char	*m_strIsHan;		//문자 정보버퍼
///////////////////////////////////////////////////////////////

	char	m_sKeyBuf[3];		// 한글조합중 ime내용을 담을 공간
	char	*m_pszStr;			// 실제적으로 보여줄 텍스트를 위한 공간
	int		m_nCharCount;		//
	int		m_nMaxLen;			// 입력 제한을 위한
	BOOL	m_bIsPassword;

	HWND    m_hWnd;
	HIMC	m_hIMC;

	// for taiwan
	char	*m_pInputString;
	int		m_nTotalBytes, m_nCurrentBytes;
	BOOL	m_bSkip;
	char	m_cFChar;
	// end taiwan

	// for Japanese
	INT				m_iStringPoint;		// 완료된 String 의 커서 위치
	INT				m_iFepCurPoint;		// 변환중인 String 의 커서 위치
	std::string		m_strBuff;			// 완료된 String
	std::string		m_strFep;			// 변화중인 String
	std::string		m_strStat;
	bool			m_bDraw;			// 그려야 되나 안되나...
	int				m_convertedPos[2];

	LPCANDIDATELIST m_pCandidateList;	// 변환시 표시될 폰트 리스트
	INT				m_iCandidateIndex;	// 변환중 선택되어 있는 인덱스


	bool			m_bLetterFlag;
public:
//////// 채팅에디트  ///////////////////////////////////////////
	void CursLMove(INT iState);		//커서 왼쪽 이동
	void CursRMove(INT iState);		//커서 오른쪽이동
//	void CursUMove(INT iState);		//커서 위로이동
//	void CursDMove(INT iState);		//커서 아래이동
	void ClearCopyBuff();//복사버퍼 비움
//////////////////////////////////////////////////////////////
//	void LetterTableUpdate();//문자 테이블 업데이트
//	void GetLetterCursor(int cursor);//문자 테이블 커서위치
//	void LetterTableReset();


	LONG OnIMEComposition(WPARAM dwCommand, LPARAM dwData); // WM_IME_COMPOSITION 메세지에서 처리
	BOOL GetResultString();				// 한글조합완료시
	BOOL GetResultString2();				// 일본어 조합 완료시.
	BOOL GetCompString(WPARAM dwCommand, LONG flag);	// 한글 조합중
	void OnChar(UINT nChar);			// WM_CHAR 메세지에서 처리
	void DrawCompString(LPSTR str);		// 한글조합중일경우 IME 내용을 출력용버퍼인 m_pszStr로 복사한다
	// 한글제외...
	void DrawResultString(LPSTR str);	// 한글조합완료일경우 IME 내용을 출력용버퍼인 m_pszStr로 복사한다
	//수정 한글 조합중,완료시...

	inline BOOL Enter()  				// IME
	{
		if(m_hWnd != NULL)
			return (BOOL)(m_hIMC = ImmGetContext(m_hWnd));
		else return FALSE;
	}

	inline void Leave()
	{
		ImmReleaseContext(m_hWnd, m_hIMC);
	};

	// For Japanese character
	void	AddJString();
	void	OnJChar(UINT nChar);

	// For Chinese Taiwan
	LRESULT OnTaiwanMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	BOOL	OnTaiwanBackSpace();

	// For Japanese
	bool	OnJKeyDown(CHAR cChar); // 일본용 KeyDown 메세지.(원석이 추가)
	LRESULT OnJapanMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	delJChar(); // 일본어용 Del 키 처리.

	void clearString();

	bool OnJapanChar(CHAR cChar);
	bool backChar();
	INT moveCount(INT iCount);
	INT getStringCount(INT iStringPoint);
	INT getStringPoint(INT iStringCount);

	bool OnKeyDown(CHAR cChar);
	bool delChar();

	bool OnJapanDrawText();
	bool getString(std::string& strBuff) const;
	bool getAttrPos();
	INT getConvertPos() const;
	INT getCursorCount() const;

	virtual void drawIMEString() {};
	virtual void drawCandidate() {};

	INT getCountCandidate();
	INT getCandidateIndex() const;
	INT getCandidateStart(INT iPageCount) const;
	INT getCandidateEnd(INT iPageCount) const;
	VOID setCandidatePage(INT iStartCount);
	LPCSTR getCandidateString(INT iIndex) const;
	INT getPageCount(INT iIndex);
	void getConversionStatus();

	void SetCloseIMC();
//넷마블 계정입력
	void ConvertNetId(BOOL check);
};

#endif // _HAN_IME_H_