#pragma once
#include "LHUI_Base.h"
#include "TextBox.h"

class CBasicButton;
class CBasicStatic;
class CMutiListBox;
class CTextOutBox;

typedef struct _LHUIBoxSet
{
	bool initialized;
	int db_index;
	int item_vnum;
	int item_count;
	char get_date[32];

} LHUIBoxSet , * PLHUIBoxSet;

#define MAX_TAKEBOX	10


enum TakeBox_Type
{
	TakeBox_Type__Character,
	TakeBox_Type__User,
	TakeBox_Type__ServerMove,

	TakeBox_Type__EndOfEnum
};

enum TakeBox_State
{
	TakeBox_State__None = -1,
	TakeBox_State__Send = 0,
	TakeBox_State__Recv = 1,
	TakeBox_State__Finish = 2,
	TakeBox_State__Fail = 3 ,

	TakeBox_State__EndOfEnum
};

class LHUI_TakeBox : public LHUI_Base
{
public:
	TakeBox_Type m_takebox_type;
	TakeBox_State m_takebox_state;

	bool m_send_message;
	bool m_recv_message;
	bool m_recived_data;			// 메세지 보냈다...다받을때 까지...즐!!!

	int m_num_item;					// 현재 패이지에서 아이템 개수 요놈을 가지고 페이지를 계산하자!!!
	int m_max_page;
	int m_current_page;
	int m_max_item;
	int m_max_item_prev;

	int m_selected_item;	// listbox에서 그냥 얻어오자!!

	DWORD m_send_delay;
	std::string m_strSearch;

public:
	CBasicStatic * m_background;
	CBasicStatic * m_ImgPage;
	CBasicStatic * m_ImgSearch;
	CBasicButton * m_btn_close;
	CBasicButton * m_btn_get;
	CBasicButton * m_btn_getall;

	CBasicButton * m_btn_first;
	CBasicButton * m_btn_prev;
	CBasicButton * m_btn_next;
	CBasicButton * m_btn_end;
	CBasicButton * m_btn_search;

	CTextOutBox *  m_textout_page;

	CMutiListBox * m_lst_item;

	LHUIBoxSet m_box_list[MAX_TAKEBOX];

	CTextBox		m_InputSearch;


public:
	void ResetAll();

	bool Request_List();
	bool Request_List_Search();
	bool Request_List(int page);
	bool Request_Item(int index);
	bool Request_ItemAll();

	void SetTakebox(int num_page, int page, int num_item);
	void SetTakebox(TakeBox_Type type, int num_page, int page, int num_item);
	void InsertItem(int db_index, int vnum, int count, char* date);
	void EditItem(int db_index, int count);

	void InsertItem_Test(int db_index, int vnum, int count, char* date);
	void SetRecived(int finished);

	void Refresh();
	void ClearList();

	void SetTakeboxType(TakeBox_Type type)
	{
		m_takebox_type = type;
	}

	BOOL IsFocus()
	{
		return m_InputSearch.GetEnable();
	}

public:
	virtual void Init();
	virtual void LoadRes();
	virtual void DeleteRes();
	virtual void Draw();

	virtual LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


public:
	LHUI_TakeBox(void);
	LHUI_TakeBox(TakeBox_Type type);
	virtual ~LHUI_TakeBox(void);
};
