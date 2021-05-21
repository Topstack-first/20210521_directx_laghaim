#ifndef _G_DEV_LOGBOX_H_
#define _G_DEV_LOGBOX_H_

#include "Config.h"

BOOL CALLBACK Proc_DevNet( HWND, UINT, WPARAM, LPARAM );            //--- Network Log
BOOL CALLBACK Proc_DevString( HWND, UINT, WPARAM, LPARAM );         //--- String Log
BOOL CALLBACK Proc_DevSkill( HWND, UINT, WPARAM, LPARAM);           //--- Dev skill
BOOL CALLBACK Proc_Main( HWND, UINT, WPARAM, LPARAM );              //--- Main Proc
BOOL CALLBACK Proc_DevPointLog( HWND, UINT, WPARAM, LPARAM );		//--- Point Log

void G_DevCreateDlg();
void G_DevAddLogNetwork(char *a_svr_str, char *a_cli_str); //-- Net Log
void G_DevAddLogString(char *a_str);			//-- String Log
void G_DevAddSkillProp(int val_a, int val_b);	//-- Skill Property
void G_DevAddPointString(char *str);			//-- Point string log
void G_CatLocalTimeToStr( char* OUT strOUT, char* IN strIN);		//--

#endif // _G_DEV_LOGBOX_H_
