#include "stdafx.h"
#define D3D_OVERLOADS



#include <math.h>
#include <ddraw.h>
#include <d3d.h>
#include "ddutil.h"
#include "dxutil.h"
#include "main.h"
#include "IndexedTexture.h"
// nate 2004 - 7
#include "ImageDefine.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "nkcharacter.h"
#include "headers.h"
#include "msgpopup.h"
#include "tcpipcon.h"
#include "GuardTower.h"






extern int g_EffectTextr[];
extern HINSTANCE g_hDllInst;


#define LARGE_TEXTBOX_CX	100
#define SMALL_TEXTBOX_CX	74
#define SMALL2_TEXTBOX_CX   70

#define _CH_TOP		0
#define _CH_BODY	1
#define _CH_SHIELD	2
#define _CH_NONE	3

//////////kwon

char* ids_guardtower_att_mode_multi = NULL;
char* ids_guardtower_att_mode_damage = NULL;
char* ids_guardtower_att_mode_normal = NULL;
char* ids_guardtower_top_level_normal = NULL;
char* ids_guardtower_top_level_alittlehigh =NULL;  //5
char* ids_guardtower_top_level_high =NULL;
char* ids_guardtower_top_level_veryhigh =NULL;
char* ids_guardtower_top_name_fire_normal = NULL;
char* ids_guardtower_top_name_fire_advance = NULL;
char* ids_guardtower_top_name_fire_tower = NULL;  //10
char* ids_guardtower_top_name_fire_greatfire = NULL; //11
char* ids_guardtower_top_name_light_normal = NULL;
char* ids_guardtower_top_name_light_advance = NULL;
char* ids_guardtower_top_name_light_tower = NULL;
char* ids_guardtower_top_name_light_greatlight = NULL;
char* ids_guardtower_top_name_stone_normal = NULL;
char* ids_guardtower_top_name_stone_advance = NULL;
char* ids_guardtower_top_name_stone_tower = NULL;
char* ids_guardtower_top_name_stone_greatlight = NULL;

char* ids_guardtower_body_mode_clay = NULL;  //20
char* ids_guardtower_body_mode_stone = NULL;  //21
char* ids_guardtower_body_mode_steel = NULL;

char* ids_guardtower_body_level_normal = NULL;
char* ids_guardtower_body_level_middle = NULL;
char* ids_guardtower_body_level_veryhigh = NULL;

char* ids_guardtower_shield_level_none = NULL;
char* ids_guardtower_shield_level_normal = NULL;
char* ids_guardtower_shield_level_alittlestrong = NULL;
char* ids_guardtower_shield_level_strong = NULL;
char* ids_guardtower_shield_level_verystrong = NULL;


char *att_mode[3]; //"다수공격", "% 데미지", "일반공격"
// TOP
char *top_level[4];//"보통", "약간높음", "높음", "아주높음"
char *top_name_fire[4];//"노멀 파이어",  "어드밴스 파이어", "파이어 포탑", "그레이트 파이어"
char *top_name_light[4];//"노멀 라이트닝", "어드밴스 라이트닝", "라이트닝 포탑", "그레이트 라이트닝"
char *top_name_stone[4];//"노멀 스톤", "어드밴스 스톤", "스톤 포탑", "그레이트 스톤"
// BODY
char *body_mode[3];//"클레이", "스톤", "스틸"
char *body_level[3];//"보통", "중간", "아주높음"
// SHIELD
char *shield_level[5];//"없음", "보통", "약간강함", "강함", "매우강함"


#include "resource.h"

#include "g_stringmanager.h"


void GuardTowerPage_LoadDLLStringTable()
{
	att_mode[0] = (char*)G_STRING(IDS_GUARDTOWER_ATT_MODE_MULTI);//"다수공격", "% 데미지", "일반공격"};
	att_mode[1] = (char*)G_STRING(IDS_GUARDTOWER_ATT_MODE_DAMAGE);
	att_mode[2] = (char*)G_STRING(IDS_GUARDTOWER_ATT_MODE_NORMAL);

	// TOP
	top_level[0] = (char*)G_STRING(IDS_GUARDTOWER_TOP_LEVEL_NORMAL);//"보통", "약간높음", "높음", "아주높음"};
	top_level[1] = (char*)G_STRING(IDS_GUARDTOWER_TOP_LEVEL_ALITTLE_HIGH);
	top_level[2] = (char*)G_STRING(IDS_GUARDTOWER_TOP_LEVEL_HIGH);
	top_level[3] = (char*)G_STRING(IDS_GUARDTOWER_TOP_LEVEL_VERYHIGH);

	top_name_fire[0]  = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_FIRE_NORMAL);//"노멀 파이어",  "어드밴스 파이어", "파이어 포탑", "그레이트 파이어"};
	top_name_fire[1]  = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_FIRE_ADVANCE);
	top_name_fire[2]  = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_FIRE_TOWER);
	top_name_fire[3]  = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_FIRE_GREATFIRE);

	top_name_light[0] = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_LIGHT_NORMAL);//"노멀 라이트닝", "어드밴스 라이트닝", "라이트닝 포탑", "그레이트 라이트닝"};
	top_name_light[1] = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_LIGHT_ADVANCE);
	top_name_light[2] = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_LIGHT_TOWER);
	top_name_light[3] = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_LIGHT_GREATLIGHT);

	top_name_stone[0] = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_STONE_NORMAL);//"노멀 스톤", "어드밴스 스톤", "스톤 포탑", "그레이트 스톤"};
	top_name_stone[1] = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_STONE_ADVANCE);
	top_name_stone[2] = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_STONE_TOWER);
	top_name_stone[3] = (char*)G_STRING(IDS_GUARDTOWER_TOP_NAME_STONE_GREATLIGHT);

	// BODY
	body_mode[0]  = (char*)G_STRING(IDS_GUARDTOWER_BODY_MODE_CLAY);//"클레이", "스톤", "스틸"};
	body_mode[1]  = (char*)G_STRING(IDS_GUARDTOWER_BODY_MODE_STONE);
	body_mode[2]  = (char*)G_STRING(IDS_GUARDTOWER_BODY_MODE_STEEL);

	body_level[0] = (char*)G_STRING(IDS_GUARDTOWER_BODY_LEVEL_NORMAL);//"보통", "중간", "아주높음"};
	body_level[1] = (char*)G_STRING(IDS_GUARDTOWER_BODY_LEVEL_MIDDLE);
	body_level[2] = (char*)G_STRING(IDS_GUARDTOWER_BODY_LEVEL_VERYHIGH);


	// SHIELD
	shield_level[0] = (char*)G_STRING(IDS_GUARDTOWER_SHIELD_LEVEL_NONE);//"없음", "보통", "약간강함", "강함", "매우강함"};
	shield_level[1] = (char*)G_STRING(IDS_GUARDTOWER_SHIELD_LEVEL_NORMAL);
	shield_level[2] = (char*)G_STRING(IDS_GUARDTOWER_SHIELD_LEVEL_ALITTLESTRONG);
	shield_level[3] = (char*)G_STRING(IDS_GUARDTOWER_SHIELD_LEVEL_STRONG);
	shield_level[4] = (char*)G_STRING(IDS_GUARDTOWER_SHIELD_LEVEL_VERYSTRONG);

///////////////
// 	HINSTANCE hInst = g_hDllInst;
// 	char temp[1024];
//
// 	char error[100];
// 	strcpy(error, "Fail!!! Load Dll String");
//
// 	if(LoadString(hInst, IDS_GUARDTOWER_ATT_MODE_MULTI, temp, 1024) != NULL){
// 		ids_guardtower_att_mode_multi = new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_att_mode_multi, temp);
// 	}else{
// 		ids_guardtower_att_mode_multi= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_att_mode_multi, error); //1
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_ATT_MODE_DAMAGE, temp, 1024) != NULL){
// 		ids_guardtower_att_mode_damage= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_att_mode_damage, temp);
// 	}else{
// 		ids_guardtower_att_mode_damage= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_att_mode_damage, error);	//2
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_ATT_MODE_NORMAL, temp, 1024) != NULL){
// 		ids_guardtower_att_mode_normal= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_att_mode_normal, temp);
// 	}else{
// 		ids_guardtower_att_mode_normal= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_att_mode_normal, error);		//3
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_LEVEL_NORMAL, temp, 1024) != NULL){
// 		ids_guardtower_top_level_normal= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_level_normal, temp);
// 	}else{
// 		ids_guardtower_top_level_normal= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_level_normal, error);		//4
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_LEVEL_ALITTLE_HIGH, temp, 1024) != NULL){
// 		ids_guardtower_top_level_alittlehigh= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_level_alittlehigh, temp);
// 	}else{
// 		ids_guardtower_top_level_alittlehigh= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_level_alittlehigh, error);		//5
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_LEVEL_HIGH, temp, 1024) != NULL){
// 		ids_guardtower_top_level_high= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_level_high, temp);
// 	}else{
// 		ids_guardtower_top_level_high= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_level_high, error);		//6
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_LEVEL_VERYHIGH, temp, 1024) != NULL){
// 		ids_guardtower_top_level_veryhigh= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_level_veryhigh, temp);
// 	}else{
// 		ids_guardtower_top_level_veryhigh= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_level_veryhigh, error);		//7
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_FIRE_NORMAL, temp, 1024) != NULL){
// 		ids_guardtower_top_name_fire_normal= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_fire_normal, temp);
// 	}else{
// 		ids_guardtower_top_name_fire_normal= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_fire_normal, error);		//8
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_FIRE_ADVANCE, temp, 1024) != NULL){
// 		ids_guardtower_top_name_fire_advance= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_fire_advance, temp);
// 	}else{
// 		ids_guardtower_top_name_fire_advance= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_fire_advance, error);		//9
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_FIRE_TOWER, temp, 1024) != NULL){
// 		ids_guardtower_top_name_fire_tower= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_fire_tower, temp);
// 	}else{
// 		ids_guardtower_top_name_fire_tower= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_fire_tower, error);		//10
// 	}
//     if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_FIRE_GREATFIRE, temp, 1024) != NULL){
// 		ids_guardtower_top_name_fire_greatfire = new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_fire_greatfire, temp);
// 	}else{
// 		ids_guardtower_top_name_fire_greatfire= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_fire_greatfire, error); //11
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_LIGHT_NORMAL, temp, 1024) != NULL){
// 		ids_guardtower_top_name_light_normal= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_light_normal, temp);
// 	}else{
// 		ids_guardtower_top_name_light_normal= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_light_normal, error);	//12
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_LIGHT_ADVANCE, temp, 1024) != NULL){
// 		ids_guardtower_top_name_light_advance= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_light_advance, temp);
// 	}else{
// 		ids_guardtower_top_name_light_advance= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_light_advance, error);		//13
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_LIGHT_TOWER, temp, 1024) != NULL){
// 		ids_guardtower_top_name_light_tower= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_light_tower, temp);
// 	}else{
// 		ids_guardtower_top_name_light_tower= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_light_tower, error);		//14
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_LIGHT_GREATLIGHT, temp, 1024) != NULL){
// 		ids_guardtower_top_name_light_greatlight= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_light_greatlight, temp);
// 	}else{
// 		ids_guardtower_top_name_light_greatlight= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_light_greatlight, error);		//15
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_STONE_NORMAL, temp, 1024) != NULL){
// 		ids_guardtower_top_name_stone_normal= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_stone_normal, temp);
// 	}else{
// 		ids_guardtower_top_name_stone_normal= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_stone_normal, error);		//16
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_STONE_ADVANCE, temp, 1024) != NULL){
// 		ids_guardtower_top_name_stone_advance= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_stone_advance, temp);
// 	}else{
// 		ids_guardtower_top_name_stone_advance= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_stone_advance, error);		//17
// 	}
// 		if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_STONE_TOWER, temp, 1024) != NULL){
// 		ids_guardtower_top_name_stone_tower= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_stone_tower, temp);
// 	}else{
// 		ids_guardtower_top_name_stone_tower= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_stone_tower, error);		//18
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_TOP_NAME_STONE_GREATLIGHT, temp, 1024) != NULL){
// 		ids_guardtower_top_name_stone_greatlight= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_top_name_stone_greatlight, temp);
// 	}else{
// 		ids_guardtower_top_name_stone_greatlight= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_top_name_stone_greatlight, error);		//19
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_BODY_MODE_CLAY, temp, 1024) != NULL){
// 		ids_guardtower_body_mode_clay= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_body_mode_clay, temp);
// 	}else{
// 		ids_guardtower_body_mode_clay= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_body_mode_clay, error);		//20
// 	}
//     if(LoadString(hInst, IDS_GUARDTOWER_BODY_MODE_STONE, temp, 1024) != NULL){
// 		ids_guardtower_body_mode_stone = new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_body_mode_stone, temp);
// 	}else{
// 		ids_guardtower_body_mode_stone= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_body_mode_stone, error); //21
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_BODY_MODE_STEEL, temp, 1024) != NULL){
// 		ids_guardtower_body_mode_steel= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_body_mode_steel, temp);
// 	}else{
// 		ids_guardtower_body_mode_steel= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_body_mode_steel, error);	//22
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_BODY_LEVEL_NORMAL, temp, 1024) != NULL){
// 		ids_guardtower_body_level_normal= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_body_level_normal, temp);
// 	}else{
// 		ids_guardtower_body_level_normal= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_body_level_normal, error);		//23
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_BODY_LEVEL_MIDDLE, temp, 1024) != NULL){
// 		ids_guardtower_body_level_middle= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_body_level_middle, temp);
// 	}else{
// 		ids_guardtower_body_level_middle= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_body_level_middle, error);		//24
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_BODY_LEVEL_VERYHIGH, temp, 1024) != NULL){
// 		ids_guardtower_body_level_veryhigh= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_body_level_veryhigh, temp);
// 	}else{
// 		ids_guardtower_body_level_veryhigh= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_body_level_veryhigh, error);		//25
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_SHIELD_LEVEL_NONE, temp, 1024) != NULL){
// 		ids_guardtower_shield_level_none= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_shield_level_none, temp);
// 	}else{
// 		ids_guardtower_shield_level_none= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_shield_level_none, error);		//26
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_SHIELD_LEVEL_NORMAL, temp, 1024) != NULL){
// 		ids_guardtower_shield_level_normal= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_shield_level_normal, temp);
// 	}else{
// 		ids_guardtower_shield_level_normal= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_shield_level_normal, error);		//27
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_SHIELD_LEVEL_ALITTLESTRONG, temp, 1024) != NULL){
// 		ids_guardtower_shield_level_alittlestrong= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_shield_level_alittlestrong, temp);
// 	}else{
// 		ids_guardtower_shield_level_alittlestrong= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_shield_level_alittlestrong, error);		//28
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_SHIELD_LEVEL_STRONG, temp, 1024) != NULL){
// 		ids_guardtower_shield_level_strong= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_shield_level_strong, temp);
// 	}else{
// 		ids_guardtower_shield_level_strong= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_shield_level_strong, error);		//29
// 	}
// 	if(LoadString(hInst, IDS_GUARDTOWER_SHIELD_LEVEL_VERYSTRONG, temp, 1024) != NULL){
// 		ids_guardtower_shield_level_verystrong= new char[strlen(temp)+2];
// 		strcpy(ids_guardtower_shield_level_verystrong, temp);
// 	}else{
// 		ids_guardtower_shield_level_verystrong= new char[strlen(error)+2];
// 		strcpy(ids_guardtower_shield_level_verystrong, error);		//30
// 	}

// 	att_mode[0] = ids_guardtower_att_mode_multi;//"다수공격", "% 데미지", "일반공격"};
// 	att_mode[1] = ids_guardtower_att_mode_damage;
// 	att_mode[2] = ids_guardtower_att_mode_normal;
//
// // TOP
// 	top_level[0] = ids_guardtower_top_level_normal;//"보통", "약간높음", "높음", "아주높음"};
// 	top_level[1] = ids_guardtower_top_level_alittlehigh;
// 	top_level[2] = ids_guardtower_top_level_high;
// 	top_level[3] = ids_guardtower_top_level_veryhigh;
//
// 	top_name_fire[0]  = ids_guardtower_top_name_fire_normal;//"노멀 파이어",  "어드밴스 파이어", "파이어 포탑", "그레이트 파이어"};
// 	top_name_fire[1]  = ids_guardtower_top_name_fire_advance;
// 	top_name_fire[2]  = ids_guardtower_top_name_fire_tower;
// 	top_name_fire[3]  = ids_guardtower_top_name_fire_greatfire;
//
// 	top_name_light[0] = ids_guardtower_top_name_light_normal;//"노멀 라이트닝", "어드밴스 라이트닝", "라이트닝 포탑", "그레이트 라이트닝"};
// 	top_name_light[1] = ids_guardtower_top_name_light_advance;
// 	top_name_light[2] = ids_guardtower_top_name_light_tower;
// 	top_name_light[3] = ids_guardtower_top_name_light_greatlight;
//
// 	top_name_stone[0] = ids_guardtower_top_name_stone_normal;//"노멀 스톤", "어드밴스 스톤", "스톤 포탑", "그레이트 스톤"};
// 	top_name_stone[1] = ids_guardtower_top_name_stone_advance;
// 	top_name_stone[2] = ids_guardtower_top_name_stone_tower;
// 	top_name_stone[3] = ids_guardtower_top_name_stone_greatlight;
//
// // BODY
// 	body_mode[0]  = ids_guardtower_body_mode_clay;//"클레이", "스톤", "스틸"};
// 	body_mode[1]  = ids_guardtower_body_mode_stone;
// 	body_mode[2]  = ids_guardtower_body_mode_steel;
//
// 	body_level[0] = ids_guardtower_body_level_normal;//"보통", "중간", "아주높음"};
// 	body_level[1] = ids_guardtower_body_level_middle;
// 	body_level[2] = ids_guardtower_body_level_veryhigh;
//
//
// // SHIELD
// 	shield_level[0] = ids_guardtower_shield_level_none;//"없음", "보통", "약간강함", "강함", "매우강함"};
// 	shield_level[1] = ids_guardtower_shield_level_normal;
// 	shield_level[2] = ids_guardtower_shield_level_alittlestrong;
// 	shield_level[3] = ids_guardtower_shield_level_strong;
// 	shield_level[4] = ids_guardtower_shield_level_verystrong;
}


void	GuardTowerPage_DeleteDLLStringTable()
{
	SAFE_DELETE_ARRAY(ids_guardtower_att_mode_multi);
	SAFE_DELETE_ARRAY(ids_guardtower_att_mode_damage);
	SAFE_DELETE_ARRAY(ids_guardtower_att_mode_normal);
	SAFE_DELETE_ARRAY(ids_guardtower_top_level_normal);
	SAFE_DELETE_ARRAY(ids_guardtower_top_level_alittlehigh);
	SAFE_DELETE_ARRAY(ids_guardtower_top_level_high);
	SAFE_DELETE_ARRAY(ids_guardtower_top_level_veryhigh);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_fire_normal);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_fire_advance);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_fire_tower);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_fire_greatfire);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_light_normal);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_light_advance);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_light_tower);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_light_greatlight);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_stone_normal);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_stone_advance);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_stone_tower);
	SAFE_DELETE_ARRAY(ids_guardtower_top_name_stone_greatlight);
	SAFE_DELETE_ARRAY(ids_guardtower_body_mode_clay);
	SAFE_DELETE_ARRAY(ids_guardtower_body_mode_stone);
	SAFE_DELETE_ARRAY(ids_guardtower_body_mode_steel);
	SAFE_DELETE_ARRAY(ids_guardtower_body_level_normal);
	SAFE_DELETE_ARRAY(ids_guardtower_body_level_middle);
	SAFE_DELETE_ARRAY(ids_guardtower_body_level_veryhigh);
	SAFE_DELETE_ARRAY(ids_guardtower_shield_level_none);
	SAFE_DELETE_ARRAY(ids_guardtower_shield_level_normal);
	SAFE_DELETE_ARRAY(ids_guardtower_shield_level_alittlestrong);
	SAFE_DELETE_ARRAY(ids_guardtower_shield_level_strong);
	SAFE_DELETE_ARRAY(ids_guardtower_shield_level_verystrong);
}

// ATTACT MODE


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuardTower::CGuardTower(int vnum, int idx, int shield)
	: m_bActive(TRUE)
{
	int i;
	if(!g_pDisplay || !GET_D3DDEVICE())
		return;

	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);

	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;

	//------------------------------------------------------------
	//	BACKGROUND
	int X = 0, Y = 0;
	m_pBack		=  NULL;
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/guard_tower/guard_tower.bmp");
	if( m_pBack )
	{
		m_pBack->SetColorKey(RGB(0, 0, 0));
		m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY =m_pBack->Ypos;
	}

	//------------------------------------------------------------
	//	SURFACE TOP, BODY, SHIELD
	for(i=0; i<_NUM_TOP; i++)
		m_pSurTop[i] = NULL;
	for(i=0; i<_NUM_BODY; i++)
		m_pSurBody[i] = NULL;
	for(i=0; i<_NUM_SHIELD; i++)
		m_pSurShield[i] = NULL;

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurTop[0], "interface/guard_tower/top_fire.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurTop[1], "interface/guard_tower/top_light.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurTop[2], "interface/guard_tower/top_stone.bmp");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurBody[0], "interface/guard_tower/body_klay.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurBody[1], "interface/guard_tower/body_stone.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurBody[2], "interface/guard_tower/body_steel.bmp");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurShield[0], "interface/guard_tower/shield0.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurShield[1], "interface/guard_tower/shield1.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurShield[2], "interface/guard_tower/shield2.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurShield[3], "interface/guard_tower/shield3.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurShield[4], "interface/guard_tower/shield4.bmp");

	for(i=0; i<_NUM_ATT_MODE; i++)
	{
		if( m_pSurTop[i] )
		{
			m_pSurTop[i]->SetColorKey(RGB(0, 0, 0));
			m_pSurTop[i]->Xpos = X+62;
			m_pSurTop[i]->Ypos = Y+20;
		}
	}
	for(i=0; i<_NUM_BODY; i++)
	{
		if( m_pSurBody[i] )
		{
			m_pSurBody[i]->SetColorKey(RGB(0, 0, 0));
			m_pSurBody[i]->Xpos = X+62;
			m_pSurBody[i]->Ypos = Y+66;
		}
	}
	for(i=0; i<_NUM_SHIELD; i++)
	{
		if( m_pSurShield[i] )
		{
			m_pSurShield[i]->SetColorKey(RGB(0, 0, 0));
			m_pSurShield[i]->Xpos = X+81;
			m_pSurShield[i]->Ypos = Y+206;
		}
	}

	//------------------------------------------------------------
	//	BUTTON
	m_pBtn1		=  NULL;
	m_pBtn2		=  NULL;
	m_pBtn1		= new CBasicButton();
	m_pBtn2		= new CBasicButton();
	if( m_pBtn1 )
		m_pBtn1->SetFileName("common/btn_buy");
	if( m_pBtn2 )
		m_pBtn2->SetFileName("common/btn_cancel");
	if( m_pBtn1 )
		m_pBtn1->LoadRes();
	if( m_pBtn2 )
		m_pBtn2->LoadRes();

	if( m_pBtn1 )
		m_pBtn1->PosX	= X+62;
	if( m_pBtn1 )
		m_pBtn1->PosY	= Y+318;
	if( m_pBtn2 )
		m_pBtn2->PosX	= X+217;
	if( m_pBtn2 )
		m_pBtn2->PosY	= Y+318;


	for(i=0; i<3; i++)
	{
		m_pBtnRight[i] = NULL;
		m_pBtnLeft[i] = NULL;
		m_pBtnRight[i] = new CBasicButton();
		m_pBtnLeft[i] = new CBasicButton();

		if( m_pBtnRight[i] )
			m_pBtnRight[i]->SetFileName("common/btn_rarrow");
		if( m_pBtnLeft[i] )
			m_pBtnLeft[i]->SetFileName("common/btn_larrow");
		if( m_pBtnRight[i] )
			m_pBtnRight[i]->LoadRes();
		if( m_pBtnLeft[i] )
			m_pBtnLeft[i]->LoadRes();
	}
	if( m_pBtnRight[0] )
		m_pBtnRight[0]->PosX = X+152;
	if( m_pBtnRight[0] )
		m_pBtnRight[0]->PosY = Y+30;
	if( m_pBtnRight[1] )
		m_pBtnRight[1]->PosX = X+152;
	if( m_pBtnRight[1] )
		m_pBtnRight[1]->PosY = Y+123;
	if( m_pBtnRight[2] )
		m_pBtnRight[2]->PosX = X+152;
	if( m_pBtnRight[2] )
		m_pBtnRight[2]->PosY = Y+212;

	if( m_pBtnLeft[0] )
		m_pBtnLeft[0]->PosX = X+33;
	if( m_pBtnLeft[0] )
		m_pBtnLeft[0]->PosY = Y+30;
	if( m_pBtnLeft[1] )
		m_pBtnLeft[1]->PosX = X+33;
	if( m_pBtnLeft[1] )
		m_pBtnLeft[1]->PosY = Y+123;
	if( m_pBtnLeft[2] )
		m_pBtnLeft[2]->PosX = X+33;
	if( m_pBtnLeft[2] )
		m_pBtnLeft[2]->PosY = Y+212;

	//------------------------------------------------------------
	//	TEXTOUT
	m_pTop_mode			=  NULL;
	m_pTop_level		=  NULL;
	m_pTop_name			=  NULL;
	m_pBody_name		=  NULL;
	m_pBody_level		=  NULL;
	m_pShield			=  NULL;
	m_pCur_money		=  NULL;
	m_pPay_money		=  NULL;

	m_pTop_mode			=  new CTextOutBox();
	m_pTop_level		=  new CTextOutBox();
	m_pTop_name			=  new CTextOutBox();
	m_pBody_name		=  new CTextOutBox();
	m_pBody_level		=  new CTextOutBox();
	m_pShield			=  new CTextOutBox();
	m_pCur_money		=  new CTextOutBox();
	m_pPay_money		=  new CTextOutBox();

	if( m_pTop_mode )
		m_pTop_mode->Init(12, _FCOLOR_SKY,0, 0);
	if( m_pTop_level )
		m_pTop_level->Init(12, _FCOLOR_GREEN,0, 0);
	if( m_pTop_name )
		m_pTop_name->Init(12, _FCOLOR_RED,0, 0);
	if( m_pBody_name )
		m_pBody_name->Init(12, _FCOLOR_RED,0, 0);
	if( m_pBody_level )
		m_pBody_level->Init(12, _FCOLOR_GREEN,0, 0);
	if( m_pShield )
		m_pShield->Init(12, _FCOLOR_GREEN,0, 0);
	if( m_pCur_money )
		m_pCur_money->Init(12, _FCOLOR_YELLOW,0, 0);
	if( m_pPay_money )
		m_pPay_money->Init(12, _FCOLOR_YELLOW,0, 0);

	/*파이어 클레이 수호탑 : 179
	파이어 스톤 수호탑 : 180
	파이어 스틸 수호탑 : 181
	라이트닝 클레이 수호탑 : 182
	라이트닝 스톤 수호탑 : 183
	라이트닝 스틸 수호탑 : 184
	스톤 클레이 수호탑 : 185
	스톤 스톤 수호탑 : 186
	스톤 스틸 수호탑 : 187*/

	/*price_top_fire[0].x = 51840;// 건설비용
	price_top_fire[0].y = (price_top_fire[0].x*3)/100; // 보상비용 이다..
	price_top_fire[1].x = 5760000;
	price_top_fire[1].y = 1728000;
	price_top_fire[2].x = 12960000;
	price_top_fire[2].y = 3888000;
	price_top_fire[3].x = 29160000;
	price_top_fire[3].y = 8748000;

	price_top_light[0].x = 250000;
	price_top_light[0].y = 75000;
	price_top_light[1].x = 1562500;
	price_top_light[1].y = 468750;
	price_top_light[2].x = 6250000;
	price_top_light[2].y = 1875000;
	price_top_light[3].x = 24502500;
	price_top_light[3].y = 7350750;

	price_top_stone[0].x = 1440000;
	price_top_stone[0].y = 432000;
	price_top_stone[1].x = 3240000;
	price_top_stone[1].y = 972000;
	price_top_stone[2].x = 7290000;
	price_top_stone[2].y = 2187000;
	price_top_stone[3].x = 16402500;
	price_top_stone[3].y = 4920750;

	price_body[0].x = 1000000;
	price_body[0].y = 300000;
	price_body[1].x = 3000000;
	price_body[1].y = 900000;
	price_body[2].x = 9000000;
	price_body[2].y = 2700000;

	price_shield[0].x = 0;
	price_shield[0].y = 0;
	price_shield[1].x = 320000;
	price_shield[1].y = 96000;
	price_shield[2].x = 720000;
	price_shield[2].y = 216000;
	price_shield[3].x = 1620000;
	price_shield[3].y = 486000;
	price_shield[4].x = 3645000;
	price_shield[4].y = 1093500;	*/


	m_iprice_top[0] = 116640; // 파이어
	m_iprice_top[1] = 18750; // 라이트
	m_iprice_top[2] = 38880; // 스톤

	m_iprice_body[0] = 90000; // 클레이
	m_iprice_body[1] = 270000; // 스톤
	m_iprice_body[2] = 810000; // 스틸

	m_iprice_shield[0] = 0; // 없음
	m_iprice_shield[1] = 38400; // 보통
	m_iprice_shield[2] = 86400; // 약간 강함
	m_iprice_shield[3] = 194400; // 강함
	m_iprice_shield[4] = 437400; // 매우강함

	GuardTowerPage_LoadDLLStringTable();

	switch(vnum)
	{
	case 179:
		m_iAtt_mode = 0;		// fire, light, stone
		m_iBody_mode = 0;		// "클레이", "스톤", "스틸"
		break;
	case 180:
		m_iAtt_mode = 0;		// fire, light, stone
		m_iBody_mode = 1;		// "클레이", "스톤", "스틸"
		break;
	case 181:
		m_iAtt_mode = 0;		// fire, light, stone
		m_iBody_mode = 2;		// "클레이", "스톤", "스틸"
		break;
	case 182:
		m_iAtt_mode = 1;		// fire, light, stone
		m_iBody_mode = 0;		// "클레이", "스톤", "스틸"
		break;
	case 183:
		m_iAtt_mode = 1;		// fire, light, stone
		m_iBody_mode = 1;		// "클레이", "스톤", "스틸"
		break;
	case 184:
		m_iAtt_mode = 1;		// fire, light, stone
		m_iBody_mode = 2;		// "클레이", "스톤", "스틸"
		break;
	case 185:
		m_iAtt_mode = 2;		// fire, light, stone
		m_iBody_mode = 0;		// "클레이", "스톤", "스틸"
		break;
	case 186:
		m_iAtt_mode = 2;		// fire, light, stone
		m_iBody_mode = 1;		// "클레이", "스톤", "스틸"
		break;
	case 187:
		m_iAtt_mode = 2;		// fire, light, stone
		m_iBody_mode = 2;		// "클레이", "스톤", "스틸"
		break;
	}

	m_iTop_level = 2;		// "보통", "약간높음", "높음", "아주높음"
	m_iBody_level = 1;		// "보통", "높음", "아주높음"
	m_iShield_level = shield;	// "없음", "보통", "약간강한", "강한", "매우강한"
	m_iMobIdx = idx;

	m_iOld_Att_mode = m_iAtt_mode;
	m_iOld_Body_mode = m_iBody_mode;
	m_iOld_Shield_level = m_iShield_level;

	if( m_pTop_mode )
		m_pTop_mode->SetString(att_mode[m_iAtt_mode]);
	if( m_pTop_level )
		m_pTop_level->SetString(top_level[m_iTop_level]);

	if( m_pTop_name )
	{
		if(m_iAtt_mode == 0)
			m_pTop_name->SetString(top_name_fire[m_iTop_level]);
		else if(m_iAtt_mode == 1)
			m_pTop_name->SetString(top_name_light[m_iTop_level]);
		else if(m_iAtt_mode == 2)
			m_pTop_name->SetString(top_name_stone[m_iTop_level]);
	}

	if( m_pBody_level )
		m_pBody_level->SetString(body_level[m_iBody_level]);
	if( m_pBody_name )
		m_pBody_name->SetString(body_mode[m_iBody_mode]);
	if( m_pShield )
		m_pShield->SetString(shield_level[m_iShield_level]);
	if( m_pCur_money )
		m_pCur_money->SetString(g_pRoh->m_Money,true);
	if( m_pPay_money )
		m_pPay_money->SetString("0");

	m_iPrice_Top =  m_iPrice_Body = m_iPrice_Shield = 0;
}

CGuardTower::~CGuardTower()
{
	SAFE_DELETE(m_pBack);

	int i = 0;
	for(i=0; i<_NUM_ATT_MODE; i++)
		SAFE_DELETE(m_pSurTop[i]);
	for(i=0; i<_NUM_BODY; i++)
		SAFE_DELETE(m_pSurBody[i]);
	for(i=0; i<_NUM_SHIELD; i++)
		SAFE_DELETE(m_pSurShield[i]);

	for(i=0; i<3; i++)
	{
		SAFE_DELETE(m_pBtnRight[i]);
		SAFE_DELETE(m_pBtnLeft[i]);
	}

	SAFE_DELETE(m_pBtn1);
	SAFE_DELETE(m_pBtn2);
	SAFE_DELETE(m_pTop_mode);
	SAFE_DELETE(m_pTop_level);
	SAFE_DELETE(m_pTop_name);
	SAFE_DELETE(m_pBody_name);
	SAFE_DELETE(m_pBody_level);
	SAFE_DELETE(m_pShield);
	SAFE_DELETE(m_pCur_money);
	SAFE_DELETE(m_pPay_money);

	GuardTowerPage_DeleteDLLStringTable();
}




void CGuardTower::Draw()
{
	if( !g_pDisplay || !m_pBack )
		return;

	RenderOj(m_pBack->Xpos, m_pBack->Ypos, m_pBack->GetWidth(), m_pBack->GetHeight());

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	g_pDisplay->Blt(m_pSurBody[m_iBody_mode]->Xpos, m_pSurBody[m_iBody_mode]->Ypos, m_pSurBody[m_iBody_mode]);
	g_pDisplay->Blt(m_pSurTop[m_iAtt_mode]->Xpos, m_pSurTop[m_iAtt_mode]->Ypos, m_pSurTop[m_iAtt_mode]);
	g_pDisplay->Blt(m_pSurShield[m_iShield_level]->Xpos, m_pSurShield[m_iShield_level]->Ypos, m_pSurShield[m_iShield_level]);


	for(int i=0; i<3; i++)
	{
		m_pBtnRight[i]->Draw();
		m_pBtnLeft[i]->Draw();
	}

	if( m_pBtn1 )
		m_pBtn1->Draw();
	if( m_pBtn2 )
		m_pBtn2->Draw();

	if( m_pTop_name )
		m_pTop_name->SetPos(m_iMainX+228+25+((LARGE_TEXTBOX_CX/2)-m_pTop_name->GetBoxWidth()/2), m_iMainY+39+5);
	if( m_pTop_level )
		m_pTop_level->SetPos(m_iMainX+250+25+(SMALL_TEXTBOX_CX/2-m_pTop_level->GetBoxWidth()/2), m_iMainY+64+5);
	if( m_pTop_mode )
		m_pTop_mode->SetPos(m_iMainX+250+25+(SMALL_TEXTBOX_CX/2-m_pTop_mode->GetBoxWidth()/2), m_iMainY+90+5);
	if( m_pBody_name )
		m_pBody_name->SetPos(m_iMainX+228+25+(LARGE_TEXTBOX_CX/2-m_pBody_name->GetBoxWidth()/2), m_iMainY+145+5);
	if( m_pBody_level )
		m_pBody_level->SetPos(m_iMainX+250+25+(SMALL_TEXTBOX_CX/2-m_pBody_level->GetBoxWidth()/2), m_iMainY+172+5);
	if( m_pShield )
		m_pShield->SetPos(m_iMainX+228+25+(LARGE_TEXTBOX_CX/2-m_pShield->GetBoxWidth()/2), m_iMainY+225+5);
	if( m_pCur_money )
		m_pCur_money->SetPos(m_iMainX+84+25+(SMALL2_TEXTBOX_CX/2-m_pCur_money->GetBoxWidth()/2), m_iMainY+280+5);
	if( m_pPay_money )
		m_pPay_money->SetPos(m_iMainX+258+25+(SMALL2_TEXTBOX_CX/2-m_pPay_money->GetBoxWidth()/2), m_iMainY+280+5);

	if( m_pTop_mode )
		m_pTop_mode->Draw();
	if( m_pTop_level )
		m_pTop_level->Draw();
	if( m_pTop_name )
		m_pTop_name->Draw();
	if( m_pBody_name )
		m_pBody_name->Draw();
	if( m_pBody_level )
		m_pBody_level->Draw();
	if( m_pShield )
		m_pShield->Draw();
	if( m_pCur_money )
		m_pCur_money->Draw();
	if( m_pPay_money )
		m_pPay_money->Draw();
}



LRESULT CGuardTower::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y, i;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if(IsInside(x, y))
		{
			if( m_pBtn1 )
				m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn2 )
				m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			for(i=0; i<3; i++)
			{
				if(m_pBtnRight[i] && m_pBtnRight[i]->m_iBtnFlg != BTN_DISABLE)
					m_pBtnRight[i]->MsgProc(hWnd, msg, wParam, lParam);
				if(m_pBtnLeft[i] && m_pBtnLeft[i]->m_iBtnFlg != BTN_DISABLE)
					m_pBtnLeft[i]->MsgProc(hWnd, msg, wParam, lParam);
			}
			m_bActive = TRUE;
			return 4;
		}
		else
		{
			m_bActive=FALSE;
			return 2;
		}
		break;
	case WM_LBUTTONUP:

		if(m_bActive)
		{
			if( m_pBtn1 )
				m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn2 )
				m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);

			for(i=0; i<3; i++)
			{
				if( m_pBtnRight[i] )
					m_pBtnRight[i]->MsgProc(hWnd, msg, wParam, lParam);
				if( m_pBtnLeft[i] )
					m_pBtnLeft[i]->MsgProc(hWnd, msg, wParam, lParam);
			}
			if(m_pBtnRight[0] && m_pBtnRight[0]->m_iBtnFlg == BTN_ACTION)
			{
				// top - right click
				ChangeTop(0);
				ShowCurPrice();
				return 1;
			}
			else if(m_pBtnRight[1] && m_pBtnRight[1]->m_iBtnFlg == BTN_ACTION)
			{
				// body - right click
				ChangeBody(0);
				ShowCurPrice();
				return 1;
			}
			else if(m_pBtnRight[2] && m_pBtnRight[2]->m_iBtnFlg == BTN_ACTION)
			{
				// shield - right click
				ChangeShield(0);
				ShowCurPrice();
				return 1;
			}
			else if(m_pBtnLeft[0] && m_pBtnLeft[0]->m_iBtnFlg == BTN_ACTION)
			{
				// top - left click
				ChangeTop(1);
				ShowCurPrice();
				return 1;
			}
			else if(m_pBtnLeft[1] && m_pBtnLeft[1]->m_iBtnFlg == BTN_ACTION)
			{
				// body - left click
				ChangeBody(1);
				ShowCurPrice();
				return 1;
			}
			else if(m_pBtnLeft[2] && m_pBtnLeft[2]->m_iBtnFlg == BTN_ACTION)
			{
				// shield - left click
				ChangeShield(1);
				ShowCurPrice();
				return 1;
			}
			else if(m_pBtn1 && m_pBtn1->m_iBtnFlg == BTN_ACTION)  // 확인 버튼
			{
				SendChMsg();
				return 2;
			}
			else if(m_pBtn2 && m_pBtn2->m_iBtnFlg == BTN_ACTION)  // 취소 버튼
			{
				return 2;
			}
			for(i=0; i<3; i++)
			{
				if(m_pBtnRight[i] && m_pBtnRight[i]->m_iBtnFlg != BTN_DISABLE)
					m_pBtnRight[i]->m_iBtnFlg = BTN_NORMAL;
				if(m_pBtnLeft[i] && m_pBtnLeft[i]->m_iBtnFlg != BTN_DISABLE)
					m_pBtnLeft[i]->m_iBtnFlg = BTN_NORMAL;
			}
			if( m_pBtn1 )
				m_pBtn1->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtn2 )
				m_pBtn2->m_iBtnFlg = BTN_NORMAL;

			break;
		}
		else
		{
			return 2;
		}
		break;
	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			for(i=0; i<3; i++)
			{
				if(m_pBtnRight[i] && m_pBtnRight[i]->m_iBtnFlg != BTN_DISABLE)
					m_pBtnRight[i]->MsgProc(hWnd, msg, wParam, lParam);
				if(m_pBtnLeft[i] && m_pBtnLeft[i]->m_iBtnFlg != BTN_DISABLE)
					m_pBtnLeft[i]->MsgProc(hWnd, msg, wParam, lParam);
			}
			if( m_pBtn1 )
				m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn2 )
				m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		break;
	}

	return 0;
}


void CGuardTower::ShowCurPrice()
{
	char tmp[30] = "";
	if( m_pPay_money )
		m_pPay_money->SetString(m_iPrice_Top+m_iPrice_Body+m_iPrice_Shield,true);
}

void CGuardTower::SendChMsg()
{
	int a, b;
	a = m_iAtt_mode;
	b = m_iBody_mode;

	int idx = m_iMobIdx;
	int shield = m_iShield_level;
	int vnum;

	if(a==0 && b==0)   vnum = 179;
	else if(a==0 && b==1)	vnum = 180;
	else if(a==0 && b==2)	vnum = 181;
	else if(a==1 && b==0)	vnum = 182;
	else if(a==1 && b==1)	vnum = 183;
	else if(a==1 && b==2)	vnum = 184;
	else if(a==2 && b==0)	vnum = 185;
	else if(a==2 && b==1)	vnum = 186;
	else if(a==2 && b==2)	vnum = 187;


	char tmp[100] = "";
	sprintf(tmp, "tchange %d %d %d\n", idx, vnum, shield);
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);
}


int CGuardTower::GetCurPrice_Body()
{

	if(m_iBody_mode == m_iOld_Body_mode)
		return 0;

	return  m_iprice_body[m_iBody_mode] - (m_iprice_body[m_iOld_Body_mode] * 30)/100;
}

int CGuardTower::GetCurPrice_Shield()
{

	if(m_iShield_level == m_iOld_Shield_level)
		return 0;

	return m_iprice_shield[m_iShield_level] - (m_iprice_shield[m_iOld_Shield_level]*30) / 100;
}


int CGuardTower::GetCurPrice_Top()
{
	if(m_iAtt_mode == m_iOld_Att_mode)
		return 0;

	return m_iprice_top[m_iAtt_mode] - (m_iprice_top[m_iOld_Att_mode]*30)/100;
}


void CGuardTower::ChangeTop(int right_left)
{
	if(right_left == 0)
	{
		// right1 click
		if(m_iAtt_mode == _NUM_ATT_MODE-1)
		{
			m_iAtt_mode = 0;
		}
		else
		{
			m_iAtt_mode++;
		}

	}
	else
	{
		// left1 click
		if(m_iAtt_mode == 0)
		{
			m_iAtt_mode = _NUM_ATT_MODE-1;

		}
		else
		{
			m_iAtt_mode--;
		}
	}
	if( m_pTop_mode )
		m_pTop_mode->SetString(att_mode[m_iAtt_mode]);

	if( m_pTop_name )
	{
		if(m_iAtt_mode == 0)
			m_pTop_name->SetString(top_name_fire[m_iTop_level]);
		else if(m_iAtt_mode == 1)
			m_pTop_name->SetString(top_name_light[m_iTop_level]);
		else if(m_iAtt_mode == 2)
			m_pTop_name->SetString(top_name_stone[m_iTop_level]);
	}

	m_iPrice_Top = GetCurPrice_Top();
}

void CGuardTower::ChangeBody(int right_left)
{
	if(right_left == 0)
	{
		// right1 click
		if(m_iBody_mode == _NUM_BODY-1)
		{
			m_iBody_mode = 0;

		}
		else
		{
			m_iBody_mode++;
		}

	}
	else
	{
		// left1 click
		if(m_iBody_mode == 0)
		{
			m_iBody_mode = _NUM_BODY-1;
		}
		else
		{
			m_iBody_mode--;
		}
	}
	if( m_pBody_name )
		m_pBody_name->SetString(body_mode[m_iBody_mode]);

	m_iPrice_Body = GetCurPrice_Body();
}

void CGuardTower::ChangeShield(int right_left)
{
	if(right_left == 0)
	{
		// right1 click
		if(m_iShield_level == _NUM_SHIELD-1)
		{
			m_iShield_level = 0;
		}
		else
		{
			m_iShield_level++;
		}
	}
	else
	{
		// left1 click
		if(m_iShield_level == 0)
		{
			m_iShield_level = _NUM_SHIELD-1;
		}
		else
		{
			m_iShield_level--;
		}
	}
	if( m_pShield )
		m_pShield->SetString(shield_level[m_iShield_level]);

	m_iPrice_Shield  = GetCurPrice_Shield();
}

void CGuardTower::RenderOj(int xpos, int ypos, int cx, int cy)
{
	if( !GET_D3DDEVICE() )
		return;

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ScreenWidth;
	vpNew.dwHeight = m_ScreenHeight;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	TLVec[0] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos,      0), 1, 0x00ffffff, 0, 0, 0);
	TLVec[1] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos,     0), 1, 0x00ffffff, 0, 1, 0);
	TLVec[2] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos+cy,  0), 1, 0x00ffffff, 0, 0, 1);
	TLVec[3] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos+cy,  0), 1, 0x00ffffff, 0, 1, 1);


	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_TEXTR_BLACK2 ]);


		GET_D3DDEVICE()->SetTexture(0, lpTexture);
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);
		GET_D3DDEVICE()->EndScene();
	}
	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}


BOOL CGuardTower::IsInside(int x, int y)
{
	if( !m_pBack )
		return false;

	return m_pBack->IsIn(x,y);
}