#include "stdafx.h"
#include "headers.h"
#include "BaseInterface.h"
#include "main.h"
#include "ControlSocial.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "ListBox.h"
#include "ControlGate.h"
#include "Land.h"
#include "UIMgr.h"
#include "g_stringmanager.h"
#include "Country.h"
#include "Mouse.h"

#include "CommonConfig.h"

extern BOOL g_bAdmin2;
extern DWORD g_dwClientCountry;			// 0: Korean 1: Japanese  2: English
extern BOOL	g_bActivePackage;
extern BOOL	g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.

BOOL	g_bSafari = FALSE;

CControlGate::CControlGate()
	: m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_bSpecialGate(FALSE)
	, m_byPopupType(0)
	, m_GateSubType(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));
	memset(m_ItemArray, 0, sizeof(m_ItemArray));

	m_pSurfaceBack = NULL;
	m_GateList = NULL;

	// nate 2005-03-11 : Matrix
	ZeroMemory( m_byWarp, sizeof( m_byWarp ) );

	strcpy(m_strFtoS[0], "go_world 4 0\n");
	strcpy(m_strFtoS[1], "go_world 4 1\n");
	strcpy(m_strFtoS[2], "go_world 4 2\n");

	strcpy(m_strStoF[0], "go_world 0 0\n");
	strcpy(m_strStoF[1], "go_world 0 1\n");
	strcpy(m_strStoF[2], "go_world 0 2\n");
	strcpy(m_strStoF[3], "go_world 0 3\n");

	strcpy(m_strStoF[0], "go_world 3 0\n");

	Init();
}

CControlGate::~CControlGate()
{
	DeleteRes();
	SAFE_DELETE( m_GateList );
}

void CControlGate::Init(int nCurWorldNum)
{
	m_CloseBtn.SetFileName("common/btn_close_01");
	m_GateList = new CListBox(" ", GATE_LIST_X, GATE_LIST_Y, GATE_LIST_WIDTH, GATE_LIST_HEIGHT);
	if( m_GateList )
	{
		m_GateList->SetImage("common/scroll_dn_t01", "common/scroll_up_t01", "interface/common/scroll_thumb_01.bmp");
		m_GateList->SetSelectedLine(-1);
		m_GateList->SetTextColor(GATE_LIST_FCOLOR);
		//-- IDS_GATE_WORLD_MOVE : 월드이동
		m_GateList->AddString(G_STRING(IDS_GATE_WORLD_MOVE));	// 0
	}

	m_GateType = 0; // 게이트 종류를 기억해두기 위해서 사용. ///국가대항전 시 매트릭스 광장 게이트 때문에 필요해서 추가했다. 이후 게이트들은 이걸로 체크해서 메시지 처리를 해주자. 07-03-13 원석
	m_ItemCurrent = 0; //드미트론 점령전 - 지역 번호 및 요금
}

void CControlGate::SetCurWorld(int nCurWorldNum)
{
	ResetCurWorld();
	if (m_GateList == NULL)
	{
		m_GateList = new CListBox(" ", m_pSurfaceBack->Xpos+45, m_pSurfaceBack->Ypos+86, 191, 226);
		if( !m_GateList )
			return;

		m_GateList->SetImage("common/scroll_dn_t01", "common/scroll_up_t01", "interface/common/scroll_thumb_01.bmp");
		m_GateList->SetSelectedLine(-1);
		m_GateList->SetTextColor(GATE_LIST_FCOLOR);

		if (nCurWorldNum == WORLD_FIELD)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4

			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_SHAILON)); // 3

			if(CheckUpdateVersionID(IDS_GATE_WHITEHORN))
				m_GateList->AddString(G_STRING(IDS_GATE_WHITEHORN)); // 7

			if(CheckUpdateVersionID(IDS_LHSTRING1829))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1829)); // 20

			if(CheckUpdateVersionID(IDS_WORLD_10))
				m_GateList->AddString(G_STRING(IDS_WORLD_10)); // 19

			if(CheckUpdateVersionID(IDS_LHSTRING1896))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1896)); // 23

			if(CheckUpdateVersionID(IDS_LHSTRING1897))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1897)); // 24

			if(CheckUpdateVersionID(IDS_LHSTRING1830))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1830)); // Kruma 21

			if(CheckUpdateVersionID(IDS_LHSTRING4555))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4555)); // Hightland 25

			if(CheckUpdateVersionID(IDS_LHSTRING4556))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4556)); // Tritian 26

		}
		else if (nCurWorldNum == WORLD_START)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM)); // 0

			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_SHAILON));		// 3

			if(CheckUpdateVersionID(IDS_MAP_PHAROS))
				m_GateList->AddString(G_STRING(IDS_MAP_PHAROS));  //6

			if(CheckUpdateVersionID(IDS_GATE_WHITEHORN))
				m_GateList->AddString(G_STRING(IDS_GATE_WHITEHORN)); // 7

			if(CheckUpdateVersionID(IDS_LHSTRING1829))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1829)); // 20

			if(CheckUpdateVersionID(IDS_WORLD_10))
				m_GateList->AddString(G_STRING(IDS_WORLD_10)); // 19

			if(CheckUpdateVersionID(IDS_LHSTRING1896))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1896)); // 23

			if(CheckUpdateVersionID(IDS_LHSTRING1897))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1897)); // 24

			if(CheckUpdateVersionID(IDS_LHSTRING1830))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1830)); // Kruma 21

			if(CheckUpdateVersionID(IDS_LHSTRING4555))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4555)); // Hightland 25

			if(CheckUpdateVersionID(IDS_LHSTRING4556))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4556)); // Tritian 26

			if(CheckUpdateVersionID(IDS_LHSTRING4557))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4557));

			if(CheckUpdateVersionID(IDS_LHSTRING4558))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4558));

			/*if(CheckUpdateVersionID(IDS_MATRIX ))
				m_GateList->AddString(G_STRING(IDS_MATRIX ));*/


		}
		else if (nCurWorldNum == WORLD_TRITIAN)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4
		}

		else if (nCurWorldNum == WORLD_DISPOSAL)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4

			if(CheckUpdateVersionID(IDS_MAP_DMITRON ))
				m_GateList->AddString(G_STRING(IDS_MAP_DMITRON )); // 20

			if(CheckUpdateVersionID(IDS_LHSTRING1892))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1892)); // 22

		}

		else if (nCurWorldNum == WORLD_FORLORN_S2)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4
		}

		else if (nCurWorldNum == WORLD_FORLORN_S3)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4
		}

		else if (nCurWorldNum == WORLD_FORLORN_S4)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4
		}

		else if (nCurWorldNum == WORLD_VIDBLINE)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4
		}

		else if (nCurWorldNum == WORLD_HIGHTLAND)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4

		}

		else if (nCurWorldNum == WORLD_SPACE)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4

			if(CheckUpdateVersionID(IDS_LHSTRING4660))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4660));
			if(CheckUpdateVersionID(IDS_LHSTRING4661))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4661));
			if(CheckUpdateVersionID(IDS_LHSTRING4662))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4662));
			if(CheckUpdateVersionID(IDS_LHSTRING4663))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4663));
		}

		else if (nCurWorldNum == WORLD_GUILD)
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	// 4

			if(CheckUpdateVersionID(IDS_GATE_LOST_REALM))
				m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM)); // 3

			if(CheckUpdateVersionID(IDS_GATE_WHITEHORN))
				m_GateList->AddString(G_STRING(IDS_GATE_WHITEHORN)); // 7

			if(CheckUpdateVersionID(IDS_LHSTRING1829))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1829)); // 20

			if(CheckUpdateVersionID(IDS_WORLD_10))
				m_GateList->AddString(G_STRING(IDS_WORLD_10)); // 19

			if(CheckUpdateVersionID(IDS_LHSTRING1896))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1896)); // 23

			if(CheckUpdateVersionID(IDS_LHSTRING1897))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1897)); // 24

			if(CheckUpdateVersionID(IDS_LHSTRING1830))
				m_GateList->AddString(G_STRING(IDS_LHSTRING1830)); // Kruma 21

			if(CheckUpdateVersionID(IDS_LHSTRING4555))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4555)); // Hightland 25

			if(CheckUpdateVersionID(IDS_LHSTRING4556))
				m_GateList->AddString(G_STRING(IDS_LHSTRING4556)); // Tritian 26
		}
		else if(nCurWorldNum == WORLD_SKY)
		{
			if(CheckUpdateVersionID(IDS_GATE_SEARUSTE))
				m_GateList->AddString(G_STRING(IDS_GATE_SEARUSTE));
		}
		else if( nCurWorldNum == WORLD_WHITEHORN )
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM)); // 0
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA)); // 4
		}
		else if( nCurWorldNum == WORLD_DUNGEON4 )
		{
			if(CheckUpdateVersionID(IDS_GATE_DMITRON))
			{
				m_GateList->AddString(G_STRING(IDS_GATE_DMITRON));
			}
		}
		else if( nCurWorldNum == WORLD_DMITRON )
		{
			if( m_GateSubType == 100)
			{
				if(CheckUpdateVersionID(IDS_WORLD_13))
					m_GateList->AddString(G_STRING(IDS_WORLD_13));	// 튜란			불칸 보스 존

				if(CheckUpdateVersionID(IDS_WORLD_14))
					m_GateList->AddString(G_STRING(IDS_WORLD_14));	// 발카리야		카이립톤 보스 존

				if(CheckUpdateVersionID(IDS_WORLD_15))
					m_GateList->AddString(G_STRING(IDS_WORLD_15));	// 바르샤		에이디아 보스

				if(CheckUpdateVersionID(IDS_WORLD_16))
					m_GateList->AddString(G_STRING(IDS_WORLD_16));	// 퀘이즈		휴먼 보스 존

				if(CheckUpdateVersionID(IDS_WORLD_17))
					m_GateList->AddString(G_STRING(IDS_WORLD_17));	// 베어그리드	하이브리 보스 존

				if(CheckUpdateVersionID(IDS_WORLD_18))
					m_GateList->AddString(G_STRING(IDS_WORLD_18));	// 마르보덴		최종 보스 존

				int limit_level = 350;
				if( g_pRoh->m_Level >= limit_level )
				{
					if(CheckUpdateVersionID(IDS_WORLD_18))
						m_GateList->AddString(G_STRING(IDS_LHSTRING1829));	// 펄론루인s1
				}


			}
			else
			{
				m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));

				if(CheckUpdateVersionID(IDS_GATE_WHITEHORN_DUNGEON))
					m_GateList->AddString(G_STRING(IDS_GATE_WHITEHORN_DUNGEON));
			}
		}
		//  [11/12/2008 parkmj] 일본요청 - 고고학자 벨리언 클릭시 이동창 띄움
		else if( (nCurWorldNum >= WORLD_BOSSRAID_BULKAN && nCurWorldNum <= WORLD_BOSSRAID_HYBRIDER) && g_dwClientCountry == CTRY_JPN )
		{
			m_GateList->AddString(G_STRING(IDS_WORLD_09));   // #define	IDS_WORLD_09	1372	// 드미트론
		}
		else if( nCurWorldNum == WORLD_MATRIX_2009 )
		{
			if( g_pRoh && !g_pRoh->m_GuildIndex )
			{
				m_byWarp[ 0 ] = 3;
				m_GateList->AddString( G_STRING( IDS_WAITROOM ) );
			}
			else
			{
				int nIndex = 0;
				switch( g_pRoh->m_GuildIndex )
				{
				case 1:
					nIndex = 4;
					break; // 툰드마루
				case 2:
					nIndex = 5;
					break; // 안그마르
				}

				if(pCMyApp->m_pUIMgr->m_nWarpGateIndex != nIndex )
				{
					return;
				}

				BYTE byEventState = 0;

				for(int i=0; i<5; i++)
				{
					if( pCMyApp->m_pUIMgr->m_nStoneState[ i+6 ] == g_pRoh->m_GuildIndex )
					{
						m_byWarp[ byEventState ] = 20 + i;	// 현재 인덱스 기억
						m_GateList->AddString( G_STRING(IDS_GSTONE_01 + i) );
						byEventState++;
					}
				}

				m_byWarp[ byEventState ] = 3;
				m_GateList->AddString( G_STRING( IDS_WAITROOM ) );
				pCMyApp->m_pUIMgr->m_nWarpGateIndex = 0;
			}
		}
		//=================================================================
		// nate 2005-03-22
		// Matrix : 워프게이트
		else if(nCurWorldNum == WORLD_MATRIX )
		{
			if( g_pRoh && !g_pRoh->m_GuildIndex )
			{
				m_byWarp[ 0 ] = 3;
				m_GateList->AddString( G_STRING( IDS_WAITROOM ) );
			}
			else
			{
				int nIndex = 0;	// 자기 본진 게이트 찾기
				switch( g_pRoh->m_GuildIndex )
				{
				case 1:
					nIndex = 164;
					break;
				case 2:
					nIndex = 94;
					break;
				case 3:
					nIndex = 163;
					break;
				}

				// 타 본진의 게이트는 사용이 불가
				if( pCMyApp->m_pUIMgr
						&& ( pCMyApp->m_pUIMgr->m_nWarpGateIndex == 94
							 || pCMyApp->m_pUIMgr->m_nWarpGateIndex == 163
							 || pCMyApp->m_pUIMgr->m_nWarpGateIndex == 164 ))
				{
					if( pCMyApp->m_pUIMgr->m_nWarpGateIndex != nIndex )
					{
						m_GateList->SetPosDim(m_pSurfaceBack->Xpos+45, m_pSurfaceBack->Ypos+86, 191, 226);
						m_GateList->LoadRes();
						return;
					}
				}
				// 타 주둔지 게이트 사용금지
				if( pCMyApp->m_pUIMgr
						&& ( pCMyApp->m_pUIMgr->m_nWarpGateIndex == 87
							 || pCMyApp->m_pUIMgr->m_nWarpGateIndex == 88
							 || pCMyApp->m_pUIMgr->m_nWarpGateIndex == 89 ))
				{
					if( pCMyApp->m_pUIMgr->m_nStoneState[ pCMyApp->m_pUIMgr->m_nWarpGateIndex - 78 ] != g_pRoh->m_GuildIndex )
					{
						m_GateList->SetPosDim(m_pSurfaceBack->Xpos+45, m_pSurfaceBack->Ypos+86, 191, 226);
						m_GateList->LoadRes();
						return;
					}
				}

				char szTemp[ 100 ];
				BYTE byMain = 0;
				BYTE byEventState = 0;
				for( int i = 9 ; i < 12 ; i ++ )
				{
					// 9 ~ 11 : G.P.Stone 번호
					if( pCMyApp->m_pUIMgr->m_nStoneState[ i ] == g_pRoh->m_GuildIndex )
					{
						byMain = 1;
						if( pCMyApp->m_pUIMgr->m_nWarpGateIndex == i + 78 )	// i + 78( 87, 88, 89 ) : 오브젝트의 인텍스와 비교
							continue;

						m_byWarp[ byEventState++ ] = i;	// 현재 인덱스 기억
						sprintf( szTemp, G_STRING( IDS_ARMY_POST ), i - 8 );
						m_GateList->AddString( szTemp );
					}
				}
				// 본진 추가 여부 판단
				if( pCMyApp->m_pUIMgr->m_nWarpGateIndex != nIndex && byMain )
				{
					// 94 : 본진 워프게이트 번호
					m_GateList->AddString( G_STRING( IDS_BASECAMP ) );
					m_byWarp[ byEventState++ ] = 1;
				}

				m_byWarp[ byEventState ] = 3;
				m_GateList->AddString( G_STRING( IDS_WAITROOM ) );

				pCMyApp->m_pUIMgr->m_nWarpGateIndex = 0;
			}

			// 드미트론 공성전 - 내부 포탈 : 아무것도 하지 않는다.
		}
		else if(nCurWorldNum == 200)
		{

			//=================================================================
		}
		else if(nCurWorldNum == 111)
		{
			// 존 이동장치 30~60레벨, 이전 11이었으나 존 번호가 10번대로 넘어감에 따라 111로 바꾸었음.(by 원석)
			//-- IDS_GATE_LAGRAMIA : 라그라미아 신전
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : 로스트렐름 성
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : 데카둔
			if(CheckUpdateVersionID(IDS_GATE_DEKADUNE))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));

			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		else if(nCurWorldNum == 112)
		{
			// 존 이동장치 60~90레벨, 이전 12이었으나 존 번호가 10번대로 넘어감에 따라 112로 바꾸었음.(by 원석)
			//-- IDS_GATE_LAGRAMIA : 라그라미아 신전
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : 로스트렐름 성
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : 데카둔
			if(CheckUpdateVersionID(IDS_GATE_DEKADUNE))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));
			//-- IDS_GATE_DEKALEN : 데카렌
			if(CheckUpdateVersionID(IDS_GATE_DEKALEN))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKALEN));

			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		else if(nCurWorldNum == 113)
		{
			// 존 이동장치 90~120레벨, 이전 13이었으나 존 번호가 10번대로 넘어감에 따라 113로 바꾸었음.(by 원석)
			//-- IDS_GATE_LAGRAMIA : 라그라미아 신전
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : 로스트렐름 성
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : 데카둔
			if(CheckUpdateVersionID(IDS_GATE_DEKADUNE))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));
			//-- IDS_GATE_DEKALEN : 데카렌
			if(CheckUpdateVersionID(IDS_GATE_DEKALEN))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKALEN));
			//-- IDS_GATE_SHAILON : 샤일론 필드
			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_SHAILON));

			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		//존 이동장치 Lev 120이상
		else if(nCurWorldNum == 114)
		{
			// 존 이동장치 120레벨 이상, 이전 14이었으나 존 번호가 10번대로 넘어감에 따라 114로 바꾸었음.(by 원석)
			//-- IDS_GATE_LAGRAMIA : 라그라미아 신전
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : 로스트렐름 성
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : 데카둔
			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));
			//-- IDS_GATE_DEKALEN : 데카렌
			if(CheckUpdateVersionID(IDS_GATE_DEKALEN))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKALEN));
			//-- IDS_GATE_SHAILON : 샤일론 필드
			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_SHAILON));
			//-- IDS_GATE_SIRUSTE : 시루스트 던전
			if(CheckUpdateVersionID(IDS_GATE_SIRUSTE))
				m_GateList->AddString(G_STRING(IDS_GATE_SIRUSTE));
			//-- IDS_MAP_PHAROS : 천공의성
			if(CheckUpdateVersionID(IDS_MAP_PHAROS))
				m_GateList->AddString(G_STRING(IDS_MAP_PHAROS));
			//-- IDS_GATE_WHITEHORN :그레이트 화이트 혼
			if(CheckUpdateVersionID(IDS_GATE_WHITEHORN))
				m_GateList->AddString(G_STRING(IDS_GATE_WHITEHORN));
			if(g_pRoh->m_Level > 249)	//화이트혼던젼 레벨제한
			{
				//-- IDS_DUNGEON4 : 제누스 레버너티
				if(CheckUpdateVersionID(IDS_DUNGEON4))
					m_GateList->AddString(G_STRING(IDS_DUNGEON4));
			}
			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		else if(nCurWorldNum == 915)
		{
			//-- IDS_MAP_PHAROS : 천공의성
			if(CheckUpdateVersionID(IDS_MAP_PHAROS))
			{
				m_GateList->AddString(G_STRING(IDS_MAP_PHAROS));
				pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
				pCMyApp->m_pUIMgr->m_bParosKit = TRUE;
			}
		}
		// 30821
		else if (nCurWorldNum == 916)
		{
			//-- IDS_GATE_LAGRAMIA : 라그라미아 신전
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			m_bSpecialGate = TRUE;
		}
		else if (nCurWorldNum == 917)
		{
			//-- IDS_GATE_DMITRON : 드미트론
			if(CheckUpdateVersionID(IDS_GATE_DMITRON))
			{
				m_GateList->AddString(G_STRING(IDS_GATE_DMITRON));
				pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
				pCMyApp->m_pUIMgr->m_bDmitron = TRUE;
			}
		}
		// [2008/6/23 Theodoric] 마르보덴 이동장치
		else if( nCurWorldNum == 918 )
		{
			if(CheckUpdateVersionID(IDS_WORLD_18))
			{
				//-- IDS_WORLD_18 : 마르보덴
				m_GateList->AddString( G_STRING(IDS_WORLD_18) );
				pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
				pCMyApp->m_pUIMgr->m_bMarboden = TRUE;
			}
		}

		//package 유저, 전 존을 갈수 있음
		else if (nCurWorldNum == 117)
		{
			//040531 추가 // 존 이동장치 패키지 유저., 이전 17이었으나 존 번호가 10번대로 넘어감에 따라 117로 바꾸었음.(by 원석)
			//-- IDS_GATE_LAGRAMIA : 라그라미아 신전
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : 로스트렐름 성
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : 데카둔
			if(CheckUpdateVersionID(IDS_GATE_DEKADUNE))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));
			//-- IDS_GATE_DEKALEN : 데카렌
			if(CheckUpdateVersionID(IDS_GATE_DEKALEN))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKALEN));
			//-- IDS_GATE_SHAILON : 샤일론 필드
			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_SHAILON));
			//-- IDS_GATE_SIRUSTE : 시루스트 던전
			if(CheckUpdateVersionID(IDS_GATE_SIRUSTE))
				m_GateList->AddString(G_STRING(IDS_GATE_SIRUSTE));
			//-- IDS_MAP_PHAROS : 천공의성
			if(CheckUpdateVersionID(IDS_MAP_PHAROS))
				m_GateList->AddString(G_STRING(IDS_MAP_PHAROS));
			//-- IDS_GATE_WHITEHORN :그레이트 화이트 혼
			if(CheckUpdateVersionID(IDS_GATE_WHITEHORN))
				m_GateList->AddString(G_STRING(IDS_GATE_WHITEHORN));
			//-- IDS_DUNGEON4 : 제누스 레버너티
			if(CheckUpdateVersionID(IDS_DUNGEON4))
				m_GateList->AddString(G_STRING(IDS_DUNGEON4));

			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		else if( nCurWorldNum == 1010 ) ///국가대항전 시에 매트릭스 광장 워프게이트 방식 변경
		{
			//-- IDS_GATE_MATWAR : 뫼비우스 전장
			m_GateList->AddString( G_STRING(IDS_GATE_MAT_WAR) );
			//-- IDS_GATE_GREENISH : P.K 존
			m_GateList->AddString( G_STRING(IDS_GATE_MAT_PK) );

			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));//Arnold


			if( g_bGlobalMatrix ) // 국가대항전일 경우만...
			{
				//-- IDS_GATE_G_GREENISH : 국가별 P.K 존
				m_GateList->AddString( G_STRING(IDS_GATE_MAT_GPK) );
			}
		}
		else if( nCurWorldNum == 1011 ) // [2008/10/31 Theodoric] WORLD_MATRIX_2009
		{
			//-- IDS_GATE_MATWAR : 뫼비우스 전장
			if(CheckUpdateVersionID(IDS_GATE_MAT_WAR))
				m_GateList->AddString( G_STRING(IDS_GATE_MAT_WAR) );
			//-- IDS_GATE_GREENISH : P.K 존
			m_GateList->AddString( G_STRING(IDS_GATE_MAT_PK) );

			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));//Arnold

			if( g_bGlobalMatrix ) // 국가대항전일 경우만...
			{
				//-- IDS_GATE_G_GREENISH : 국가별 P.K 존
				m_GateList->AddString( G_STRING(IDS_GATE_MAT_GPK) );
			}
		}
		else if( nCurWorldNum == 120 )
		{
//			m_GateList->AddString( G_STRING(IDS_DMITRON_BATTLE_ZONE6) );
			m_GateList->AddString( G_STRING(IDS_GATE_LOST_REALM) );
			m_GateList->AddString( G_STRING(IDS_GATE_LAGRAMIA) );
		}
		else if( nCurWorldNum == 121 )
		{
			m_GateList->AddString( G_STRING(IDS_GATE_LOST_REALM) );
			m_GateList->AddString( G_STRING(IDS_GATE_LAGRAMIA) );
		}
		else if( nCurWorldNum == 18 )
		{
			m_GateList->AddString(G_STRING(IDS_GATE_DMITRON));	// 드미트론
		}
		else if( nCurWorldNum == 24 )
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	 // Add Portal S4 to Laglamia
			m_GateList->AddString(G_STRING(IDS_LHSTRING1896)); // Add Portal S4 to S3
		}
		else
		{
			// by evilkiki 2010.01.
			// 존이동 아이템 사용시 게이트 오픈 리스트 번호
			// 500부터~순서대로...500, 501, 502....
			// 550부터는 조합
			// string recource 매칭
			switch( nCurWorldNum )
			{
			case 500:
				m_GateList->AddString( G_STRING(IDS_GATE_LOST_REALM) );
				break;
			case 501:
				if(CheckUpdateVersionID(IDS_GATE_DEKALEN))
					m_GateList->AddString( G_STRING(IDS_GATE_DEKALEN) );
				break;
			case 502:
				if(CheckUpdateVersionID(IDS_GATE_DEKADUNE))
					m_GateList->AddString( G_STRING(IDS_GATE_DEKADUNE) );
				break;
			case 503:
				if(CheckUpdateVersionID(IDS_GATE_SHAILON))
					m_GateList->AddString( G_STRING(IDS_GATE_SHAILON) );
				break;
			case 504:
				m_GateList->AddString( G_STRING(IDS_GATE_LAGRAMIA) );
				break;
			case 505:
				if(CheckUpdateVersionID(IDS_GATE_SIRUSTE))
					m_GateList->AddString( G_STRING(IDS_GATE_SIRUSTE) );
				break;
			case 506:
				if(CheckUpdateVersionID(IDS_MAP_PHAROS))
					m_GateList->AddString( G_STRING(IDS_MAP_PHAROS) );
				break;
			case 507:
				if(CheckUpdateVersionID(IDS_GATE_WHITEHORN))
					m_GateList->AddString( G_STRING(IDS_GATE_WHITEHORN) );
				break;
			case 508:
				if(CheckUpdateVersionID(IDS_DUNGEON4))
					m_GateList->AddString( G_STRING(IDS_DUNGEON4) );
				break;
			case 509:
				if(CheckUpdateVersionID(IDS_GATE_DMITRON))
					m_GateList->AddString( G_STRING(IDS_GATE_DMITRON) );
				break;
			case 513:
				if(CheckUpdateVersionID(IDS_WORLD_13))
					m_GateList->AddString( G_STRING(IDS_WORLD_13) );
				break;
			case 514:
				if(CheckUpdateVersionID(IDS_WORLD_14))
					m_GateList->AddString( G_STRING(IDS_WORLD_14) );
				break;
			case 515:
				if(CheckUpdateVersionID(IDS_WORLD_15))
					m_GateList->AddString( G_STRING(IDS_WORLD_15) );
				break;
			case 516:
				if(CheckUpdateVersionID(IDS_WORLD_16))
					m_GateList->AddString( G_STRING(IDS_WORLD_16) );
				break;
			case 517:
				if(CheckUpdateVersionID(IDS_WORLD_17))
					m_GateList->AddString( G_STRING(IDS_WORLD_17) );
				break;
			case 518:
				if(CheckUpdateVersionID(IDS_WORLD_18))
					m_GateList->AddString( G_STRING(IDS_WORLD_18) );
				break;

			default:
				break;
			}
		}
		m_GateList->SetPosDim(m_pSurfaceBack->Xpos+45, m_pSurfaceBack->Ypos+86, 191, 226);
		m_GateList->LoadRes();
	}

	m_GateType = nCurWorldNum; // 게이트 종류를 기억해두기 위해서 사용. ///국가대항전 시 매트릭스 광장 게이트 때문에 필요해서 추가했다. 이후 게이트들은 이걸로 체크해서 메시지 처리를 해주자. 07-03-13 원석
}

void CControlGate::ResetCurWorld()
{
	m_bSpecialGate = FALSE;
	SAFE_DELETE( m_GateList );
}

void CControlGate::LoadRes()
{
	if( !g_pDisplay || !g_pNk2DFrame )
		return;
	m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
	m_ScreenHeight = g_pNk2DFrame->GetClientHeight();

	SAFE_DELETE(m_pSurfaceBack);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/gate/Gate_Back.bmp" );
	if( m_pSurfaceBack )
	{
		m_pSurfaceBack->SetColorKey(TRANS_COLOR);
		m_pSurfaceBack->Xpos = (g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth())>>1;
		m_pSurfaceBack->Ypos = (g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight())>>1;

		m_CloseBtn.PosX = m_pSurfaceBack->Xpos+209;
		m_CloseBtn.PosY = m_pSurfaceBack->Ypos+15;
		m_CloseBtn.LoadRes();
	}

	if (m_GateList)
	{
		m_GateList->SetPosDim(m_pSurfaceBack->Xpos+45, m_pSurfaceBack->Ypos+86, 191, 226);
		m_GateList->LoadRes();
	}
}

void CControlGate::SetSubType(int nSubType) // 드미트론 점령전 - 이동 게이트 서브 타입
{
	m_GateSubType = nSubType;
}

void CControlGate::DeleteRes()
{
	SAFE_DELETE(m_pSurfaceBack);
	m_CloseBtn.DeleteRes();
	if (m_GateList)
		m_GateList->DeleteRes();
}

void CControlGate::Draw()
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);
	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	if (g_pDisplay && m_pSurfaceBack)
		g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack);

	m_CloseBtn.Draw(m_pSurfaceBack->Xpos + 209 , m_pSurfaceBack->Ypos + 15 );
	if( m_GateList )
		m_GateList->Draw();
}

BOOL CControlGate::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn( x, y );
}

LRESULT CControlGate::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y, sel;
	int i = 0;

	switch ( msg )
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		{
			y = HIWORD (lParam);
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if (g_pNk2DFrame && m_CloseBtn.GetState() == BTN_ACTION)
			{
				g_pNk2DFrame->ShowInterfaceWindow(FALSE, GATE);
				m_CloseBtn.SetState(BTN_NORMAL);
			}
			if( m_GateList )
				m_GateList->GetMessage(hWnd, msg, wParam, lParam );
			if (!IsInside(x, y))
				return 0;
		}
		return 1;
	case WM_LBUTTONDOWN:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if (IsInside(x, y))
			{
				// Msg Popup창이 떠있을때는 존 리스트가 클릭이 안되게 리턴~
				//if( g_pNk2DFrame->Check_All_MsgPopUp() )
				//	return 1;

				if( pCMyApp->m_pUIMgr && m_CloseBtn.IsInside(x, y) )
				{
					m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
					pCMyApp->m_pUIMgr->m_bMoveKit  = FALSE; //1204
					pCMyApp->m_pUIMgr->m_bParosKit = FALSE;
					pCMyApp->m_pUIMgr->m_bDmitron  = FALSE;
					pCMyApp->m_pUIMgr->m_bMarboden = FALSE; // [2008/6/23 Theodoric] 마르보덴 이동장치
				}

				if (m_GateList && (sel = m_GateList->GetMessage(hWnd, msg, wParam, lParam )) >= 0)
				{
					g_pNk2DFrame->ShowInterfaceWindow(FALSE, GATE);
					m_CloseBtn.SetState(BTN_NORMAL);

					if (!(pCMyApp->GetHeroSwimming()))
					{
						pCMyApp->StopHero();

						if (g_pTcpIp)
						{
							if( pCMyApp->m_pUIMgr->m_bMoveKit && !g_pRoh->IsDead() )
							{
								//1203 1218
								char m_commOut[512];

								if(pCMyApp->m_pUIMgr->m_bParosKit)
								{
									if(sel == 0)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_SKY, 0);
									pCMyApp->m_pUIMgr->m_bParosKit = FALSE;
								}
								else if(pCMyApp->m_pUIMgr->m_bDmitron)
								{
									if(sel == 0)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DMITRON, 0);
									pCMyApp->m_pUIMgr->m_bDmitron = FALSE;
								}
								// [2008/6/23 Theodoric] 마르보덴 이동장치
								else if(pCMyApp->m_pUIMgr->m_bMarboden)
								{
									if(sel == 0)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_BOSSRAID_FINALBOSS, 0);
									pCMyApp->m_pUIMgr->m_bMarboden = FALSE;
								}
								else if( g_bActivePackage )
								{
									if(sel == 0)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_START, 0);
									else if(sel == 1)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_FIELD, 0);
									else if(sel == 2 && g_pRoh->m_Level >= 600)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON2, 0);
									else if(sel == 3 && g_pRoh->m_Level >= 700)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON1, 0);
									else if(sel == 4)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_GUILD, 0);
									else if(sel == 5)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON_S, 0);
									else if(sel == 6 && g_pRoh->m_Level >= 750)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_SKY, 0);
									else if(sel == 7)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_WHITEHORN, 0);//040531 추가
									//wan:2004-11 : Whitehorn Dungeon
									else if(sel == 8)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON4, 0);
								}
								else if (g_pRoh->m_Level <=30)
								{
									if(sel == 0)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_START, 0);
								}
								else if (g_pRoh->m_Level >30 && g_pRoh->m_Level <=60)
								{
									if( sel == 0 )
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_START, 0);
									else if(sel == 1)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_FIELD, 0);
									else if(sel == 2 && g_pRoh->m_Level >= 600 )
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON2, 0);
								}
								else if (g_pRoh->m_Level >60 && g_pRoh->m_Level<=90)
								{
									if(sel == 0)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_START, 0);
									else if(sel == 1)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_FIELD, 0);
									else if(sel == 2 && g_pRoh->m_Level >= 600 )
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON2, 0);
									else if(sel == 3 && g_pRoh->m_Level >= 700)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON1, 0);
								}
								else if ( g_pRoh->m_Level >90 && g_pRoh->m_Level<=120 )
								{
									if(sel == 0)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_START, 0);
									else if(sel == 1)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_FIELD, 0);
									else if(sel == 2 && g_pRoh->m_Level >= 600)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON2, 0);
									else if(sel == 3 && g_pRoh->m_Level >= 700)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON1, 0);
									else if(sel == 4)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_GUILD, 0);
								}
								else if(g_pRoh->m_Level >120)
								{
									if(sel == 0)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_START, 0);
									else if(sel == 1)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_FIELD, 0);
									else if(sel == 2 && g_pRoh->m_Level >= 600)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON2, 0);
									else if(sel == 3 && g_pRoh->m_Level >= 700)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON1, 0);
									else if(sel == 4)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_GUILD, 0);
									else if(sel == 5)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON_S, 0);
									else if(sel == 6 && g_pRoh->m_Level >= 750)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_SKY, 0);//040531 추가
									else if(sel == 7)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_WHITEHORN, 0);
									//wan:2004-11 : Whitehorn Dungeon - 화이트혼 던젼은 레벨250이상만 갈 수 있음 레벨제한
									else if(sel == 8)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON4, 0);

								}

								if (g_pTcpIp)
									g_pTcpIp->SendNetMessage(m_commOut);

								pCMyApp->PrepareChangeWorld();

								// 패키지 구입했으면 귀환장치를 지우지 말자...
								if( !g_bActivePackage )
								{
									SAFE_DELETE(g_pRoh->m_QuickSlot[g_pNk2DFrame->m_Slot_Num]);
								}
							}
							else if ( !g_pRoh->IsDead() )
							{
								if (pCMyApp->m_nCurWorldIdx == WORLD_TRITIAN)
								{
									if(sel == 0)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, sel);
								}

								else if (pCMyApp->m_nCurWorldIdx == WORLD_DISPOSAL)
								{
									if(sel == 0)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, sel);
									else if( sel == 1 && g_pRoh->m_Level >= 300)	// Dmitron
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_DMITRON, 0);
									else if( sel == 2 && g_pRoh->m_Level >= 400 )	// Forlorn Ruin S2 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FORLORN_S2, 0);
								}

								else if (pCMyApp->m_nCurWorldIdx == WORLD_FORLORN_S2)
								{
									if(sel == 0)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, sel);
								}

								else if (pCMyApp->m_nCurWorldIdx == WORLD_FORLORN_S3)
								{
									if(sel == 0)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, sel);
								}

								else if (pCMyApp->m_nCurWorldIdx == WORLD_FORLORN_S4)
								{
									if(sel == 0)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, sel);
								}

								else if (pCMyApp->m_nCurWorldIdx == WORLD_VIDBLINE)
								{
									if(sel == 0)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, sel);
								}

								else if (pCMyApp->m_nCurWorldIdx == WORLD_HIGHTLAND)
								{
									if(sel == 0)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, sel);
								}

								else if (pCMyApp->m_nCurWorldIdx == WORLD_SPACE)
								{
									if(sel == 0)
										{
											char m_commOut[512];
											sprintf( m_commOut, "go_world %d %d\n", WORLD_START, 0);
											if( g_pTcpIp )
											{
												g_pTcpIp->SendNetMessage( m_commOut, TRUE );
											}
											return 1;
										}
									else if( sel == 1 && g_pRoh->m_Level >= 910)	// 
									{
										if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												g_pTcpIp->SendNetMessage( "mat_jump 4\n" );
											}
									}
									else if( sel == 2 && g_pRoh->m_Level >= 910)	// 
									{
										if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												g_pTcpIp->SendNetMessage( "mat_jump 5\n" );
											}
									}
									else if( sel == 3 && g_pRoh->m_Level >= 915 )
										{
											char m_commOut[512];
											sprintf( m_commOut, "go_world %d %d\n", WORLD_DUN910, 0);
											if( g_pTcpIp )
											{
												g_pTcpIp->SendNetMessage( m_commOut, TRUE );
											}
											return 1;
										}
									else if( sel == 4 && g_pRoh->m_Level >= 935 )
										{
											char m_commOut[512];
											sprintf( m_commOut, "go_world %d %d\n", WORLD_MAP930, 0);
											if( g_pTcpIp )
											{
												g_pTcpIp->SendNetMessage( m_commOut, TRUE );
											}
											return 1;
										}
								}
								
								//1203 1218
								else if (pCMyApp->m_nCurWorldIdx == WORLD_FIELD)
								{
									if(sel == 0)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, sel);
									else if(sel == 1) //샤일론 필드
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_GUILD, 0);
									else if(sel == 2)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_WHITEHORN, 0);
									else if( sel == 3 && g_pRoh->m_Level >= 350 )	// Forlorn Ruin S1 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_DISPOSAL, 0);
									else if( sel == 4 && g_pRoh->m_Level >= 450)	// mobius
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_MATRIX_2009, 0);
									else if( sel == 5 && g_pRoh->m_Level >= 500 )	// Forlorn Ruin S3 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FORLORN_S3, 0);
									else if( sel == 6 && g_pRoh->m_Level >= 600)	// Forlorn Ruin S4 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FORLORN_S4, 0);
									else if( sel == 7 && g_pRoh->m_Level >= 800)	// KRUMA
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_VIDBLINE, 0);
									else if( sel == 8 && g_pRoh->m_Level >= 850)	// HIGHTLAND
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_HIGHTLAND, 0);
									else if( sel == 9 )	// Tritian
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_TRITIAN, 0);
								}
								else if (pCMyApp->m_nCurWorldIdx == WORLD_START)
								{
									if(sel == 0) //샤일론 필드
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FIELD, sel);
									else if(sel == 1) //샤일론 필드
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_GUILD, 0);
									else if(sel == 2)	// 천공의성
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_SKY, 0);
									else if( sel == 3 )	// Great white horn
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_WHITEHORN, 0);
									else if( sel == 4 && g_pRoh->m_Level >= 350 )	// Forlorn Ruin S1 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_DISPOSAL, 0);
									else if( sel == 5 && g_pRoh->m_Level >= 450)	// mobius
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_MATRIX_2009, 0);
									else if( sel == 6 && g_pRoh->m_Level >= 500 )	// Forlorn Ruin S3 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FORLORN_S3, 0);
									else if( sel == 7 && g_pRoh->m_Level >= 600)	// Forlorn Ruin S4 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FORLORN_S4, 0);
									else if( sel == 8 && g_pRoh->m_Level >= 800)	// KRUMA
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_VIDBLINE, 0);
									else if( sel == 9 && g_pRoh->m_Level >= 850)	// HIGHTLAND
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_HIGHTLAND, 0);
									else if( sel == 10 )	// Tritian
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_TRITIAN, 0);
									else if( sel == 11 && g_pRoh->m_Level >= 890 )	// Tritian
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_SPACE, 0);
									else if( sel == 12 && g_pRoh->m_Level >= 850 )	// Tritian
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_ROYALE, 0);
									/*else if( sel == 11 )
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_MATRIX, 0);*/

								}
								else if (pCMyApp->m_nCurWorldIdx == WORLD_GUILD)
								{
									if(sel == 0)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, sel);
									else if(sel == 1) //샤일론 필드
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FIELD, 0);
									else if(sel == 2)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_WHITEHORN, 0);
									else if( sel == 3 && g_pRoh->m_Level >= 350 )	// Forlorn Ruin S1 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_DISPOSAL, 0);
									else if( sel == 4 && g_pRoh->m_Level >= 450)	// mobius
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_MATRIX_2009, 0);
									else if( sel == 5 && g_pRoh->m_Level >= 500 )	// Forlorn Ruin S3 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FORLORN_S3, 0);
									else if( sel == 6 && g_pRoh->m_Level >= 600)	// Forlorn Ruin S4 Add Warp
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FORLORN_S4, 0);
									else if( sel == 7 && g_pRoh->m_Level >= 800)	// KRUMA
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_VIDBLINE, 0);
									else if( sel == 8 && g_pRoh->m_Level >= 850)	// HIGHTLAND
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_HIGHTLAND, 0);
									else if( sel == 9 )	// Tritian
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_TRITIAN, 0);
								}
								else if (pCMyApp->m_nCurWorldIdx == WORLD_SKY)
								{
									if(sel==0 && !m_bSpecialGate)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, 5, 0);
									// 천공성 두번째 게이트, lolo id 294
									else if (sel == 0 && m_bSpecialGate)
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_START, 0);
								}
								else if( pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN )
								{
									if( sel == 0 )
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_FIELD, 0 );
									else if( sel == 1 )
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_START, 0 );
									// 								else if( sel == 2 )
									// 									pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_DUNGEON4, 0 );
								}
								/*else if( pCMyApp->m_nCurWorldIdx == WORLD_FORLORN_S4 ) // Add Warp from s4
								{
									if( sel == 0 )
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_START, 0 );
									else if( sel == 1 )
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_FORLORN_S3, 0 ); // Back to S3 from S4
								}*/
								// nate 2005-07-15 : Dmitron - 이동 게이트를 통한 포탈
								else if( pCMyApp->m_nCurWorldIdx == WORLD_DUNGEON4 )
								{
									if( sel == 0 )
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_DMITRON, 0 );
								}

								//  [11/12/2008 parkmj] 일본요청 - 고고학자 벨리언 클릭시 이동창 띄움
								else if( (pCMyApp->m_nCurWorldIdx >= WORLD_BOSSRAID_BULKAN && pCMyApp->m_nCurWorldIdx <= WORLD_BOSSRAID_HYBRIDER)
										 && g_dwClientCountry == CTRY_JPN
									   )
								{
									if( sel == 0 )
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_DMITRON, 0 );
								}

								else if( m_GateSubType == 100 && pCMyApp->m_nCurWorldIdx == WORLD_DMITRON)
								{
									if( g_dwClientCountry == CTRY_KOR || g_dwClientCountry == CTRY_TH)
									{
										if(g_pRoh->m_Level < 275 )
										{
											g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_FINALBOSS_LIMIT_LEV), TYPE_NOR_OK, 1);
											return 1;
										}
									}

									switch( sel )
									{
									case 0: // 튜란 - 불칸 보스 존
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_BULKAN, 0 );
											break;
										}

									case 1: // 발카리아 - 카이립톤 보스 존
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_KAILIPTON, 0 );
											break;
										}

									case 2: // 바르샤 - 에이디아 보스 존
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_AIDIA, 0 );
											break;
										}

									case 3: // 퀘이즈 - 휴먼 보스 존
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_HUMAN, 0 );
											break;
										}

									case 4: // 베어그리드 - 하이브리더 보스 존
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_HYBRIDER, 0 );
											break;
										}

									case 5: // 위제스터 -  보스 존
										{
											if( CommonConfig::Instance()->GetBossRaidChannel() == false )
											{										
											  if( g_SvrType != ST_12_OVER )
												{
													if( g_pRoh->GetSvrChannelNum() != 2 ) // 마지막 보스는 2번 채널에서만 들어갈 수 있따.
													{
														g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_FINALBOSS_LIMIT_CHNL), TYPE_NOR_OK, 1);
														return 1;
													}
												}
											}

											CItem *pKeyItem = NULL;
											pKeyItem = g_pRoh->FindItemByVnumFromAll( 1930 ); // 듀란성 열쇠를 가지고 있는지 찾는다.

											if( pKeyItem ) // 열쇠가 있다면
											{
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_FINALBOSS, 0 );
												break;
											}
											else
											{
												g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_FINALBOSS_LIMIT_ITEM), TYPE_NOR_OK, 1);
											}
											return 1;
										}
									case 6:
										{
											int limit_level = 350;
											if( g_pRoh->m_Level >= limit_level )
											{
												CItem *pKeyItem = NULL;
												pKeyItem = g_pRoh->FindItemByVnumFromAll( 2730 ); // 버려진페허 이동장치(기간제)

												if( pKeyItem )
												{
													pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_DISPOSAL, 0 );
													break;
												}
												else
												{
													g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LHSTRING1887), TYPE_NOR_OK, 1);
												}
												return 1;
											}
										}
									}
								}
								else if( pCMyApp->m_nCurWorldIdx == WORLD_DMITRON ||
										 pCMyApp->m_nCurWorldIdx == WORLD_DMITRON_BATTLE )
								{
									if(m_GateSubType == 5)
									{
										if (pCMyApp->m_nCurWorldIdx == WORLD_DMITRON)
										{
											if( sel == m_ItemCurrent-2 )
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_START, 0 );
											else if( sel == m_ItemCurrent-1 )
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_DUNGEON4, 0 );
											else
											{

												char m_commOut[512];
												DmitronGate* pGate = (DmitronGate*)m_ItemArray[sel];
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												if (g_pTcpIp)
												{
													sprintf(m_commOut, "occp bt trans_r %d %d %d\n",
															m_GateSubType, pGate->Zone, pGate->Toll );
													g_pTcpIp->SendNetMessage( m_commOut );
												}
											}
										}
										else if(pCMyApp->m_nCurWorldIdx == WORLD_DMITRON_BATTLE)
										{
											if( sel == m_ItemCurrent-1 )
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_DMITRON, 0 );
											else
											{
												char m_commOut[512];
												DmitronGate* pGate = (DmitronGate*)m_ItemArray[sel];
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												if (g_pTcpIp)
												{
													sprintf(m_commOut, "occp bt trans_r %d %d %d\n",
															m_GateSubType, pGate->Zone, pGate->Toll );
													g_pTcpIp->SendNetMessage( m_commOut );
												}
											}
										}
									}
									else
									{
										char m_commOut[512];
										DmitronGate* pGate = (DmitronGate*)m_ItemArray[sel];
										pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
										if (g_pTcpIp)
										{
											sprintf(m_commOut, "occp bt trans_r %d %d %d\n",
													m_GateSubType, pGate->Zone, pGate->Toll );
											g_pTcpIp->SendNetMessage( m_commOut );
										}
									}
								}
								// nate 2005-03-22 - Matrix : 메트릭스에서 워프 게이프 이동
								else if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX )
								{
									///////////////////////////////////////////////////////////////////
									///국가대항전 시에 매트릭스 광장 워프게이트 방식 변경
									if( m_GateType == 1010 )
									{
										if(sel == 0) // 뫼비우스 전장
										{
											/*if( g_bGlobalMatrix && pCMyApp->m_pUIMgr->m_bOnBattle )  ///국가전이동딜레이 // 경기 시작후에만 딜레이 적용
											{
												if( pCMyApp->m_NvsnMoveDelay == 0 ) // (이미 딜레이중인경우 무시.)
												{
													// 순간적으로 많은 인원이 전장에 이동 못하도록 랜덤 딜레이를 적용시켜준다.
													pCMyApp->m_NvsnMoveDelay = timeGetTime() + RandomNum(3000,15000);
												}

												return 1;
											}*/

											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												g_pTcpIp->SendNetMessage( "mat_jump 1\n" );
											}
										}
										else if(sel == 1)	// 그리니쉬 스퀘어
										{
											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												g_pTcpIp->SendNetMessage( "mat_jump 2\n" );
											}
										}
										else if(sel == 2)//Arnold
										{
											char m_commOut[512];
											sprintf( m_commOut, "go_world %d %d\n", WORLD_START, 0);
											if( g_pTcpIp )
											{
												g_pTcpIp->SendNetMessage( m_commOut, TRUE );
											}
											return 1;
										}
										else if( g_bGlobalMatrix && sel == 3 )	// 국가별 그리니쉬 스퀘어 // 국가대항전 시에만 가능
										{
											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												g_pTcpIp->SendNetMessage( "mat_jump 2 1\n" ); // 뒤에 1이 더 붙은 PK 지역으로의 점프 패킷.
											}
										}
									}
									else
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, m_byWarp[ sel ], 0 );
									///////////////////////////////////////////////////////////////////
								}
								else if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009 )
								{
									///////////////////////////////////////////////////////////////////
									///국가대항전 시에 매트릭스 광장 워프게이트 방식 변경
									if( m_GateType == 1011 )
									{
										if(sel == 0) // 뫼비우스 전장
										{
											if( g_bGlobalMatrix && pCMyApp->m_pUIMgr->m_bOnBattle )  ///국가전이동딜레이 // 경기 시작후에만 딜레이 적용
											{
												if( pCMyApp->m_NvsnMoveDelay == 0 ) // (이미 딜레이중인경우 무시.)
												{
													// 순간적으로 많은 인원이 전장에 이동 못하도록 랜덤 딜레이를 적용시켜준다.
													pCMyApp->m_NvsnMoveDelay = timeGetTime() + RandomNum(3000,15000);
												}

												return 1;
											}

											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												g_pTcpIp->SendNetMessage( "mat_jump 1\n" );
											}
										}
										else if(sel == 1)	// 그리니쉬 스퀘어
										{
											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												g_pTcpIp->SendNetMessage( "mat_jump 2\n" );
											}
										}
										else if(sel == 2)//Arnold
										{
											char m_commOut[512];
											sprintf( m_commOut, "go_world %d %d\n", WORLD_START, 0);
											if( g_pTcpIp )
											{
												g_pTcpIp->SendNetMessage( m_commOut, TRUE );
											}
											return 1;
										}
										else if( g_bGlobalMatrix && sel == 3 )	// 국가별 그리니쉬 스퀘어 // 국가대항전 시에만 가능
										{
											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // 대쉬 문제 등을 위한 체크 변수.
												g_pTcpIp->SendNetMessage( "mat_jump 2 1\n" ); // 뒤에 1이 더 붙은 PK 지역으로의 점프 패킷.
											}
										}
									}
									else
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, m_byWarp[ sel ], 0 );
									///////////////////////////////////////////////////////////////////
								}
								else if( pCMyApp->m_nCurWorldIdx == 18 )
								{
									if( sel == 0 )
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_DMITRON, 0 );
								}
							}

							pCMyApp->m_pUIMgr->m_bMoveKit  = FALSE;
							pCMyApp->m_pUIMgr->m_bParosKit = FALSE;
							pCMyApp->m_pUIMgr->m_bDmitron  = FALSE;
							pCMyApp->m_pUIMgr->m_bMarboden = FALSE; // [2008/6/23 Theodoric] 마르보덴 이동장치
						}
					}
					pCMyApp->SetCurVillageID(sel);
				}
			}
			else
			{
				g_pNk2DFrame->ShowInterfaceWindow(FALSE, GATE);
				pCMyApp->m_pUIMgr->m_bMoveKit  = FALSE; //1203
				pCMyApp->m_pUIMgr->m_bParosKit = FALSE;
				pCMyApp->m_pUIMgr->m_bDmitron  = FALSE;
				pCMyApp->m_pUIMgr->m_bMarboden = FALSE; // [2008/6/23 Theodoric] 마르보덴 이동장치
				return 1;
			}
		}
		return 1;
	}
	return 1;
}

void CControlGate::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

LRESULT CControlGate::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if(nMsg == 1 && g_pTcpIp) // 확인이 눌린 경우.
	{
		switch(m_byPopupType)
		{
		case 1:
			g_pTcpIp->SendNetMessage( "mat_jump 1\n" );	// 뫼비우스 이동
			break;
		case 2:
			g_pTcpIp->SendNetMessage( "trans 1 1\n" );	// 고대의 패허 이동
			break;
		}
	}
	else if(nMsg == -1 && g_pTcpIp) // 취소가 눌린 경우.
	{
		g_pNk2DFrame->CloseAllWindow();

		if( m_byPopupType == 1 ) // 07-01-19 수정
			g_pTcpIp->SendNetMessage( "mat_jump 12\n" );	// 월드컵 경기장으로 이동
	}
	return 0L;
}

//드미트론 공성전
void CControlGate::RemoveAll()
{
	for(int i = 0; i < 100; ++i)
		m_ItemArray[i] = 0;
	m_ItemCurrent = 0;
}

void CControlGate::EndItem()
{
	if (m_GateList)
	{
		m_GateList->SetPosDim(m_pSurfaceBack->Xpos+45, m_pSurfaceBack->Ypos+86, 191, 226);
		m_GateList->LoadRes();
	}
}

void CControlGate::AddItem(char* name, void* value)
{
	m_ItemArray[m_ItemCurrent++] = value;
	m_GateList->AddString(name);
}

int  CControlGate::GetSubType()
{
	return m_GateSubType;
}

//  [12/1/2008 ppmmjj83] 일본전용 체크
bool CControlGate::IsJapan(int sel)
{
	if( g_dwClientCountry == CTRY_JPN )
	{
		// 튜란존은 티켓이 필요없음 으로 그냥 보내줌
		if( sel == 0 )
		{
			pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_BULKAN, 0 );
			return true;
		}

		// 마그리아 티켓이 있는지 체크
		CItem *pKeyItem = NULL;
		pKeyItem = g_pRoh->FindItemByVnumFromAll( 2169 );

		if( pKeyItem && (g_pRoh->CheckWherePackItem(pKeyItem) >= 0 && g_pRoh->CheckWherePackItem(pKeyItem) <= 2) )
		{
			switch( sel )
			{
			case 0:
				pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_BULKAN, 0 );
				break;
			case 1:
				pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_KAILIPTON, 0 );  // 2009.02.18 일본테섭 업데이트
				break;
			case 2:
				pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_AIDIA, 0 );
				break;
			case 3:
				pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_HUMAN, 0 );
				break;
			case 4:
				pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_HYBRIDER, 0 );
				break;

			default:
				g_pNk2DFrame->InsertPopup("Error Message : Magria Ticket", TYPE_NOR_OK);
				return true;
			}
		}
		else
		{
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_HAVE_NOITEM),TYPE_NOR_OK, 1);
		}

		return true;
	}

	return false;
}

bool CControlGate::CheckUpdateVersionID(unsigned int zoneID)
{
	return true;	
}
