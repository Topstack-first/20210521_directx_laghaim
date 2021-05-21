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

// ����, �ݰ� �����ϱ�
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
	};				// ������ �Է¶��� ���۸� ����
	char *GetChar()
	{
		return m_sKeyBuf;
	};				// ���� �Է����� ��¥�� ����

	// IME �޽��� ó�� �Լ�
	void	OnImeStartComposition();
	void	OnImeEndComposition();
	void	OnImeComposition( DWORD wParam, DWORD lParam );

	// �޽��� ó�� ��ƾ
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
					 LPARAM lParam );

	void OnKeyDownKorea(UINT nChar);
	bool delCharKorea();
	INT moveCountKorea(INT iCount);
	INT getStringCountKorea(INT iStringPoint);
	INT getStringPointKorea(INT iStringCount);


	int		m_iWhatChar[LETTER_MAX_Y][LETTER_MAX_X];	//������ ����//�ѱ۱��� ����  �ѱ��ʼ�:1,����:2����,����:3����:4
	int		m_iCharIndex[LETTER_MAX_Y][LETTER_MAX_X];	//������ ��ġ
	char	m_LetterTable[LETTER_MAX_Y][LETTER_MAX_X];	//�������̺�
	int		m_iLetterCursor_x;		//�������̺� Ŀ����ġ
	int		m_iLetterCursor_y;

protected:
//////// ä�ÿ���Ʈ  ///////////////////////////////////////////
	BOOL	m_bIsComp;		//�ѱ� �������ΰ�..
	BOOL	m_bIsBlock;		//���� ��������..

	INT				m_iCursPoint;	//Ŀ����ġ
	INT				m_iBlockSP;		//��������ġ
	INT				m_iBlockLP;		//������ġ
	INT				m_InputViewSP;	  //ä���Է�â ���̴� ������ġ

	char	*m_CopyIsHan;
	char	*m_CopyBuff;		//����������¥����
	char	*m_strInputView;		//ä���Է�â����
	char	*m_strIsHan;		//���� ��������
///////////////////////////////////////////////////////////////

	char	m_sKeyBuf[3];		// �ѱ������� ime������ ���� ����
	char	*m_pszStr;			// ���������� ������ �ؽ�Ʈ�� ���� ����
	int		m_nCharCount;		//
	int		m_nMaxLen;			// �Է� ������ ����
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
	INT				m_iStringPoint;		// �Ϸ�� String �� Ŀ�� ��ġ
	INT				m_iFepCurPoint;		// ��ȯ���� String �� Ŀ�� ��ġ
	std::string		m_strBuff;			// �Ϸ�� String
	std::string		m_strFep;			// ��ȭ���� String
	std::string		m_strStat;
	bool			m_bDraw;			// �׷��� �ǳ� �ȵǳ�...
	int				m_convertedPos[2];

	LPCANDIDATELIST m_pCandidateList;	// ��ȯ�� ǥ�õ� ��Ʈ ����Ʈ
	INT				m_iCandidateIndex;	// ��ȯ�� ���õǾ� �ִ� �ε���


	bool			m_bLetterFlag;
public:
//////// ä�ÿ���Ʈ  ///////////////////////////////////////////
	void CursLMove(INT iState);		//Ŀ�� ���� �̵�
	void CursRMove(INT iState);		//Ŀ�� �������̵�
//	void CursUMove(INT iState);		//Ŀ�� �����̵�
//	void CursDMove(INT iState);		//Ŀ�� �Ʒ��̵�
	void ClearCopyBuff();//������� ���
//////////////////////////////////////////////////////////////
//	void LetterTableUpdate();//���� ���̺� ������Ʈ
//	void GetLetterCursor(int cursor);//���� ���̺� Ŀ����ġ
//	void LetterTableReset();


	LONG OnIMEComposition(WPARAM dwCommand, LPARAM dwData); // WM_IME_COMPOSITION �޼������� ó��
	BOOL GetResultString();				// �ѱ����տϷ��
	BOOL GetResultString2();				// �Ϻ��� ���� �Ϸ��.
	BOOL GetCompString(WPARAM dwCommand, LONG flag);	// �ѱ� ������
	void OnChar(UINT nChar);			// WM_CHAR �޼������� ó��
	void DrawCompString(LPSTR str);		// �ѱ��������ϰ�� IME ������ ��¿������ m_pszStr�� �����Ѵ�
	// �ѱ�����...
	void DrawResultString(LPSTR str);	// �ѱ����տϷ��ϰ�� IME ������ ��¿������ m_pszStr�� �����Ѵ�
	//���� �ѱ� ������,�Ϸ��...

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
	bool	OnJKeyDown(CHAR cChar); // �Ϻ��� KeyDown �޼���.(������ �߰�)
	LRESULT OnJapanMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	delJChar(); // �Ϻ���� Del Ű ó��.

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
//�ݸ��� �����Է�
	void ConvertNetId(BOOL check);
};

#endif // _HAN_IME_H_