#include "stdafx.h"
#define D3D_OVERLOADS


#include <assert.h>
#include <d3d.h>
#include "Compas.h"
#include "IndexedTexture.h"
// nate 2004 - 7 m_pTestTexture제거
#include "ImageDefine.h"
#include "main.h"
#include "Nk2DFrame.h"
#include "ControlMap.h"
#include "UIMgr.h"
#include "NkMob.h"

extern int		gMapWidth;							// 맵의 x축 넓이
extern int		gMapHeight;
extern int		gMapExtention;					// 스케일 팩터 - 정수형





using namespace std;
extern BOOL g_bDungeon;

CCompas::CCompas()
	: m_fDir(0)
	, m_fMobDir(0)
	, m_iCurDeckadum(0)
{
	m_pCross = NULL;
	m_Mag = 1.0f;
	m_nHalfWidth = COMPAS_HALF_WIDTH_BASE;
	m_nHalfHeight = COMPAS_HALF_HEIGHT_BASE;

	// 좌측 하단에서 시계방향으로 정의.
	m_SrcCompasTurn[0] = D3DTLVERTEX(D3DVECTOR((-COMPAS_HALF_WIDTH_TURN), COMPAS_HALF_HEIGHT_TURN, 0), 1, 0xffffffff, 0, 0, 1.0f );
	m_SrcCompasTurn[1] = D3DTLVERTEX(D3DVECTOR((-COMPAS_HALF_WIDTH_TURN), -COMPAS_HALF_HEIGHT_TURN, 0), 1, 0xffffffff, 0, 0, 0 );
	m_SrcCompasTurn[2] = D3DTLVERTEX(D3DVECTOR(COMPAS_HALF_WIDTH_TURN, -COMPAS_HALF_HEIGHT_TURN, 0), 1, 0xffffffff, 0, 1.0f, 0 );
	m_SrcCompasTurn[3] = D3DTLVERTEX(D3DVECTOR(COMPAS_HALF_WIDTH_TURN, COMPAS_HALF_HEIGHT_TURN, 0), 1, 0xffffffff, 0, 1.0f, 1.0f );

	m_OutCompas[0] = m_SrcCompasTurn[0];
	m_OutCompas[1] = m_SrcCompasTurn[1];
	m_OutCompas[2] = m_SrcCompasTurn[2];
	m_OutCompas[3] = m_SrcCompasTurn[3];

	m_wIdx[0] = 0 ;
	m_wIdx[1] = 1;
	m_wIdx[2] = 2;
	m_wIdx[3] = 0;
	m_wIdx[4] = 2;
	m_wIdx[5] = 3;

	m_ScreenWidth = 800;
	m_ScreenHeight = 600;
	m_Scale = 1.0f;

	m_CurMapPosX = 0;
	m_CurMapPosZ = 0;
}

CCompas::~CCompas()
{
	DeleteRes();
}

// 그림 파일은 북쪽이 오른쪽으로 가 있어야 한다.
void CCompas::LoadRes()
{
	if( !g_pDisplay )
		return;

	DeleteRes();

	g_pDisplay->CreateSurfaceFromBitmap(&m_pCross, "data/effect/textr/2d/cross.bmp");
	if( m_pCross )
		m_pCross->SetColorKey(RGB(0, 0, 0));

	m_PosBox.Init(PLAYERPOS_FONTSIZE, RGB(252, 166, 76), 0, 0, -1, -1, TRUE);
	m_PosBox.SetVisible(TRUE);

	m_bg.Init( "compas/base" );
	if( g_pNk2DFrame )
		m_bg.Align(3,g_pNk2DFrame->GetClientWidth(),g_pNk2DFrame->GetClientHeight());
}

void CCompas::DeleteRes()
{
	SAFE_DELETE( m_pCross );

	m_PosBox.DeleteRes();
	m_bg.DeleteRes();
}

void CCompas::Init(int width, int height)
{
	m_ScreenWidth = width;
	m_ScreenHeight = height;

	m_nHalfWidth = (int) COMPAS_HALF_WIDTH_BASE;
	m_nHalfHeight = (int) COMPAS_HALF_HEIGHT_BASE;

	m_SrcCompasTurn[0] = D3DTLVERTEX(D3DVECTOR(-(COMPAS_HALF_WIDTH_TURN), (COMPAS_HALF_HEIGHT_TURN), 0), 1, 0xffffffff, 0, 0, 1.0f );
	m_SrcCompasTurn[1] = D3DTLVERTEX(D3DVECTOR(-(COMPAS_HALF_WIDTH_TURN), -(COMPAS_HALF_HEIGHT_TURN), 0), 1, 0xffffffff, 0, 0, 0 );
	m_SrcCompasTurn[2] = D3DTLVERTEX(D3DVECTOR((COMPAS_HALF_WIDTH_TURN), -(COMPAS_HALF_HEIGHT_TURN), 0), 1, 0xffffffff, 0, 1.0f, 0 );
	m_SrcCompasTurn[3] = D3DTLVERTEX(D3DVECTOR((COMPAS_HALF_WIDTH_TURN), (COMPAS_HALF_HEIGHT_TURN), 0), 1, 0xffffffff, 0, 1.0f, 1.0f );
}

void CCompas::Scale(float scale)
{
}

////////////////////////////////////////////////////////////////////
// Name : CalDirection
// Desc : 카메라와 플레이어의 각을 이용하여 나침반의 회전각을 정한다.
//  카메라에서 플레이어를 봤을때, 나침반은 카메라와 반대방향으로 회전을 한다.
//  즉 나침반의 움직임은 카메라에서 플레이어를 봤을때, 반시계 방향으로 회전을 하며
//  위상차는 180도이다.
////////////////////////////////////////////////////////////////////
void CCompas::CalDirection(D3DVECTOR &vCam, D3DVECTOR &vAt)
{
	D3DVECTOR vDir;

	vDir.x = vAt.x - vCam.x;
	vDir.y = vAt.y - vCam.y;
	vDir.z = vAt.z - vCam.z;

	// 카메라와 플레이어의 각도는 x와 z로 측정을 한다.
	m_fDir = atan2f(vDir.z, vDir.x);		// 카메라에서 플레이어를 본 각도.
	m_fMobDir = - m_fDir + (g_PI / 2);
	m_fDir = m_fDir + g_PI;

	if (m_fDir >= g_2_PI)
		m_fDir -= g_2_PI;
}

void CCompas::Render(int sx, int sy)
{
	if (!g_pNk2DFrame->GetbViewTop())
        return;
	m_bg.Draw();

	if( !GET_D3DDEVICE() )
		return;

	D3DUtil_SetRotateZMatrix( m_matRotateZ, m_fDir );
	for (int i=0; i < 4; i++)
		D3DMath_VectorMatrixMultiply((D3DVECTOR &)m_OutCompas[i], (D3DVECTOR &)m_SrcCompasTurn[i], m_matRotateZ);

	for (int i=0; i < 4; i++)
	{
		m_OutCompas[i].sx += (g_pNk2DFrame->GetClientWidth() - COMPAS_HALF_WIDTH_TURN - 5 );
		m_OutCompas[i].sy += (COMPAS_HALF_WIDTH_TURN + 8) ;
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DWORD dwMag;
	GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_MAGFILTER, &dwMag);
	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_COMPAS_TURN ] ) );
	// 렌더링..
	GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, m_OutCompas, 4, m_wIdx, 6, NULL);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, dwMag);

	RECT rect, rect2, rect3, rect4;
	rect2.left = 6;
	rect2.top = 0;
	rect2.right = 9;
	rect2.bottom = 3;
	rect3.left = 9;
	rect3.top = 0;
	rect3.right = 12;
	rect3.bottom = 3;
	rect4.left = 12;
	rect4.top = 0;
	rect4.right = 15;
	rect4.bottom = 3;

	m_Mag = 2;
	float mobMag = 25 / m_Mag;
	if (g_bDungeon)
		mobMag /= 1.5;

	D3DUtil_SetRotateZMatrix( m_matRotateZ, m_fMobDir);
	rect.left = 0;
	rect.top = 0;
	rect.right = 3;
	rect.bottom = 3;

	CNkMob *pMob;
	LHMobMap_it prMob;
	if( pCMyApp->m_pUIMgr )
	{
		for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
		{
			// 공격 대상 찾기.
			pMob = (*prMob).second;

			if (pMob && !(pMob->IsDead()) && !(pMob->m_Flag & NPC_NORADER))
			{
				D3DVECTOR mobVect = D3DVECTOR(pMob->m_wx - g_pRoh->m_wx, pMob->m_wz - g_pRoh->m_wz, 0);
				D3DMath_VectorMatrixMultiply(mobVect, mobVect, m_matRotateZ);
				if ((pMob->m_Flag & NPC_SHOPKEEPER) || (pMob->m_Flag & NPC_GUARD) || (pMob->m_Flag & NPC_SUMMON))
				{}
				else if( g_pDisplay )
				{
					int xx = mobVect.x / mobMag + ( g_pNk2DFrame->GetClientWidth() - COMPAS_HALF_WIDTH_TURN - 5);
					//int yy = -mobVect.y / mobMag + (sy);//+ COMPAS_HALF_WIDTH_TURN );
					int yy = -mobVect.y / mobMag + (sy);//+ COMPAS_HALF_WIDTH_TURN );
					g_pDisplay->Blt(xx, yy, m_pCross, &rect);
				}
			}
		}
	}

	rect.left = 3;
	rect.top = 0;
	rect.right = 6;
	rect.bottom = 3;

	CNkCharacter *pCha;
	LHCharacterMap_it prCha;

	BOOL bCrystalOwner = FALSE;
	CNkCharacter *pNkOwner = NULL;
	if( pCMyApp->m_pUIMgr )
	{
		for (prCha = pCMyApp->m_pUIMgr->m_character_map.begin(); prCha != pCMyApp->m_pUIMgr->m_character_map.end(); prCha++)
		{
			// 공격 대상 찾기.
			pCha = (*prCha).second;

			if (pCha && !(pCha->IsDead()))
			{
				D3DVECTOR mobVect = D3DVECTOR(pCha->m_wx - g_pRoh->m_wx, pCha->m_wz - g_pRoh->m_wz, 0);
				D3DMath_VectorMatrixMultiply(mobVect, mobVect, m_matRotateZ);

				int xx = mobVect.x / mobMag + ( g_pNk2DFrame->GetClientWidth() - COMPAS_HALF_WIDTH_TURN - 5 );
				int yy = -mobVect.y / mobMag + (sy );//+ COMPAS_HALF_WIDTH_TURN );

				if (pCha->m_bCrystalOwner)
				{
					bCrystalOwner = TRUE;
					pNkOwner = pCha;
				}
				else if(g_pDisplay && pCha->m_bMyGuild)//길드원은 파란색
					g_pDisplay->Blt(xx, yy, m_pCross, &rect3);
				else if(g_pDisplay && pCha->m_bMyParty)//파티원은 흰색.
					g_pDisplay->Blt(xx, yy, m_pCross, &rect4);
				else if( g_pDisplay )
					g_pDisplay->Blt(xx, yy, m_pCross, &rect);
			}
		}
	}

	// 수정을 가지고 있는 사람을 맨 마지막에 그린다.
	if (g_pDisplay && bCrystalOwner)
	{
		D3DVECTOR mobVect = D3DVECTOR(pNkOwner->m_wx - g_pRoh->m_wx, pNkOwner->m_wz - g_pRoh->m_wz, 0);
		D3DMath_VectorMatrixMultiply(mobVect, mobVect, m_matRotateZ);
		g_pDisplay->Blt(mobVect.x / mobMag + sx -1, -mobVect.y / mobMag + sy -1, mobVect.x / mobMag + sx -1 + 5, -mobVect.y / mobMag + sy -1+5, m_pCross, &rect2);	// 공성전 땀시
	}

	Show_Pos_Text();
}


BOOL CCompas::IsInside(int x, int y)
{
	return m_bg.IsIn(x,y);
}

//----------------------------------------------------------------------------
//-- CCompas::Show_Pos_Text()
//----------------------------------------------------------------------------
//-- 레이다에 현재 좌표를 표시한다.
//-- 2004.11.02
//-- Lyul / 수정완료
//-- AUTO 를 방지 하기 위해 마을에서는 표현하지 않는다.
void CCompas::Show_Pos_Text()
{
	float x = 0;
	float z = 0;

	if( g_pRoh )
	{
		x = g_pRoh->m_wx;
		z = g_pRoh->m_wz;
	}

	int a = (x/gMapExtention);
	int b = (int) (gMapHeight-z/gMapExtention);

	//-- 샤일론 일경우
	if(pCMyApp->GetCurWorld() == 3)
	{
		if(x > 6880 && x < 7912 && z > 11236 && z < 11952)
			return;
	}
	//-- 로스트렐름
	if(pCMyApp->GetCurWorld() == 4)
	{
		if(x > 6796 && x < 7266 && z > 4988 && z < 5454)
			return;
	}
	//-- 데카르디
	if(pCMyApp->GetCurWorld() == 0)
	{
		if(x > 5515 && x < 7690 && z > 5438 && z < 7150)
			return;
	}
	//-- 천공
	if(pCMyApp->GetCurWorld() == 6)
	{
		if(x > 952 && x < 1749 && z > 1054 && z < 1445)
			return;
		if(x > 11356 && x < 12149 && z > 11293 && z < 12141)
			return;
	}
	//-- 화이트 혼
	if(pCMyApp->GetCurWorld() == 7)
	{
		if(x > 8356 && x < 9379 && z > 7466 && z < 8314)
			return;
	}
	// 각 던전,필드에 따라 조건식을 설정한다
	if(a >=133 && pCMyApp->GetCurWorld() == 1)//0401 aa>133
		a -= -133;

	if(pCMyApp->GetCurWorld() == 5)
	{
		if( g_pNk2DFrame
				&& g_pNk2DFrame->GetInterface(MAP)
				&& ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_iSeaRoost==1)		//2층
			a = a - 55;
		else if(g_pNk2DFrame
				&& g_pNk2DFrame->GetInterface(MAP)
				&&  ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_iSeaRoost==2)	//3층
		{
			a = a - 98;
			b = b - 73;
		}
		else if (pCMyApp->IsInsideRect(x, z, 1100,5120,5120,3660))
			a = a - 55;
		else if (pCMyApp->IsInsideRect(x, z, 1960,3660,4040,2220))
		{
			//m_iSeaRoost = 2;
			a = a - 98;
			b = b - 73;
		}
	}

	if(pCMyApp->GetCurWorld() == 2)
	{
		//데카둔
		if( g_pNk2DFrame
				&& g_pNk2DFrame->GetInterface(MAP)
				&& ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_iCurDeckadum==0)
			b = b - 128;
		else if( g_pNk2DFrame
				 && g_pNk2DFrame->GetInterface(MAP)
				 && ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_iCurDeckadum==2)
		{
			a = a - 128;
			b = b - 128;
		}
		else if(g_pNk2DFrame
				&& g_pNk2DFrame->GetInterface(MAP)
				&& ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_iCurDeckadum==3)
			a = a -(128+64);
		else if (pCMyApp->IsInsideRect(x, z, 80, 2480, 2500, 80))
			b = b - 128;
		else if (pCMyApp->IsInsideRect(x, z, 2720, 2480,5040, 60))
		{
			//	m_iCurDeckadum = 2;
			a = a - 128;
			b = b - 128;
		}
		else if (pCMyApp->IsInsideRect(x, z, 3780, 5080, 5080, 3780))
			a = a -(128+64);
	}

	char strPos[30];

	if( pCMyApp->GetCurWorld() == 1
			|| pCMyApp->GetCurWorld() == 2
			|| pCMyApp->GetCurWorld() == 5 )
	{
		m_CurMapPosX = (x/(gMapExtention/4));
		m_CurMapPosZ = (int)(gMapHeight*4-(z/(gMapExtention/4)));
	}
	else
	{
		m_CurMapPosX = (x/(gMapExtention/5));
		m_CurMapPosZ = (int)(gMapHeight*5-(z/(gMapExtention/5)));
	}
	sprintf(strPos,"%d, %d", m_CurMapPosX, m_CurMapPosZ);
	m_PosBox.SetString(strPos);

	//  [8/13/2009 ppmmjj83] 콤파스 좌표 수정
	m_PosBox.SetPos(g_pNk2DFrame->GetClientWidth() - 56, 73);
//	m_PosBox.SetPos(g_pNk2DFrame->GetClientWidth() - 127, 4);
	m_PosBox.Draw();
}
