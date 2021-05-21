#include "stdafx.h"

#define D3D_OVERLOADS

#include <d3d.h>
#include <ddraw.h>
#include <time.h>
#include "ddutil.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "sfxUtil.h"
#include "GUtil.h"
#include "SFXBaseShape.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "basicbutton.h"
#include "controlbottom.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "UIMgr.h"
#include "AlphaPopUp.h"
#include "TcpUtil.h"
#include "headers.h"
#include "string_res.h"
#include "g_stringmanager.h"
#include "Country.h"

//#include "HelpScript.h"
#include "ControlHelpPet.h"





extern HFONT G_GetFont(int h, int w,const char  * const fontname);
extern BOOL g_bFixedWidthFont;

extern DWORD				g_dwClientCountry;

int g_HelpPetAniFrame[MAX_HELPPET_ANI] =   // 프레임 총갯수.
{
	12, // HELPPET_ANI_NORMAL
	28, // HELPPET_ANI_FIND
	12, // HELPPET_ANI_THROW
	16, // HELPPET_ANI_ANGRY ///펫도움말확장
	26,  // HELPPET_ANI_SLEEPY ///펫도움말확장
	12, // HELPPET_ANI_SEARCH ///펫도움말확장
};

///펫도움말
CControlHelpPet::CControlHelpPet()
	: m_hWnd(NULL)
{
	memset(m_InterfaceTexIndex, 0, sizeof(m_InterfaceTexIndex));

	m_pIndexedTexture = NULL;
	m_pTextSurface = NULL;

	Init();
}

CControlHelpPet::~CControlHelpPet()
{
	DeleteRes();

	Init();
}

void CControlHelpPet::Init()
{
	// 펫 부분
	m_nType = HELPPET_TYPE_RABIYONG; // 펫의 종류.
	m_nAnimation = HELPPET_ANI_NORMAL; // 펫의 애니메이션 번호.
	m_nDir = 0;
	m_nMoveFlag = 0;

	SAFE_DELETE( m_pIndexedTexture );
	m_pIndexedTexture = new IndexedTexture( 18 );
	if( m_pIndexedTexture )
		m_pIndexedTexture->SetTexturePath("interface/HelpPet/");

	ZeroMemory( &m_PetRect, sizeof(RECT) );
	ZeroMemory( &m_TalksRect, sizeof(RECT) );

	ZeroMemory( &m_TalksText, sizeof(CMultiText) ); // 새로 생기면 깔끔히 지워줘야한다.
	ZeroMemory( m_TLVec, sizeof(m_TLVec) );

	ZeroMemory( m_AniIndex, sizeof(int)*MAX_HELPPET_ANI );

	m_nNowFrame = 0; // 현재 애니메이션 프레임.
	m_dwFrameRate = 0; // 한 프레임 넘어가는 데 필요한 프레임 레이트. 틱 단위임에 유의.
	m_dwPetAniChageTick = 0; // 펫 동작과 관련된 부분의 기억 틱.
	m_dwOldFrameTick = 0; // 이전 틱.

	m_bPetDrag = FALSE;
	m_DragDistX = 0;
	m_DragDistY = 0;
	m_ClickX = 0; // 클릭한 위치 기억.
	m_ClickY = 0;
	m_ThrowPower = 0;
	m_dwPetDragTick = 0;

	m_dwOldConCheckTick = 0;
	m_dwTalkStartTick = 0;
	m_TalkIndex = -1;

	m_Font = G_GetFont(HELP_USE_FONT_SIZE,0,NULL);

	m_HelpManager.Init();

	SAFE_DELETE( m_pTextSurface );

	m_TextOutBox.Init(16, RGB(10, 10, 10), 0, 0, -1, -1, TRUE );

	m_PrevBtn2.SetFileName( "HelpPet/prev" );
	m_NextBtn2.SetFileName( "HelpPet/next" );
	m_CloseBtn2.SetFileName( "HelpPet/close" );
	m_PrevBtn.SetFileName( "HelpPet/pre_2" );
	m_NextBtn.SetFileName( "HelpPet/next_2" );
	m_CloseBtn.SetFileName( "HelpPet/close" );
	m_SearchBtn.SetFileName( "HelpPet/btn_search" );

	m_bContents = FALSE;
	m_nNowContentsPage = 0;
	m_bContentsDrag = FALSE;

	m_ListContentsSurface.clear();
	m_ListContentsSurface.reserve(0);

	m_BoundCount = 0; ///펫도움말확장

	m_Searching = 0; ///펫도움말확장
	ZeroMemory( &m_SearchRect, sizeof(RECT) );
	m_SearchInput.Init( 0, 0, 10*2, RGB(255, 255, 255), RGB(250, 250, 250) );
	m_SearchInput.SetfontSize();
	m_SearchInput.ClearText();

	m_SearchResultText.Reset();
	m_SearchResultIndex.clear();
	m_NowViewResult = 0; // 현재 보고있는 검색결과 위치. (페이지 관리 때문에 필요)
	m_TotalResult = 0; // 검색 결과 갯수.
	m_NowSelResult = 0;

	m_nSleepCount = 0;
}

void CControlHelpPet::DeleteRes()
{
	m_SearchInput.DeleteRes();     // 상점주인이 써놓은 멘트 출력용 서피스.

	CSurface *pImage;
	vector<CSurface*>::iterator prImage;
	for( prImage = m_ListContentsSurface.begin(); prImage != m_ListContentsSurface.end(); ++prImage )
	{
		pImage = *prImage;
		SAFE_DELETE( pImage );
	}
	m_ListContentsSurface.clear();


	m_TextOutBox.DeleteRes();

	m_SearchBtn.DeleteRes();
	m_PrevBtn.DeleteRes(); // 이전 판매 목록 보기 버튼.
	m_NextBtn.DeleteRes();
	m_CloseBtn.DeleteRes();
	m_CloseBtn2.DeleteRes();
	m_NextBtn2.DeleteRes();
	m_PrevBtn2.DeleteRes(); // 이전 판매 목록 보기 버튼.

	SAFE_DELETE( m_pTextSurface );
	SAFE_DELETE( m_pIndexedTexture );
}

void CControlHelpPet::LoadRes( int Type )
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nType = Type;

	switch( m_nType )
	{
	case HELPPET_TYPE_RABIYONG:

		m_PetRect.left = 450; // 펫의 위치
		m_PetRect.top = 350; // 펫의 위치
		m_PetRect.right = 100; // 펫의 크기
		m_PetRect.bottom = 100; // 펫의 크기

		// 그래픽 데이터도 타입별로 로딩
		m_AniIndex[HELPPET_ANI_NORMAL] = m_pIndexedTexture->CreateTextureFromFile( "Ravy/fly.tga" ); // 평상시 날라다니는 녀석.
		m_pIndexedTexture->CreateTextureFromFile( "Ravy/drag.tga" ); // 드래그 중 바둥바둥. 노멀 애니메이션 +1 로 인식.
		m_AniIndex[HELPPET_ANI_FIND] = m_pIndexedTexture->CreateTextureFromFile( "Ravy/Find.tga" ); // 헬프 찾음.
		m_pIndexedTexture->CreateTextureFromFile( "Ravy/Find2.tga" ); // 이 녀석은 +1로 인식.
		m_AniIndex[HELPPET_ANI_ANGRY] = m_pIndexedTexture->CreateTextureFromFile( "Ravy/angry.tga" ); // 화냄. ///펫도움말확장
		m_AniIndex[HELPPET_ANI_SLEEPY] = m_pIndexedTexture->CreateTextureFromFile( "Ravy/sleepy.tga" ); // 졸기
		m_pIndexedTexture->CreateTextureFromFile( "Ravy/sleepy2.tga" ); // 이 녀석은 +1로 인식.

		break;
	}

	// 인터페이스 로딩.
	m_InterfaceTexIndex[HELPPET_INTERFACE_TALKBOX] = m_pIndexedTexture->CreateTextureFromFile( "help_bubble.tga" ); // 말풍선.
	m_InterfaceTexIndex[HELPPET_INTERFACE_CONTENTSBOX] = m_pIndexedTexture->CreateTextureFromFile( "help_bk.tga" ); // 내용 배경
	m_InterfaceTexIndex[HELPPET_INTERFACE_SEARCHBOX] = m_pIndexedTexture->CreateTextureFromFile( "help_search_bk.tga" ); // 검색 창 배경
	m_InterfaceTexIndex[HELPPET_INTERFACE_SEARCHMENT] = m_pIndexedTexture->CreateTextureFromFile( "help_search.tga" ); // 검색 창 배경

	RestoreSurfaces();

	m_ContentsRect.right = 550;
	m_ContentsRect.bottom = 350;
	m_ContentsRect.left = (m_ClientRc.right - m_ContentsRect.right) / 2;

	m_ContentsRect.top = (m_ClientRc.bottom - m_ContentsRect.bottom) / 2-70;

	// 헬프 스크립트를 읽어온다.
	m_HelpManager.LoadDta( "./interface/HelpPet/Help.dta" );
}

HRESULT CControlHelpPet::RestoreSurfaces()
{
	m_PrevBtn2.LoadRes();
	m_NextBtn2.LoadRes();
	m_CloseBtn2.LoadRes();
	m_PrevBtn.LoadRes();
	m_NextBtn.LoadRes();
	m_CloseBtn.LoadRes();
	m_SearchBtn.LoadRes();

	m_pIndexedTexture->RestoreAllTextures( GET_D3DDEVICE() );

	return 0L;
}
/*
void CControlHelpPet::SetPetState( int State ) // 상태 변경.
{
	m_nState = State;

	m_dwStateChangeTick = timeGetTime();
}
*/
void CControlHelpPet::SetPetAnimation( int AniNum ) // 애니메이션 변경
{
	switch( m_nAnimation ) // 이전의 애니메이션에 따라서 작동.
	{
	case HELPPET_ANI_NORMAL: // 그냥 파닥파닥.
		break;
	case HELPPET_ANI_FIND: // 할 말(헬프)를 찾았다!
		break;
	case HELPPET_ANI_THROW:
		m_ThrowPower = 0;

//		if( AniNum == HELPPET_ANI_FIND ) // 던지는 중에 찾으면 애니메이션 전환없이 대사
//			return;

		break;

	case HELPPET_ANI_ANGRY: // 화냄. ///펫도움말확장
		TerminateTalkBox(); // 바운딩 카운트를 말해줬던 대화창을 없애기 위해.
		break;
	case HELPPET_ANI_SLEEPY: // 졸기
		m_PetRect.top -= 10.0f; // 내렸던 위치를 다시 올려준다.
		m_PetRect.bottom -= 10.0f; // 내렸던 위치를 다시 올려준다.
		break;

	case HELPPET_ANI_SEARCH: // 검색 중. ///펫도움말확장
		break;
	}

	m_nAnimation = AniNum;

	switch( m_nAnimation ) // 현재의 애니메이션에 따라서 작동.
	{
	case HELPPET_ANI_NORMAL: // 그냥 파닥파닥.
		m_dwFrameRate = 60;
		break;
	case HELPPET_ANI_FIND: // 할 말(헬프)를 찾았다!
		m_dwFrameRate = 60;
		break;
	case HELPPET_ANI_THROW: ///펫도움말확장
		m_BoundCount = 0;
		break;
	case HELPPET_ANI_ANGRY: // 화냄. ///펫도움말확장
		m_dwFrameRate = 100;

		break;
	case HELPPET_ANI_SLEEPY: // 졸기
		m_dwFrameRate = 200;
		m_PetRect.top += 10.0f; // 위치를 약간 내려준다.
		m_PetRect.bottom += 10.0f; // 위치를 약간 내려준다.

		m_nSleepCount = 0; // 초기화.

		break;

	case HELPPET_ANI_SEARCH: // 검색 중. ///펫도움말확장
		m_dwFrameRate = 60;
		break;
	}

	m_nNowFrame = 0;
	m_dwPetAniChageTick = timeGetTime(); // 애니메이션이 바뀐 틱을 기억.
}

void CControlHelpPet::FrameMove()
{
	DWORD dwElapseTick = timeGetTime() - m_dwOldFrameTick;


	switch( m_nAnimation ) // 현재의 애니메이션에 따라서 작동.
	{
	case HELPPET_ANI_NORMAL: // 그냥 파닥파닥.

		// 드래그 예외처리.
		if( m_bPetDrag ) // 혹시 드래그 중이면...
			m_dwFrameRate = 50;  // 빠르게 파닥파닥
		else
			m_dwFrameRate = 60;

		// 우선 현재의 프레임을 구한다.
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate ) % g_HelpPetAniFrame[m_nAnimation];

		// 높이 있으면 자동으로 떨어지게...
		if( !m_dwTalkStartTick && !m_bPetDrag ) // 말하는 중엔 이동 안하게... // 드래그 중에도.
		{
			if( m_PetRect.top < m_ClientRc.bottom-HELPPET_LIKE_HEIGHT )
			{
//				m_PetRect.top += 0.008f * dwElapseTick * ((float)(m_PetRect.top+1)*0.5f);
				m_PetRect.top += 0.08f * dwElapseTick;

				if( m_PetRect.top > m_ClientRc.bottom-HELPPET_LIKE_HEIGHT )
					m_PetRect.top = m_ClientRc.bottom-HELPPET_LIKE_HEIGHT;
			}

			if( m_nMoveFlag == -1 ) // 좌로 움직이는 중.
			{
				m_nDir = 0;
				m_PetRect.left -= 0.04f * dwElapseTick;
			}
			else if( m_nMoveFlag == 1 ) // 우로 움직이는 중.
			{
				m_nDir = 1;
				m_PetRect.left += 0.04f * dwElapseTick;
			}
			else if( m_nMoveFlag == 0 ) // 제자리 있을 경우. ///펫도움말확장
			{
				if( m_PetRect.top >= m_ClientRc.bottom-HELPPET_LIKE_HEIGHT && (rand()%1000) < 15 ) // 졸기. 밑에 있을때만적용.
//				if( m_PetRect.top >= m_ClientRc.bottom-HELPPET_LIKE_HEIGHT && (rand()%1000) < 500 ) // 졸기. 테스트용
				{
					SetPetAnimation( HELPPET_ANI_SLEEPY );
				}
			}

			if( (rand()%1000) < 20 ) // 움직임 방향 변경 확률
			{
				m_nMoveFlag = (rand()%3) - 1;

				if( (rand()%1000) < 20 ) // 제자리에 서있을 확률을 높여준다.
					m_nMoveFlag = 0;
			}
		}

		break;

	case HELPPET_ANI_FIND: // 할 말(헬프)를 찾았다!
		// 우선 현재의 프레임을 구한다.
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate );

		if( m_nNowFrame >= g_HelpPetAniFrame[m_nAnimation] ) // 한바퀴 돌았으면..
			SetPetAnimation( HELPPET_ANI_NORMAL ); // 노말로 돌려준다.

		break;

	case HELPPET_ANI_THROW: // 유저가 집어 던진 경우.
		// 우선 현재의 프레임을 구한다.
		m_nNowFrame = 7; // 프레임 고정

		// 위치 처리.
		if( m_ThrowPower ) // 파워가 남아있다면...
		{
			// 날려보낸다.
			m_PetRect.left += m_ClickX * (m_ThrowPower*0.001f) * ( (int)dwElapseTick * 0.01f );
			m_PetRect.top += m_ClickY * (m_ThrowPower*0.001f) * ( (int)dwElapseTick * 0.01f );

			// 바운딩 체크.
			if( m_PetRect.left < 0 )
			{
				m_PetRect.left = 0;

				// 방향을 바꿔준다.
				m_ClickX *= -1;
				m_BoundCount++; ///펫도움말확장
			}
			if( m_PetRect.top < 0 )
			{
				m_PetRect.top = 0;
				// 방향을 바꿔준다.
				m_ClickY *= -1;
				m_BoundCount++; ///펫도움말확장
			}
			if( m_PetRect.left+m_PetRect.right > m_ClientRc.right )
			{
				m_PetRect.left = m_ClientRc.right - m_PetRect.right;
				// 방향을 바꿔준다.
				m_ClickX *= -1;
				m_BoundCount++; ///펫도움말확장
			}
			if( m_PetRect.top+m_PetRect.bottom > m_ClientRc.bottom )
			{
				m_PetRect.top = m_ClientRc.bottom - m_PetRect.bottom;
				// 방향을 바꿔준다.
				m_ClickY *= -1;
				m_BoundCount++; ///펫도움말확장
			}

			m_ThrowPower -= dwElapseTick; // 남은 힘 감소.
			if( m_ThrowPower < 0 ) // 마이너스가 됐다면...
			{
				m_ThrowPower = 0;
				m_dwPetAniChageTick = timeGetTime(); // 대기 시간을 주기 위해 초기화.
			}
		}
		else // 더이상 안 날라 갈때
		{
			if( timeGetTime() - m_dwPetAniChageTick < 1300 ) // 약간 어리둥절 타임을 준다.
			{
				// 어지러운 듯 방향을 바꾼다.
				if( (timeGetTime() - m_dwPetAniChageTick)%100 < 30 )
				{
					m_nDir ^= 1; // 토글
				}
			}
			else // 화내기.
			{
				// 튕겨진 숫자를 말해주자. ///펫도움말확장
				char TempString[256];
				// IDS_PETHELP_BOUNDCOUNT : %d번 튕겼어!!
				sprintf( TempString, (char *)G_STRING(IDS_PETHELP_BOUNDCOUNT), m_BoundCount );
				ExcuteHelpTalk( TempString );

				SetPetAnimation( HELPPET_ANI_ANGRY );
			}
			/*
						else // 완전히 끝난경우.
						{
							SetPetAnimation( HELPPET_ANI_NORMAL ); // 노말로 돌려준다.
						}
			*/
		}

		break;

	case HELPPET_ANI_ANGRY: // 화냄. ///펫도움말확장

		// 우선 현재의 프레임을 구한다.
		/*
				m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate ) % g_HelpPetAniFrame[m_nAnimation]; // 루프

				if( timeGetTime() - m_dwPetAniChageTick > 2000 ) // 일정시간이 지났으면.
					SetPetAnimation( HELPPET_ANI_NORMAL ); // 노말로 돌려준다.
		*/
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate );

		if( m_nNowFrame >= g_HelpPetAniFrame[m_nAnimation] ) // 한바퀴 돌았으면..
			SetPetAnimation( HELPPET_ANI_NORMAL ); // 노말로 돌려준다.

		break;
	case HELPPET_ANI_SLEEPY: // 졸기

		// 우선 현재의 프레임을 구한다.
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate );

		if( m_bPetDrag ) // 집었으면 바로 깨자.
		{
			SetPetAnimation( HELPPET_ANI_NORMAL ); // 노말로 돌려준다.
			if( m_nDir == 0 ) // 연속 졸기 방지용.
				m_nMoveFlag = -1;
			else
				m_nMoveFlag = 1;

			break;
		}

		if( m_nSleepCount < 2 ) // 길게 자게하기위함.
		{
			int LoopStart = 2; // 반복되는 프레임 구간
			int LoopEnd = 12;  // 반복되는 프레임 구간
			int LoopLength = LoopEnd - LoopStart;

			if( m_nNowFrame >= LoopEnd )
			{
				int k = m_nNowFrame - LoopEnd;
				m_nSleepCount = k / LoopLength;
				k %= LoopLength;
				m_nNowFrame = LoopStart + k;
			}
		}
		else // 이제 일어날때...
		{
			// 마무리 프레임을 이어주기 위해서 계산
			int LoopStart = 2; // 반복되는 프레임 구간
			int LoopEnd = 12;  // 반복되는 프레임 구간
			int LoopLength = LoopEnd - LoopStart;

			m_nNowFrame = (     m_nNowFrame - ( LoopEnd+(LoopLength*m_nSleepCount) )    );

			if( m_nNowFrame >= g_HelpPetAniFrame[m_nAnimation] ) // 한바퀴 돌았으면..(또는 클릭이나 드래그 해도 깬다.)
			{
				SetPetAnimation( HELPPET_ANI_NORMAL ); // 노말로 돌려준다.
				if( m_nDir == 0 ) // 연속 졸기 방지용.
					m_nMoveFlag = -1;
				else
					m_nMoveFlag = 1;

			}
		}

		break;

	case HELPPET_ANI_SEARCH: // 검색 중. ///펫도움말확장

		// 우선 현재의 프레임을 구한다.
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate ) % g_HelpPetAniFrame[m_nAnimation];

		break;

	}

	// 화면 넘어가면 보정.
	if( m_PetRect.left < 0 )
		m_PetRect.left = 0;
	if( m_PetRect.top < 0 )
		m_PetRect.top = 0;
	if( m_PetRect.left+m_PetRect.right > m_ClientRc.right )
		m_PetRect.left = m_ClientRc.right - m_PetRect.right;
	if( m_PetRect.top+m_PetRect.bottom > m_ClientRc.bottom )
		m_PetRect.top = m_ClientRc.bottom - m_PetRect.bottom;


//	m_PrevBtn.SetPosition(m_pBackSur->Xpos+180, m_pBackSur->Ypos+565 );

	CheckHelpCondition( HELP_CON_TYPE_A_LEVEL, g_pRoh->m_Level, FALSE ); // 레벨대 조건을 주기적으로 체크해주기 위해서...

	m_dwOldFrameTick = timeGetTime(); // 틱 갱신.
}

void CControlHelpPet::Draw() // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
{
	if( !GET_D3DDEVICE() )
		return;

	char strTemp[256];

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;
	DWORD  dwTextureState[3];
	DWORD dwZenable;

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZENABLE, &dwZenable );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );



	float FrameWidthRatio = 0.0f; // 한 프레임의 가로 비율
	int FrameWidth = HELPPET_FRAME_WIDTH; // 현재 애니메이션의 한 텍스쳐에서의 가로 프레임 갯수.
	float tu0,tv0,tu1,tv1,tu2,tv2;
	DWORD color;

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		//-----------------------TLVERTEX 를 이용한 맵핑후 출력(화면의 실제 좌표를 입력한다)
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

		color = RGBA_MAKE ( 255, 255, 255, 255 );

		// 컨텐츠 박스를 출력해야 하는 경우.
		if( m_bContents )
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
			tu1 = 0.0f;
			tv1 = 0.0f;
			tu2 = 1.0f;
			tv2 = 1.0f;
			m_TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_ContentsRect.left,m_ContentsRect.top+m_ContentsRect.bottom,0), 1, color, 0, tu1, tv2 );
			m_TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_ContentsRect.left,m_ContentsRect.top,0), 1, color, 0, tu1, tv1 );
			m_TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_ContentsRect.left+m_ContentsRect.right,m_ContentsRect.top,0), 1, color, 0, tu2, tv1 );
			m_TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_ContentsRect.left+m_ContentsRect.right,m_ContentsRect.top+m_ContentsRect.bottom,0), 1, color, 0, tu2, tv2 );
			lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_InterfaceTexIndex[HELPPET_INTERFACE_CONTENTSBOX] );
			GET_D3DDEVICE()->SetTexture(0, lpTexture );
			GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

			// 컨텐츠 jpg 출력
			int k = m_ListContentsSurface.size();
			if( m_nNowContentsPage < m_ListContentsSurface.size()  && m_ListContentsSurface[m_nNowContentsPage] && !(GetKeyState(VK_SHIFT) & 0x80) )
			{
				RECT ConInside;
				ConInside.left = m_ContentsRect.left+22;
				ConInside.top = m_ContentsRect.top+41;
				ConInside.right = 507;
				ConInside.bottom = 256;

				g_pDisplay->Blt( ConInside.left, ConInside.top, ConInside.left+ConInside.right
								 , ConInside.top+ConInside.bottom, (CSurface *)(m_ListContentsSurface[m_nNowContentsPage]) );
			}

			// 버튼 출력
			m_PrevBtn2.PosX = m_ContentsRect.left+(m_ContentsRect.right/2)-63;
			m_PrevBtn2.PosY = m_ContentsRect.top+305;
			m_PrevBtn2.Draw();

			m_NextBtn2.PosX = m_ContentsRect.left+(m_ContentsRect.right/2)+30;
			m_NextBtn2.PosY = m_ContentsRect.top+305;
			m_NextBtn2.Draw();

			m_CloseBtn2.PosX = m_ContentsRect.left+m_ContentsRect.right-35;
			m_CloseBtn2.PosY = m_ContentsRect.top+6;
			m_CloseBtn2.Draw();

			// 페이지 번호 출력.
			sprintf( strTemp, "%d / %d", m_nNowContentsPage+1, m_ListContentsSurface.size() );
//			m_TextOutBox.SetString( strTemp, RGB(250,250,10), RGB(0,0,0) );
//			m_TextOutBox.SetString( strTemp, RGB(5,5,5) );
			m_TextOutBox.SetString_withOutLine( strTemp, RGB(5,5,5), RGB(5,5,5) );
			m_TextOutBox.Draw( m_ContentsRect.left+4, m_ContentsRect.top+308, (m_ContentsRect.right) );
		}

		// 메인 창
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , TRUE);

		// 우선은 펫.
		// 프레임에 따른 tu,tv 조절.
		FrameWidthRatio = (1.0f/(float)FrameWidth); // 한 프레임의 가로 비율
		tu0 = (float)(m_nNowFrame % FrameWidth) * FrameWidthRatio;
		tv0 = (float)(m_nNowFrame / FrameWidth) * FrameWidthRatio;

		// 방향따라 좌우를 뒤집는다.
		if( m_nDir == 0 ) // 왼쪽.
		{
			tu1 = tu0;
			tu2 = tu0+FrameWidthRatio;
		}
		else
		{
			tu1 = tu0+FrameWidthRatio;
			tu2 = tu0;
		}
		tv1 = tv0;
		tv2 = tv0+FrameWidthRatio; // 세로도 가로랑 똑같이 나눈다는 가정하에 작업.

		if( m_nAnimation == HELPPET_ANI_THROW ) // 던지기 모션 예외처리
		{
			// 텍스쳐 좌표를 돌려서 뱅글밸글 도는 효과를 낸다.
			/*
						float k = (float)(timeGetTime()-m_dwPetAniChageTick); // 던지기 시작한 시간을 기준으로..

						tu1 = tu1 * cos(k) - tv1 * sin(k);
						tv1 = tv1 * sin(k) + tv1 * cos(k);
						tu2 = tu2 * cos(k) - tv2 * sin(k);
						tv2 = tv2 * sin(k) + tv2 * cos(k);
			*/
		}

		m_TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_PetRect.left,m_PetRect.top+m_PetRect.bottom,0), 1, color, 0, tu1, tv2 );
		m_TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_PetRect.left,m_PetRect.top,0), 1, color, 0, tu1, tv1 );
		m_TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_PetRect.left+m_PetRect.right,m_PetRect.top,0), 1, color, 0, tu2, tv1 );
		m_TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_PetRect.left+m_PetRect.right,m_PetRect.top+m_PetRect.bottom,0), 1, color, 0, tu2, tv2 );

		if( m_nAnimation == HELPPET_ANI_THROW ) // 던지기 모션 예외처리
		{
			// 회전 시킨다.
			float k = (float)( (timeGetTime()-m_dwPetAniChageTick)%314 ) * 0.01f * (float)(m_ThrowPower*0.01f) ; // 던지기 시작한 시간을 기준으로..// 던지는 힘을 함께 계산에 넣어줌으로써 이동안할땐 회전이 안되게 잡아준다.

			if( m_ThrowPower && !m_ClickX ) // 날라가는 방향따라서.
				k *= -1.0f; // 반대로

			D3DMATRIX m;
			D3DUtil_SetRotateZMatrix( m, k );

			D3DVECTOR TempVec;
			D3DVECTOR TempVec2 = D3DVECTOR( m_PetRect.left+(m_PetRect.right/2), m_PetRect.top+(m_PetRect.bottom/2), 0.0f ); // 펫의 중심좌표.
			int j;

			j = 0;
			TempVec = D3DVECTOR( m_TLVec[j].sx, m_TLVec[j].sy, m_TLVec[j].sz );
			TempVec -= TempVec2;
			D3DMath_VectorMatrixMultiply( TempVec, TempVec, m );
			TempVec += TempVec2;
			m_TLVec[j].sx = TempVec.x;
			m_TLVec[j].sy = TempVec.y;
			m_TLVec[j].sz = TempVec.z;
			j = 1;
			TempVec = D3DVECTOR( m_TLVec[j].sx, m_TLVec[j].sy, m_TLVec[j].sz );
			TempVec -= TempVec2;
			D3DMath_VectorMatrixMultiply( TempVec, TempVec, m );
			TempVec += TempVec2;
			m_TLVec[j].sx = TempVec.x;
			m_TLVec[j].sy = TempVec.y;
			m_TLVec[j].sz = TempVec.z;
			j = 2;
			TempVec = D3DVECTOR( m_TLVec[j].sx, m_TLVec[j].sy, m_TLVec[j].sz );
			TempVec -= TempVec2;
			D3DMath_VectorMatrixMultiply( TempVec, TempVec, m );
			TempVec += TempVec2;
			m_TLVec[j].sx = TempVec.x;
			m_TLVec[j].sy = TempVec.y;
			m_TLVec[j].sz = TempVec.z;
			j = 3;
			TempVec = D3DVECTOR( m_TLVec[j].sx, m_TLVec[j].sy, m_TLVec[j].sz );
			TempVec -= TempVec2;
			D3DMath_VectorMatrixMultiply( TempVec, TempVec, m );
			TempVec += TempVec2;
			m_TLVec[j].sx = TempVec.x;
			m_TLVec[j].sy = TempVec.y;
			m_TLVec[j].sz = TempVec.z;

		}


		// 텍스쳐를 찾아준다.
		switch( m_nAnimation ) // 현재의 애니메이션에 따라서 작동.
		{
		case HELPPET_ANI_NORMAL: // 그냥 파닥파닥.

			if( m_bPetDrag )
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation]+1 ); // 바둥바둥 애니메이션.
			else
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );

			break;
		case HELPPET_ANI_FIND: // 할 말(헬프)를 찾았다!

			if( m_nNowFrame > 16 ) // 페이지가 넘어가는 경우.
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation]+1 ); // 다음 녀석으로...
			else
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );

			break;
		case HELPPET_ANI_THROW: // 그냥 파닥파닥을 같이 쓴다.
			lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );
			break;

		case HELPPET_ANI_ANGRY: // 화냄. ///펫도움말확장
			lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );
			break;

		case HELPPET_ANI_SLEEPY: // 졸기

			if( m_nNowFrame > 16 ) // 페이지가 넘어가는 경우.
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation]+1 ); // 다음 녀석으로...
			else
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );

			break;

		case HELPPET_ANI_SEARCH: // 검색.
			lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[HELPPET_ANI_NORMAL] ); // 일단 지금은 노말꺼 쓴다.
			break;

		}

		GET_D3DDEVICE()->SetTexture(0, lpTexture );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);


		// 말풍선을 출력해줘야하는 경우.
		if( m_Searching ) // 검색 중. 말보다 우선 순위가 높다. ///펫도움말확장
		{
			// 말풍선의 위치를 실시간으로 계산해준다.(화면 밖으로 나가는 경우 방향을 바꿔주기 때문에...
			// 기본은 펫의 좌상단 방향.
			m_SearchRect.left = m_PetRect.left-m_SearchRect.right-HELP_TALKBOX_PLUS_POSX; // 기본 위치.
			m_SearchRect.top = m_PetRect.top-m_SearchRect.bottom-HELP_TALKBOX_PLUS_POSY; // 기본위치
			tu1 = 0.0f;
			tv1 = 0.0f;
			tu2 = 1.0f;
			tv2 = 1.0f;

			// 좌우 뒤집어야 하는지 체크.
			if( m_SearchRect.left < 0 ) // 넘어간다면...
			{
				// 뒤집어도 반대쪽으로 넘어간다면 그건 스크립트 입력의 문제다.
				m_SearchRect.left = m_PetRect.left+m_PetRect.right+HELP_TALKBOX_PLUS_POSX;

				tu1 = 1.0f;
				tu2 = 0.0f;
			}

			// 상하 뒤집어야 하는지 체크.
			if( m_SearchRect.top < 0 ) // 넘어간다면...
			{
				// 뒤집어도 반대쪽으로 넘어간다면 그건 스크립트 입력의 문제다.
				m_SearchRect.top = m_PetRect.top+m_PetRect.bottom+HELP_TALKBOX_PLUS_POSY;

				tv1 = 0.995f;
				tv2 = 0.0f;
			}

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

			if( m_Searching == 1 ) // 검색어 입력 모드.
			{
				m_TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left,m_SearchRect.top+m_SearchRect.bottom,0), 1, color, 0, tu1, tv2 );
				m_TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left,m_SearchRect.top,0), 1, color, 0, tu1, tv1 );
				m_TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+m_SearchRect.right,m_SearchRect.top,0), 1, color, 0, tu2, tv1 );
				m_TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+m_SearchRect.right,m_SearchRect.top+m_SearchRect.bottom,0), 1, color, 0, tu2, tv2 );

				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_InterfaceTexIndex[HELPPET_INTERFACE_TALKBOX] );
				GET_D3DDEVICE()->SetTexture(0, lpTexture );
				GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);



				RECT TempRect;

				TempRect.left = 10;
				TempRect.top = 7;
				TempRect.right = 170;
				TempRect.bottom = 25;

				if( tv1 > 0.0f ) // 상하가 뒤집힌 경우는 약간 더 조정을 해준다.
				{
					TempRect.top += 13;
					TempRect.bottom += 13;
				}


				m_TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+TempRect.left,m_SearchRect.top+TempRect.bottom,0), 1, color, 0, 0.0f, 1.0f );
				m_TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+TempRect.left,m_SearchRect.top+TempRect.top,0), 1, color, 0, 0.0f, 0.0f );
				m_TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+TempRect.right,m_SearchRect.top+TempRect.top,0), 1, color, 0, 1.0f, 0.0f );
				m_TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+TempRect.right,m_SearchRect.top+TempRect.bottom,0), 1, color, 0, 1.0f, 1.0f );

				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_InterfaceTexIndex[HELPPET_INTERFACE_SEARCHMENT] );
				GET_D3DDEVICE()->SetTexture(0, lpTexture );
				GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);



				TempRect.left = 10;
				TempRect.top = 30;
				TempRect.right = 140;
				TempRect.bottom = 50;

				if( tv1 > 0.0f ) // 상하가 뒤집힌 경우는 약간 더 조정을 해준다.
				{
					TempRect.top += 13;
					TempRect.bottom += 13;
				}

				m_TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+TempRect.left,m_SearchRect.top+TempRect.bottom,0), 1, color, 0, 0.0f, 1.0f );
				m_TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+TempRect.left,m_SearchRect.top+TempRect.top,0), 1, color, 0, 0.0f, 0.0f );
				m_TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+TempRect.right,m_SearchRect.top+TempRect.top,0), 1, color, 0, 1.0f, 0.0f );
				m_TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+TempRect.right,m_SearchRect.top+TempRect.bottom,0), 1, color, 0, 1.0f, 1.0f );

				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_InterfaceTexIndex[HELPPET_INTERFACE_SEARCHBOX] );
				GET_D3DDEVICE()->SetTexture(0, lpTexture );
				GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

				m_SearchInput.SetPos( m_SearchRect.left+TempRect.left+4, m_SearchRect.top+TempRect.top+3 );
				m_SearchInput.Draw(); // 좌표 여기다 넣어주자.

				m_SearchBtn.PosX = m_SearchRect.left+m_SearchRect.right-50;
				m_SearchBtn.PosY = m_SearchRect.top+TempRect.top;
				m_SearchBtn.Draw();

				m_CloseBtn.PosX = m_SearchRect.left+m_SearchRect.right-25;
				m_CloseBtn.PosY = m_SearchRect.top+TempRect.top-24;
				m_CloseBtn.Draw();
			}
			else if( m_Searching == 2 ) // 검색 결과 모드.
			{
				m_TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left,m_SearchRect.top+m_SearchRect.bottom,0), 1, color, 0, tu1, tv2 );
				m_TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left,m_SearchRect.top,0), 1, color, 0, tu1, tv1 );
				m_TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+m_SearchRect.right,m_SearchRect.top,0), 1, color, 0, tu2, tv1 );
				m_TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+m_SearchRect.right,m_SearchRect.top+m_SearchRect.bottom,0), 1, color, 0, tu2, tv2 );

				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_InterfaceTexIndex[HELPPET_INTERFACE_TALKBOX] );
				GET_D3DDEVICE()->SetTexture(0, lpTexture );
				GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

				int ReverseY = 0; // 뒤집어 졌을때 위치 보정값.

				if( tv1 > 0.0f ) // 상하가 뒤집힌 경우는 약간 더 조정을 해준다.
				{
//					if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // 한 화면 이상인 경우.
					ReverseY += 34;
//					else
//						ReverseY += 12;
				}

				// 검색 결과 들 출력.
				if( m_TotalResult )
				{
					// IDS_PETHELP_SEARCHRESULT : * 검색 결과입니다. *
					sprintf( strTemp, (char *)G_STRING(IDS_PETHELP_SEARCHRESULT) );
					m_TextOutBox.SetString_withOutLine( strTemp, RGB(150,250,100), RGB(20,20,20) );
					m_TextOutBox.Draw( m_SearchRect.left+15, m_SearchRect.top+ReverseY+15 );

//					int ControlLineY = 30; // 다음 페이지 버튼 및 페이지 수 표시 의 위치용.
					int ControlLineY = HELPPET_SEARCHRESULT_CONTROLPOSY; // 다음 페이지 버튼 및 페이지 수 표시 의 위치용.

					// 검색 결과 출력.
					for( int k = 0 ; k < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++k )
					{
						if( m_NowViewResult+k < m_TotalResult ) // 마지막 페이지일 경우를 고려해서...
						{
							sprintf( strTemp, "%d. %s", m_NowViewResult+k+1, m_SearchResultText.m_Text[m_NowViewResult+k] );

							if( m_NowSelResult-1 == k ) // 선택된 선택지
								m_TextOutBox.SetString_withOutLine( strTemp, RGB(50,50,50), RGB(250,250,50) );
							else
								m_TextOutBox.SetString_withOutLine( strTemp, RGB(250,250,250), RGB(100,100,100) );
							m_TextOutBox.Draw( m_SearchRect.left+6, m_SearchRect.top+HELPPET_SEARCHRESULT_SEL_START_Y+(k*HELPPET_SEARCHRESULT_SEL_LINE_HEIGHT)+ReverseY );

//							ControlLineY += 20; // 라인수를 증가. 고정 시키기로 했음.
						}
					}

//					if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // 여러 페이지를 표시해야하는 경우.
					{
//						if( m_NowViewResult > 0 ) // 맨 앞 페이지가 아니라면.
						{
							// 앞 방향 화살표를 표시한다.
							m_PrevBtn.PosX = m_SearchRect.left+(m_SearchRect.right/2)-63;
							m_PrevBtn.PosY = m_SearchRect.top+ControlLineY+ReverseY;
							m_PrevBtn.Draw();
						}

//						if( m_TotalResult - m_NowViewResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // 뒤로 표시해야할 페이지가 더 있는 경우.
						{
							// 뒷 방향 화살표를 표시한다.
							m_NextBtn.PosX = m_SearchRect.left+(m_SearchRect.right/2)+30;
							m_NextBtn.PosY = m_SearchRect.top+ControlLineY+ReverseY;
							m_NextBtn.Draw();
						}

						// 현재 페이지를 표시한다.
						sprintf( strTemp, "%d/%d", m_NowViewResult/HELPPET_SEARCHRESULT_VIEWINPAGE+1, m_TotalResult/HELPPET_SEARCHRESULT_VIEWINPAGE+1 );
//						m_TextOutBox.SetString_withOutLine( strTemp, RGB(5,5,5), RGB(5,5,5) );
						m_TextOutBox.SetString( strTemp, RGB(5,5,5) );
						m_TextOutBox.Draw( m_SearchRect.left+4, m_SearchRect.top+ControlLineY+3+ReverseY, (m_SearchRect.right) );
					}
				}

				m_CloseBtn.PosX = m_SearchRect.left+m_SearchRect.right-30;
				m_CloseBtn.PosY = m_SearchRect.top+10+ReverseY;
				m_CloseBtn.Draw();
			}
		}
		else if( m_dwTalkStartTick && (timeGetTime()-m_dwTalkStartTick > HELPPET_TALKBOX_VIEWTERM || m_nAnimation == HELPPET_ANI_ANGRY) ) // 찾은 후 약간 뒤부터 말풍선을 출력해준다. ///펫도움말확장
		{
			// 말풍선의 위치를 실시간으로 계산해준다.(화면 밖으로 나가는 경우 방향을 바꿔주기 때문에...
			// 기본은 펫의 좌상단 방향.
			m_TalksRect.left = m_PetRect.left-m_TalksRect.right-HELP_TALKBOX_PLUS_POSX; // 기본 위치.
			m_TalksRect.top = m_PetRect.top-m_TalksRect.bottom-HELP_TALKBOX_PLUS_POSY; // 기본위치
			tu1 = 0.0f;
			tv1 = 0.0f;
			tu2 = 1.0f;
			tv2 = 1.0f;

			// 좌우 뒤집어야 하는지 체크.
			if( m_TalksRect.left < 0 ) // 넘어간다면...
			{
				// 뒤집어도 반대쪽으로 넘어간다면 그건 스크립트 입력의 문제다.
				m_TalksRect.left = m_PetRect.left+m_PetRect.right+HELP_TALKBOX_PLUS_POSX;

				tu1 = 1.0f;
				tu2 = 0.0f;
			}

			// 상하 뒤집어야 하는지 체크.
			if( m_TalksRect.top < 0 ) // 넘어간다면...
			{
				// 뒤집어도 반대쪽으로 넘어간다면 그건 스크립트 입력의 문제다.
				m_TalksRect.top = m_PetRect.top+m_PetRect.bottom+HELP_TALKBOX_PLUS_POSY;

				tv1 = 0.995f;
				tv2 = 0.0f;
			}

			if( m_pTextSurface )
			{
				m_pTextSurface->Xpos = m_TalksRect.left+(15);
				m_pTextSurface->Ypos = m_TalksRect.top+(10);

				if( tv1 > 0.0f ) // 상하가 뒤집힌 경우는 약간 더 조정을 해준다.
				{
					// 이 경우 그림 상 텍스트 위치도 저정해줘야한다.
					m_pTextSurface->Ypos += 5;
				}

			}


			m_TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_TalksRect.left,m_TalksRect.top+m_TalksRect.bottom,0), 1, color, 0, tu1, tv2 );
			m_TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_TalksRect.left,m_TalksRect.top,0), 1, color, 0, tu1, tv1 );
			m_TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_TalksRect.left+m_TalksRect.right,m_TalksRect.top,0), 1, color, 0, tu2, tv1 );
			m_TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_TalksRect.left+m_TalksRect.right,m_TalksRect.top+m_TalksRect.bottom,0), 1, color, 0, tu2, tv2 );


			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

			lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_InterfaceTexIndex[HELPPET_INTERFACE_TALKBOX] );
			GET_D3DDEVICE()->SetTexture(0, lpTexture );
			GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

			// 텍스트 서피스를 말풍선 위치에 맞춰서 찍어준다.
			int tempY = 0;
			if( m_pTextSurface )
			{
				g_pDisplay->Blt( m_pTextSurface->Xpos, m_pTextSurface->Ypos, m_pTextSurface );
				tempY = m_pTextSurface->Ypos-4;
			}

			m_CloseBtn.PosX = m_TalksRect.left+m_TalksRect.right-25;
			m_CloseBtn.PosY = tempY;
			m_CloseBtn.Draw();

			if( timeGetTime()-m_dwTalkStartTick > HELPPET_TALKBOX_CLOSETIME ) // 일정시간이상 출력했으면..
			{
				TerminateTalkBox();
			}
		}

		GET_D3DDEVICE()->EndScene();
	}

	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, dwZenable);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

}

LRESULT CControlHelpPet::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = 0, y = 0;
	BOOL bInputEnable = FALSE; // 입력창 포커스 전환 체크용 ///일본전각반각버그수정

	if( m_SearchInput.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() ) // 채팅입력창에 포커스가 가있다면... ///일본전각반각버그수정
	{
		m_SearchInput.SetEnable( FALSE ); // 강제로 포커스를 풀어버린다.(처리가 애매해서 이렇게 무식하게 함)
		pCMyApp->ConvertImmStatus(); // 텍스트 입력 포커스를 끌땐 이걸 해줘야하는 듯. ///일본전각반각버그수정
	}

	switch (msg)
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( IsInsidePet(x, y) ) // 펫을 클릭한 경우.
		{
			if( m_nAnimation != HELPPET_ANI_THROW )
			{
				m_ClickX = x; // 클릭한 위치를 기억.
				m_ClickY = y;
			}

			m_bPetDrag = TRUE;
			m_dwPetDragTick = timeGetTime();

			m_DragDistX = m_PetRect.left - x;
			m_DragDistY = m_PetRect.top - y;


			return 1;
		}

		if( m_Searching || m_dwTalkStartTick ) // 말 풍선 보이는 상태.
		{
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			if( IsInsideSearchBox(x,y) )
			{
				if( m_Searching == 1 )
				{
					m_SearchBtn.MsgProc(hWnd, msg, wParam, lParam);

					bInputEnable = m_SearchInput.GetEnable(); // 기억해두었다가... ///일본전각반각버그수정

					m_SearchInput.MsgProc( hWnd, msg, wParam, lParam ); // 포커스를 끄고 켜고를 위해서...

					if( bInputEnable == FALSE && m_SearchInput.GetEnable() ) // 꺼져있던 상태에서 켜졌다면... ///일본전각반각버그수정
						pCMyApp->RestoreImmStatus(); // 이걸 해주면 일본에서 말한 전각 반각 버그가 사라지지 않을까 싶다.
				}
				else if( m_Searching == 2 )
				{
					m_PrevBtn.MsgProc(hWnd, msg, wParam, lParam);
					m_NextBtn.MsgProc(hWnd, msg, wParam, lParam);
				}

				return 1; // 말 풍선 위를 클릭한건 딴 메세지 무시하도록...
			}
		}

		if( m_bContents )
		{
			m_PrevBtn2.MsgProc(hWnd, msg, wParam, lParam);
			m_NextBtn2.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn2.MsgProc(hWnd, msg, wParam, lParam);

			if( IsInsideContents(x, y) )
			{
				m_bContentsDrag = TRUE;
				m_DragDistX = m_ContentsRect.left - x;
				m_DragDistY = m_ContentsRect.top - y;
				return 1;
			}
		}


		break;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 어디서 땟든 드래그 플래그는 풀어준다.

		if( IsInsidePet(x, y) && m_dwPetDragTick && timeGetTime() - m_dwPetDragTick < 300 ) // 금방 뗀거면 클릭으로 인정.
		{
			if( m_nAnimation == HELPPET_ANI_THROW ) // 만약 던지기 중에 클릭이라면..
			{
				//펫도움말확장
				if( timeGetTime() - m_dwPetAniChageTick < 500 ) // 약간 어리둥절 타임을 준다. // 이 시간안에 클릭하면 한번더 튕길수 있게 해준다.
				{
					m_ClickX = m_PetRect.left+(m_PetRect.right/2) - x; // 방향을 펫을 클릭한 위치의 반대로...
					m_ClickX *= 20; // 이동 속도를 위해서 증가 처리
					m_ClickY = m_PetRect.top+(m_PetRect.bottom/2) - y; // 방향을 펫을 클릭한 위치의 반대로...
					m_ClickY *= 20; // 이동 속도를 위해서 증가 처리
					m_ThrowPower = 500; // 더 오래 튕길수있게 해준다.
				}
			}
			else // 집어 던지기 처리.
			{
				float Power = DistPlToPlAbs( (float)m_ClickX, (float)m_ClickY, (float)x, (float)y ); // 움직인 거리에 힘은 비례한다.

				if( Power >= 30 ) // 일정거리 이상 빠르게 움직여서 땐 경우만 던지기로 인정.
				{
					m_ClickX = x - m_ClickX; // 던진 방향으로 사용된다.
					m_ClickY = y - m_ClickY;

//					m_ThrowPower = Power * 10; // 던진 거리.
//					m_ThrowPower = (timeGetTime() - m_dwPetDragTick) * 10; // 놓은 시간에 비례.
					m_ThrowPower = (Power*2) + (( 300-(timeGetTime() - m_dwPetDragTick) )*2); // 던진 거리에 비례. 놓은 시간에 반비례.

					SetPetAnimation( HELPPET_ANI_THROW ); // 던지기 상태로.
				}
				else if( m_nAnimation != HELPPET_ANI_THROW )
				{
					// 괴롭혀서 올라가게 만들기 처리를 해주자.
					m_PetRect.top -= 40.0f;

					// 좌우로도 약간 움직임.
					m_PetRect.left += RandomNum( -35.0f, 35.0f );

					// 화면 넘어가면 보정.
					if( m_PetRect.left < 0 )
						m_PetRect.left = 0;
					if( m_PetRect.top < 0 )
						m_PetRect.top = 0;
					if( m_PetRect.left+m_PetRect.right > m_ClientRc.right )
						m_PetRect.left = m_ClientRc.right - m_PetRect.right;
					if( m_PetRect.top+m_PetRect.bottom > m_ClientRc.bottom )
						m_PetRect.top = m_ClientRc.bottom - m_PetRect.bottom;

					m_nDir ^= 1; // 방향도 토글. 깜짝 놀란듯이.
				}
			}

			m_bPetDrag = FALSE; // 리턴때문에 여기서도 한번더.
			m_dwPetDragTick = 0;

			return 1;
		}

		m_bPetDrag = FALSE;
		m_dwPetDragTick = 0;

		if( m_Searching || m_dwTalkStartTick ) // 말 풍선 보이는 상태.
		{
			m_NowSelResult = 0; // 초기화.

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_CloseBtn.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				TerminateTalkBox();
				m_Searching = 0;

				m_CloseBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.

				return 1;
			}

			if( (m_NowSelResult = IsInsideSearchBox(x,y)) )
			{
				if( m_Searching == 1 )
				{
					m_SearchBtn.MsgProc(hWnd, msg, wParam, lParam);

					if( m_SearchBtn.GetState()==BTN_ACTION )
					{
						// 검색 시작.
						HelpSearch( m_SearchInput.GetStrBuf() );

						m_SearchBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.

						return 1;
					}
				}
				else if( m_Searching == 2 )
				{
					m_PrevBtn.MsgProc(hWnd, msg, wParam, lParam);
					if( m_PrevBtn.GetState()==BTN_ACTION )
					{
						if( m_NowViewResult > 0 )
							m_NowViewResult -= HELPPET_SEARCHRESULT_VIEWINPAGE;

						if( m_NowViewResult < 0 )
							m_NowViewResult = 0;

						// 말풍선 사이즈 재계산.
						CMultiText TempMultiText; // CAlphaPopUp의 PutString 이 이 클래스만 함수가 있어서 보이는 단위로 변환하기 용.
						int MinWidth = 0; // 최소 사이즈 보장용.
						CAlphaPopUp TempPopup; // 사이즈 계산용.
						TempPopup.Init(16); // 폰트 사이즈. m_TextOutBox 랑 같은 폰트 크기여야한다.
						TempPopup.LoadRes(m_ClientRc.right, m_ClientRc.bottom );

						// IDS_PETHELP_SEARCHRESULT : * 검색 결과입니다. *
						TempMultiText.AddString( (char *)G_STRING(IDS_PETHELP_SEARCHRESULT) );
						TempPopup.PutString( &TempMultiText );
						MinWidth = TempPopup.GetSurWidth();
						TempMultiText.Reset();

						for( int i = 0 ; i < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++i )
						{
							if( m_NowViewResult+i < m_TotalResult )
								TempMultiText.AddString( m_SearchResultText.m_Text[m_NowViewResult+i] ); // 처음 화면에 해당하는 줄들을 인서트.
						}

						TempPopup.PutString( &TempMultiText );

						if( TempPopup.GetSurWidth() < MinWidth ) // 검색 결과 입니다 글자보다 짧다면 검색 결과입니다. 글자 길이에 맞춰준다.
							m_SearchRect.right = MinWidth+HELPPET_SEARCHRESULT_WIDTHSPACE; // 가장 긴 스트링을 기준으로 잡아준다. 여백 및 클로즈 버튼을 고려.
						else
							m_SearchRect.right = TempPopup.GetSurWidth()+HELPPET_SEARCHRESULT_WIDTHSPACE; // 가장 긴 스트링을 기준으로 잡아준다. 여백 및 클로즈 버튼을 고려.

						m_SearchRect.bottom = HELPPET_SEARCHRESULT_HEIGHT; // 그냥 고정 크기로 가자.
						/*
												if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // 한 화면 이상인 경우.
													m_SearchRect.bottom = (HELPPET_SEARCHRESULT_VIEWINPAGE+1) * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE + 40; // 버튼 크기도 고려.
												else // 한 화면인 경우.
													m_SearchRect.bottom = m_TotalResult * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE; // 마지막은 여백.
						*/

						m_PrevBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
						return 1;
					}

					m_NextBtn.MsgProc(hWnd, msg, wParam, lParam);
					if( m_NextBtn.GetState()==BTN_ACTION )
					{
						if( m_NowViewResult < m_TotalResult-HELPPET_SEARCHRESULT_VIEWINPAGE )
							m_NowViewResult += HELPPET_SEARCHRESULT_VIEWINPAGE;

						if( m_NowViewResult > m_TotalResult )
							m_NowViewResult = m_TotalResult;

						// 말풍선 사이즈 재계산.
						CMultiText TempMultiText; // CAlphaPopUp의 PutString 이 이 클래스만 함수가 있어서 보이는 단위로 변환하기 용.
						int MinWidth = 0; // 최소 사이즈 보장용.
						CAlphaPopUp TempPopup; // 사이즈 계산용.
						TempPopup.Init(16); // 폰트 사이즈. m_TextOutBox 랑 같은 폰트 크기여야한다.
						TempPopup.LoadRes( m_ClientRc.right, m_ClientRc.bottom );

						// IDS_PETHELP_SEARCHRESULT : * 검색 결과입니다. *
						TempMultiText.AddString( (char *)G_STRING(IDS_PETHELP_SEARCHRESULT) );
						TempPopup.PutString( &TempMultiText );
						MinWidth = TempPopup.GetSurWidth();
						TempMultiText.Reset();

						for( int i = 0 ; i < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++i )
						{
							if( m_NowViewResult+i < m_TotalResult )
								TempMultiText.AddString( m_SearchResultText.m_Text[m_NowViewResult+i] ); // 처음 화면에 해당하는 줄들을 인서트.
						}

						TempPopup.PutString( &TempMultiText );

						if( TempPopup.GetSurWidth() < MinWidth ) // 검색 결과 입니다 글자보다 짧다면 검색 결과입니다. 글자 길이에 맞춰준다.
							m_SearchRect.right = MinWidth+HELPPET_SEARCHRESULT_WIDTHSPACE; // 가장 긴 스트링을 기준으로 잡아준다. 여백 및 클로즈 버튼을 고려.
						else
							m_SearchRect.right = TempPopup.GetSurWidth()+HELPPET_SEARCHRESULT_WIDTHSPACE; // 가장 긴 스트링을 기준으로 잡아준다. 여백 및 클로즈 버튼을 고려.

						m_SearchRect.bottom = HELPPET_SEARCHRESULT_HEIGHT; // 그냥 고정 크기로 가자.
						/*
												if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // 한 화면 이상인 경우.
													m_SearchRect.bottom = (HELPPET_SEARCHRESULT_VIEWINPAGE+1) * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE + 40; // 버튼 크기도 고려.
												else // 한 화면인 경우.
													m_SearchRect.bottom = m_TotalResult * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE; // 마지막은 여백.
						*/

						m_NextBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.

						return 1;
					}

					// 선택지를 클릭했을 경우 처리.
					if( m_NowSelResult > 0 ) // 선택지 위가 아닐 경우 -1 이나 0이 올수있다.
					{
						int k = m_NowViewResult+(m_NowSelResult-1);

						if( k < m_TotalResult ) // 정상 범위일때만.
							ExcuteHelpContents( m_SearchResultIndex[k] ); // 내용을 보여준다.
					}
				}

				return 1; // 말 풍선 위를 클릭한건 딴 메세지 무시하도록...
			}
		}

		if( m_bContents )
		{
			m_bContentsDrag = FALSE;

			m_PrevBtn2.MsgProc(hWnd, msg, wParam, lParam);
			if( m_PrevBtn2.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				--m_nNowContentsPage;

				if( m_nNowContentsPage < 0 )
				{
					m_nNowContentsPage = 0;
				}

				m_PrevBtn2.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
				return 1;
			}

			m_NextBtn2.MsgProc(hWnd, msg, wParam, lParam);
			if( m_NextBtn2.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				++m_nNowContentsPage;

				if( m_nNowContentsPage > m_ListContentsSurface.size()-1 )
				{
					m_nNowContentsPage = m_ListContentsSurface.size()-1;
				}

				m_NextBtn2.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
				return 1;
			}

			m_CloseBtn2.MsgProc(hWnd, msg, wParam, lParam);
			if( m_CloseBtn2.GetState()==BTN_ACTION ) // 상점닫기 버튼 클릭시
			{
				TerminateContents();

				m_CloseBtn2.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.

				return 1;
			}
		}

		break;

	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_Searching || m_dwTalkStartTick ) // 말 풍선 보이는 상태.
		{
			m_NowSelResult = 0; // 초기화.

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			if( (m_NowSelResult = IsInsideSearchBox(x,y)) )
			{
				if( m_Searching == 1 )
					m_SearchBtn.MsgProc(hWnd, msg, wParam, lParam);
				else if( m_Searching == 2 )
				{
					m_PrevBtn.MsgProc(hWnd, msg, wParam, lParam);
					m_NextBtn.MsgProc(hWnd, msg, wParam, lParam);
				}
			}
		}

		if( m_bContents )
		{
			m_PrevBtn2.MsgProc(hWnd, msg, wParam, lParam);
			m_NextBtn2.MsgProc(hWnd, msg, wParam, lParam);
			m_CloseBtn2.MsgProc(hWnd, msg, wParam, lParam);

			if( m_bContentsDrag )
			{
				m_ContentsRect.left = x + m_DragDistX;
				m_ContentsRect.top = y + m_DragDistY;

				// 화면 넘어가면 보정.
				if( m_ContentsRect.left < 0 )
					m_ContentsRect.left = 0;
				if( m_ContentsRect.top < 0 )
					m_ContentsRect.top = 0;
				if( m_ContentsRect.left+m_ContentsRect.right > m_ClientRc.right )
					m_ContentsRect.left = m_ClientRc.right - m_ContentsRect.right;
				if( m_ContentsRect.top+m_ContentsRect.bottom > m_ClientRc.bottom )
					m_ContentsRect.top = m_ClientRc.bottom - m_ContentsRect.bottom;
			}
		}

		if( m_bPetDrag && !m_ThrowPower ) // 드래그 중이면...
		{
			m_PetRect.left = x + m_DragDistX;
			m_PetRect.top = y + m_DragDistY;

			// 화면 넘어가면 보정.
			if( m_PetRect.left < 0 )
				m_PetRect.left = 0;
			if( m_PetRect.top < 0 )
				m_PetRect.top = 0;
			if( m_PetRect.left+m_PetRect.right > m_ClientRc.right )
				m_PetRect.left = m_ClientRc.right - m_PetRect.right;
			if( m_PetRect.top+m_PetRect.bottom > m_ClientRc.bottom )
				m_PetRect.top = m_ClientRc.bottom - m_PetRect.bottom;
		}
		else if( IsInsidePet(x, y) ) // 펫에 마우스 오버시.
		{
		}

		break;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( IsInsidePet(x, y) )
		{
			// 현재 브라질만 적용되어 있음
			if( g_dwClientCountry != CTRY_KOR )
			{
				// 검색 기능을 켜준다.
				m_Searching = 1;
				m_SearchRect.right = 200;
				m_SearchRect.bottom = 70;

				m_SearchInput.SetEnable( TRUE ); // 바로 입력할 수있도록 셋팅.
				pCMyApp->RestoreImmStatus(); ///일본전각반각버그수정
			}

			return 1; // 딴 메세지 무시하도록...
		}
		else if( m_dwTalkStartTick && timeGetTime()-m_dwTalkStartTick > HELPPET_TALKBOX_VIEWTERM
				 && IsInsideTalkBox(x,y) ) // 찾은 후 약간 뒤부터 말풍선을 출력해준다.
		{
			TerminateTalkBox();
			return 1;
		}


		break;

	case WM_LBUTTONDBLCLK:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_dwTalkStartTick && IsInsideTalkBox(x, y) ) // 말칸의 대사를 더블클릭한 경우.
		{
			// 상세 페이지를 열어준다.

			if( ExcuteHelpContents(m_TalkIndex) == FALSE && m_TalkIndex != -1 )
			{
				// 보여줄 컨텐츠가 없는 경우.
				// IDS_PETHELP_NOWREADY : 이 내용은 아직 준비 중이야.
				ExcuteHelpTalk( (char *)G_STRING(IDS_PETHELP_NOWREADY) );
				return 1; // 밑에 TerminateTalkBox 안하기 위해 캔슬.
			}

			// 더블 클릭했으면 지금 대사는 꺼준다.
			TerminateTalkBox();

			return 1;
		}
//		else if( IsInsidePet(x, y) ) // 검색 기능 활성화.
//		{
//		}
		else if( m_bContents && IsInsideContents(x, y) )
		{
			return 1;
		}

		break;

	case WM_KEYDOWN:

		if( wParam ==VK_RETURN )
		{
			if( m_Searching == 1 && m_SearchInput.GetEnable() ) // 찾기 입력중 엔터면...
			{
				// 검색 시작.
				HelpSearch( m_SearchInput.GetStrBuf() );
			}
		}

		break;

	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:

		if( m_SearchInput.GetEnable() )
			m_SearchInput.MsgProc( hWnd, msg, wParam, lParam );

		if( m_SearchInput.GetEnable() )
			return 1;
		else
			return 0;

		break;

	}

	return 0;
}

BOOL CControlHelpPet::IsInsidePet( int x, int y )
{
	if (x >= m_PetRect.left  && x < m_PetRect.left + m_PetRect.right &&
			y >= m_PetRect.top && y < m_PetRect.top + m_PetRect.bottom )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CControlHelpPet::IsInsideTalkBox( int x, int y )
{
	if( !m_pTextSurface )
		return FALSE;

//	if (x >= m_TalksRect.left  && x < m_TalksRect.left + m_TalksRect.right &&
//		y >= m_TalksRect.top && y < m_TalksRect.top + m_TalksRect.bottom )
	if (x >= m_pTextSurface->Xpos  && x < m_pTextSurface->Xpos + m_pTextSurface->GetWidth() &&
			y >= m_pTextSurface->Ypos && y < m_pTextSurface->Ypos + m_pTextSurface->GetHeight() )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CControlHelpPet::IsInsideContents( int x, int y )
{
	if (x >= m_ContentsRect.left  && x < m_ContentsRect.left + m_ContentsRect.right &&
			y >= m_ContentsRect.top && y < m_ContentsRect.top + m_ContentsRect.bottom )
	{
		return TRUE;
	}

	return FALSE;
}

int CControlHelpPet::IsInsideSearchBox( int x, int y )
{
	int Result = 0;

	if (x >= m_SearchRect.left  && x < m_SearchRect.left + m_SearchRect.right &&
			y >= m_SearchRect.top && y < m_SearchRect.top + m_SearchRect.bottom )
	{
		Result = -1; // 박스 안에만 있음.

		// 선택지 선택 여부 확인.
		// 상하 뒤집어야 하는지 체크.
		int ReverseY = 0;
		if( m_SearchRect.top < 0 ) // 넘어간다면...
		{
			ReverseY += 34;
		}

		int NowCheckY = 0;

		for( int k = 0 ; k < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++k )
		{
			NowCheckY = m_SearchRect.top+HELPPET_SEARCHRESULT_SEL_START_Y+(k*HELPPET_SEARCHRESULT_SEL_LINE_HEIGHT)+ReverseY; // 이번에 체크할 선택지의 y 위치.

			if (x >= m_SearchRect.left+6  && x < m_SearchRect.left+6+m_SearchRect.right &&
					y >= NowCheckY && y < NowCheckY+HELPPET_SEARCHRESULT_SEL_LINE_HEIGHT-2 )
			{
				return (k+1);
			}
		}

		return Result;
	}

	return Result;
}


int CControlHelpPet::CheckHelpCondition( int ConType, int Arg, BOOL bForced ) // 등록된 헬프들의 조건을 돌면서 만족하는 녀석이 있는지 체크한다.
{
	// 캔슬 조건들 부터 체크.
	if( !bForced ) // 강제 체크 조건도 있다.
	{
		if( m_nAnimation == HELPPET_ANI_FIND || timeGetTime()-m_dwOldConCheckTick < 60000 ) // 평상시만 검사. 일정 주기로만 검사.
			return -1;
	}
	else if( ConType != HELP_CON_TYPE_F_SERVER ) // 강제 체크라고 해도 표시 중엔 안들어가게. (서버명령은 예외)
	{
		if( m_dwTalkStartTick && timeGetTime()-m_dwTalkStartTick <= HELPPET_TALKBOX_CLOSETIME ) // 말풍선이 표시중이면...
			return -1;
	}

	int i = 0;
	int len = 0;
	int TempInt1 = 0;
	int TempInt2 = 0;
	char *arg = NULL; // 참조 포인터.
	char strTemp2[50];

	sHelpData *TempHelp;
	list<sHelpData*>::iterator prHelp;
	for( prHelp = m_HelpManager.m_listHelp.begin(); prHelp != m_HelpManager.m_listHelp.end(); ++prHelp )
	{
		TempHelp = *prHelp;

		// 예외 조건 체크.
		switch( TempHelp->m_ExceptionCode )
		{
		case HELP_EXCEP_CODE_ONLY_ACHERON: // 아케론에서만 적용.

			if (g_SvrType != ST_ADULT_ONLY) // 아케론이 아니면.
				continue; // 스킵.
			break;
		}


		// 쿨타임 체크.
		if( TempHelp->m_ActivateTick && TempHelp->m_CoolTime > 0 && ConType != HELP_CON_TYPE_F_SERVER ) // 서버에서 보내온 경우는 강제 이므로 쿨타임 체크안하게 수정 07-09-03
		{
			if( timeGetTime()-TempHelp->m_ActivateTick < (TempHelp->m_CoolTime*1000) ) // 아직 쿨타임이 안끝난경우.
			{
				continue; // 스킵
			}
			else // 쿨타임이 끝난경우.
			{
				TempHelp->m_ActivateTick = 0; // 초기화 해준다.
			}
		}

		if( ConType == HELP_CON_TYPE_F_SERVER ) // 서버에서의 강제 실행(조건 타입과 상관없이 가능.)
		{
			if( Arg == TempHelp->m_Index ) // 서버로부터 헬프 인덱스가 날라온다.
			{
				ExcuteHelpTalk( TempHelp );
				return TempHelp->m_Index; // 인덱스를 리턴해준다. 먼저 찾는 녀석을 우선시 한다. 즉, 우선 순위를 높이려면 위로 올려라..
			}
		}
		else if( TempHelp->m_ConditionType == ConType ) // 같은 조건을 가진 녀석이면...
		{
			// 조건 체크.
			switch( TempHelp->m_ConditionType )
			{
			case HELP_CON_TYPE_F_LEVELUP: // 레벨 도달 조건.
				if( atoi(TempHelp->m_strConArg) == Arg ) // 인자를 단순 숫자 하나만 받는다.
				{
					ExcuteHelpTalk( TempHelp );
					return TempHelp->m_Index; // 인덱스를 리턴해준다. 먼저 찾는 녀석을 우선시 한다. 즉, 우선 순위를 높이려면 위로 올려라..
				}
				break;
			case HELP_CON_TYPE_A_LEVEL: // 레벨대 수시 체크 조건.

				// 이 녀석은 인자가 두개임에 유의.
				sscanf( TempHelp->m_strConArg, "%d %d", &TempInt1, &TempInt2 );

				if( g_pRoh->m_Level >= TempInt1 && g_pRoh->m_Level <= TempInt2 )
				{
					ExcuteHelpTalk( TempHelp );
					return TempHelp->m_Index; // 인덱스를 리턴해준다. 먼저 찾는 녀석을 우선시 한다. 즉, 우선 순위를 높이려면 위로 올려라..
				}

				break;
			case HELP_CON_TYPE_F_ITEM:

				if( !TempHelp->m_strConArg )
					break;

				arg = AnyOneArg( TempHelp->m_strConArg, strTemp2 ); // 갯수
				TempInt1 = atoi( strTemp2 );

				if( TempInt1 > 100 ) // 너무 크면 뭔가 잘못 입력된 것으로 간주.
				{
					sprintf( strTemp2, "%d PetHelp Item Condition wrong!", TempHelp->m_Index );
					MessageBox( NULL, strTemp2, "Waring!", MB_OK );
				}

				for( i = 0 ; i < TempInt1 ; ++i )
				{
					arg = AnyOneArg(arg, strTemp2 );

					if( atoi(strTemp2) == Arg )
					{
						ExcuteHelpTalk( TempHelp );
						return TempHelp->m_Index; // 인덱스를 리턴해준다. 먼저 찾는 녀석을 우선시 한다. 즉, 우선 순위를 높이려면 위로 올려라..
					}
				}


				break;
			/*          // 서버에서 실행시키는걸 해당 타입만 되는게 아니라 전 타입이 가능하도록 수정했다. 즉, 서버 타입 조건은 별다른 클라이언트 체크 조건이 없이 사용하는데만 쓰이고 인자도 무시된다.
						case HELP_CON_TYPE_F_SERVER:
							if( atoi(TempHelp->m_strConArg) == Arg ) // 인자를 단순 숫자 하나만 받는다.
							{
								ExcuteHelpTalk( TempHelp );
								return TempHelp->m_Index; // 인덱스를 리턴해준다. 먼저 찾는 녀석을 우선시 한다. 즉, 우선 순위를 높이려면 위로 올려라..
							}
							break;
			*/
			case HELP_CON_TYPE_F_NPC_IN:

				if( atoi(TempHelp->m_strConArg) == Arg ) // 인자를 단순 숫자 하나만 받는다.
				{
					ExcuteHelpTalk( TempHelp );
					return TempHelp->m_Index; // 인덱스를 리턴해준다. 먼저 찾는 녀석을 우선시 한다. 즉, 우선 순위를 높이려면 위로 올려라..
				}
				break;
			}

		}
	}


	m_dwOldConCheckTick = timeGetTime();

	return -1;
}

BOOL CControlHelpPet::ExcuteHelpTalk( int HelpIndex ) // 대사를 발동 시킨다.
{
	sHelpData *TempHelp = NULL;
	list<sHelpData*>::iterator prHelp;
	for( prHelp = m_HelpManager.m_listHelp.begin(); prHelp != m_HelpManager.m_listHelp.end(); ++prHelp )
	{
		if( (*prHelp)->m_Index == HelpIndex ) // 같은 녀석이면...
		{
			TempHelp = *prHelp;

			break;
		}
	}

	if( !TempHelp )
		return FALSE;

	return ExcuteHelpTalk( TempHelp );
}

BOOL CControlHelpPet::ExcuteHelpTalk( sHelpData *TempHelp ) // 대사를 발동 시킨다.
{
	if( !TempHelp )
		return FALSE;

	m_Searching = 0; // 검색 중이었으면 검색을 취소해준다.
	
	m_TalksText.Reset(); // 초기화후 넣어준다.

	if( g_pRoh->m_Chaotic_Grade ) // 이게 0이 아니면 카오상태다.
	{
		if( TempHelp->m_strChaoticTalk && strlen(TempHelp->m_strChaoticTalk) )
		{
			int PrevStringPoint = 0; // 이전에 스트링을 끊었던 위치.
			int StrReadPt = 0;
			char *strTemp = NULL;

			int ContantsTotalLen = strlen(TempHelp->m_strChaoticTalk);

			while( StrReadPt < ContantsTotalLen ) // 전체 스트링을 돌면서...
			{
				if( TempHelp->m_strChaoticTalk[StrReadPt] == '\\' && TempHelp->m_strChaoticTalk[StrReadPt+1] == 'n' ) // 개행 문자가 있을 경우에 새 라인을 만든다.
				{
					int Len = StrReadPt-PrevStringPoint;
					SAFE_DELETE_ARRAY( strTemp );
					strTemp = new char[Len+1];

					if( strTemp )
					{
						strncpy( strTemp, &TempHelp->m_strChaoticTalk[PrevStringPoint], Len );
						strTemp[Len] = '\0';

						m_TalksText.AddString( strTemp );

						StrReadPt += 2; // 이건 개행 문자분도 넘긴다.

						PrevStringPoint = StrReadPt; // 이전에 끊은 위치를 기억해둔다.

						continue; // 한번 끊어준다.
					}
				}

				if( StrReadPt >= ContantsTotalLen ) // 마지막 줄일경우 의 처리.
				{
					int Len = ContantsTotalLen-PrevStringPoint;
					SAFE_DELETE_ARRAY( strTemp );
					strTemp = new char[Len+1];

					if( strTemp )
					{
						strncpy( strTemp, &TempHelp->m_strChaoticTalk[PrevStringPoint], Len );
						strTemp[Len] = '\0';

						m_TalksText.AddString( strTemp );

						PrevStringPoint = ContantsTotalLen; // 이전에 끊은 위치를 기억해둔다.

						continue; // 한번 끊어준다.
					}
				}
			}
		}
	}
	else // 일반 상태.
	{
		if( TempHelp->m_strNormalTalk && strlen(TempHelp->m_strNormalTalk) )
		{
			int PrevStringPoint = 0; // 이전에 스트링을 끊었던 위치.
			int StrReadPt = 0;
			char *strTemp = NULL;

			int ContantsTotalLen = strlen(TempHelp->m_strNormalTalk);

			while( StrReadPt < ContantsTotalLen ) // 전체 스트링을 돌면서...
			{
				if( TempHelp->m_strNormalTalk[StrReadPt] == '\\' && TempHelp->m_strNormalTalk[StrReadPt+1] == 'n' ) // 개행 문자가 있을 경우에 새 라인을 만든다.
				{
					int Len = StrReadPt-PrevStringPoint;
					SAFE_DELETE_ARRAY( strTemp );
					strTemp = new char[Len+1];

					if( strTemp )
					{
						strncpy( strTemp, &TempHelp->m_strNormalTalk[PrevStringPoint], Len );
						strTemp[Len] = '\0';

						m_TalksText.AddString( strTemp );

						StrReadPt += 2; // 이건 개행 문자분도 넘긴다.

						PrevStringPoint = StrReadPt; // 이전에 끊은 위치를 기억해둔다.

						continue; // 한번 끊어준다.
					}
				}

				// 끊을게 아니면 진행처리.
				if( TempHelp->m_strNormalTalk[StrReadPt] < 0 || TempHelp->m_strNormalTalk[StrReadPt] > 127 ) // 우선 현재 문자가 2바이트 코드인지를 확인
				{
					// 2 바이트 문자일 경우.
					StrReadPt += 2; // 2 바이트니까 다음 읽을 포인틀르 2증가시켜서 체크. (타이밍을 여기서 하는 이유는 아래 길이 구하기 때문.)
				}
				else // 1바이트 코드
				{
					StrReadPt += 1;
				}

				if( StrReadPt >= ContantsTotalLen ) // 마지막 줄일경우 의 처리.
				{
					int Len = ContantsTotalLen-PrevStringPoint;
					SAFE_DELETE_ARRAY( strTemp );
					strTemp = new char[Len+1];

					if( strTemp )
					{
						strncpy( strTemp, &TempHelp->m_strNormalTalk[PrevStringPoint], Len );
						strTemp[Len] = '\0';

						m_TalksText.AddString( strTemp );

						PrevStringPoint = ContantsTotalLen; // 이전에 끊은 위치를 기억해둔다.

						continue; // 한번 끊어준다.
					}
				}
			}
		}
	}

	if( m_TalksText.m_LineCount <= 0 ) // 텍스트가 없는 경우..
		return FALSE;

	// 필요한 사각형 영역을 계산.
	int GreatestCount=0; // 가장큰 문자열의 개수를 얻는다
	SIZE size;
	int TextWidth = 0;
	int TextSurWidth = 0;
	int TextSurHeight = 0;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);
	int FontWidth  = sizeFont.cx;

	// 가장큰 문자열의 개수를 얻는다
	ZeroMemory( &m_TalksRect, sizeof(RECT) );
	for(int i = 0; i < m_TalksText.m_LineCount; i++)
	{
		// 입력받은 텍스트를 루프를 돌면서 가장긴 문자열 체크
		int len = strlen(m_TalksText.m_Text[i]);
		// 문자열의 폭이 고정되었는가?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// 폰트 사이즈 반환
			GetTextExtentPoint32(hdc, m_TalksText.m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextWidth = size.cx;
		};
	}

	if (g_bFixedWidthFont)
	{
		// 가장긴 문자열과 라인수에 의해서 서피스 생성
		TextSurWidth = GreatestCount*FontWidth;
		TextSurHeight = m_TalksText.m_LineCount*HELP_USE_LINE_HEIGHT;
	}
	else
	{
		TextSurWidth = TextWidth;
		TextSurHeight = m_TalksText.m_LineCount*HELP_USE_LINE_HEIGHT;

	}

	SAFE_DELETE(m_pTextSurface);

	// 계산된 크기로 서피스 생성
	g_pDisplay->CreateSurface( &m_pTextSurface, TextSurWidth, TextSurHeight );
	if( !m_pTextSurface )
		return FALSE;

	m_pTextSurface->SetWidth( TextSurWidth ); // 이건 꼭 지정되어야 한다.
	m_pTextSurface->SetHeight( TextSurHeight );


	m_TalksRect.right = TextSurWidth+HELP_TALKBOX_XPLUS_SIZE+20; // 사이즈를 기억해둔다. 말풍선 여백을 고려해서 좀더 넉넉하게 한다.
	m_TalksRect.bottom = TextSurHeight+HELP_TALKBOX_YPLUS_SIZE;

	// fillcolor를 이용한 textsurface 클리어하기
	m_pTextSurface->Clear();

	// 들어온 문자열 가운데로 정렬해서 DrawText
	int Xpos = 0;
	int Ypos = 0;

	Ypos = 0;
	for( int i = 0 ; i < m_TalksText.m_LineCount ; ++i )
	{
		if (g_bFixedWidthFont)
			Xpos =  ((TextSurWidth/2) - ((strlen(m_TalksText.m_Text[i])*FontWidth) /2));
		else
		{
			GetTextExtentPoint32(hdc, m_TalksText.m_Text[i], strlen(m_TalksText.m_Text[i]), &size);
			Xpos =  TextSurWidth/2 - size.cx/2;
		}

		//-- Lyul 수정 -- Shadow : 1
//		m_pTextSurface->DrawText(m_Font, m_TalksText.m_Text[i], Xpos, Ypos,
//			m_TalksText.m_TextBackColor[i], m_TalksText.m_TextColor[i], 1 ,RGB(10, 10, 10));
		//-- Lyul 수정 -- Normal : 0
		m_pTextSurface->DrawText( m_Font, m_TalksText.m_Text[i], Xpos, Ypos,
								  m_TalksText.m_TextBackColor[i], m_TalksText.m_TextColor[i], 0);

		Ypos += HELP_USE_LINE_HEIGHT;
	}
	m_pTextSurface->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	SetPetAnimation( HELPPET_ANI_FIND ); // 찾았다 애니메이션으로 바꿔준다.

	// 마지막으로 쿨타임 설정을 해준다.
	TempHelp->m_ActivateTick = m_dwTalkStartTick = timeGetTime(); // 말풍선 제어 틱도 설정.

	m_TalkIndex = TempHelp->m_Index;

	return TRUE;
}

BOOL CControlHelpPet::ExcuteHelpTalk( const char *strTalk ) // 직접 대사를 입력하는 경우...
{
	m_Searching = 0; // 검색 중이었으면 검색을 취소해준다.

	m_TalksText.Reset(); // 초기화후 넣어준다.

	if( strTalk )
	{
		int PrevStringPoint = 0; // 이전에 스트링을 끊었던 위치.
		int StrReadPt = 0;
		char *strTemp = NULL;

		int ContantsTotalLen = strlen(strTalk);

		while( StrReadPt < ContantsTotalLen ) // 전체 스트링을 돌면서...
		{
			if( strTalk[StrReadPt] == '\\' && strTalk[StrReadPt+1] == 'n' ) // 개행 문자가 있을 경우에 새 라인을 만든다.
			{
				int Len = StrReadPt-PrevStringPoint;
				SAFE_DELETE_ARRAY( strTemp );
				strTemp = new char[Len+1];

				if( strTemp )
				{
					strncpy( strTemp, &strTalk[PrevStringPoint], Len );
					strTemp[Len] = '\0';

					m_TalksText.AddString( strTemp );

					StrReadPt += 2; // 이건 개행 문자분도 넘긴다.

					PrevStringPoint = StrReadPt; // 이전에 끊은 위치를 기억해둔다.

					continue; // 한번 끊어준다.
				}
			}

			// 끊을게 아니면 진행처리.
			if( strTalk[StrReadPt] < 0 || strTalk[StrReadPt] > 127 ) // 우선 현재 문자가 2바이트 코드인지를 확인
			{
				// 2 바이트 문자일 경우.
				StrReadPt += 2; // 2 바이트니까 다음 읽을 포인틀르 2증가시켜서 체크. (타이밍을 여기서 하는 이유는 아래 길이 구하기 때문.)
			}
			else // 1바이트 코드
			{
				StrReadPt += 1;
			}


			if( StrReadPt >= ContantsTotalLen ) // 마지막 줄일경우 의 처리.
			{
				int Len = ContantsTotalLen-PrevStringPoint;
				SAFE_DELETE_ARRAY( strTemp );
				strTemp = new char[Len+1];

				if( strTemp )
				{
					strncpy( strTemp, &strTalk[PrevStringPoint], Len );
					strTemp[Len] = '\0';

					m_TalksText.AddString( strTemp );

					PrevStringPoint = ContantsTotalLen; // 이전에 끊은 위치를 기억해둔다.

					continue; // 한번 끊어준다.
				}
			}
		}
	}

	// 필요한 사각형 영역을 계산.
	int GreatestCount=0; // 가장큰 문자열의 개수를 얻는다
	SIZE size;
	int TextWidth = 0;
	int TextSurWidth = 0;
	int TextSurHeight = 0;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);
	int FontWidth  = sizeFont.cx;

	// 가장큰 문자열의 개수를 얻는다
	ZeroMemory( &m_TalksRect, sizeof(RECT) );
	for(int i = 0; i < m_TalksText.m_LineCount; i++)
	{
		// 입력받은 텍스트를 루프를 돌면서 가장긴 문자열 체크
		int len = strlen(m_TalksText.m_Text[i]);
		// 문자열의 폭이 고정되었는가?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// 폰트 사이즈 반환
			GetTextExtentPoint32(hdc, m_TalksText.m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextWidth = size.cx;
		};
	}

	if (g_bFixedWidthFont)
	{
		// 가장긴 문자열과 라인수에 의해서 서피스 생성
		TextSurWidth = GreatestCount*FontWidth;
		TextSurHeight = m_TalksText.m_LineCount*HELP_USE_LINE_HEIGHT;
	}
	else
	{
		TextSurWidth = TextWidth;
		TextSurHeight = m_TalksText.m_LineCount*HELP_USE_LINE_HEIGHT;

	}

	SAFE_DELETE(m_pTextSurface);

	// 계산된 크기로 서피스 생성
	g_pDisplay->CreateSurface( &m_pTextSurface, TextSurWidth, TextSurHeight );
	if( !m_pTextSurface )
		return FALSE;

	m_TalksRect.right = TextSurWidth+HELP_TALKBOX_XPLUS_SIZE + 20; // 사이즈를 기억해둔다. 말풍선 여백을 고려해서 좀더 넉넉하게 한다.
	m_TalksRect.bottom = TextSurHeight+HELP_TALKBOX_YPLUS_SIZE;

	// fillcolor를 이용한 textsurface 클리어하기
	m_pTextSurface->Clear();

	// 들어온 문자열 가운데로 정렬해서 DrawText
	int Xpos = 0;
	int Ypos = 0;

	Ypos = 0;
	for( int i = 0 ; i < m_TalksText.m_LineCount ; ++i )
	{
		if (g_bFixedWidthFont)
			Xpos =  ((TextSurWidth/2) - ((strlen(m_TalksText.m_Text[i])*FontWidth) /2));
		else
		{
			GetTextExtentPoint32(hdc, m_TalksText.m_Text[i], strlen(m_TalksText.m_Text[i]), &size);
			Xpos =  TextSurWidth/2 - size.cx/2;
		}

		//-- Lyul 수정 -- Shadow : 1
//		m_pTextSurface->DrawText(m_Font, m_TalksText.m_Text[i], Xpos, Ypos,
//			m_TalksText.m_TextBackColor[i], m_TalksText.m_TextColor[i], 1 ,RGB(10, 10, 10));
		//-- Lyul 수정 -- Normal : 0
		m_pTextSurface->DrawText( m_Font, m_TalksText.m_Text[i], Xpos, Ypos,
								  m_TalksText.m_TextBackColor[i], m_TalksText.m_TextColor[i], 0);

		Ypos += HELP_USE_LINE_HEIGHT;
	}
	m_pTextSurface->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

//	SetPetState( HELPPET_STATE_FIND ); // 찾았다 스테이터스로 바꿔준다.
	SetPetAnimation( HELPPET_ANI_FIND ); // 찾았다 애니메이션으로 바꿔준다.

	// 마지막으로 쿨타임 설정을 해준다.
	m_dwTalkStartTick = timeGetTime(); // 말풍선 제어 틱도 설정.
	m_TalkIndex = -1;

	return TRUE;
}



BOOL CControlHelpPet::ExcuteHelpContents( int HelpIndex ) // 자세한 내용 보여주기를 발동 시킨다.
{
	if( HelpIndex < 0 )
		return FALSE;

	TerminateContents(); // 초기화를 위해.
	m_Searching = 0; // 검색 중이였으면 검색 취소.

	// 일단 한번 깔끔하게 지운다.
	CSurface *pImage;
	vector<CSurface*>::iterator prImage;
	for( prImage = m_ListContentsSurface.begin(); prImage != m_ListContentsSurface.end(); ++prImage )
	{
		pImage = *prImage;
		SAFE_DELETE( pImage );
	}
	m_ListContentsSurface.clear();


	CSurface *TempSurface = NULL;
	int i = 0;
	char strTemp[512];
	sHelpData *TempHelp = NULL;
	list<sHelpData*>::iterator prHelp;
	for( prHelp = m_HelpManager.m_listHelp.begin(); prHelp != m_HelpManager.m_listHelp.end(); ++prHelp )
	{
		TempHelp = *prHelp;
		if( TempHelp->m_Index == HelpIndex ) // 같은 녀석이면...
		{
			if( TempHelp->m_listImage.size() <= 0 ) // 이미지가 등록이 안된 경우다.
				return FALSE;

			// JPG 그림들을 로드해서 리스트에 넣어준다.
			m_ListContentsSurface.reserve( TempHelp->m_listImage.size() ); // 벡터 갯수 예약.

			int k = 0;
			for( i = 0 ; i < TempHelp->m_listImage.size() ; ++i )
			{
				sprintf( strTemp, "./interface/HelpPet/Details/%s", TempHelp->m_listImage[k] ); // 경로조정.
				g_pDisplay->CreateSurfaceFromJPG( &TempSurface, strTemp );
				m_ListContentsSurface.push_back( TempSurface ); // 널이면 널이라도 넣자?

				++k;
			}

			m_bContents = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

void CControlHelpPet::TerminateContents() // 보고있던 컨텐츠 부분을 초기화 해주는 녀석.
{
	m_nNowContentsPage = 0;
	m_bContents = FALSE;
}

void CControlHelpPet::TerminateTalkBox() // 말풍선을 지운다.
{
	ZeroMemory( &m_TalksRect, sizeof(RECT) );
	SAFE_DELETE( m_pTextSurface );

	m_dwTalkStartTick = 0; // 말풍선을 꺼준다.
	m_TalkIndex = -1;
}

BOOL CControlHelpPet::HelpSearch( char *SearchWord ) // 헬프 검색 기능.
{
	m_SearchResultText.Reset();
	m_SearchResultIndex.clear(); // 일단 싹 지우고.

	sHelpData *TempHelp = NULL;
	list<sHelpData*>::iterator prHelp;
	for( prHelp = m_HelpManager.m_listHelp.begin(); prHelp != m_HelpManager.m_listHelp.end(); ++prHelp )
	{
		TempHelp = *prHelp;

		bool bSearchAtThis = FALSE; // 이 아이템에서 오케이 되었는지 여부. 다음 검색어를 스킵하기 위해 체크.
		int ItemNameLen = 0;
		int SearchLen = strlen( SearchWord );
		int j = 0;

		if( TempHelp->m_strSearchTitle && strlen(TempHelp->m_strSearchTitle) ) // 검색어 제목이 있을때만 검색하자.
		{
			if( TempHelp->m_strSearchWord1 && strlen(TempHelp->m_strSearchWord1) ) // 제 1 겸색어 체크.
			{
				ItemNameLen = strlen( TempHelp->m_strSearchWord1 );

				for( j = 0 ; j <= ItemNameLen-SearchLen ; ++j )
				{
					if( strncmp(&TempHelp->m_strSearchWord1[j],SearchWord,SearchLen) == 0 ) // 같은 이름이 있으면...
					{
						m_SearchResultText.AddString( TempHelp->m_strSearchTitle );
						m_SearchResultIndex.push_back( TempHelp->m_Index );

						bSearchAtThis = TRUE;

						break; // 이 검색어 체크의 루프를 넘긴다.
					}
				}
			}

			if( !bSearchAtThis && TempHelp->m_strSearchWord2 && strlen(TempHelp->m_strSearchWord2) ) // 제 1 검색어에서 못찾은 경우. 제 2 검색어를 체크한다.
			{
				ItemNameLen = strlen( TempHelp->m_strSearchWord2 );

				for( j = 0 ; j <= ItemNameLen-SearchLen ; ++j )
				{
					if( strncmp(&TempHelp->m_strSearchWord2[j],SearchWord,SearchLen) == 0 ) // 같은 이름이 있으면...
					{
						m_SearchResultText.AddString( TempHelp->m_strSearchTitle );
						m_SearchResultIndex.push_back( TempHelp->m_Index );

						bSearchAtThis = TRUE;

						break; // 이 검색어 체크의 루프를 넘긴다.
					}
				}
			}

			if( !bSearchAtThis && TempHelp->m_strSearchWord3 && strlen(TempHelp->m_strSearchWord3) ) // 제 1 검색어에서 못찾은 경우. 제 2 검색어를 체크한다.
			{
				ItemNameLen = strlen( TempHelp->m_strSearchWord3 );

				for( j = 0 ; j <= ItemNameLen-SearchLen ; ++j )
				{
					if( strncmp(&TempHelp->m_strSearchWord3[j],SearchWord,SearchLen) == 0 ) // 같은 이름이 있으면...
					{
						m_SearchResultText.AddString( TempHelp->m_strSearchTitle );
						m_SearchResultIndex.push_back( TempHelp->m_Index );

						bSearchAtThis = TRUE;

						break; // 이 검색어 체크의 루프를 넘긴다.
					}
				}
			}
		}
	}

	// 결과 표시용 수치들 셋팅.
	m_Searching = 2; // 검색 결과 표시 상태로 바꾼다.
	m_TotalResult = m_SearchResultText.m_LineCount; // 총 갯수를 저장.
	m_NowViewResult = 0; // 이건 초기화.

	// 사이즈 계산.
	CMultiText TempMultiText; // CAlphaPopUp의 PutString 이 이 클래스만 함수가 있어서 보이는 단위로 변환하기 용.
	int MinWidth = 0; // 최소 사이즈 보장용.
	CAlphaPopUp TempPopup; // 사이즈 계산용.
	TempPopup.Init(16); // 폰트 사이즈. m_TextOutBox 랑 같은 폰트 크기여야한다.
	TempPopup.LoadRes( m_ClientRc.right, m_ClientRc.bottom );

	if( !m_SearchResultText.m_LineCount )
	{
		// IDS_PETHELP_SEARCHFAILED : 검색어에 해당하는 결과가 없습니다.
		ExcuteHelpTalk( (char *)G_STRING(IDS_PETHELP_SEARCHFAILED) );
		return FALSE;
	}
	else
	{
		// IDS_PETHELP_SEARCHRESULT : * 검색 결과입니다. *
		TempMultiText.AddString( (char *)G_STRING(IDS_PETHELP_SEARCHRESULT) );

		TempPopup.PutString( &TempMultiText );
		MinWidth = TempPopup.GetSurWidth();
		TempMultiText.Reset();

		for( int i = 0 ; i < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++i )
		{
			if( i < m_TotalResult )
				TempMultiText.AddString( m_SearchResultText.m_Text[i] ); // 처음 화면에 해당하는 줄들을 인서트.
		}
	}


	TempPopup.PutString( &TempMultiText );

	if( TempPopup.GetSurWidth() < MinWidth ) // 검색 결과 입니다 글자보다 짧다면 검색 결과입니다. 글자 길이에 맞춰준다.
		m_SearchRect.right = MinWidth+HELPPET_SEARCHRESULT_WIDTHSPACE; // 가장 긴 스트링을 기준으로 잡아준다. 여백 및 클로즈 버튼을 고려.
	else
		m_SearchRect.right = TempPopup.GetSurWidth()+HELPPET_SEARCHRESULT_WIDTHSPACE; // 가장 긴 스트링을 기준으로 잡아준다. 여백 및 클로즈 버튼을 고려.

	m_SearchRect.bottom = HELPPET_SEARCHRESULT_HEIGHT; // 그냥 고정 크기로 가자.
	/*
		if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // 한 화면 이상인 경우.
			m_SearchRect.bottom = (HELPPET_SEARCHRESULT_VIEWINPAGE+1) * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE + 40; // 버튼 크기도 고려.
		else // 한 화면인 경우.
			m_SearchRect.bottom = m_TotalResult * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE; // 마지막은 여백.
	*/

	if( !m_SearchResultIndex.size() )
	{
		return FALSE; // 찾은 결과가 없다.
	}


	return TRUE;
}

