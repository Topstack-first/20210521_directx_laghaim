#include "stdafx.h"


#include "main.h"
#include "UIMgr.h"
#include "headers.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "controlPetStatus.h"
#include "ControlInven.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "Item.h"
#include "g_stringmanager.h"
#include "createcharacterpage.h"
#include "Mouse.h"

extern DWORD				g_dwLangType;
extern int g_PetSkillGroup[PET_EVOLUTION_TYPENUM][PET_SKILL_MAX];

//	by evilkiki 2008.08.04
//	return value 위험성으로 함수 수정
extern void GetPetSkillName( int skill_index , OUT char * rv); // 펫 스킬 인덱스로 부터 이름을 얻어오는 함수.
//extern char *GetPetSkillName( int skill_index );

#define MYPET_BAG_X			190
#define MYPET_BAG_Y			221
#define MYPET_BAG_X_NEW		190
#define MYPET_BAG_Y_NEW		221
#define MYPET_BAG_WIDTH		54
#define MYPET_BAG_HEIGHT	54

#define MYPET_NECKLACE_X			201
#define MYPET_NECKLACE_Y			157
#define MYPET_NECKLACE_WIDTH		26
#define MYPET_NECKLACE_HEIGHT		26

#define MYPET_DRAKE_ITEM_WIDTH		54
#define MYPET_DRAKE_ITEM_HEIGHT		54


CControlPetStatus::CControlPetStatus()
	: m_ScreenCx(0)
	, m_ScreenCy(0)
	, m_iMainX(0)
	, m_iMainY(0)
	, m_NeckXpos(0)
	, m_NeckYpos(0)
	, m_slot_index(0)
	, m_newpet_offset_y(213)
{
	m_pBack  = NULL;
	m_pBackNew = NULL;
	m_pBtn_Close = NULL;
	m_ModeYGap = 0;

	int i = 0;
	for(i=0; i<NUM_MAX_INFO; i++)
	{
		// nate 2005-04-06
		// 스트링 최기화 값이 없어서 쓰레기값이 출력됬었음
		m_Info[ i ].str[ 0 ] = NULL;
		m_Info[i].text_sur = NULL;
	}

	m_bBagLock = FALSE;
	m_timeInvenLock = 0;

	m_DrawSkillIconX = 0;
	m_DrawSkillIconY = 0;

	for( i = 0 ; i < PET_SKILL_MAX*PET_EVOLUTION_TYPENUM ; ++i )
	{
		m_pSkillIcon[i] = NULL; // 스킬을 배웠는지의 아이콘. 스킬 갯수 만큼 가지고 있는다.
	}
	m_pSkillIconName = NULL;
}


CControlPetStatus::~CControlPetStatus()
{
	DeleteRes();
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBackNew);
	SAFE_DELETE(m_pBtn_Close);

	int i = 0;
	for(i=0; i<NUM_MAX_INFO; i++)
		SAFE_DELETE(m_Info[i].text_sur);

	for( i = 0 ; i < PET_SKILL_MAX*PET_EVOLUTION_TYPENUM ; ++i )
		SAFE_DELETE(m_pSkillIcon[i]);

	SAFE_DELETE( m_pSkillIconName );
}

void CControlPetStatus::LoadRes()
{
	if(!g_pDisplay)
		return;

	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);
	m_ScreenCx = rc.right;
	m_ScreenCy = rc.bottom;

	int close_btn_x, close_btn_y;
	//-------------------------------------------------------------------
	// BACKGROUND
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/PetStatus/pet_status.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBackNew, "interface/PetStatus/pet_inven.bmp");	
	
	if( m_pBack )
	{
		m_pBack->SetColorKey( TRANS_COLOR );
		m_pBack->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBack->GetWidth();
		m_pBack->Ypos = 0;

		close_btn_x = m_pBack->Xpos+212;
		close_btn_y = m_pBack->Ypos+317;
		m_NeckXpos = m_pBack->Xpos + 216;
		m_NeckYpos = m_pBack->Ypos + 204;
		m_DrawSkillIconX = m_pBack->Xpos+60;
		m_DrawSkillIconY = m_pBack->Ypos+390;		
	}

	if( m_pBackNew )
	{
		m_pBackNew->SetColorKey( TRANS_COLOR );
		m_pBackNew->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBackNew->GetWidth();
		m_pBackNew->Ypos = 0;
	}

	//-------------------------------------------------------------------
	// BUTTON
	m_pBtn_Close	= new CBasicButton();
	if( m_pBtn_Close )
	{
		m_pBtn_Close->SetFileName("common/btn_close_01");
		m_pBtn_Close->LoadRes();
		m_pBtn_Close->SetPosition(m_pBack->Xpos + 271 , m_pBack->Ypos + 5 );
	}
	//-------------------------------------------------------------------
	// TEXTOUTBOX
	int i = 0;
	for(i=0; i<NUM_MAX_INFO; i++)
	{
		m_Info[i].text_sur = new CTextOutBox();
		if( m_Info[i].text_sur )
		{
			m_Info[i].text_sur->Init(12, RGB(255, 210, 0),0, 0);
		}
	}

	m_Info[PetState_Info__Name].x = m_pBack->Xpos+57;// 이름
	m_Info[PetState_Info__Name].y = m_pBack->Ypos+79;
	m_Info[PetState_Info__Name].cx =211;
	m_Info[PetState_Info__Lv].x = m_pBack->Xpos+57;// 레벨
	m_Info[PetState_Info__Lv].y = m_pBack->Ypos+103;
	m_Info[PetState_Info__Lv].cx =107;
	m_Info[PetState_Info__Exp].x = m_pBack->Xpos+211;// 경험치
	m_Info[PetState_Info__Exp].y = m_pBack->Ypos+103;
	m_Info[PetState_Info__Exp].cx = 58;
	m_Info[PetState_Info__x1].x = m_pBack->Xpos+57;// 충성도
	m_Info[PetState_Info__x1].y = m_pBack->Ypos+123;
	m_Info[PetState_Info__x1].cx = 107;
	m_Info[PetState_Info__x2].x = m_pBack->Xpos+211;// 포만도
	m_Info[PetState_Info__x2].y = m_pBack->Ypos+123;
	m_Info[PetState_Info__x2].cx = 58;
	m_Info[PetState_Info__Hp].x = m_pBack->Xpos+57;// 생명력
	m_Info[PetState_Info__Hp].y = m_pBack->Ypos+161;
	m_Info[PetState_Info__Hp].cx = 107;
	m_Info[PetState_Info__NA].x = m_pBack->Xpos+57;// 근접공격
	m_Info[PetState_Info__NA].y = m_pBack->Ypos+201;
	m_Info[PetState_Info__NA].cx = 107;
	m_Info[PetState_Info__FA].x = m_pBack->Xpos+57;// 원격공격
	m_Info[PetState_Info__FA].y = m_pBack->Ypos+223;
	m_Info[PetState_Info__FA].cx = 107;
	m_Info[PetState_Info__ND].x = m_pBack->Xpos+57;// 근접방어
	m_Info[PetState_Info__ND].y = m_pBack->Ypos+265;
	m_Info[PetState_Info__ND].cx = 107;
	m_Info[PetState_Info__FD].x = m_pBack->Xpos+57;// 원격방어
	m_Info[PetState_Info__FD].y = m_pBack->Ypos+287;
	m_Info[PetState_Info__FD].cx = 107;
	m_Info[PetState_Info__MixLv].x = m_pBack->Xpos+57;// 조합레벨
	m_Info[PetState_Info__MixLv].y = m_pBack->Ypos+330;
	m_Info[PetState_Info__MixLv].cx = 107;
	m_Info[PetState_Info__Speed].x = m_pBack->Xpos+57;// 이동속도
	m_Info[PetState_Info__Speed].y = m_pBack->Ypos+351;
	m_Info[PetState_Info__Speed].cx = 107;
	m_Info[PetState_Info__GetLv].x = m_pBack->Xpos+57;// 획득레벨
	m_Info[PetState_Info__GetLv].y = m_pBack->Ypos+373;
	m_Info[PetState_Info__GetLv].cx = 107;

	for( i = 0 ; i < PET_SKILL_MAX*PET_EVOLUTION_TYPENUM ; ++i )
	{
		// BUTTON
		m_pSkillIcon[i]	= new CBasicButton();
		if( m_pSkillIcon[i] )
		{
			char strTemp[256];
			sprintf( strTemp, "skill/pet_skill/icon_skill%03d", i );

			m_pSkillIcon[i]->SetFileName( strTemp );
			m_pSkillIcon[i]->LoadRes();
		}
	}

	m_pSkillIconName = new CAlphaPopUp;
	if( m_pSkillIconName )
	{
		m_pSkillIconName->Init(14);
		m_pSkillIconName->LoadRes( m_ScreenCx, m_ScreenCy );
	}

	SetEquipArea();

	// 글자 복구
	RestoreSurfaces();
}

void CControlPetStatus::SetEquipArea()
{
	m_EquipSlot[PET_PIECE_HORN].m_X = m_pBackNew->Xpos + 174 - 60;
	m_EquipSlot[PET_PIECE_HORN].m_Y = m_pBackNew->Ypos + 93;
	m_EquipSlot[PET_PIECE_HORN].m_W = MYPET_DRAKE_ITEM_WIDTH;
	m_EquipSlot[PET_PIECE_HORN].m_H = MYPET_DRAKE_ITEM_HEIGHT;

	m_EquipSlot[PET_PIECE_HEAD].m_X = m_pBackNew->Xpos + 174;
	m_EquipSlot[PET_PIECE_HEAD].m_Y = m_pBackNew->Ypos + 93;
	m_EquipSlot[PET_PIECE_HEAD].m_W = MYPET_DRAKE_ITEM_WIDTH;
	m_EquipSlot[PET_PIECE_HEAD].m_H = MYPET_DRAKE_ITEM_HEIGHT;

	m_EquipSlot[PET_PIECE_BODY].m_X = m_pBackNew->Xpos + 153;
	m_EquipSlot[PET_PIECE_BODY].m_Y = m_pBackNew->Ypos + 161;
	m_EquipSlot[PET_PIECE_BODY].m_W = MYPET_DRAKE_ITEM_WIDTH;
	m_EquipSlot[PET_PIECE_BODY].m_H = MYPET_DRAKE_ITEM_HEIGHT;

	m_EquipSlot[PET_PIECE_WINGS].m_X = m_pBackNew->Xpos + 38;
	m_EquipSlot[PET_PIECE_WINGS].m_Y = m_pBackNew->Ypos + 128;
	m_EquipSlot[PET_PIECE_WINGS].m_W = MYPET_DRAKE_ITEM_WIDTH;
	m_EquipSlot[PET_PIECE_WINGS].m_H = MYPET_DRAKE_ITEM_HEIGHT;

	m_EquipSlot[PET_PIECE_LEGS].m_X = m_pBackNew->Xpos + 156;
	m_EquipSlot[PET_PIECE_LEGS].m_Y = m_pBackNew->Ypos + 229;
	m_EquipSlot[PET_PIECE_LEGS].m_W = MYPET_DRAKE_ITEM_WIDTH;
	m_EquipSlot[PET_PIECE_LEGS].m_H = MYPET_DRAKE_ITEM_HEIGHT;

	m_EquipSlot[PET_PIECE_TAIL].m_X = m_pBackNew->Xpos + 42;
	m_EquipSlot[PET_PIECE_TAIL].m_Y = m_pBackNew->Ypos + 217;
	m_EquipSlot[PET_PIECE_TAIL].m_W = MYPET_DRAKE_ITEM_WIDTH;
	m_EquipSlot[PET_PIECE_TAIL].m_H = MYPET_DRAKE_ITEM_HEIGHT;	
}

void CControlPetStatus::Reposition()
{
	if( g_MyPetStatus.m_Class >= 3 )
	{
		if( m_pBackNew )
		{
			m_NeckXpos = m_pBackNew->Xpos + 219;
			m_NeckYpos = m_pBackNew->Ypos + 412;			
			m_DrawSkillIconX = m_pBackNew->Xpos + 173;
			m_DrawSkillIconY = m_pBackNew->Ypos + 364;
		}

		m_Info[PetState_Info__Name].x = m_pBack->Xpos+61;// 이름
		m_Info[PetState_Info__Name].y = m_pBack->Ypos + m_newpet_offset_y +79;
		m_Info[PetState_Info__Name].cx =211;
		m_Info[PetState_Info__Lv].x = m_pBack->Xpos+61;// 레벨
		m_Info[PetState_Info__Lv].y = m_pBack->Ypos + m_newpet_offset_y +103;
		m_Info[PetState_Info__Lv].cx =107;
		m_Info[PetState_Info__Exp].x = m_pBack->Xpos+215;// 경험치
		m_Info[PetState_Info__Exp].y = m_pBack->Ypos + m_newpet_offset_y +103;
		m_Info[PetState_Info__Exp].cx = 58;
		m_Info[PetState_Info__x1].x = m_pBack->Xpos+61;// 충성도
		m_Info[PetState_Info__x1].y = m_pBack->Ypos + m_newpet_offset_y +123;
		m_Info[PetState_Info__x1].cx = 107;
		m_Info[PetState_Info__x2].x = m_pBack->Xpos+215;// 포만도
		m_Info[PetState_Info__x2].y = m_pBack->Ypos + m_newpet_offset_y +123;
		m_Info[PetState_Info__x2].cx = 58;
		m_Info[PetState_Info__Hp].x = m_pBack->Xpos+61;// 생명력
		m_Info[PetState_Info__Hp].y = m_pBack->Ypos + m_newpet_offset_y +159;
		m_Info[PetState_Info__Hp].cx = 107;
		m_Info[PetState_Info__NA].x = m_pBack->Xpos+61;// 근접공격
		m_Info[PetState_Info__NA].y = m_pBack->Ypos + m_newpet_offset_y +197;
		m_Info[PetState_Info__NA].cx = 107;
		m_Info[PetState_Info__FA].x = m_pBack->Xpos+61;// 원격공격
		m_Info[PetState_Info__FA].y = m_pBack->Ypos + m_newpet_offset_y +219;
		m_Info[PetState_Info__FA].cx = 107;
		m_Info[PetState_Info__ND].x = m_pBack->Xpos+61;// 근접방어
		m_Info[PetState_Info__ND].y = m_pBack->Ypos + m_newpet_offset_y +259;
		m_Info[PetState_Info__ND].cx = 107;
		m_Info[PetState_Info__FD].x = m_pBack->Xpos+61;// 원격방어
		m_Info[PetState_Info__FD].y = m_pBack->Ypos + m_newpet_offset_y +281;
		m_Info[PetState_Info__FD].cx = 107;
		m_Info[PetState_Info__MixLv].x = m_pBack->Xpos+61;// 조합레벨
		m_Info[PetState_Info__MixLv].y = m_pBack->Ypos + m_newpet_offset_y +309;
		m_Info[PetState_Info__MixLv].cx = 107;
		m_Info[PetState_Info__Speed].x = m_pBack->Xpos+61;// 이동속도
		m_Info[PetState_Info__Speed].y = m_pBack->Ypos + m_newpet_offset_y +330;
		m_Info[PetState_Info__Speed].cx = 107;
		m_Info[PetState_Info__GetLv].x = m_pBack->Xpos+61;// 획득레벨
		m_Info[PetState_Info__GetLv].y = m_pBack->Ypos + m_newpet_offset_y +352;
		m_Info[PetState_Info__GetLv].cx = 107;
	}
	else
	{	
		if( m_pBack )
		{
			m_NeckXpos = m_pBack->Xpos + 216;
			m_NeckYpos = m_pBack->Ypos + 204;
			m_DrawSkillIconX = m_pBack->Xpos+60;
			m_DrawSkillIconY = m_pBack->Ypos+390;	
		}		

		m_Info[PetState_Info__Name].x = m_pBack->Xpos+57;// 이름
		m_Info[PetState_Info__Name].y = m_pBack->Ypos+79;
		m_Info[PetState_Info__Name].cx =211;
		m_Info[PetState_Info__Lv].x = m_pBack->Xpos+57;// 레벨
		m_Info[PetState_Info__Lv].y = m_pBack->Ypos+103;
		m_Info[PetState_Info__Lv].cx =107;
		m_Info[PetState_Info__Exp].x = m_pBack->Xpos+211;// 경험치
		m_Info[PetState_Info__Exp].y = m_pBack->Ypos+103;
		m_Info[PetState_Info__Exp].cx = 58;
		m_Info[PetState_Info__x1].x = m_pBack->Xpos+57;// 충성도
		m_Info[PetState_Info__x1].y = m_pBack->Ypos+123;
		m_Info[PetState_Info__x1].cx = 107;
		m_Info[PetState_Info__x2].x = m_pBack->Xpos+211;// 포만도
		m_Info[PetState_Info__x2].y = m_pBack->Ypos+123;
		m_Info[PetState_Info__x2].cx = 58;
		m_Info[PetState_Info__Hp].x = m_pBack->Xpos+57;// 생명력
		m_Info[PetState_Info__Hp].y = m_pBack->Ypos+161;
		m_Info[PetState_Info__Hp].cx = 107;
		m_Info[PetState_Info__NA].x = m_pBack->Xpos+57;// 근접공격
		m_Info[PetState_Info__NA].y = m_pBack->Ypos+201;
		m_Info[PetState_Info__NA].cx = 107;
		m_Info[PetState_Info__FA].x = m_pBack->Xpos+57;// 원격공격
		m_Info[PetState_Info__FA].y = m_pBack->Ypos+223;
		m_Info[PetState_Info__FA].cx = 107;
		m_Info[PetState_Info__ND].x = m_pBack->Xpos+57;// 근접방어
		m_Info[PetState_Info__ND].y = m_pBack->Ypos+265;
		m_Info[PetState_Info__ND].cx = 107;
		m_Info[PetState_Info__FD].x = m_pBack->Xpos+57;// 원격방어
		m_Info[PetState_Info__FD].y = m_pBack->Ypos+287;
		m_Info[PetState_Info__FD].cx = 107;
		m_Info[PetState_Info__MixLv].x = m_pBack->Xpos+57;// 조합레벨
		m_Info[PetState_Info__MixLv].y = m_pBack->Ypos+330;
		m_Info[PetState_Info__MixLv].cx = 107;
		m_Info[PetState_Info__Speed].x = m_pBack->Xpos+57;// 이동속도
		m_Info[PetState_Info__Speed].y = m_pBack->Ypos+351;
		m_Info[PetState_Info__Speed].cx = 107;
		m_Info[PetState_Info__GetLv].x = m_pBack->Xpos+57;// 획득레벨
		m_Info[PetState_Info__GetLv].y = m_pBack->Ypos+373;
		m_Info[PetState_Info__GetLv].cx = 107;
	}
}

void CControlPetStatus::DeleteRes()
{
	SAFE_SURFACE_DESTROY(m_pBack);
	SAFE_SURFACE_DESTROY(m_pBackNew);
	SAFE_DELETE_RES(m_pBtn_Close);

	int i = 0;
	for(i=0; i<NUM_MAX_INFO; i++)
		SAFE_DELETE_RES(m_Info[i].text_sur);

	for( i = 0 ; i < PET_SKILL_MAX*PET_EVOLUTION_TYPENUM ; ++i )
		SAFE_DELETE_RES(m_pSkillIcon[i]);

	SAFE_DELETE_RES( m_pSkillIconName );
}

CItem* CControlPetStatus::Draw()
{
	if( !g_pDisplay )
		return NULL;

	if( g_MyPetStatus.m_Class >= 3 )
	{
		if( m_pBackNew )
			g_pDisplay->Blt(m_pBackNew->Xpos, m_pBackNew->Ypos, m_pBackNew);
	}
	else
	{
		if( m_pBack )
			g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	}
	if( m_pBtn_Close )	m_pBtn_Close->Draw();

	int i = 0;
	for(i=0; i<NUM_MAX_INFO; i++)
	{
		if(m_Info[i].text_sur)
			m_Info[i].text_sur->Draw();
	}

	// 바구니/목걸이 그려주기
	if (g_pNk2DFrame && g_pNk2DFrame->GetItemRender() )
	{
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();

		if( g_MyPetStatus.m_Class >= 3 )
		{
			if (g_MyPetStatus.m_pBag)
			{
				g_pNk2DFrame->RenderItemInUI( m_pBack->Xpos + 209, m_pBack->Ypos + 474,
					g_MyPetStatus.m_pBag , FALSE, FALSE, TRUE, TRUE );
			}
			if (g_MyPetStatus.m_pNecklace)
			{
				g_pNk2DFrame->RenderItemInUI( m_pBack->Xpos + 216, m_pBack->Ypos + 416 ,
					g_MyPetStatus.m_pNecklace , FALSE, FALSE, FALSE, TRUE );
			}
		}
		else
		{
			if (g_MyPetStatus.m_pBag)
			{
				g_pNk2DFrame->RenderItemInUI( m_pBack->Xpos + 205, m_pBack->Ypos + 264,
					g_MyPetStatus.m_pBag , FALSE, FALSE, TRUE, TRUE );
			}
			if (g_MyPetStatus.m_pNecklace)
			{
				g_pNk2DFrame->RenderItemInUI( m_pBack->Xpos + 216, m_pBack->Ypos + 203 ,
					g_MyPetStatus.m_pNecklace , FALSE, FALSE, FALSE, TRUE );
			}
		}		

		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}

	if( g_MyPetStatus.m_Class )
	{
		int SkillIconDrawCount = 0;
		int SkillIndex = 0; // 서버랑 주고 받는 스킬 인덱스.
		int MouseOnSkillIndex = -1; // 마우스가 올려져있는 스킬의 인덱스.

		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);
		CMultiText multiText;

		for( i = 0 ; i < PET_SKILL_MAX ; ++i ) // 자기 펫이 배울수있는 스킬 갯수 만큼.
		{
			SkillIndex = g_PetSkillGroup[g_MyPetStatus.m_Class-1][i];
			m_pSkillIcon[SkillIndex]->Draw( m_DrawSkillIconX+(SkillIconDrawCount*SIZE_OF_SKILL_ICON), m_DrawSkillIconY, !g_MyPetStatus.m_SkillLevel[i] ); // 스킬을 배웠는지의 아이콘. 스킬 갯수 만큼 가지고 있는다.
			++SkillIconDrawCount;

			// 마우스 온 체크.
			if( m_pSkillIconName )
			{
				if( m_pSkillIcon[SkillIndex]
						&& (point.x >= m_pSkillIcon[SkillIndex]->PosX && point.x < m_pSkillIcon[SkillIndex]->PosX + m_pSkillIcon[SkillIndex]->Width)
						&& (point.y >= m_pSkillIcon[SkillIndex]->PosY && point.y < m_pSkillIcon[SkillIndex]->PosY + m_pSkillIcon[SkillIndex]->Height) )
				{
					MouseOnSkillIndex = SkillIndex;
				}
			}
		}

		// 아이콘을 다 그린후 이름 팝업을 그려야 위에 그려진다.
		if( MouseOnSkillIndex != -1 ) // 올라간게 있으면...
		{
			if( m_pSkillIconName->GetIndex() != MouseOnSkillIndex ) // 이전 보던게 아닐때만...
			{
				char strTemp[50];
				GetPetSkillName(MouseOnSkillIndex, strTemp);
				multiText.AddString( strTemp, RGB(255, 255, 255) );
				m_pSkillIconName->PutString( &multiText );
				m_pSkillIconName->SetIndex( MouseOnSkillIndex );
			}
			m_pSkillIconName->Draw( point.x, point.y );
		}
	}

	CItem *pOverItem = NULL;
	if( g_MyPetStatus.m_Class == 3 )
	{
		pOverItem = DrawEquip();
	}

	if( pOverItem != NULL )
	{
		g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE;
	}

	return pOverItem;
}

CItem* CControlPetStatus::DrawEquip()
{
	CItem *pOverItem = NULL;

	if( g_pNk2DFrame->GetItemRender() && g_pRoh )
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);		

		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();
		// 장비 그리기...
		for (int i = 0; i < PET_PIECE_NUM; i++)
		{
			if( g_MyPetStatus.m_Wearing[i] == NULL )
				continue;

			CLolos* pLolo = GET_LOLO(g_MyPetStatus.m_Wearing[i]);
			if( pLolo == NULL )
				continue;

			g_pNk2DFrame->RenderItemInUI( m_EquipSlot[i].m_X + (m_EquipSlot[i].m_W - ITEM_RENDER_WIDTH) / 2 ,
				m_EquipSlot[i].m_Y + (m_EquipSlot[i].m_H - ITEM_RENDER_HEIGHT) / 2 ,
				g_MyPetStatus.m_Wearing[i] , FALSE ,FALSE, TRUE, TRUE);

			if (point.x >= m_EquipSlot[i].m_X
				&& point.x < m_EquipSlot[i].m_X + m_EquipSlot[i].m_W
				&& point.y >= m_EquipSlot[i].m_Y
				&& point.y < m_EquipSlot[i].m_Y + m_EquipSlot[i].m_H)
			{
				pOverItem = g_MyPetStatus.m_Wearing[i];
			}
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}

	return pOverItem;
}

void CControlPetStatus::SetPetName(char* name)
{
	if(name != NULL)
	{
		m_Info[PetState_Info__Name].text_sur->SetString(name);
		strcpy(m_Info[PetState_Info__Name].str, m_Info[PetState_Info__Name].text_sur->m_str);
	}
}

void CControlPetStatus::SetPetStatus(int type, __int64 val, int val2)
{
	char tmp[30];
	int value = 0;
	int plus = 0;
	if(type == 0)  // PET_VITAL{
	{
		if (pCMyApp->m_pMyPet)
			value = pCMyApp->m_pMyPet->m_MaxVital;
		sprintf(tmp, "%lld/%d(%d)", val, value, val2);
		if( m_Info[5].text_sur )
			m_Info[5].text_sur->SetString(tmp);
		strcpy(m_Info[5].str, tmp);
	}
	else if(type == 1)  //PET_DAM_RANGE
	{
		if (pCMyApp->m_pMyPet)
			value = pCMyApp->m_pMyPet->m_RangeAtt;
		plus = value*g_MyPetStatus.loyalty/200;
		sprintf(tmp, "%d(%lld)%s%d", value, val, ((plus >= 0) ? "+" : ""), plus);
		if( m_Info[7].text_sur )
			m_Info[7].text_sur->SetString(tmp);
		strcpy(m_Info[7].str, m_Info[7].text_sur->m_str);
	}
	else if(type == 2)   //PET_DAM_MELEE
	{
		if (pCMyApp->m_pMyPet)
			value = pCMyApp->m_pMyPet->m_MeleeAtt;
		plus = value*g_MyPetStatus.loyalty/200;
		sprintf(tmp, "%d(%lld)%s%d", value, val, ((plus >= 0) ? "+" : ""), plus);
		if( m_Info[6].text_sur )
			m_Info[6].text_sur->SetString(tmp);
		strcpy(m_Info[6].str, m_Info[6].text_sur->m_str);
	}
	else if(type == 3)   //PET_ARMOR_RANGE
	{
		if (pCMyApp->m_pMyPet)
			value = pCMyApp->m_pMyPet->m_RangeDef;
		sprintf(tmp, "%d(%lld)", value, val);
		if( m_Info[9].text_sur )
			m_Info[9].text_sur->SetString(tmp);
		strcpy(m_Info[9].str, m_Info[9].text_sur->m_str);
	}
	else if(type == 4)  //PET_ARMOR_MELEE
	{
		if (pCMyApp->m_pMyPet)
			value = pCMyApp->m_pMyPet->m_MeleeDef;
		sprintf(tmp, "%d(%lld)", value, val);
		if( m_Info[8].text_sur )
			m_Info[8].text_sur->SetString(tmp);
		strcpy(m_Info[8].str, m_Info[8].text_sur->m_str);
	}
	else if(type == 5)  //PET_SPEED
	{
		if( m_Info[11].text_sur )
			m_Info[11].text_sur->SetString(val);
		strcpy(m_Info[11].str, m_Info[11].text_sur->m_str);
	}
	else if(type == 6)  //PET_COMPOSE
	{
		if( m_Info[10].text_sur )
			m_Info[10].text_sur->SetString(val);
		strcpy(m_Info[10].str, m_Info[10].text_sur->m_str);
	}
	else if(type == 7)  //PET_PICK
	{
		if( m_Info[12].text_sur )
			m_Info[12].text_sur->SetString(val);
		strcpy(m_Info[12].str, m_Info[12].text_sur->m_str);
	}
	else if(type == 8)  //PET_LOYALTY
	{
		if( m_Info[3].text_sur )
			m_Info[3].text_sur->SetString(val);
		strcpy(m_Info[3].str, m_Info[3].text_sur->m_str);
		SetPetStatus(PET_DAM_RANGE, g_MyPetStatus.range_att);
		SetPetStatus(PET_DAM_MELEE, g_MyPetStatus.melee_att);
	}
	else if(type == 9)  //PET_HUNGER
	{
		if( m_Info[4].text_sur )
			m_Info[4].text_sur->SetString(val);
		strcpy(m_Info[4].str, m_Info[4].text_sur->m_str);
	}
	else if(type == 10)  //PET_LEVEL
	{
		if( m_Info[1].text_sur )
			m_Info[1].text_sur->SetString(val);
		strcpy(m_Info[1].str, m_Info[1].text_sur->m_str);
	}
	else if(type == 11)  //PET_EXP
	{
		if( m_Info[2].text_sur )
			m_Info[2].text_sur->SetString(val);
		strcpy(m_Info[2].str, m_Info[2].text_sur->m_str);
	}

	for(int i=0; i<NUM_MAX_INFO; i++)
	{
		if( m_Info[i].text_sur )
			m_Info[i].text_sur->SetPos((m_Info[i].x+m_Info[i].cx/2)- m_Info[i].text_sur->m_Width/2, m_Info[i].y);
	}
}

LRESULT CControlPetStatus::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !g_pRoh )
		return 0;

	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if(IsInside(x, y))
		{
			if (IsNecklaceSide(x, y))
			{
				DWORD curTime = timeGetTime();
				if (curTime > m_timeInvenLock + 1000) // 1초가 지났으면
					m_bBagLock = false;
				else
					return 1;

				if(!m_bBagLock)
				{
					m_bBagLock = true;
					m_timeInvenLock = timeGetTime();
					if (g_pRoh->m_ExtraSlot)
					{
						//넣을때
						if (g_pRoh->m_ExtraSlot->m_Vnum == 966 || g_pRoh->m_ExtraSlot->m_Vnum == 967)
						{
							if( g_MyPetStatus.m_Class != 0 ) // 진화 펫은 966,967 못낌. ///라비용진화
							{
								//-- IDS_CANT_USE_ITEM : 해당 아이템을 사용할 수 없습니다.
								g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANT_USE_ITEM), TYPE_NOR_OK, 1 );
								return 1;

							}
							else if( (g_pRoh->m_ExtraSlot->m_Vnum == 966 && g_MyPetStatus.level < 80)
									 || (g_pRoh->m_ExtraSlot->m_Vnum == 967 && g_MyPetStatus.level < 60) )
							{
								//-- IDS_CANNOT_WEAR_ITEM : 착용 가능 레벨이 아닙니다.
								g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANNOT_WEAR_ITEM), TYPE_NOR_OK, 1 );
								return 1;

							}
							g_pRoh->m_ExtraSlot = g_MyPetStatus.SetNecklace(g_pRoh->m_ExtraSlot);
							LH_SEND_NET_MSG("p_bag neck\n");
						}
					}
					else
					{
						//뺄때
						g_pRoh->m_ExtraSlot = g_MyPetStatus.SetNecklace(g_pRoh->m_ExtraSlot);
						if (!(g_pRoh->m_ExtraSlot))
						{
							// 암짓도 안한거여~~!!
						}
						else
						{
							LH_SEND_NET_MSG("p_bag neck\n");
						}
					}
					return 1;
				}
			}
			if( m_pBtn_Close )
				m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
		}
		break;

	case WM_LBUTTONUP:
		if(m_pBtn_Close && IsInside(x, y))
		{
			m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtn_Close->m_iBtnFlg ==  BTN_ACTION)
			{
				// 창을 닫아라~
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowPetStatusWindow(FALSE);
				m_pBtn_Close->m_iBtnFlg = BTN_NORMAL;
				return 1;
			}
			m_pBtn_Close->m_iBtnFlg = BTN_NORMAL;
		}
		break;

	case WM_MOUSEMOVE:
		{
			if(IsInside(x, y) == 1)
			{
				pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
				return 1;
			}	
		}
		break;
	}
	return 0;

}

LRESULT	CControlPetStatus::ProcInven(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	if( g_pRoh->m_ExtraSlot == NULL )
		return 1;

	if( IsInsideEquip(x, y) == FALSE )
		return 1;

	// ##TODO : DrawItemInfo		

	return 1;
}

BOOL CControlPetStatus::IsInsideEquip(int x, int y)
{
	if (x >= m_EquipSlot[PET_PIECE_WINGS].m_X
		&& x < m_EquipSlot[PET_PIECE_HEAD].m_X + m_EquipSlot[PET_PIECE_HEAD].m_W
		&& y >= m_EquipSlot[PET_PIECE_HEAD].m_Y
		&& y < m_EquipSlot[PET_PIECE_LEGS].m_Y + m_EquipSlot[PET_PIECE_LEGS].m_H )
		return TRUE;
	return FALSE;
}

void CControlPetStatus::RestoreSurfaces()
{
	for(int i=0; i<NUM_MAX_INFO; i++)
	{
		if( m_Info[i].text_sur )
		{
			m_Info[i].text_sur->SetString(m_Info[i].str);
			m_Info[i].text_sur->SetPos((m_Info[i].x+m_Info[i].cx/2)- m_Info[i].text_sur->m_Width/2, m_Info[i].y);
		}
	}
}


BOOL CControlPetStatus::IsInside(int x, int y)
{
	if( g_MyPetStatus.m_Class == 3 )
	{
		if( !m_pBackNew )
			return FALSE;
		return m_pBackNew->IsIn( x, y );
	}

	if( !m_pBack )
		return FALSE;
	return m_pBack->IsIn( x, y );
}


BOOL CControlPetStatus::IsBagSide(int x, int y)
{
	if( g_MyPetStatus.m_Class == 3 )
	{
		if (x >= (m_pBack->Xpos + MYPET_BAG_X)
			&& x < (m_pBack->Xpos + MYPET_BAG_X + MYPET_BAG_WIDTH)
			&& y >= (m_pBack->Ypos + MYPET_BAG_Y_NEW)
			&& y < (m_pBack->Ypos + MYPET_BAG_Y_NEW + MYPET_BAG_HEIGHT) )
			return TRUE;
	}
	else
	{
		if (x >= (m_pBack->Xpos + MYPET_BAG_X)
			&& x < (m_pBack->Xpos + MYPET_BAG_X + MYPET_BAG_WIDTH)
			&& y >= (m_pBack->Ypos + MYPET_BAG_Y)
			&& y < (m_pBack->Ypos + MYPET_BAG_Y + MYPET_BAG_HEIGHT) )
			return TRUE;
	}	

	return FALSE;
}

BOOL CControlPetStatus::IsNecklaceSide(int x, int y)
{
	if (x >= (m_NeckXpos)
			&& x < (m_NeckXpos + MYPET_NECKLACE_WIDTH)
			&& y >= (m_NeckYpos)
			&& y < (m_NeckYpos + MYPET_NECKLACE_HEIGHT) )
		return TRUE;

	return FALSE;
}

LRESULT CControlPetStatus::NamePopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	if (nMsg == 1)
	{
		// 어떠한 검색을 해줘야할까~
		char buf[40] = "";
		int  len = 0;
		strcpy(buf, szMoney);
		len = strlen(buf);

		if(len ==0)
		{
			//-- IDS_UIMGR_PET_INPUT_NAME : 이름을 입력해 주세요.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_PET_INPUT_NAME), TYPE_EXCHANGE, 22 );
			return 1;
		}
		else if(len < 4 || len > 12)
		{
			//-- IDS_UIMGR_PET_MAX_STR : 이름은 4~12자 이내 이어야 합니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_PET_MAX_STR), TYPE_EXCHANGE, 22 );
			return 1;
		}

		for(int i=0; i<len; i++)
		{
			if(buf[i] == *" ")
			{
				//-- IDS_UIMGR_PET_UNABLE_EMPTY : 공백문자는 사용할 수 없습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_PET_UNABLE_EMPTY), TYPE_EXCHANGE, 22 );
				return 1;
			}
			else if(buf[i] == *"'")
			{
				//-- IDS_UIMGR_PET_UNABLE_STR : '작은따옴표'는 사용할 수 없습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_PET_UNABLE_STR), TYPE_EXCHANGE, 22 );
				return 1;
			}
		}


		char commuout[100] = "";
		if( m_slot_index < 0 )
			return 0;
		
		pCMyApp->m_pUIMgr->UsePremium( m_slot_index , szMoney );

		return 1;
	}

	return 0L;
}