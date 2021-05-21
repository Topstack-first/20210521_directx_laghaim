#ifndef _QUEST_DATA_H_
#define _QUEST_DATA_H_

#include "Config.h"

#include "sheet.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "BaseInterface.h"

#include <string>
#include <vector>


enum QuestData_No
{
	QuestData_No00 = 0 ,
	QuestData_No01,		// 퀘스트명
	QuestData_No02,		// 요구레벨
	QuestData_No03,		//
	QuestData_No04,
	QuestData_No05,
	QuestData_No06,
	QuestData_No07,

	QuestData_EndOfEnum
};

#define TEXTSURFACE_COUNT	8

class CSheet;
class CNkCharacter;
class CTextOutBox;
class CMsgPopUp;


typedef struct
{
	int     nVnum;
	char	  *mob_name;

} MOBNAMESTRUCT;

class CQuestData : public CBaseInterface
{
protected:
	CSheet*			m_pSheet;
	CSurface*		m_pBackSur;
	CSurface*		m_pTextSur[TEXTSURFACE_COUNT];
	CBasicButton	m_GiveupBtn; // 포기
	CBasicButton	m_VerifyBtn; //X버튼

	//  [11/18/2009 ppmmjj83] 확인 버튼 추가
	CBasicButton	m_CheckBtn; //확인

	HWND			m_hWnd;
	RECT			m_ClientRc;
	HFONT			m_Font;
	float			m_fStretchX, m_fStretchY;
	int				m_FontWidth,     m_FontHeight;
	int				m_CurClientMode;
	char			m_Tmp[100];
	DWORD			m_TextColor;
	DWORD			m_TextBackColor;
	CNkCharacter	*m_pNkCha;

public:
	CQuestData();
	virtual ~CQuestData();

//////////
	HRESULT	RestoreSurfaces();
	void Init(int FontSize);
	void DeleteRes();
	void LoadRes();
	void Draw();
	void SetSheetPtr( CSheet* pSheet );
	void PutString(int idx, char* String, int x, int y);
	void SetFont (int f_w, int f_h, char *fontname);
	BOOL IsInside(int x, int y) ;
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Action(int);
	void DrawBack(int xpos, int ypos);
	int m_ScreenWidth;
	int m_ScreenHeight;

	void SetName();
	void SetNoQuest();
	void SetKillMob0(int num);
	void SetKillMob1(int num);
	void SetKillMob2(int num);
	void SetKillMob3(int num);
	void SetKillMob4(int num);
	void Reload(int index, int kill_num);

/////////

	int	 Read(const char *fileName, int q_num);
	void Clear();
	void ReadInt(int *i_data, PFILE *fp, int &int_seed);
	void ReadString(char *str, PFILE *fp, int &int_seed, char &char_seed);
	void ReadStringDup(char* &str, PFILE *fp, int &int_seed, char &char_seed);

	int m_Index;
	std::string m_Name;
	int m_Type;
	int m_Sub;
	int m_EndType;
	int m_MinLevel;
	int m_MaxLevel;
	int m_Zone;
	int m_Laim;
	int m_Exp;

	int m_Data0;
	int m_Data1;
	int m_Data2;
	int m_Data3;
	int m_Data4;
	int m_Data5;
	int m_Data6;
	int m_Data7;
	int m_Data8;
	int m_Data9;

	std::string m_String0;
	std::string m_String1;
	std::string m_String2;
	std::string m_String3;
	std::string m_String4;
	std::string m_String5;
	std::string m_Content;

	CTextOutBox**   m_pListSur;	 // List Normal Img

	CTextOutBox*    m_pBonusText;	 // 보상 x배 의 표시용 텍스트.

	CBasicButton*   m_pArrowUp;
	CBasicButton*   m_pArrowDown;
	CSurface	*   m_pBarSur;

	char ** m_sListStr;
	char    m_sPreVal[100];
	BOOL    m_bMainActive;
	BOOL    m_bScroll;
	BOOL	m_bDown;		// 마우스버튼이 클릭됬는가?

	int		m_iCurType;
	int		m_RenderXpos, m_RenderYpos;  // 백그라운드의 출력좌표
	int		m_RenderWidth, m_RenderHeight;
	int		m_iMaxListCnt;
	int     m_iCurListNum;
	int     m_iListBox_X, m_iListBox_Y;
	int     m_iListBox_Cx, m_iListBox_Cy;

	int     m_iStartViewCnt;
	int     m_iEndViewCnt;
	int     m_iViewCnt;

	enum	STATE	{ NONE, SCROLLBAR_POLL, SCROLLBAR_FLAT};

	STATE	m_eState;

	void Init(int MaxListCnt, int ViewCnt);
	void SetMaxListCnt(int cnt);
	void CreListSurface();
	void Show_Title();
	void Show_List();
	void Show_ArrowBtn();
	void SetBarPosToGo (int y);
	void CalculateBarPos();
	void GoHome();
	void GoEnd();
	void List_UpScroll();
	void List_DownScroll();
	BOOL  ReadInfoFromTxt(char *filename);
	int  GetFileLineCnt(char* fname);
	BOOL IsInside_BackGround(int x, int y);
	BOOL IsInside_Text(int index, int x, int y);
	int IsInside_ScrollArea(int x, int y);
	void PreSetValForInBox(int Str, int Int, int Dex, int Con, int Cha);
	BOOL SetAddStr(char* str);
	BOOL QuestMessage();
	void CloseRes();
	void AddStringMulti(char *string, int len);
	BOOL m_bReload;

	void	ReadMobNameFile(char *file_name);//0402

	char MobName1[30];
	char MobName2[30];
	char MobName3[30];
	char MobName4[30];
	char MobName5[30];
	int m_nNpcNameNum;
	BOOL m_bNoQuest;
	int m_nMobNameLength[5];

	CSurface *m_pAdventure;
	CSurface *m_pIsumiron;

	int GetScrollSize();
	void PopUpMsg(int msg);

	void LoadDLLStringTable();
	void DeleteDLLStringTable();
//==========================================================================================
	// nate 2004 - 4 : quest
	int m_nQuestTotalPoint;
	CSurface* m_pToy;
	void SetAddPoint( int nAddPoint );
	void SetTotalPoint( int nTotalPoint );
	void SetRemainPoint( int nRemainPoint );
	void SetNextQuest();
//==========================================================================================
	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
};

#endif // _QUEST_DATA_H_
