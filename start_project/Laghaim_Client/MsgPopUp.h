#ifndef _MSG_POPUP_H_
#define _MSG_POPUP_H_

#include "Config.h"

#include "BasicButton.h"
#include "TextBox.h"
#include "CheckBox.h"
#include "LHUI_TakeBox.h"

class CSurface;
class CTextBox;
class CheckBox;

class CaruResultData
{
public:
	CaruResultData();
	~CaruResultData();

public:
	void SetListCount(const int count);
	void SetIndex(const int num, const std::string& index);
	void SetItemName(const int num, const std::string& itemName);
	void SetItemCount(const int num, const std::string& itemCount);

	void SetFont(int f_w, int f_h, char* fontname);
	void SetTextColor(DWORD color);
	void SetPos(int x, int y);

	void Draw();
	void Restore();

private:
	int m_listCount;
	DWORD m_FontColor;
	HFONT m_Font;
	int m_posX;
	int m_posY;

	CSurface* m_pSurfIndex[MAX_TAKEBOX];
	CSurface* m_pSurfItemName[MAX_TAKEBOX];
	CSurface* m_pSurfItemCount[MAX_TAKEBOX];
	std::string m_Index[MAX_TAKEBOX];
	std::string m_ItemName[MAX_TAKEBOX];
	std::string m_ItemCount[MAX_TAKEBOX];
};

class CMsgPopUp
{

public:

	HWND	  m_hWnd;
	RECT	  m_ClientRc;

	CTextBox		m_InputLine;
	CTextBox		m_InputValue1; // 금괴 입력용. ///개인상점
	CTextBox		m_InputValue2; // 은괴 입력용. ///개인상점
	CTextBox		m_InputValue3; // 동괴 입력용. ///개인상점
	CTextBox		m_InputValue4; // 은괴 입력용. ///개인상점
	CTextBox		m_InputValue5; // 동괴 입력용. ///개인상점
	CSurface*		m_pBack;		 // 메세지팝업 배경화면

	CBasicButton	m_Btn1;			 // "확인" 버튼
	CBasicButton	m_Btn2;			 // "취소" 버튼
	CBasicButton	m_Btn3;			 // "기타등등" 버튼


	CSurface *m_pSurf;				 // 첫째줄   스트링
	CSurface *m_pSurf2;				 // 두번째줄 스트링
	CSurface *m_pSurf3;				 // 세번째줄 스트링	///메세지박스3줄확장

	HFONT m_Font;
	DWORD m_FontColor;
	char m_Tmp1[100], m_Tmp2[100], m_Tmp3[100];	 // 입력된 스트링을 두개로 나눈다			// 06-08-18 3줄까지 확장.///메세지박스3줄확장
	int  m_FontSize;

	int  m_StrLimit;
	bool m_bMultLine;
	int  m_nCurExistBtn;

	int  m_CurType;					 // 현재 msgpopup 형태는
	BOOL m_bActive;					 // 출력해도 좋은가??

	CheckBox m_chkBox;
	CaruResultData* m_pCaruResultData;

public:
	CMsgPopUp();
	virtual ~CMsgPopUp();

	void Init(int type);
	void TextOutInit();
	void LoadRes();
	void DeleteRes();
	void Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Action( int i );
	BOOL CheckMsgExist();
	void PutString(char* String);
	void ClearSurface(CSurface* surface);
	void DrawText();
	void SetFont (int f_w, int f_h, char *fontname);
	void Text_SetString(char *string);
	void RestoreAll();

	char*	GetCheckPW();  // 페스워드 체크를 위한 스트링을 얻느다
	char*	GetInputValue1();  ///개인상점
	char*	GetInputValue2();  ///개인상점
	char*	GetInputValue3();  ///개인상점
	char*	GetInputValue4();
	char*	GetInputValue5();
	void SetTextColor(DWORD color);
	void ChangeFName(CBasicButton* pBtn, char* name); // 버튼 파일이름을 변경한다

	void SetCheckBoxText(const std::string& text);
	bool IsCheckBoxChecked();
};

struct POPUP
{
	POPUP() : pArgs(NULL), pNext(NULL), nProcess(0)
	{
	}
	CMsgPopUp pMsg_Popup;
	int nProcess;
	POPUP* pNext;
	void* pArgs; // 드미트론 점령전 - 인자 전달
};

#endif // _MSG_POPUP_H_