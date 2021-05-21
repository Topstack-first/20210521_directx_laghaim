#include "stdafx.h"
#include "L_MultiEditBox.h"
#include "LoadEffectSound.h"
#include "headers.h"

#define MEB_font_height 14
#define GAP_font_height 16
#define MEB_font_width  7
#define MEB_START_X 2
#define MEB_START_Y 2


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

L_MultiEditBox::L_MultiEditBox()
	: m_Font(NULL)
	, m_nX(0)
	, m_nY(0)
	, oFont(NULL)
	, m_hdc(NULL)
{
	memset(m_cBuff, 0, sizeof(m_cBuff));

	l_On_comp  =false;
	mb_bool_ReadOnly = false;
	mb_bool_Focus =false;
	mi_Core_cursor_idx =0; //-- must be
	mi_Core_Size = 0; //-- must be
	mi_cursor_x = 0; //-- default
	mi_cursor_y = 0; //-- default
	mi_disp_line_num = 0; //-- default
	mi_MAX_column = 10; //-- dafault
	mi_MAX_row = 5; //-- dafault
	mi_START_row = 0; //-- must be
	mpc_CoreString = NULL; //-- default
	mpc_OutputString = NULL; //-- default
	mpc_JohabString = NULL;
	mpi_Cursor_row_table = NULL;   //-- ?????? row ?????? ??????.
	mpi_Cursor_column_table = NULL;//-- ?????? Column ?????? ??????.
	mi_lasttime = timeGetTime();    //-- must be
	mb_show_cursor = false;      //-- default
	mi_PROMPTTIME = 400;            //-- ????? ????.
	m_pSurf = NULL;
	m_pPromptSurf = NULL;
	m_bEenter = false;

	m_pBack_CandidateSurf = NULL;
	m_pCandidateSurf = NULL;
	pCandidateList = NULL;
	m_CandyFont = NULL;
	m_SpaceStr = NULL;

	m_iCandidateIndex = 0;
	m_iCandidateStart = 0;
	m_iCandidateEnd = 0;
	m_heightCandidate = 0;
}
//----------------------------------------------------------------------------
//-- Reset
//----------------------------------------------------------------------------
void L_MultiEditBox::Reset()
{
	l_On_comp  =false;
	//-- String ?????
	ZeroMemory((void*)mpc_CoreString, mi_Core_Size*sizeof(char));
	ZeroMemory((void*)mpc_OutputString, mi_Core_Size*sizeof(char));
	ZeroMemory((void*)mpc_JohabString, mi_Core_Size*sizeof(char));
	mb_bool_Focus =false;
	mi_Core_cursor_idx =0; //-- must be
	mi_cursor_x = 0; //-- default
	mi_cursor_y = 0; //-- default
	mi_START_row = 0; //-- must be
}
//----------------------------------------------------------------------------
//-- Destructor
//----------------------------------------------------------------------------
L_MultiEditBox::~L_MultiEditBox()
{
	m_pSurf = NULL;
	m_pPromptSurf = NULL;

	SAFE_DELETE_ARRAY(mpc_CoreString);
	SAFE_DELETE_ARRAY(mpc_OutputString);
	SAFE_DELETE_ARRAY(mpc_JohabString);
	SAFE_DELETE_ARRAY(mpi_Cursor_row_table);
	SAFE_DELETE_ARRAY(mpi_Cursor_column_table);
	SAFE_DELETE_ARRAY(m_SpaceStr);
	SAFE_DELETE(m_pBack_CandidateSurf);
	SAFE_DELETE(m_pCandidateSurf);

	DeleteObject(m_CandyFont);
}

//----------------------------------------------------------------------------
//-- Set Core String
//----------------------------------------------------------------------------
//-- ???? Size ?? 3????.
//-- Core String Setting
void L_MultiEditBox::Init(const int a_core_str_size ,CSurface *a_pSurf, CSurface *a_pprom,int nX, int nY,const bool a_readonly)
{
	m_pSurf = a_pSurf;
	m_pPromptSurf = a_pprom;

	mb_bool_ReadOnly = a_readonly;
	//-- ???? Size?? 3???? ???
	if(a_core_str_size <3 )mi_Core_Size = 3;
	else mi_Core_Size = a_core_str_size;

	mpc_CoreString = new char[mi_Core_Size];
	mpc_OutputString = new char[mi_Core_Size];
	mpc_JohabString = new char[mi_Core_Size];
	mpi_Cursor_column_table = new int[mi_Core_Size];
	mpi_Cursor_row_table =  new int[mi_Core_Size];

	//-- String ?????
	ZeroMemory((void*)mpc_CoreString, mi_Core_Size*sizeof(char));
	ZeroMemory((void*)mpc_OutputString, mi_Core_Size*sizeof(char));

	m_Font = G_GetFont(MEB_font_height,MEB_font_width,"??????");
	//m_CandyFont = G_GetFont(MEB_font_height,MEB_font_width,"??????");

	Gen_cursor_table();

	HDC hDC = GetDC(NULL);
	int ptHeight = -MulDiv(9, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	//int ptWidth = -MulDiv(fontSize, GetDeviceCaps(hDC, LOGPIXELSX), 72);
	int ptHeight_candy = MulDiv(9, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);

	int CandyWidth = (ptHeight_candy-1)/2;
	int CandyHeight = ptHeight_candy;

	g_pDisplay->CreateSurface(&m_pCandidateSurf, CandyWidth*20, CandyHeight);
	m_pCandidateSurf->SetColorKey(RGB(0, 0, 0));
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack_CandidateSurf, "interface/common/Back_Candy.bmp");
	m_pBack_CandidateSurf->SetColorKey(RGB(0, 0, 0));

	m_SpaceStr = new char[500+1];
	for (int i = 0; i < 500; i++)
	{
		m_SpaceStr[i] = ' ';
	}
	m_SpaceStr[500] = '\0';

	m_nX = nX;
	m_nY = nY;
}

//----------------------------------------------------------------------------
//-- Set Core String
//----------------------------------------------------------------------------
//-- Core String Setting
void L_MultiEditBox::Set_CoreString(const char * const a_string)
{
	int idx = 0;
	int a_idx = 0;
	int len = strlen(a_string);
	ZeroMemory((void*)mpc_CoreString, mi_Core_Size*sizeof(char));

	do
	{
		if(a_string[a_idx]!='\r')
		{
			mpc_CoreString[idx] = a_string[a_idx];
			idx++;
		}
		else
		{
			idx = idx-1;
			idx = idx+1;
		}
		a_idx++;
	}
	while(idx<mi_Core_Size && a_idx<len );
	//-- String ?????

	Gen_cursor_table();

}
//----------------------------------------------------------------------------
//-- Set Row , Column
//----------------------------------------------------------------------------
//-- Display ?? Culling ?? ListBox ????? ???
void L_MultiEditBox::Set_RowColm(const int a_row, const int a_column )
{
	mi_MAX_column = a_column;
	mi_MAX_row = a_row;
	Gen_cursor_table();
}

//----------------------------------------------------------------------------
//-- Set Start Row
//----------------------------------------------------------------------------
//-- Scroll ??????????
//-- true: Argument ?? ???ûÛ? ?????? ????????
//-- false : ??????????
void L_MultiEditBox::Set_Start_row(const int a_start_row)
{
	mi_START_row = a_start_row;
	//-- ??????

	//-- ?????????? ???é»?? Line ???? ??????
	if(mi_START_row + mi_MAX_row > mi_disp_line_num)
		mi_START_row = mi_disp_line_num - mi_MAX_row;

	//-- 0???? ??????
	if(mi_START_row<0)mi_START_row = 0;
	Gen_cursor_table();
}


//----------------------------------------------------------------------------
//-- _OnKEY_PgUp
//----------------------------------------------------------------------------
//--
void L_MultiEditBox::_OnKEY_PgUp()
{
	int i;
	for(i=0; i<mi_MAX_row - 2; i++)
	{
		Dec_Start_row();
		//-- Writable ??????????? ????
		if(!mb_bool_ReadOnly)
			_MoveCursor(2,false); //-- Up
	}
}

//----------------------------------------------------------------------------
//-- _OnKEY_PgDown
//----------------------------------------------------------------------------
//--
void L_MultiEditBox::_OnKEY_PgDown()
{
	int i;
	for(i=0; i<mi_MAX_row - 2; i++)
	{
		Inc_Start_row();

		//-- Writable ??????????? ????
		if(!mb_bool_ReadOnly)
			_MoveCursor(3,false); //-- Down
	}
}

//----------------------------------------------------------------------------
//-- Back Space
//----------------------------------------------------------------------------
//-- ?????? Delete char ?? ??àÇ???.
void L_MultiEditBox::_OnKEY_Backspace()
{
	//-- ??????????????? ?????
	if(mi_Core_cursor_idx<1)
		return;

	//-- 0 ?????? ??????????virtual ???? ?????? ?????????
	//-- ?????????? LF ???????? ????
	if(mi_cursor_x==0 && mpc_CoreString[mi_Core_cursor_idx-1]!=0x0a)
		_MoveCursor(0,false);

	_MoveCursor(0,false);
	_OnKEY_Deletechar();
	_Inverse_Gen_cursor();
}
//----------------------------------------------------------------------------
//-- Delete char
//----------------------------------------------------------------------------
void L_MultiEditBox::_OnKEY_Deletechar()
{
	char* lpc_buffer;
	bool han_flag = false;

	lpc_buffer = new char [mi_Core_Size];
	//-- ????????????aif (g_dwLangType == 1)
	if((mpc_CoreString[mi_Core_cursor_idx] & 0x80) && g_dwLangType != 1)
		han_flag = true;

	//  [12/4/2008 ppmmjj83] ???? ?????IME ????
	else if( g_dwLangType == 1 && ((unsigned char)mpc_CoreString[mi_Core_cursor_idx] >= 0x81 //129
								   && (unsigned char)mpc_CoreString[mi_Core_cursor_idx] < 0xa0) //160
			 || ((unsigned char)mpc_CoreString[mi_Core_cursor_idx] >= 0xe0 //224
				 && (unsigned char)mpc_CoreString[mi_Core_cursor_idx] < 0xeb))//235)
		han_flag = true;

	//-- ?èÁ??? ?????? ?????? ????????????^^
	if(han_flag)
	{
		memcpy((void*)lpc_buffer,(void*)&(mpc_CoreString[mi_Core_cursor_idx+1]),
			   mi_Core_Size - mi_Core_cursor_idx -1);

		memcpy((void*)&(mpc_CoreString[mi_Core_cursor_idx]),(void*)lpc_buffer,
			   mi_Core_Size - mi_Core_cursor_idx -1);

		mpc_CoreString[mi_Core_Size-1] = 0;
	}

	memcpy((void*)lpc_buffer,(void*)&(mpc_CoreString[mi_Core_cursor_idx+1]),
		   mi_Core_Size - mi_Core_cursor_idx -1);

	memcpy((void*)&(mpc_CoreString[mi_Core_cursor_idx]),(void*)lpc_buffer,
		   mi_Core_Size - mi_Core_cursor_idx -1);

	mpc_CoreString[mi_Core_Size-1] = 0;

	Gen_cursor_table();
	//-- safe delete
	SAFE_DELETE_ARRAY(lpc_buffer);
}

//----------------------------------------------------------------------------
//-- Insert Wide char
//----------------------------------------------------------------------------
void L_MultiEditBox::InsertWChar(const char *a_inWchar)
{
	//-- Focus ?????return;
	if(!mb_bool_Focus)
		return;

	char *lpc_buffer;

	//-- NULL ???????? ?? ?? ???????? ????? ???? ????.(2char)
	if(mpc_CoreString[mi_Core_Size-2] || mpc_CoreString[mi_Core_Size-3])
		return;

	lpc_buffer = new char [mi_Core_Size];

	//-- TEMP
	memcpy((void*)lpc_buffer,(void*)&(mpc_CoreString[mi_Core_cursor_idx]),
		   mi_Core_Size - mi_Core_cursor_idx );

	mpc_CoreString[mi_Core_cursor_idx] = a_inWchar[0];
	mpc_CoreString[mi_Core_cursor_idx+1] = a_inWchar[1];

	memcpy((void*)&(mpc_CoreString[mi_Core_cursor_idx+2]),(void*)lpc_buffer,
		   mi_Core_Size - mi_Core_cursor_idx -2);


	//-- ???? ?????????? ???? ???
	if(mi_Core_Size-2 > mi_Core_cursor_idx ) mi_Core_cursor_idx+=2;
	Gen_cursor_table();
	_Inverse_Gen_cursor();

	//-- safe delete
	SAFE_DELETE_ARRAY(lpc_buffer);
}
//----------------------------------------------------------------------------
//-- Insert IME char
//----------------------------------------------------------------------------
//-- a_On_comp - true : ?????? / flase : ????????
//--
void L_MultiEditBox::InsertIMEChar(const char *a_inWchar,const bool a_On_comp)
{
	//-- Focus ?????return;
	if(!mb_bool_Focus)
		return;

	char *lpc_buffer;
	//--------------------------------------------------------------------------
	//-- ???? ?????? ???? ?? ??
	if(!l_On_comp && a_On_comp)
	{
		//-- NULL ???????? ?? ?? ???????? ????? ???? ????.(2char)
		if(mpc_CoreString[mi_Core_Size-2] || mpc_CoreString[mi_Core_Size-3])
			return;

		lpc_buffer = new char [mi_Core_Size];

		//-- TEMP
		memcpy((void*)lpc_buffer,(void*)&(mpc_CoreString[mi_Core_cursor_idx]),
			   mi_Core_Size - mi_Core_cursor_idx );

		mpc_CoreString[mi_Core_cursor_idx] = a_inWchar[0];
		mpc_CoreString[mi_Core_cursor_idx+1] = a_inWchar[1];

		memcpy((void*)&(mpc_CoreString[mi_Core_cursor_idx+2]),(void*)lpc_buffer,
			   mi_Core_Size - mi_Core_cursor_idx -2);

		Gen_cursor_table();
		l_On_comp = true;
		//-- safe delete
		SAFE_DELETE_ARRAY(lpc_buffer);
	}
	//--------------------------------------------------------------------------
	//-- ???????? ??
	else if (l_On_comp && a_On_comp)
	{
		//-- Back Space ?? ???????? ?????? ???? ??
		if(!a_inWchar[0] && !a_inWchar[1])
		{
			lpc_buffer = new char [mi_Core_Size];
			memcpy((void*)lpc_buffer,(void*)&(mpc_CoreString[mi_Core_cursor_idx+2]),
				   mi_Core_Size - mi_Core_cursor_idx -2);

			memcpy((void*)&(mpc_CoreString[mi_Core_cursor_idx]),(void*)lpc_buffer,
				   mi_Core_Size - mi_Core_cursor_idx -2);

			mpc_CoreString[mi_Core_Size-1] =0;
			mpc_CoreString[mi_Core_Size-2] =0;

			Gen_cursor_table();
			l_On_comp = false;
			//-- safe delete
			SAFE_DELETE_ARRAY(lpc_buffer);
		}
		else
		{
			mpc_CoreString[mi_Core_cursor_idx] = a_inWchar[0];
			mpc_CoreString[mi_Core_cursor_idx+1] = a_inWchar[1];
			Gen_cursor_table();
		}

	}
	//--------------------------------------------------------------------------
	//-- ???? ????? ??
	else if (l_On_comp && !a_On_comp)
	{
		mpc_CoreString[mi_Core_cursor_idx] = a_inWchar[0];
		mpc_CoreString[mi_Core_cursor_idx+1] = a_inWchar[1];


		//-- ???? ?????????? ???? ???
		if(mi_Core_Size-2 > mi_Core_cursor_idx )
			mi_Core_cursor_idx+=2;

		Gen_cursor_table();
		_Inverse_Gen_cursor();
		l_On_comp = false;
	}
}
//----------------------------------------------------------------------------
//-- Insert char
//----------------------------------------------------------------------------
void L_MultiEditBox::InsertChar(const char a_inchar)
{
	//-- Focus ?????return;
	if(!mb_bool_Focus)
		return;

	char *lpc_buffer;

	//-- NULL ?????????? setting ??????????FULL
	//-- ????? ?????? 0 ?? ???????????? ?? 0 ????. ??FULL ????.
	if(mpc_CoreString[mi_Core_Size-2])
		return;

	lpc_buffer = new char [mi_Core_Size];

	//-- TEMP
	memcpy((void*)lpc_buffer,(void*)&(mpc_CoreString[mi_Core_cursor_idx]),
		   mi_Core_Size - mi_Core_cursor_idx);

	mpc_CoreString[mi_Core_cursor_idx] = a_inchar;

	memcpy((void*)&(mpc_CoreString[mi_Core_cursor_idx+1]),(void*)lpc_buffer,
		   mi_Core_Size - mi_Core_cursor_idx -1);

	//-- ?????????????? ???? ???
	if(mi_Core_Size-1 > mi_Core_cursor_idx )
		mi_Core_cursor_idx++;

	Gen_cursor_table();
	_Inverse_Gen_cursor();

	//-- safe delete
	SAFE_DELETE_ARRAY(lpc_buffer);
}

//----------------------------------------------------------------------------
//-- Gen Cursor table
//----------------------------------------------------------------------------
void L_MultiEditBox::Gen_cursor_table()
{
	int idx; //-- Core String ?? Index
	int output_y = 0; //-- ????? Line ?? ???
	int line_left =0;
	int line_size=0;
	bool han_lock =false;

	if(!mpc_CoreString)
		return;
	if(!mpc_OutputString)
		return;

	memset(mpi_Cursor_row_table,-1,mi_Core_Size*sizeof(int));
	memset(mpi_Cursor_column_table,-1,mi_Core_Size*sizeof(int));
	//--------------------------------------------------------------------------
	//-- Core String Access
	do
	{
		//-- Line ?? ??????? ?????? ?????? ?????? ????
		line_size = line_left + mi_MAX_column ;
		if(line_size > mi_Core_Size)
			line_size = mi_Core_Size; //-- ???????

		//------------------------------------------------------------------------
		//-- ????Line ?? NULL ???? Line Feed?? ??????.
		idx= line_left;

		do
		{
			//-- NULL ?????? Linefeed ????. loop ?? ?????? ????????.
			if(mpc_CoreString[idx] == 0 || mpc_CoreString[idx] == 0x0a)
			{
				line_size = idx +1;
			}

			//-- Cursor Gen
			assert(idx< mi_Core_Size);
			assert(idx>=0);

			mpi_Cursor_row_table[idx] = output_y;
			mpi_Cursor_column_table[idx] = idx - line_left;

			//-- Set Hangul flag - ???? ?????? ???? Set
			if((mpc_CoreString[idx] & 0x80) && g_dwLangType != 1)
				han_lock = !han_lock;

			//  [12/4/2008 ppmmjj83] ???? ?????IME ????
			else if( g_dwLangType == 1 && ((unsigned char)mpc_CoreString[idx] >= 0x81 //129
										   && (unsigned char)mpc_CoreString[idx] < 0xa0) //160
					 || ((unsigned char)mpc_CoreString[idx] >= 0xe0 //224
						 && (unsigned char)mpc_CoreString[idx] < 0xeb))
			{
				han_lock = !han_lock;
			}
			else
				han_lock = false;

			//-- ???? Column
			idx++;
		}
		while(idx<line_size);


		//-- ????? ?????? ?????? ???? ?????? ??????? line???? ???? ????.
		if(han_lock)
			line_size --;

		//-- ???? Row ????
		han_lock = false;
		output_y ++;
		line_left = line_size;

		//-- ????Line ?? right ?? NULL ?? ???? ?? ???????
	}
	while(mpc_CoreString[line_size-1]!=0);

	//-- ????? ?????? ????
	//ASSERT(idx< mi_Core_Size);
	assert(idx>=0);

	if(idx< mi_Core_Size)
	{
		mpi_Cursor_row_table[idx] = output_y;
		mpi_Cursor_column_table[idx] = 0;
	}

	mi_disp_line_num = output_y;
}

//----------------------------------------------------------------------------
//-- Draw Multi Text
//----------------------------------------------------------------------------
//-- ?? ?????? ??????? LF,NULL, size?? ?????? ???????? ??àÇ??? ???? ???
//-- ???????? ???? NULL ,???? ?è¿??? ??????.
//-- !!!! Draw ?????? ??? Line ?? ??????? ?????? ???????.
//-- ??? ????? Access ???? ??? ???????? ???????????.
//-- ??.. ?????? const ?? ???????? ????
void L_MultiEditBox::DrawMultiText()
{
	assert(mpc_CoreString);
	assert(mpc_OutputString);

	int line_left=0;
	int line_size= 0;
	int output_y = 0;

	//--------------------------------------------------------------------------
	//-- ?? Line ???? ???????. mi_Cursor_row_table ???
	//--------------------------------------------------------------------------
	do
	{
		//-- ???Line ?? Y / ???????? ??????.
		output_y = mpi_Cursor_row_table[line_left];
		line_size = line_left;

		//------------------------------------------------------------------------
		//-- ???Line ?? ??????? ??????.
		do
		{
			line_size ++;
		}
		while(line_size < mi_Core_Size && mpi_Cursor_row_table[line_size] == output_y);

		//--String ????
		ZeroMemory((void*)mpc_OutputString, mi_Core_Size*sizeof(char));
		memcpy((void*)mpc_OutputString,(void*)&(mpc_CoreString[line_left]), line_size - line_left);

		//-- Line Feed ????
		if(mpc_OutputString[line_size - line_left -1] ==0x0a)
			mpc_OutputString[line_size - line_left -1] =0;

		//------------------------------------------------------------------------
		//-- Text Out  : DC (Temp)
		if(output_y - mi_START_row < mi_MAX_row && output_y >= mi_START_row)
		{
			if( m_bEenter )
			{
				//jobab
				m_pSurf->DrawText(m_Font, mpc_JohabString,
								  MEB_START_X+(mi_cursor_x*7) ,MEB_START_Y+(mi_cursor_y*16),
								  RGB(0,180,0),RGB(255,255,255));

				// total
				m_pSurf->DrawText(m_Font, mpc_OutputString,
								  MEB_START_X ,MEB_START_Y  + (output_y - mi_START_row )*GAP_font_height,
								  RGB(0,0,0),_FCOLOR_YELLOW);

				// Candidate
				DrawCandidate();
			}
			else
			{
				m_pSurf->DrawText(m_Font, mpc_OutputString,
								  MEB_START_X ,MEB_START_Y  + (output_y - mi_START_row )*GAP_font_height,
								  RGB(0,0,0),_FCOLOR_YELLOW);
			}

		}

		line_left = line_size; //-- ???? Line ?? ??????

	}
	while(line_size < mi_Core_Size);


	//--------------------------------------------------------------------------
	//-- Cursor ??? ReadOnly ?????? Ignore
	//--------------------------------------------------------------------------
	if(!mb_bool_ReadOnly && mb_bool_Focus)
	{
		DWORD l_curTime = timeGetTime();
		if (l_curTime - mi_lasttime >= mi_PROMPTTIME)
		{
			mb_show_cursor = !mb_show_cursor;
			mi_lasttime = l_curTime;
		}

		if(mb_show_cursor)
		{
			m_pSurf->DrawText(m_Font, "|",
							  mi_cursor_x*MEB_font_width -1,
							  MEB_START_Y + (mi_cursor_y- mi_START_row)*GAP_font_height,
							  RGB(0,0,0),RGB(255,255,255),3);//_FCOLOR_YELLOW,3);
		}
	}
}

//  [1/15/2009 ppmmjj83]
void L_MultiEditBox::DrawCandidate(void)
{
	INT	i = 0, j = 0;
	int nCandidateSize = ImmGetCandidateList(m_hIMC, 0, NULL, 0);
	if( !nCandidateSize ) return;

	pCandidateList = (LPCANDIDATELIST)new BYTE[nCandidateSize];
	ImmGetCandidateList(m_hIMC, 0, pCandidateList, nCandidateSize);

	if( pCandidateList->dwCount )
	{
		INT	iCandidateHeight = 0;
		INT	iIndex = pCandidateList->dwSelection;

		if(m_iCandidateIndex != iIndex)			// ´Ù¸¥ ÆùÆ® ¼±ÅÃ½Ã ±×·ÁÁÖ±â
		{
			INT		iPageCount = getPageCount(iIndex);
			INT		iStart = getCandidateStart(iPageCount);
			INT		iEnd = getCandidateEnd(iPageCount);
			SIZE	sizeCandidate;

			for( i = 0; i < 9; i++ )
				for( j = 0; j < 512; j++ )
					m_cBuff[i][j] = '\0';

			for(i=0; iStart+i < iEnd; i++)
			{
				sprintf(m_cBuff[i], "%d:%s", i+1, getCandidateString(iStart+i));
				getFontSize(&sizeCandidate, m_Font, m_cBuff[i], -1);
			}

			m_iCandidateIndex = iIndex;
			m_iCandidateStart = iStart;
			m_iCandidateEnd = iEnd;
			m_heightCandidate = sizeCandidate.cy;

			setCandidatePage(m_iCandidateStart);
		}


		g_pDisplay->Blt( m_nX -210, m_nY-10, m_pBack_CandidateSurf);

		for(i = 0; (m_iCandidateStart + i) < m_iCandidateEnd; i++)
		{
			m_pCandidateSurf->DrawText(m_Font, m_SpaceStr, 0, 0, RGB(255,0,255), _FCOLOR_GREEN);

			if( (m_iCandidateStart + i) == m_iCandidateIndex )
			{
				m_pCandidateSurf->DrawText(m_Font, m_cBuff[i], 0, 0, RGB(255,0,255), RGB(100,255,255));
				g_pDisplay->Blt(m_nX -210+25, m_nY-10 + 20 + iCandidateHeight, m_pCandidateSurf);
			}
			else
			{
				m_pCandidateSurf->DrawText(m_Font, m_cBuff[i], 0, 0, RGB(255,0,255) , RGB(100,100,255));
				g_pDisplay->Blt(m_nX -210 + 25, m_nY-10 + 20 + iCandidateHeight, m_pCandidateSurf);
			}

			iCandidateHeight += m_heightCandidate;
		}

	}
	else
	{
		//reset candidate
		for( i = 0; i < 9; i++ )
			for( j = 0; j < 512; j++ )
				m_cBuff[i][j] = '\0';

		m_iCandidateIndex = 0;
		m_iCandidateStart = 0;
		m_iCandidateEnd = 0;
		m_heightCandidate = 0;
	}

	//g_pDisplay->Blt( m_nX -210, m_nY-10, m_pBack_CandidateSurf);
}

bool L_MultiEditBox::getFontSize(LPSIZE pSize,HFONT hFont,LPCTSTR pText,INT iLength)
{
	if(iLength < 0)
		iLength = strlen(pText);

	SIZE	sizeFont;
	HDC		hdmDC = CreateCompatibleDC(NULL);
	HFONT	holdFont = (HFONT)SelectObject(hdmDC,hFont);

	// ÆùÆ® String ÀÇ Æø°ú ³ÐÀÌ ¾ò±â
	GetTextExtentPoint(hdmDC,pText,iLength,&sizeFont);
	SelectObject(hdmDC,holdFont);
	DeleteDC(hdmDC);
	*pSize = sizeFont;

	return true;
}


VOID L_MultiEditBox::setCandidatePage(INT iStartCount)
{
	if( pCandidateList )
		pCandidateList->dwPageStart = iStartCount;
	return;
}

LPCSTR L_MultiEditBox::getCandidateString(INT iIndex) const
{
	if(!pCandidateList)
		return NULL;

	// ÆùÆ® ¸®½ºÆ® Áß ÀÎµ¦½º¿¡ ¸Â´Â ÆùÆ®¸¦ ¹ÝÈ¯
	return (LPCSTR)((LPBYTE)pCandidateList+pCandidateList->dwOffset[iIndex]);
}

INT L_MultiEditBox::getCandidateEnd(INT iPageCount) const
{
	// ÇÑ ÆäÀÌÁö¿¡ º¸¿©Áö´Â ÆùÆ®ÀÇ ÃÖ´ë °³¼ö
	int iCount = 0;

	iCount = (iPageCount * 9) + 9;

	if(pCandidateList && iCount > (int)pCandidateList->dwCount)
		iCount = pCandidateList->dwCount;

	return iCount;
}

INT L_MultiEditBox::getCandidateStart(INT iPageCount) const
{
	// Ã¹¹øÂ° StringÀÇ ÀÎµ¦½º
	return (iPageCount * 9);// + 1;
}

INT L_MultiEditBox::getPageCount(INT iIndex)
{
	int iPageCount = 0;

	if( iIndex >= 1 && iIndex < 9 )
		iPageCount = 0;
	if( iIndex >= 9 && iIndex < 18 )
		iPageCount = 1;
	if( iIndex >= 18 && iIndex < 27 )
		iPageCount = 2;
	if( iIndex >= 27 && iIndex < 36 )
		iPageCount = 3;
	if( iIndex >= 36 && iIndex < 45 )
		iPageCount = 4;
	if( iIndex >= 45 && iIndex < 54 )
		iPageCount = 5;
	if( iIndex >= 54 && iIndex < 63 )
		iPageCount = 6;
	if( iIndex >= 63 && iIndex < 72 )
		iPageCount = 7;
	if( iIndex >= 72 && iIndex < 81 )
		iPageCount = 8;
	if( iIndex >= 81 && iIndex < 90 )
		iPageCount = 9;
	if( iIndex >= 90 && iIndex < 99 )
		iPageCount = 10;

	return iPageCount;
}

//----------------------------------------------------------------------------
//-- Move Cursor
//----------------------------------------------------------------------------
//-- ?????? ???? ???Box?? Cursor ?? ?????? ?????? Cursor?????? ????
//-- a_direction - 0 : left / 1:right / 2: up / 3: down / 4:home / 5:end
//-- ????? ???? ?? ????
bool L_MultiEditBox::_MoveCursor(const int a_direction, const bool a_ctrl)
{
	//-- ?????? ???? ??? ???? ????
	int l_cursor_x = mi_cursor_x;
	int l_cursor_y = mi_cursor_y;

	int idx = 0;                 //-- Core String ?? Index
	int output_y = 0;            //-- ????? Line ?? ???
	int line_left= 0;
	int line_size= 0;
	int line_end = 0;
	bool l_end = false;

	assert(mpc_CoreString);
	assert(mpc_OutputString);

	//--------------------------------------------------------------------------
	//-- ????? ???? ????
	//-- ???? ?????? ?????? ????????.
	switch(a_direction)
	{
	case 0 :
		l_cursor_x --;
		break;
	case 1 :
		l_cursor_x ++;
		break;
	case 2 :
		l_cursor_y --;
		l_end = true;
		break;
	case 3 :
		l_cursor_y ++;
		l_end = true;
		break;
	case 4 :
		l_cursor_x = 0;
		break;
	case 5 :
		l_cursor_x = mi_MAX_column;
		l_end = true;
		break;
	default :
		break;
	}

	//-- 1??Check 0???? ???? ???
	//--  x<0 : End , y--;
	if(l_cursor_x <0)
	{
		l_cursor_x =mi_MAX_column;
		l_cursor_y --;
		l_end = true;
	}
	if(l_cursor_y <0)
		return false;

	//--------------------------------------------------------------------------
	//-- ?? Line ???? ???????. mi_Cursor_row_table ???
	//--------------------------------------------------------------------------
	do
	{
		//-- ???Line ?? Y / ???????? ??????.
		//line_left = 0;

		line_size = line_left;

		do
		{
			line_size ++;
		}
		while(mpi_Cursor_row_table[line_size] == mpi_Cursor_row_table[line_left] );

		//-- line y ?? ??????.
		output_y = mpi_Cursor_row_table[line_left];

		//------------------------------------------------------------------------
		//-- ????Line ?????
		if(output_y == l_cursor_y)
		{
			line_end = line_left;

			//-- NULL ???? LF ???????? line ?? end ?? ??????.
			while( line_end < line_size && mpc_CoreString[line_end] !=0x0a && mpc_CoreString[line_end] !=0)
				line_end ++;

			//-- Cur_x ?? virtual ???????? ???Line ???????
			if(line_end - line_left >= l_cursor_x )
			{
				int han_idx = line_left;
				bool han_lock =false;

				//-- Core Cursor Set
				mi_Core_cursor_idx = line_left + l_cursor_x;

				//-- Hangul Check
				do
				{
					if(han_lock && han_idx == mi_Core_cursor_idx)
					{
						if(a_direction)
						{
							mi_Core_cursor_idx ++;
							l_cursor_x ++;
						}
						else
						{
							mi_Core_cursor_idx --;
							l_cursor_x --;
						}
						break;
					}

					//-- Set Hangul flag - ???? ?????? ???? Set
					if((mpc_CoreString[han_idx] & 0x80) && g_dwLangType != 1)
						han_lock = !han_lock;

					//  [12/4/2008 ppmmjj83] ???? ?????IME ????
					else if( g_dwLangType == 1 && ((unsigned char)mpc_CoreString[han_idx] >= 0x81 //129
												   && (unsigned char)mpc_CoreString[han_idx] < 0xa0) //160
							 || ((unsigned char)mpc_CoreString[han_idx] >= 0xe0 //224
								 && (unsigned char)mpc_CoreString[han_idx] < 0xeb))
						han_lock = !han_lock;
					else
						han_lock = false;

					han_idx++;
				}
				while(han_idx < line_size);

				//--------------------------------------------------------------------
				//-- Core Cursor Set
				mi_cursor_x = l_cursor_x;
				mi_cursor_y = l_cursor_y;

				//-- ????culling
				if(mi_cursor_y < mi_START_row )
					mi_START_row = mi_cursor_y ;

				if(mi_cursor_y - mi_START_row  >= mi_MAX_row )
					mi_START_row = mi_cursor_y - mi_MAX_row +1;

				return true;
			}

			//------------------------------------------------------------------------
			//-- Cur_x ?? virtual ???????? ???line ???????
			else
			{
				//-- End : ???? Line ???? ?? ??????? ???? End
				//-- line y ?? ????? ?????? End = true;
				if(l_end)
				{
					l_cursor_x = line_end - line_left;
					//------------------------------------------------------------------
					//-- Core Cursor Set
					mi_Core_cursor_idx = line_left + l_cursor_x;
					mi_cursor_x = l_cursor_x;
					mi_cursor_y = l_cursor_y;

					//-- ????culling
					if(mi_cursor_y < mi_START_row )
						mi_START_row = mi_cursor_y ;

					if(mi_cursor_y - mi_START_row  >= mi_MAX_row )
						mi_START_row = mi_cursor_y - mi_MAX_row +1;

					return true;
				}
				//-- ???? ????? Home y++; home
				else
				{
					l_cursor_x = 0;
					l_cursor_y ++;
				}
			}//-- end of line ??
		}

		//-- end of ????line
		//------------------------------------------------------------------------
		line_left = line_size ;

	}
	while(line_left < mi_Core_Size
			&& (mpc_CoreString[line_left] ||
				(line_left && mpc_CoreString[line_left-1] ==0X0A)));// && mpc_CoreString[line_size-1]);

	return false;
}

//----------------------------------------------------------------------------
//-- Regen Cursor
//----------------------------------------------------------------------------
//-- ?ðÀ? / ?èÁ??? ???cursor ?? ??????? ?????? ?????? ????? ?????.
//-- ?û¶?? mi_Core_cursor_idx ?? ???? resen ????.
void L_MultiEditBox::_Inverse_Gen_cursor()
{
	assert(mi_Core_cursor_idx< mi_Core_Size);
	assert(mi_Core_cursor_idx>=0);

	mi_cursor_x = mpi_Cursor_column_table[mi_Core_cursor_idx];
	mi_cursor_y = mpi_Cursor_row_table[mi_Core_cursor_idx];

	//-- ????culling
	if(mi_cursor_y < mi_START_row )
		mi_START_row = mi_cursor_y ;
	if(mi_cursor_y - mi_START_row  >= mi_MAX_row )
		mi_START_row = mi_cursor_y - mi_MAX_row +1;
}

LRESULT L_MultiEditBox::OnJapanMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	HIMC	hImc;
	HWND	hIMEwnd;

	// ??? IME ???????????
	hIMEwnd = ImmGetDefaultIMEWnd(hWnd);
	SendMessage(hIMEwnd, WM_CLOSE, 0, 0);

	// ??? ???? ?????????? ???é°? ??ûÛ???
	hImc	= ImmGetContext(hWnd);
	m_hIMC	= hImc;

	switch(uMsg)
	{
	case WM_IME_SETCONTEXT:			// ?????? ???? ?????????? ??????
		return 0;
	case WM_CHAR:
		if(wParam != 8 && wParam != 13)
			OnJapanChar(wParam);		// ?????????? String ????
		break;
	case WM_KEYDOWN:
		OnJKeyDown(wParam, uMsg);			// ???? ???? ?????? ???? ( 05-06-27 ???? ??????? ?“s? ????. by ??? )
		break;
	default:
		break;
	}

	// ???? ????????????? ????? ???? ?????? ???é»?? ??????????????? ???????? ????
	// ?ã³? ?????? ???????????? ????? ???????? ?????? ??? ????????? ???? ?úÌúÙ?

	return 1;
}

bool L_MultiEditBox::OnJapanDrawText(void)
{
	// ?????????? ????????
	if(!mb_bool_Focus)
		return false;

	char*	szJohabTemp;
	LONG	lByte = 0 ;
	lByte		= ImmGetCompositionString( m_hIMC, GCS_COMPSTR, NULL, 0 );

	if( lByte == 0) return false;

	szJohabTemp = new char[lByte];

	m_bEenter = true;
	ZeroMemory( szJohabTemp, lByte );
	ZeroMemory(mpc_JohabString, mi_Core_Size*sizeof(char));

	ImmGetCompositionString(m_hIMC, GCS_COMPSTR, szJohabTemp, lByte);

	memcpy(mpc_JohabString, szJohabTemp, sizeof(szJohabTemp));

	delete [] szJohabTemp;

	Gen_cursor_table();
	_Inverse_Gen_cursor();

	return true;
}

void L_MultiEditBox::OnJapanChar(CHAR cChar)
{
	static bool lb_char_MBCS= false;
	static bool lb_char_MBCS_Denied = false;

	if(!mb_bool_Focus)
		return;

	char *lpc_buffer;

	if(mpc_CoreString[mi_Core_Size-2])
		return;

	lpc_buffer = new char [mi_Core_Size];

	WCHAR strw = cChar;

	if(((unsigned char)cChar >= 0x81
			&& (unsigned char)cChar < 0xa0)
			|| ((unsigned char)cChar >= 0xe0
				&& (unsigned char)cChar < 0xeb))
	{
		lb_char_MBCS  = !lb_char_MBCS;

		//-- TEMP
		memcpy((void*)lpc_buffer,(void*)&(mpc_CoreString[mi_Core_cursor_idx]),
			   mi_Core_Size - mi_Core_cursor_idx);

		mpc_CoreString[mi_Core_cursor_idx] = cChar;

		memcpy((void*)&(mpc_CoreString[mi_Core_cursor_idx+1]),(void*)lpc_buffer,
			   mi_Core_Size - mi_Core_cursor_idx -1);

		//-- ?????????????? ???? ???
		if(mi_Core_Size-1 > mi_Core_cursor_idx )
			mi_Core_cursor_idx++;
	}

	else if( lb_char_MBCS  )
	{
		lb_char_MBCS   =false;

		//-- TEMP
		memcpy((void*)lpc_buffer,(void*)&(mpc_CoreString[mi_Core_cursor_idx]),
			   mi_Core_Size - mi_Core_cursor_idx);

		mpc_CoreString[mi_Core_cursor_idx] = cChar;

		memcpy((void*)&(mpc_CoreString[mi_Core_cursor_idx+1]),(void*)lpc_buffer,
			   mi_Core_Size - mi_Core_cursor_idx -1);

		//-- ?????????????? ???? ???
		if(mi_Core_Size-1 > mi_Core_cursor_idx )
			mi_Core_cursor_idx++;
	}
	else
	{
		//-- TEMP
		memcpy((void*)lpc_buffer,(void*)&(mpc_CoreString[mi_Core_cursor_idx]),
			   mi_Core_Size - mi_Core_cursor_idx);

		mpc_CoreString[mi_Core_cursor_idx] = cChar;

		memcpy((void*)&(mpc_CoreString[mi_Core_cursor_idx+1]),(void*)lpc_buffer,
			   mi_Core_Size - mi_Core_cursor_idx -1);

		//-- ?????????????? ???? ???
		if(mi_Core_Size-1 > mi_Core_cursor_idx )
			mi_Core_cursor_idx++;
	}

	Gen_cursor_table();
	_Inverse_Gen_cursor();

	//-- safe delete
	SAFE_DELETE_ARRAY(lpc_buffer);
}

void L_MultiEditBox::OnJKeyDown(CHAR cChar, UINT uMsg) // ??????KeyDown ??????(????? ????)
{
	if(uMsg == WM_KEYDOWN && mb_bool_Focus)
	{
		//------------------------------------------------------------------------
		//-- Read Only
		//-- left/right arrow ????
		//-- up / down ?? Scroll ????

		if(mb_bool_ReadOnly)
		{
			switch(cChar)
			{
			case VK_UP :
				Dec_Start_row();
				break;   //-- Up Scroll
			case VK_DOWN :
				Inc_Start_row();
				break; //-- Down Scroll
			case VK_PRIOR:
				_OnKEY_PgUp();
				break; //-- page up
			case VK_NEXT:
				_OnKEY_PgDown();
				break; //-- page down
			}
		}
		//------------------------------------------------------------------------
		//-- Writable
		//-- left/right arrow ????
		//--
		else
		{
			switch(cChar)
			{
			case VK_LEFT :
				_MoveCursor(0,false);
				break;
			case VK_RIGHT :
				_MoveCursor(1,false);
				break;
			case VK_UP :
				_MoveCursor(2,false);
				break;
			case VK_DOWN :
				_MoveCursor(3,false);
				break;
			case VK_HOME :
				_MoveCursor(4,false);
				break;
			case VK_END :
				_MoveCursor(5,false);
				break;
			case VK_DELETE :
				_OnKEY_Deletechar();
				break;
			case VK_BACK :
				_OnKEY_Backspace();
				break;
			case VK_RETURN:
				if( !m_bEenter )
					InsertChar(0x0A);

				break;
			case VK_PRIOR:
				_OnKEY_PgUp();
				break; //-- page up
			case VK_NEXT:
				_OnKEY_PgDown();
				break; //-- page down
			default:
				m_bEenter = false;
				OnJapanDrawText();
			}
		}
	}
}

//----------------------------------------------------------------------------
//-- Message Procedure
//----------------------------------------------------------------------------
//-- ?????? ???? Message ?? ???????? ???? ????.
LRESULT L_MultiEditBox::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	HIMC	m_hIMC=NULL;
	static bool lb_char_han = false;
	int len;
	char Cstr[5]= {0,};

	//  [10/10/2008 parkmj] test
	//	HWND	hIMEwnd;
	if (g_dwLangType == 1) // japan
	{
		return OnJapanMsg(hWnd, uMsg, wParam, lParam);
	}

	// korea
	else if(uMsg == WM_IME_COMPOSITION && mb_bool_Focus && !mb_bool_ReadOnly)
	{
		m_hIMC = ImmGetContext(hWnd);

		if (lParam & GCS_RESULTSTR)
		{
			if((len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0))>0)
			{
				ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, Cstr, len);
				Cstr[len] = 0;
				InsertIMEChar(Cstr,false);
			}
		}
		else
		{
			if (lParam & GCS_COMPSTR)
			{
				len = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0);
				ImmGetCompositionString(m_hIMC, GCS_COMPSTR, Cstr, len);
				InsertIMEChar(Cstr,true);
			}
		}

		ImmReleaseContext(hWnd, m_hIMC);
	}
	//-- Focus ?????return;
	else if(uMsg == WM_KEYDOWN && mb_bool_Focus)
	{
		//------------------------------------------------------------------------
		//-- Read Only
		//-- left/right arrow ????
		//-- up / down ?? Scroll ????

		if(mb_bool_ReadOnly)
		{
			switch(wParam)
			{
			case VK_UP :
				Dec_Start_row();
				break;   //-- Up Scroll
			case VK_DOWN :
				Inc_Start_row();
				break; //-- Down Scroll
			case VK_PRIOR:
				_OnKEY_PgUp();
				break; //-- page up
			case VK_NEXT:
				_OnKEY_PgDown();
				break; //-- page down
			}
		}
		//------------------------------------------------------------------------
		//-- Writable
		//-- left/right arrow ????
		//--
		else
		{
			switch(wParam)
			{
			case VK_LEFT :
				_MoveCursor(0,false);
				break;
			case VK_RIGHT :
				_MoveCursor(1,false);
				break;
			case VK_UP :
				_MoveCursor(2,false);
				break;
			case VK_DOWN :
				_MoveCursor(3,false);
				break;
			case VK_HOME :
				_MoveCursor(4,false);
				break;
			case VK_END :
				_MoveCursor(5,false);
				break;
			case VK_DELETE :
				_OnKEY_Deletechar();
				break;
			case VK_BACK :
				_OnKEY_Backspace();
				break;
			case VK_RETURN:
				InsertChar(0x0A);
				break;
			case VK_PRIOR:
				_OnKEY_PgUp();
				break; //-- page up
			case VK_NEXT:
				_OnKEY_PgDown();
				break; //-- page down
			}
		}
	}
	//--------------------------------------------------------------------------
	//-- On Char
	else if(uMsg == WM_CHAR && mb_bool_Focus && !mb_bool_ReadOnly)
	{
		//-- IME ?????? ??????WM_Char ?? ???ûÛ???.
		//-- ???å¿??????????? 0x80 ?? ????????
		if(wParam & 0x80)
			lb_char_han  = !lb_char_han  ;
		else if(lb_char_han  )
			lb_char_han   =false;
		else if(wParam>=32 && wParam<128) // english
			InsertChar(wParam);
	}
	return false;
}
