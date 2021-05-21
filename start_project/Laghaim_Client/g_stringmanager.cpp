#include "stdafx.h"

#include "stdio.h"
#include "g_devlogbox.h"
#include "g_stringmanager.h"

#include "LHGlobal_String.h"

extern HINSTANCE  g_hDllInst;

//-- String이 저장될 Buffer
char l_string[STRING_MAX_BUFFER];

//-- 창을 보일 것인가.
extern bool g_bWndNetMessage;

//----------------------------------------------------------------------------
//-- Gloabal Get String Function
//----------------------------------------------------------------------------
//-- 외부에서 String 을 얻는 Method
//-- Get 을 수행하였을 경우 l_string 이 Set되며 Return 된다.
//-- 다른 수행에 의해 다시 Set 되기 전에 반드시 복사를 수행해야 한다.
#define TEST_STR
char const * const G_STRING(const int a_index)
{
	return LHGlobal_String::GetSingletonPtr()->GetMsg(a_index);

// 	if( LoadString( g_hDllInst, a_index, l_string, STRING_MAX_BUFFER ) == NULL )
// 		sprintf(l_string,  "Fail!!! Load Dll String : %d" ,a_index);
//
// 	if(g_bWndNetMessage)
// 		G_DevAddLogString(l_string);
//
// 	return l_string;
}
