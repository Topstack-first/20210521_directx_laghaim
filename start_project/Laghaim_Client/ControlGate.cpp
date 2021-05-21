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
extern BOOL	g_bGlobalMatrix; ///���������� �� Ŭ���̾�Ʈ ���� ����.

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
		//-- IDS_GATE_WORLD_MOVE : �����̵�
		m_GateList->AddString(G_STRING(IDS_GATE_WORLD_MOVE));	// 0
	}

	m_GateType = 0; // ����Ʈ ������ ����صα� ���ؼ� ���. ///���������� �� ��Ʈ���� ���� ����Ʈ ������ �ʿ��ؼ� �߰��ߴ�. ���� ����Ʈ���� �̰ɷ� üũ�ؼ� �޽��� ó���� ������. 07-03-13 ����
	m_ItemCurrent = 0; //���Ʈ�� ������ - ���� ��ȣ �� ���
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
					m_GateList->AddString(G_STRING(IDS_WORLD_13));	// Ʃ��			��ĭ ���� ��

				if(CheckUpdateVersionID(IDS_WORLD_14))
					m_GateList->AddString(G_STRING(IDS_WORLD_14));	// ��ī����		ī�̸��� ���� ��

				if(CheckUpdateVersionID(IDS_WORLD_15))
					m_GateList->AddString(G_STRING(IDS_WORLD_15));	// �ٸ���		���̵�� ����

				if(CheckUpdateVersionID(IDS_WORLD_16))
					m_GateList->AddString(G_STRING(IDS_WORLD_16));	// ������		�޸� ���� ��

				if(CheckUpdateVersionID(IDS_WORLD_17))
					m_GateList->AddString(G_STRING(IDS_WORLD_17));	// ����׸���	���̺긮 ���� ��

				if(CheckUpdateVersionID(IDS_WORLD_18))
					m_GateList->AddString(G_STRING(IDS_WORLD_18));	// ��������		���� ���� ��

				int limit_level = 350;
				if( g_pRoh->m_Level >= limit_level )
				{
					if(CheckUpdateVersionID(IDS_WORLD_18))
						m_GateList->AddString(G_STRING(IDS_LHSTRING1829));	// �޷з���s1
				}


			}
			else
			{
				m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));

				if(CheckUpdateVersionID(IDS_GATE_WHITEHORN_DUNGEON))
					m_GateList->AddString(G_STRING(IDS_GATE_WHITEHORN_DUNGEON));
			}
		}
		//  [11/12/2008 parkmj] �Ϻ���û - ������� ������ Ŭ���� �̵�â ���
		else if( (nCurWorldNum >= WORLD_BOSSRAID_BULKAN && nCurWorldNum <= WORLD_BOSSRAID_HYBRIDER) && g_dwClientCountry == CTRY_JPN )
		{
			m_GateList->AddString(G_STRING(IDS_WORLD_09));   // #define	IDS_WORLD_09	1372	// ���Ʈ��
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
					break; // ���帶��
				case 2:
					nIndex = 5;
					break; // �ȱ׸���
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
						m_byWarp[ byEventState ] = 20 + i;	// ���� �ε��� ���
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
		// Matrix : ��������Ʈ
		else if(nCurWorldNum == WORLD_MATRIX )
		{
			if( g_pRoh && !g_pRoh->m_GuildIndex )
			{
				m_byWarp[ 0 ] = 3;
				m_GateList->AddString( G_STRING( IDS_WAITROOM ) );
			}
			else
			{
				int nIndex = 0;	// �ڱ� ���� ����Ʈ ã��
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

				// Ÿ ������ ����Ʈ�� ����� �Ұ�
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
				// Ÿ �ֵ��� ����Ʈ ������
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
					// 9 ~ 11 : G.P.Stone ��ȣ
					if( pCMyApp->m_pUIMgr->m_nStoneState[ i ] == g_pRoh->m_GuildIndex )
					{
						byMain = 1;
						if( pCMyApp->m_pUIMgr->m_nWarpGateIndex == i + 78 )	// i + 78( 87, 88, 89 ) : ������Ʈ�� ���ؽ��� ��
							continue;

						m_byWarp[ byEventState++ ] = i;	// ���� �ε��� ���
						sprintf( szTemp, G_STRING( IDS_ARMY_POST ), i - 8 );
						m_GateList->AddString( szTemp );
					}
				}
				// ���� �߰� ���� �Ǵ�
				if( pCMyApp->m_pUIMgr->m_nWarpGateIndex != nIndex && byMain )
				{
					// 94 : ���� ��������Ʈ ��ȣ
					m_GateList->AddString( G_STRING( IDS_BASECAMP ) );
					m_byWarp[ byEventState++ ] = 1;
				}

				m_byWarp[ byEventState ] = 3;
				m_GateList->AddString( G_STRING( IDS_WAITROOM ) );

				pCMyApp->m_pUIMgr->m_nWarpGateIndex = 0;
			}

			// ���Ʈ�� ������ - ���� ��Ż : �ƹ��͵� ���� �ʴ´�.
		}
		else if(nCurWorldNum == 200)
		{

			//=================================================================
		}
		else if(nCurWorldNum == 111)
		{
			// �� �̵���ġ 30~60����, ���� 11�̾����� �� ��ȣ�� 10����� �Ѿ�� ���� 111�� �ٲپ���.(by ����)
			//-- IDS_GATE_LAGRAMIA : ��׶�̾� ����
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : �ν�Ʈ���� ��
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : ��ī��
			if(CheckUpdateVersionID(IDS_GATE_DEKADUNE))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));

			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		else if(nCurWorldNum == 112)
		{
			// �� �̵���ġ 60~90����, ���� 12�̾����� �� ��ȣ�� 10����� �Ѿ�� ���� 112�� �ٲپ���.(by ����)
			//-- IDS_GATE_LAGRAMIA : ��׶�̾� ����
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : �ν�Ʈ���� ��
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : ��ī��
			if(CheckUpdateVersionID(IDS_GATE_DEKADUNE))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));
			//-- IDS_GATE_DEKALEN : ��ī��
			if(CheckUpdateVersionID(IDS_GATE_DEKALEN))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKALEN));

			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		else if(nCurWorldNum == 113)
		{
			// �� �̵���ġ 90~120����, ���� 13�̾����� �� ��ȣ�� 10����� �Ѿ�� ���� 113�� �ٲپ���.(by ����)
			//-- IDS_GATE_LAGRAMIA : ��׶�̾� ����
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : �ν�Ʈ���� ��
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : ��ī��
			if(CheckUpdateVersionID(IDS_GATE_DEKADUNE))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));
			//-- IDS_GATE_DEKALEN : ��ī��
			if(CheckUpdateVersionID(IDS_GATE_DEKALEN))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKALEN));
			//-- IDS_GATE_SHAILON : ���Ϸ� �ʵ�
			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_SHAILON));

			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		//�� �̵���ġ Lev 120�̻�
		else if(nCurWorldNum == 114)
		{
			// �� �̵���ġ 120���� �̻�, ���� 14�̾����� �� ��ȣ�� 10����� �Ѿ�� ���� 114�� �ٲپ���.(by ����)
			//-- IDS_GATE_LAGRAMIA : ��׶�̾� ����
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : �ν�Ʈ���� ��
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : ��ī��
			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));
			//-- IDS_GATE_DEKALEN : ��ī��
			if(CheckUpdateVersionID(IDS_GATE_DEKALEN))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKALEN));
			//-- IDS_GATE_SHAILON : ���Ϸ� �ʵ�
			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_SHAILON));
			//-- IDS_GATE_SIRUSTE : �÷罺Ʈ ����
			if(CheckUpdateVersionID(IDS_GATE_SIRUSTE))
				m_GateList->AddString(G_STRING(IDS_GATE_SIRUSTE));
			//-- IDS_MAP_PHAROS : õ���Ǽ�
			if(CheckUpdateVersionID(IDS_MAP_PHAROS))
				m_GateList->AddString(G_STRING(IDS_MAP_PHAROS));
			//-- IDS_GATE_WHITEHORN :�׷���Ʈ ȭ��Ʈ ȥ
			if(CheckUpdateVersionID(IDS_GATE_WHITEHORN))
				m_GateList->AddString(G_STRING(IDS_GATE_WHITEHORN));
			if(g_pRoh->m_Level > 249)	//ȭ��Ʈȥ���� ��������
			{
				//-- IDS_DUNGEON4 : ������ ������Ƽ
				if(CheckUpdateVersionID(IDS_DUNGEON4))
					m_GateList->AddString(G_STRING(IDS_DUNGEON4));
			}
			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		else if(nCurWorldNum == 915)
		{
			//-- IDS_MAP_PHAROS : õ���Ǽ�
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
			//-- IDS_GATE_LAGRAMIA : ��׶�̾� ����
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			m_bSpecialGate = TRUE;
		}
		else if (nCurWorldNum == 917)
		{
			//-- IDS_GATE_DMITRON : ���Ʈ��
			if(CheckUpdateVersionID(IDS_GATE_DMITRON))
			{
				m_GateList->AddString(G_STRING(IDS_GATE_DMITRON));
				pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
				pCMyApp->m_pUIMgr->m_bDmitron = TRUE;
			}
		}
		// [2008/6/23 Theodoric] �������� �̵���ġ
		else if( nCurWorldNum == 918 )
		{
			if(CheckUpdateVersionID(IDS_WORLD_18))
			{
				//-- IDS_WORLD_18 : ��������
				m_GateList->AddString( G_STRING(IDS_WORLD_18) );
				pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
				pCMyApp->m_pUIMgr->m_bMarboden = TRUE;
			}
		}

		//package ����, �� ���� ���� ����
		else if (nCurWorldNum == 117)
		{
			//040531 �߰� // �� �̵���ġ ��Ű�� ����., ���� 17�̾����� �� ��ȣ�� 10����� �Ѿ�� ���� 117�� �ٲپ���.(by ����)
			//-- IDS_GATE_LAGRAMIA : ��׶�̾� ����
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));
			//-- IDS_GATE_LOST_REALM : �ν�Ʈ���� ��
			m_GateList->AddString(G_STRING(IDS_GATE_LOST_REALM));
			//-- IDS_GATE_DEKADUNE : ��ī��
			if(CheckUpdateVersionID(IDS_GATE_DEKADUNE))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKADUNE));
			//-- IDS_GATE_DEKALEN : ��ī��
			if(CheckUpdateVersionID(IDS_GATE_DEKALEN))
				m_GateList->AddString(G_STRING(IDS_GATE_DEKALEN));
			//-- IDS_GATE_SHAILON : ���Ϸ� �ʵ�
			if(CheckUpdateVersionID(IDS_GATE_SHAILON))
				m_GateList->AddString(G_STRING(IDS_GATE_SHAILON));
			//-- IDS_GATE_SIRUSTE : �÷罺Ʈ ����
			if(CheckUpdateVersionID(IDS_GATE_SIRUSTE))
				m_GateList->AddString(G_STRING(IDS_GATE_SIRUSTE));
			//-- IDS_MAP_PHAROS : õ���Ǽ�
			if(CheckUpdateVersionID(IDS_MAP_PHAROS))
				m_GateList->AddString(G_STRING(IDS_MAP_PHAROS));
			//-- IDS_GATE_WHITEHORN :�׷���Ʈ ȭ��Ʈ ȥ
			if(CheckUpdateVersionID(IDS_GATE_WHITEHORN))
				m_GateList->AddString(G_STRING(IDS_GATE_WHITEHORN));
			//-- IDS_DUNGEON4 : ������ ������Ƽ
			if(CheckUpdateVersionID(IDS_DUNGEON4))
				m_GateList->AddString(G_STRING(IDS_DUNGEON4));

			pCMyApp->m_pUIMgr->m_bMoveKit = TRUE;
		}
		else if( nCurWorldNum == 1010 ) ///���������� �ÿ� ��Ʈ���� ���� ��������Ʈ ��� ����
		{
			//-- IDS_GATE_MATWAR : ����콺 ����
			m_GateList->AddString( G_STRING(IDS_GATE_MAT_WAR) );
			//-- IDS_GATE_GREENISH : P.K ��
			m_GateList->AddString( G_STRING(IDS_GATE_MAT_PK) );

			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));//Arnold


			if( g_bGlobalMatrix ) // ������������ ��츸...
			{
				//-- IDS_GATE_G_GREENISH : ������ P.K ��
				m_GateList->AddString( G_STRING(IDS_GATE_MAT_GPK) );
			}
		}
		else if( nCurWorldNum == 1011 ) // [2008/10/31 Theodoric] WORLD_MATRIX_2009
		{
			//-- IDS_GATE_MATWAR : ����콺 ����
			if(CheckUpdateVersionID(IDS_GATE_MAT_WAR))
				m_GateList->AddString( G_STRING(IDS_GATE_MAT_WAR) );
			//-- IDS_GATE_GREENISH : P.K ��
			m_GateList->AddString( G_STRING(IDS_GATE_MAT_PK) );

			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));//Arnold

			if( g_bGlobalMatrix ) // ������������ ��츸...
			{
				//-- IDS_GATE_G_GREENISH : ������ P.K ��
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
			m_GateList->AddString(G_STRING(IDS_GATE_DMITRON));	// ���Ʈ��
		}
		else if( nCurWorldNum == 24 )
		{
			m_GateList->AddString(G_STRING(IDS_GATE_LAGRAMIA));	 // Add Portal S4 to Laglamia
			m_GateList->AddString(G_STRING(IDS_LHSTRING1896)); // Add Portal S4 to S3
		}
		else
		{
			// by evilkiki 2010.01.
			// ���̵� ������ ���� ����Ʈ ���� ����Ʈ ��ȣ
			// 500����~�������...500, 501, 502....
			// 550���ʹ� ����
			// string recource ��Ī
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

	m_GateType = nCurWorldNum; // ����Ʈ ������ ����صα� ���ؼ� ���. ///���������� �� ��Ʈ���� ���� ����Ʈ ������ �ʿ��ؼ� �߰��ߴ�. ���� ����Ʈ���� �̰ɷ� üũ�ؼ� �޽��� ó���� ������. 07-03-13 ����
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

void CControlGate::SetSubType(int nSubType) // ���Ʈ�� ������ - �̵� ����Ʈ ���� Ÿ��
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
	static char commOutBuf[512];	// ���߿� ���� �ٸ� ���� ����.
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
				// Msg Popupâ�� ���������� �� ����Ʈ�� Ŭ���� �ȵǰ� ����~
				//if( g_pNk2DFrame->Check_All_MsgPopUp() )
				//	return 1;

				if( pCMyApp->m_pUIMgr && m_CloseBtn.IsInside(x, y) )
				{
					m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
					pCMyApp->m_pUIMgr->m_bMoveKit  = FALSE; //1204
					pCMyApp->m_pUIMgr->m_bParosKit = FALSE;
					pCMyApp->m_pUIMgr->m_bDmitron  = FALSE;
					pCMyApp->m_pUIMgr->m_bMarboden = FALSE; // [2008/6/23 Theodoric] �������� �̵���ġ
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
								// [2008/6/23 Theodoric] �������� �̵���ġ
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
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_WHITEHORN, 0);//040531 �߰�
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
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_SKY, 0);//040531 �߰�
									else if(sel == 7)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_WHITEHORN, 0);
									//wan:2004-11 : Whitehorn Dungeon - ȭ��Ʈȥ ������ ����250�̻� �� �� ���� ��������
									else if(sel == 8)
										sprintf(m_commOut, "go_zone %d %d %d\n", g_pNk2DFrame->m_Slot_Num, WORLD_DUNGEON4, 0);

								}

								if (g_pTcpIp)
									g_pTcpIp->SendNetMessage(m_commOut);

								pCMyApp->PrepareChangeWorld();

								// ��Ű�� ���������� ��ȯ��ġ�� ������ ����...
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
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
												g_pTcpIp->SendNetMessage( "mat_jump 4\n" );
											}
									}
									else if( sel == 2 && g_pRoh->m_Level >= 910)	// 
									{
										if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
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
									else if(sel == 1) //���Ϸ� �ʵ�
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
									if(sel == 0) //���Ϸ� �ʵ�
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_FIELD, sel);
									else if(sel == 1) //���Ϸ� �ʵ�
										pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_GUILD, 0);
									else if(sel == 2)	// õ���Ǽ�
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
									else if(sel == 1) //���Ϸ� �ʵ�
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
									// õ���� �ι�° ����Ʈ, lolo id 294
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
								// nate 2005-07-15 : Dmitron - �̵� ����Ʈ�� ���� ��Ż
								else if( pCMyApp->m_nCurWorldIdx == WORLD_DUNGEON4 )
								{
									if( sel == 0 )
										pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_DMITRON, 0 );
								}

								//  [11/12/2008 parkmj] �Ϻ���û - ������� ������ Ŭ���� �̵�â ���
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
									case 0: // Ʃ�� - ��ĭ ���� ��
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_BULKAN, 0 );
											break;
										}

									case 1: // ��ī���� - ī�̸��� ���� ��
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_KAILIPTON, 0 );
											break;
										}

									case 2: // �ٸ��� - ���̵�� ���� ��
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_AIDIA, 0 );
											break;
										}

									case 3: // ������ - �޸� ���� ��
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_HUMAN, 0 );
											break;
										}

									case 4: // ����׸��� - ���̺긮�� ���� ��
										{
											if( IsJapan(sel) ) break;
											else
												pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_HYBRIDER, 0 );
											break;
										}

									case 5: // �������� -  ���� ��
										{
											if( CommonConfig::Instance()->GetBossRaidChannel() == false )
											{										
											  if( g_SvrType != ST_12_OVER )
												{
													if( g_pRoh->GetSvrChannelNum() != 2 ) // ������ ������ 2�� ä�ο����� �� �� �ֵ�.
													{
														g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_FINALBOSS_LIMIT_CHNL), TYPE_NOR_OK, 1);
														return 1;
													}
												}
											}

											CItem *pKeyItem = NULL;
											pKeyItem = g_pRoh->FindItemByVnumFromAll( 1930 ); // ����� ���踦 ������ �ִ��� ã�´�.

											if( pKeyItem ) // ���谡 �ִٸ�
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
												pKeyItem = g_pRoh->FindItemByVnumFromAll( 2730 ); // ���������� �̵���ġ(�Ⱓ��)

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
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
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
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
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
										pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
										if (g_pTcpIp)
										{
											sprintf(m_commOut, "occp bt trans_r %d %d %d\n",
													m_GateSubType, pGate->Zone, pGate->Toll );
											g_pTcpIp->SendNetMessage( m_commOut );
										}
									}
								}
								// nate 2005-03-22 - Matrix : ��Ʈ�������� ���� ������ �̵�
								else if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX )
								{
									///////////////////////////////////////////////////////////////////
									///���������� �ÿ� ��Ʈ���� ���� ��������Ʈ ��� ����
									if( m_GateType == 1010 )
									{
										if(sel == 0) // ����콺 ����
										{
											/*if( g_bGlobalMatrix && pCMyApp->m_pUIMgr->m_bOnBattle )  ///�������̵������� // ��� �����Ŀ��� ������ ����
											{
												if( pCMyApp->m_NvsnMoveDelay == 0 ) // (�̹� ���������ΰ�� ����.)
												{
													// ���������� ���� �ο��� ���忡 �̵� ���ϵ��� ���� �����̸� ��������ش�.
													pCMyApp->m_NvsnMoveDelay = timeGetTime() + RandomNum(3000,15000);
												}

												return 1;
											}*/

											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
												g_pTcpIp->SendNetMessage( "mat_jump 1\n" );
											}
										}
										else if(sel == 1)	// �׸��Ͻ� ������
										{
											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
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
										else if( g_bGlobalMatrix && sel == 3 )	// ������ �׸��Ͻ� ������ // ���������� �ÿ��� ����
										{
											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
												g_pTcpIp->SendNetMessage( "mat_jump 2 1\n" ); // �ڿ� 1�� �� ���� PK ���������� ���� ��Ŷ.
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
									///���������� �ÿ� ��Ʈ���� ���� ��������Ʈ ��� ����
									if( m_GateType == 1011 )
									{
										if(sel == 0) // ����콺 ����
										{
											if( g_bGlobalMatrix && pCMyApp->m_pUIMgr->m_bOnBattle )  ///�������̵������� // ��� �����Ŀ��� ������ ����
											{
												if( pCMyApp->m_NvsnMoveDelay == 0 ) // (�̹� ���������ΰ�� ����.)
												{
													// ���������� ���� �ο��� ���忡 �̵� ���ϵ��� ���� �����̸� ��������ش�.
													pCMyApp->m_NvsnMoveDelay = timeGetTime() + RandomNum(3000,15000);
												}

												return 1;
											}

											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
												g_pTcpIp->SendNetMessage( "mat_jump 1\n" );
											}
										}
										else if(sel == 1)	// �׸��Ͻ� ������
										{
											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
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
										else if( g_bGlobalMatrix && sel == 3 )	// ������ �׸��Ͻ� ������ // ���������� �ÿ��� ����
										{
											if (g_pTcpIp)
											{
												pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // �뽬 ���� ���� ���� üũ ����.
												g_pTcpIp->SendNetMessage( "mat_jump 2 1\n" ); // �ڿ� 1�� �� ���� PK ���������� ���� ��Ŷ.
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
							pCMyApp->m_pUIMgr->m_bMarboden = FALSE; // [2008/6/23 Theodoric] �������� �̵���ġ
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
				pCMyApp->m_pUIMgr->m_bMarboden = FALSE; // [2008/6/23 Theodoric] �������� �̵���ġ
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
	if(nMsg == 1 && g_pTcpIp) // Ȯ���� ���� ���.
	{
		switch(m_byPopupType)
		{
		case 1:
			g_pTcpIp->SendNetMessage( "mat_jump 1\n" );	// ����콺 �̵�
			break;
		case 2:
			g_pTcpIp->SendNetMessage( "trans 1 1\n" );	// ����� ���� �̵�
			break;
		}
	}
	else if(nMsg == -1 && g_pTcpIp) // ��Ұ� ���� ���.
	{
		g_pNk2DFrame->CloseAllWindow();

		if( m_byPopupType == 1 ) // 07-01-19 ����
			g_pTcpIp->SendNetMessage( "mat_jump 12\n" );	// ������ ��������� �̵�
	}
	return 0L;
}

//���Ʈ�� ������
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

//  [12/1/2008 ppmmjj83] �Ϻ����� üũ
bool CControlGate::IsJapan(int sel)
{
	if( g_dwClientCountry == CTRY_JPN )
	{
		// Ʃ������ Ƽ���� �ʿ���� ���� �׳� ������
		if( sel == 0 )
		{
			pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_BULKAN, 0 );
			return true;
		}

		// ���׸��� Ƽ���� �ִ��� üũ
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
				pCMyApp->m_pUIMgr->PrepareChangeWorld( TRUE, WORLD_BOSSRAID_KAILIPTON, 0 );  // 2009.02.18 �Ϻ��׼� ������Ʈ
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
