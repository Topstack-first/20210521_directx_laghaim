#include "stdafx.h"
#define D3D_OVERLOADS

#include "event_family.h"
#include "headers.h"
#include "IndexedTexture.h"
#include "D3DMath.h"
#include "main.h"
#include "Nk2DFrame.h"

#include "ControlStatus.h"
#include "ControlTop.h"

///이벤트버프표시





CFamilyEvent* CFamilyEvent::CreateInstance(int nType)
{
	return new CFamilyEvent(nType);
}

CFamilyEvent::CFamilyEvent(int nType) : m_pIScene(NULL)
{
	m_event_flagset = 0;
	m_eType = (EVENT_TYPE)nType;

	m_pIScene = new IndexedTexture(EF_IMAGE_FRAME_TOTAL*TYPE_STATUS_TOTAL);
	m_pIScene->SetTexturePath("interface/family_event/");

	m_fPosX = 75.3f;
	m_fPosY = 0;


	if( g_pNk2DFrame->GetControlTop() && g_pNk2DFrame->GetControlStatus()->GetHeight() )
	{
		m_fPosX = 0.0f;
		m_fPosY = g_pNk2DFrame->GetControlTop()->GetHeight() + g_pNk2DFrame->GetControlStatus()->GetHeight() + 1.0f;
	}

	m_nNowRender = -1;

	m_Alpha = 1;

	m_nFade = 1;

	m_dwStartTime = timeGetTime();

	LoadTgaResource();
}

void CFamilyEvent::LoadTgaResource()
{
	if(m_pIScene == NULL) return;

	m_fHeight	= 26.f;

	m_nTgaWindowNum[EF_IMAGE_FRAME_1+EF_IMAGE_FRAME_TOTAL*0]	=	m_pIScene->CreateTextureFromFile("ev_itemdrop_1.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_2+EF_IMAGE_FRAME_TOTAL*0]	=	m_pIScene->CreateTextureFromFile("ev_itemdrop_2.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_3+EF_IMAGE_FRAME_TOTAL*0]	=	m_pIScene->CreateTextureFromFile("ev_itemdrop_3.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_4+EF_IMAGE_FRAME_TOTAL*0]	=	m_pIScene->CreateTextureFromFile("ev_itemdrop_4.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_5+EF_IMAGE_FRAME_TOTAL*0]	=	m_pIScene->CreateTextureFromFile("ev_itemdrop_5.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_6+EF_IMAGE_FRAME_TOTAL*0]	=	m_pIScene->CreateTextureFromFile("ev_itemdrop_6.sek");


	m_nTgaWindowNum[EF_IMAGE_FRAME_1+EF_IMAGE_FRAME_TOTAL*1]	=	m_pIScene->CreateTextureFromFile("ev_attack_1.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_2+EF_IMAGE_FRAME_TOTAL*1]	=	m_pIScene->CreateTextureFromFile("ev_attack_2.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_3+EF_IMAGE_FRAME_TOTAL*1]	=	m_pIScene->CreateTextureFromFile("ev_attack_3.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_4+EF_IMAGE_FRAME_TOTAL*1]	=	m_pIScene->CreateTextureFromFile("ev_attack_4.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_5+EF_IMAGE_FRAME_TOTAL*1]	=	m_pIScene->CreateTextureFromFile("ev_attack_5.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_6+EF_IMAGE_FRAME_TOTAL*1]	=	m_pIScene->CreateTextureFromFile("ev_attack_6.sek");

	m_nTgaWindowNum[EF_IMAGE_FRAME_1+EF_IMAGE_FRAME_TOTAL*2]	=	m_pIScene->CreateTextureFromFile("ev_defense_1.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_2+EF_IMAGE_FRAME_TOTAL*2]	=	m_pIScene->CreateTextureFromFile("ev_defense_2.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_3+EF_IMAGE_FRAME_TOTAL*2]	=	m_pIScene->CreateTextureFromFile("ev_defense_3.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_4+EF_IMAGE_FRAME_TOTAL*2]	=	m_pIScene->CreateTextureFromFile("ev_defense_4.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_5+EF_IMAGE_FRAME_TOTAL*2]	=	m_pIScene->CreateTextureFromFile("ev_defense_5.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_6+EF_IMAGE_FRAME_TOTAL*2]	=	m_pIScene->CreateTextureFromFile("ev_defense_6.sek");

	m_nTgaWindowNum[EF_IMAGE_FRAME_1+EF_IMAGE_FRAME_TOTAL*3]	=	m_pIScene->CreateTextureFromFile("ev_strength_1.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_2+EF_IMAGE_FRAME_TOTAL*3]	=	m_pIScene->CreateTextureFromFile("ev_strength_2.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_3+EF_IMAGE_FRAME_TOTAL*3]	=	m_pIScene->CreateTextureFromFile("ev_strength_3.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_4+EF_IMAGE_FRAME_TOTAL*3]	=	m_pIScene->CreateTextureFromFile("ev_strength_4.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_5+EF_IMAGE_FRAME_TOTAL*3]	=	m_pIScene->CreateTextureFromFile("ev_strength_5.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_6+EF_IMAGE_FRAME_TOTAL*3]	=	m_pIScene->CreateTextureFromFile("ev_strength_6.sek");

	m_nTgaWindowNum[EF_IMAGE_FRAME_1+EF_IMAGE_FRAME_TOTAL*4]	=	m_pIScene->CreateTextureFromFile("ev_expdouble_1.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_2+EF_IMAGE_FRAME_TOTAL*4]	=	m_pIScene->CreateTextureFromFile("ev_expdouble_2.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_3+EF_IMAGE_FRAME_TOTAL*4]	=	m_pIScene->CreateTextureFromFile("ev_expdouble_3.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_4+EF_IMAGE_FRAME_TOTAL*4]	=	m_pIScene->CreateTextureFromFile("ev_expdouble_4.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_5+EF_IMAGE_FRAME_TOTAL*4]	=	m_pIScene->CreateTextureFromFile("ev_expdouble_5.sek");
	m_nTgaWindowNum[EF_IMAGE_FRAME_6+EF_IMAGE_FRAME_TOTAL*4]	=	m_pIScene->CreateTextureFromFile("ev_expdouble_6.sek");

	m_pIScene->RestoreAllTextures(pCMyApp->GetDevice());
}

void CFamilyEvent::SetEventType(int type, int flag)
{
	if( flag )
	{
		switch( type )
		{
		case TYPE_DROP_PER:
			m_event_flagset |= FLAG_DROP_PER;
			break;
		case TYPE_ATTACK_PER:
			m_event_flagset |= FLAG_ATTACK_PER;
			break;
		case TYPE_DEFENSE_PER:
			m_event_flagset |= FLAG_DEFENSE_PER;
			break;
		case TYPE_STRENGTH_PER:
			m_event_flagset |= FLAG_STRENGTH_PER;
			break;
		case TYPE_EXP_PER:
			m_event_flagset |= FLAG_EXP_PER;
			break;
		}
	}
	else
	{
		switch( type )
		{
		case TYPE_DROP_PER:
			m_event_flagset &= (~FLAG_DROP_PER);
			break;
		case TYPE_ATTACK_PER:
			m_event_flagset &= (~FLAG_ATTACK_PER);
			break;
		case TYPE_DEFENSE_PER:
			m_event_flagset &= (~FLAG_DEFENSE_PER);
			break;
		case TYPE_STRENGTH_PER:
			m_event_flagset &= (~FLAG_STRENGTH_PER);
			break;
		case TYPE_EXP_PER:
			m_event_flagset &= (~FLAG_EXP_PER);
			break;
		}
	}
}


int CFamilyEvent::FrameMove()
{
	DWORD dwTime = (timeGetTime() - m_dwStartTime);

	if( dwTime > 250)
	{
		m_dwStartTime = timeGetTime();
		m_nNowRender++;

		m_Alpha = m_Alpha + ( m_nFade*(dwTime * 0.15) );
	}

	if(m_nNowRender >= EF_IMAGE_FRAME_TOTAL)
		m_nNowRender = 0;

	if(m_Alpha > 255)
		m_Alpha = 255;

	if(m_Alpha <= 0)
	{
		m_Alpha = 0;
		return 0;
	}

	return 1;
}

int CFamilyEvent::Render()
{
	if(!pCMyApp->GetDevice() || !m_pIScene || m_event_flagset == 0 )
		return 0;

	// if(!FrameMove())	return 0;
	FrameMove();

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	pCMyApp->GetDevice()->GetViewport(&vpOld);

	RECT rect;

	pCMyApp->GetDevice()->GetViewport(&vpNew);
	GetClientRect(pCMyApp->Get_hWnd(), &rect);
	vpNew.dwWidth  = rect.right - rect.left ;

	int nHeight;
	nHeight = vpNew.dwWidth;
	if ( (nHeight - (rect.bottom-rect.top)) > 0)
		nHeight = rect.bottom - rect.top;
	vpNew.dwY = ( (rect.bottom - rect.top) - nHeight ) / 2;
	vpNew.dwHeight = nHeight;

	pCMyApp->GetDevice()->SetViewport(&vpNew);

	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	pCMyApp->GetDevice()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	// 라이팅
	pCMyApp->GetDevice()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	DWORD  dwTextureState[3];
	pCMyApp->GetDevice()->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	pCMyApp->GetDevice()->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	pCMyApp->GetDevice()->GetTextureStageState(0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( pCMyApp->GetDevice()->BeginScene() ) )
	{
		//-----------------------TLVERTEX 를 이용한 맵핑후 출력(화면의 실제 좌표를 입력한다)
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

		pCMyApp->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		pCMyApp->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		pCMyApp->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		// by evilkiki 2009.06.03 이벤트 나열
		D3DTLVERTEX TLVec[4];
		int offset = 0;
		for( int i=0 ; i<5 ; i++ )
		{
			if( m_event_flagset & (1<<i) )
			{
				LPDIRECTDRAWSURFACE7 lpTexture = m_pIScene->GetSurfaceByIdx(m_nTgaWindowNum[m_nNowRender+i*EF_IMAGE_FRAME_TOTAL]);
				pCMyApp->GetDevice()->SetTexture(0, lpTexture);

				switch( i )
				{
				case TYPE_DROP_PER:
					m_fWidth = 157.f;
					break;
				case TYPE_ATTACK_PER:
					m_fWidth = 197.f;
					break;
				case TYPE_DEFENSE_PER:
					m_fWidth = 197.f;
					break;
				case TYPE_STRENGTH_PER:
					m_fWidth = 149.f;
					break;
				case TYPE_EXP_PER:
					m_fWidth = 149.f;
					break;
				}

				TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_fPosX - 0.5		    , m_fPosY - 0.5+m_fHeight*offset	, 0), 1, RGBA_MAKE( 255, 255, 255, m_Alpha), 0, 0, 0);
				TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_fPosX - 0.5+m_fWidth, m_fPosY - 0.5+m_fHeight*offset	, 0), 1, RGBA_MAKE( 255, 255, 255, m_Alpha), 0, 1, 0);
				TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_fPosX - 0.5		    , m_fPosY - 0.5+m_fHeight*(offset+1), 0), 1, RGBA_MAKE( 255, 255, 255, m_Alpha), 0, 0, 1);
				TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_fPosX - 0.5+m_fWidth, m_fPosY - 0.5+m_fHeight*(offset+1), 0), 1, RGBA_MAKE( 255, 255, 255, m_Alpha), 0, 1, 1);
				pCMyApp->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);

				offset++;
			}
		}

		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		pCMyApp->GetDevice()->EndScene();
	}

	// 원상복귀.
	pCMyApp->GetDevice()->SetViewport(&vpOld);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

	pCMyApp->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	pCMyApp->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	pCMyApp->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );


	return 1;
}

void CFamilyEvent::DeleteRes()
{
	if(m_pIScene)	m_pIScene->InvalidateAllTextures();

	SAFE_DELETE(m_pIScene);
}

CFamilyEvent::~CFamilyEvent()
{
	DeleteRes();
}

