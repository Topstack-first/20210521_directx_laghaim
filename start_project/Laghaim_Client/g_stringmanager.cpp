#include "stdafx.h"

#include "stdio.h"
#include "g_devlogbox.h"
#include "g_stringmanager.h"

#include "LHGlobal_String.h"

extern HINSTANCE  g_hDllInst;

//-- String�� ����� Buffer
char l_string[STRING_MAX_BUFFER];

//-- â�� ���� ���ΰ�.
extern bool g_bWndNetMessage;

//----------------------------------------------------------------------------
//-- Gloabal Get String Function
//----------------------------------------------------------------------------
//-- �ܺο��� String �� ��� Method
//-- Get �� �����Ͽ��� ��� l_string �� Set�Ǹ� Return �ȴ�.
//-- �ٸ� ���࿡ ���� �ٽ� Set �Ǳ� ���� �ݵ�� ���縦 �����ؾ� �Ѵ�.
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
