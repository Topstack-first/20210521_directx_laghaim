#ifndef _JIMEDLLEX_H_
#define _JIMEDLLEX_H_

#include "Config.h"

#ifndef _STAET_BUFFER3_S_
#define _STAET_BUFFER3_S_

typedef struct _STAET_BUFFER3
{
	int state;
	int x;
	int y;
	int len;
} STAET_BUFFER3;
#endif //_STAET_BUFFER3_S_

#define JIMEDLL_API extern "C" __declspec(dllimport)

//1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
//
JIMEDLL_API	int   JIME_Init(HWND hWnd);
JIMEDLL_API	void  JIME_Set_Pos(POINT po_ime, POINT po_edit);
JIMEDLL_API	void  JIME_Get_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//.
JIMEDLL_API	void  JIME_Set_Len_Text(int max);
JIMEDLL_API	void  JIME_Set_Text(char *text);
JIMEDLL_API	char *JIME_Get_Text();
JIMEDLL_API	void  JIME_Clear_Text();
JIMEDLL_API	WORD *JIME_Get_Font_32(char *text);
JIMEDLL_API	BYTE *JIME_Get_Font_16(char *text);
//.
JIMEDLL_API	int   JIME_Get_Pos_Caret();
JIMEDLL_API	void  JIME_Set_Pos_Caret(int caret);
JIMEDLL_API	int   JIME_Get_Pos_Copy1();
JIMEDLL_API	int   JIME_Get_Pos_Copy2();
JIMEDLL_API	char *JIME_Get_Text_Copy();

//2222222222222222222222222222222222222222222222222222222222222222222
//
JIMEDLL_API	void  JIME_Set_Color(BYTE col_0, BYTE col_1, BYTE col_2, BYTE col_3, BYTE  col_4);
//
JIMEDLL_API	int  JIME_Get_State();
//
JIMEDLL_API	BYTE *JIME_Get_Buffer1();
JIMEDLL_API	BYTE *JIME_Get_Buffer2();
JIMEDLL_API	BYTE *JIME_Get_Buffer3();
//
JIMEDLL_API	POINT JIME_Get_Pos_Edit();
JIMEDLL_API	POINT JIME_Get_Pos1();
JIMEDLL_API	POINT JIME_Get_Pos2();
//
JIMEDLL_API	int   JIME_Get_Width1();
JIMEDLL_API	int   JIME_Get_Width2();
JIMEDLL_API	int   JIME_Get_Width3();
//
JIMEDLL_API	int   JIME_Get_Width1_Put();
//
JIMEDLL_API	int   JIME_Get_Height1();
JIMEDLL_API	int   JIME_Get_Height2();
JIMEDLL_API	int   JIME_Get_Height3();

//33333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333
JIMEDLL_API	void            JIME_Set_On_Off(int state);    //1=On, 0=Off
JIMEDLL_API	int             JIME_Get_on_off();             //1=On, 0=Off
JIMEDLL_API	int             JIME_Get_State_Enter();        //1=Enter 사용가능, 0=Enter 사용 불가.
JIMEDLL_API void            JIME_Set_State_ime2(int state);//1=일, 2=영
JIMEDLL_API	STAET_BUFFER3  *JIME_Get_State_Buffer3();      //점선, 분절선, 출력 정보.

//44444444444444444444444444444444444444444444444444
JIMEDLL_API	int             JIME_Get_State_Shift();

#endif // _JIMEDLLEX_H_