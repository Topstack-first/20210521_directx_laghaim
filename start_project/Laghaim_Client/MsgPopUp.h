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
	CTextBox		m_InputValue1; // �ݱ� �Է¿�. ///���λ���
	CTextBox		m_InputValue2; // ���� �Է¿�. ///���λ���
	CTextBox		m_InputValue3; // ���� �Է¿�. ///���λ���
	CTextBox		m_InputValue4; // ���� �Է¿�. ///���λ���
	CTextBox		m_InputValue5; // ���� �Է¿�. ///���λ���
	CSurface*		m_pBack;		 // �޼����˾� ���ȭ��

	CBasicButton	m_Btn1;			 // "Ȯ��" ��ư
	CBasicButton	m_Btn2;			 // "���" ��ư
	CBasicButton	m_Btn3;			 // "��Ÿ���" ��ư


	CSurface *m_pSurf;				 // ù°��   ��Ʈ��
	CSurface *m_pSurf2;				 // �ι�°�� ��Ʈ��
	CSurface *m_pSurf3;				 // ����°�� ��Ʈ��	///�޼����ڽ�3��Ȯ��

	HFONT m_Font;
	DWORD m_FontColor;
	char m_Tmp1[100], m_Tmp2[100], m_Tmp3[100];	 // �Էµ� ��Ʈ���� �ΰ��� ������			// 06-08-18 3�ٱ��� Ȯ��.///�޼����ڽ�3��Ȯ��
	int  m_FontSize;

	int  m_StrLimit;
	bool m_bMultLine;
	int  m_nCurExistBtn;

	int  m_CurType;					 // ���� msgpopup ���´�
	BOOL m_bActive;					 // ����ص� ������??

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

	char*	GetCheckPW();  // �佺���� üũ�� ���� ��Ʈ���� �����
	char*	GetInputValue1();  ///���λ���
	char*	GetInputValue2();  ///���λ���
	char*	GetInputValue3();  ///���λ���
	char*	GetInputValue4();
	char*	GetInputValue5();
	void SetTextColor(DWORD color);
	void ChangeFName(CBasicButton* pBtn, char* name); // ��ư �����̸��� �����Ѵ�

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
	void* pArgs; // ���Ʈ�� ������ - ���� ����
};

#endif // _MSG_POPUP_H_