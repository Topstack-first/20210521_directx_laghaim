#ifndef _LMULTI_EDIT_BOX_H_
#define _LMULTI_EDIT_BOX_H_

#include "Config.h"



#ifndef MEB
#define MEB 1

#define isSJIS(a) ((BYTE)a >= 0x81 && (BYTE)a <= 0x9f || (BYTE)a >= 0xe0 && (BYTE)a<=0xfc)

#include <assert.h>
#include "D3DApp.h"
#include "D3DFrame.h"
#include "D3DEnum.h"
#include "D3DUtil.h"
#include "ddutil.h"

class L_MultiEditBox
{
public:

	CSurface *m_pSurf;
	CSurface *m_pPromptSurf;

	HFONT m_Font;
	L_MultiEditBox();
	virtual ~L_MultiEditBox();


	//  [1/15/2009 ppmmjj83] Candidate
	CSurface		*m_pBack_CandidateSurf;
	CSurface		*m_pCandidateSurf;
	LPCANDIDATELIST pCandidateList ;
	char *m_SpaceStr;
	HFONT m_CandyFont;
	int				m_nX;
	int				m_nY;

	CHAR m_cBuff[9][512];
	INT m_iCandidateIndex;
	INT	m_iCandidateStart;
	INT	m_iCandidateEnd;
	INT	m_heightCandidate;


	INT getPageCount(INT iIndex) ;
	INT getCandidateStart(INT iPageCount) const;
	INT getCandidateEnd(INT iPageCount) const;
	LPCSTR getCandidateString(INT iIndex) const;
	VOID setCandidatePage(INT iStartCount);

	LRESULT OnJapanMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void OnJKeyDown(CHAR cChar, UINT uMsg) ;
	void OnJapanChar(CHAR cChar) ;
	bool OnJapanDrawText(void);
	void DrawCandidate(void);
	bool getFontSize(LPSIZE pSize,HFONT hFont,LPCTSTR pText,INT iLength);

	HIMC	m_hIMC;
	bool	m_bEenter;

	//-- Focus
	virtual void FocusOn()
	{
		mb_bool_Focus = true;
	};
	virtual void FocusOff()
	{
		mb_bool_Focus = false;
	};

	//-- Init ?? Init!! ???? ?????????Core Size ?? ???????.
	virtual void Init(const int a_core_str_size ,CSurface *a_pSurf,
					  CSurface *a_pprom, int nX=0, int nY=0, const bool a_readonly = false);

	virtual void Reset();
	//-- D3DX ???? ???????? ?ç¿??
	//virtual void BindD3dDevice( );

	//-- CoreString ?? Set. Coresize ?? ???? ???? ????????? ??????.
	virtual void Set_CoreString(const char * const a_string);
	//-- Get Crestring
	virtual char *Get_CoreString()
	{
		return mpc_CoreString;
	};

	//-- Set Row, Column
	virtual void Set_RowColm(const int a_row, const int a_column );




	//-- Draw Multi Text
	virtual void DrawMultiText();

	void Gen_cursor_table();

	//-- Message Procedure
	virtual LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam,
							LPARAM lParam );

	//-- Insert Character
	virtual void InsertChar(const char a_inchar);

	//-- Insert Wide Character
	virtual void InsertWChar(const char *a_inWchar);

	virtual void InsertIMEChar(const char *a_inWchar,const bool a_On_comp);



	//-- Start_row ???
	virtual void Inc_Start_row()
	{
		Set_Start_row( mi_START_row +1);
	};

	//-- Start_row ????
	virtual void Dec_Start_row()
	{
		Set_Start_row( mi_START_row -1);
	};

	//-- ????? ???? ????? ???? ???????.
	virtual void Set_Start_row(const int a_start_row);

	//-- ???Display ?? ?????? ?????? Line ???
	virtual int Get_Start_row() const
	{
		return mi_START_row;
	};


protected:
	//-- Move Cursor
	//-- a_direction - 0 : left / 1:right / 2: up / 3: down / 4: home / 5: end
	virtual bool _MoveCursor(const int a_direction, const bool a_ctrl);

	//-- Delete Character
	virtual void _OnKEY_Deletechar();

	//-- Delete Character
	virtual void _OnKEY_Backspace();

	//-- Page Up
	virtual void _OnKEY_PgUp();

	//-- Page Down
	virtual void _OnKEY_PgDown();



	//-- RegenCursor //-- mi_Core_cursor_idx ?? ???? ???? ?????? ??????.
	virtual void _Inverse_Gen_cursor();

	//-- Page up / down
	//-- a_bool_page_up - true :up / flase : down
	//virtual void PageUpDown(const bool a_bool_page_up);




protected:

	//-- ??????????Insert ????? ???.
	bool l_On_comp ;

	//-- Read Only flag : dafault : flase
	bool mb_bool_ReadOnly; //-- true  :  Read Only
	//-- false :  Writable
	bool mb_bool_Focus;    //-- true : ???? ????? ???? ????.
	//-- false : ???? / ???? ????

	//-- ???? ???????
	bool mb_show_cursor;  //-- ?????? ????? ????
	DWORD mi_lasttime;    //-- ???? ??? ????
	DWORD mi_PROMPTTIME;    //-- ????? ????.

	//-- ???? ???????
	int mi_cursor_x; //-- Cursor x
	int mi_cursor_y; //-- Cursor y
	int mi_Core_cursor_idx; //-- ????Core String ?????? cursor ????
	int mi_disp_line_num;   //-- ???????? ?? ????Line ,  Set_start_row ???? ?ç¿?

	//-- Display row / column ???? ???
	int mi_MAX_column;      //-- X Size of MultiEdit Box
	int mi_MAX_row;         //-- Y Size of MultiEdit Box
	int mi_START_row;       //-- ?ðµ?row ?? ????? ????? ??????? row ?? ??? max_row ???? ???


	//-- Core ???
	int mi_Core_Size;       //-- Core String ?? Size

	//-- string ?? ??? ???? ??? buffer ?? ???????????????.
	//-- ??????????? ????????????? ?????? ????? ?????
	char *mpc_CoreString;   //-- Core String
	char *mpc_OutputString; //-- Output String : ????? ?????? ?ç¿?
	char *mpc_JohabString;
	int *mpi_Cursor_row_table;   //-- ?????? row ?????? ??????.
	int *mpi_Cursor_column_table;//-- ?????? Column ?????? ??????.


	HFONT oFont;
	HDC m_hdc;
};

#endif

#endif // _L_MULTI_EDIT_BOX_H_