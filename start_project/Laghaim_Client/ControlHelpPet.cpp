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

int g_HelpPetAniFrame[MAX_HELPPET_ANI] =   // ������ �Ѱ���.
{
	12, // HELPPET_ANI_NORMAL
	28, // HELPPET_ANI_FIND
	12, // HELPPET_ANI_THROW
	16, // HELPPET_ANI_ANGRY ///�굵��Ȯ��
	26,  // HELPPET_ANI_SLEEPY ///�굵��Ȯ��
	12, // HELPPET_ANI_SEARCH ///�굵��Ȯ��
};

///�굵��
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
	// �� �κ�
	m_nType = HELPPET_TYPE_RABIYONG; // ���� ����.
	m_nAnimation = HELPPET_ANI_NORMAL; // ���� �ִϸ��̼� ��ȣ.
	m_nDir = 0;
	m_nMoveFlag = 0;

	SAFE_DELETE( m_pIndexedTexture );
	m_pIndexedTexture = new IndexedTexture( 18 );
	if( m_pIndexedTexture )
		m_pIndexedTexture->SetTexturePath("interface/HelpPet/");

	ZeroMemory( &m_PetRect, sizeof(RECT) );
	ZeroMemory( &m_TalksRect, sizeof(RECT) );

	ZeroMemory( &m_TalksText, sizeof(CMultiText) ); // ���� ����� ����� ��������Ѵ�.
	ZeroMemory( m_TLVec, sizeof(m_TLVec) );

	ZeroMemory( m_AniIndex, sizeof(int)*MAX_HELPPET_ANI );

	m_nNowFrame = 0; // ���� �ִϸ��̼� ������.
	m_dwFrameRate = 0; // �� ������ �Ѿ�� �� �ʿ��� ������ ����Ʈ. ƽ �����ӿ� ����.
	m_dwPetAniChageTick = 0; // �� ���۰� ���õ� �κ��� ��� ƽ.
	m_dwOldFrameTick = 0; // ���� ƽ.

	m_bPetDrag = FALSE;
	m_DragDistX = 0;
	m_DragDistY = 0;
	m_ClickX = 0; // Ŭ���� ��ġ ���.
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

	m_BoundCount = 0; ///�굵��Ȯ��

	m_Searching = 0; ///�굵��Ȯ��
	ZeroMemory( &m_SearchRect, sizeof(RECT) );
	m_SearchInput.Init( 0, 0, 10*2, RGB(255, 255, 255), RGB(250, 250, 250) );
	m_SearchInput.SetfontSize();
	m_SearchInput.ClearText();

	m_SearchResultText.Reset();
	m_SearchResultIndex.clear();
	m_NowViewResult = 0; // ���� �����ִ� �˻���� ��ġ. (������ ���� ������ �ʿ�)
	m_TotalResult = 0; // �˻� ��� ����.
	m_NowSelResult = 0;

	m_nSleepCount = 0;
}

void CControlHelpPet::DeleteRes()
{
	m_SearchInput.DeleteRes();     // ���������� ����� ��Ʈ ��¿� ���ǽ�.

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
	m_PrevBtn.DeleteRes(); // ���� �Ǹ� ��� ���� ��ư.
	m_NextBtn.DeleteRes();
	m_CloseBtn.DeleteRes();
	m_CloseBtn2.DeleteRes();
	m_NextBtn2.DeleteRes();
	m_PrevBtn2.DeleteRes(); // ���� �Ǹ� ��� ���� ��ư.

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

		m_PetRect.left = 450; // ���� ��ġ
		m_PetRect.top = 350; // ���� ��ġ
		m_PetRect.right = 100; // ���� ũ��
		m_PetRect.bottom = 100; // ���� ũ��

		// �׷��� �����͵� Ÿ�Ժ��� �ε�
		m_AniIndex[HELPPET_ANI_NORMAL] = m_pIndexedTexture->CreateTextureFromFile( "Ravy/fly.tga" ); // ���� ����ٴϴ� �༮.
		m_pIndexedTexture->CreateTextureFromFile( "Ravy/drag.tga" ); // �巡�� �� �ٵչٵ�. ��� �ִϸ��̼� +1 �� �ν�.
		m_AniIndex[HELPPET_ANI_FIND] = m_pIndexedTexture->CreateTextureFromFile( "Ravy/Find.tga" ); // ���� ã��.
		m_pIndexedTexture->CreateTextureFromFile( "Ravy/Find2.tga" ); // �� �༮�� +1�� �ν�.
		m_AniIndex[HELPPET_ANI_ANGRY] = m_pIndexedTexture->CreateTextureFromFile( "Ravy/angry.tga" ); // ȭ��. ///�굵��Ȯ��
		m_AniIndex[HELPPET_ANI_SLEEPY] = m_pIndexedTexture->CreateTextureFromFile( "Ravy/sleepy.tga" ); // ����
		m_pIndexedTexture->CreateTextureFromFile( "Ravy/sleepy2.tga" ); // �� �༮�� +1�� �ν�.

		break;
	}

	// �������̽� �ε�.
	m_InterfaceTexIndex[HELPPET_INTERFACE_TALKBOX] = m_pIndexedTexture->CreateTextureFromFile( "help_bubble.tga" ); // ��ǳ��.
	m_InterfaceTexIndex[HELPPET_INTERFACE_CONTENTSBOX] = m_pIndexedTexture->CreateTextureFromFile( "help_bk.tga" ); // ���� ���
	m_InterfaceTexIndex[HELPPET_INTERFACE_SEARCHBOX] = m_pIndexedTexture->CreateTextureFromFile( "help_search_bk.tga" ); // �˻� â ���
	m_InterfaceTexIndex[HELPPET_INTERFACE_SEARCHMENT] = m_pIndexedTexture->CreateTextureFromFile( "help_search.tga" ); // �˻� â ���

	RestoreSurfaces();

	m_ContentsRect.right = 550;
	m_ContentsRect.bottom = 350;
	m_ContentsRect.left = (m_ClientRc.right - m_ContentsRect.right) / 2;

	m_ContentsRect.top = (m_ClientRc.bottom - m_ContentsRect.bottom) / 2-70;

	// ���� ��ũ��Ʈ�� �о�´�.
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
void CControlHelpPet::SetPetState( int State ) // ���� ����.
{
	m_nState = State;

	m_dwStateChangeTick = timeGetTime();
}
*/
void CControlHelpPet::SetPetAnimation( int AniNum ) // �ִϸ��̼� ����
{
	switch( m_nAnimation ) // ������ �ִϸ��̼ǿ� ���� �۵�.
	{
	case HELPPET_ANI_NORMAL: // �׳� �Ĵ��Ĵ�.
		break;
	case HELPPET_ANI_FIND: // �� ��(����)�� ã�Ҵ�!
		break;
	case HELPPET_ANI_THROW:
		m_ThrowPower = 0;

//		if( AniNum == HELPPET_ANI_FIND ) // ������ �߿� ã���� �ִϸ��̼� ��ȯ���� ���
//			return;

		break;

	case HELPPET_ANI_ANGRY: // ȭ��. ///�굵��Ȯ��
		TerminateTalkBox(); // �ٿ�� ī��Ʈ�� ������� ��ȭâ�� ���ֱ� ����.
		break;
	case HELPPET_ANI_SLEEPY: // ����
		m_PetRect.top -= 10.0f; // ���ȴ� ��ġ�� �ٽ� �÷��ش�.
		m_PetRect.bottom -= 10.0f; // ���ȴ� ��ġ�� �ٽ� �÷��ش�.
		break;

	case HELPPET_ANI_SEARCH: // �˻� ��. ///�굵��Ȯ��
		break;
	}

	m_nAnimation = AniNum;

	switch( m_nAnimation ) // ������ �ִϸ��̼ǿ� ���� �۵�.
	{
	case HELPPET_ANI_NORMAL: // �׳� �Ĵ��Ĵ�.
		m_dwFrameRate = 60;
		break;
	case HELPPET_ANI_FIND: // �� ��(����)�� ã�Ҵ�!
		m_dwFrameRate = 60;
		break;
	case HELPPET_ANI_THROW: ///�굵��Ȯ��
		m_BoundCount = 0;
		break;
	case HELPPET_ANI_ANGRY: // ȭ��. ///�굵��Ȯ��
		m_dwFrameRate = 100;

		break;
	case HELPPET_ANI_SLEEPY: // ����
		m_dwFrameRate = 200;
		m_PetRect.top += 10.0f; // ��ġ�� �ణ �����ش�.
		m_PetRect.bottom += 10.0f; // ��ġ�� �ణ �����ش�.

		m_nSleepCount = 0; // �ʱ�ȭ.

		break;

	case HELPPET_ANI_SEARCH: // �˻� ��. ///�굵��Ȯ��
		m_dwFrameRate = 60;
		break;
	}

	m_nNowFrame = 0;
	m_dwPetAniChageTick = timeGetTime(); // �ִϸ��̼��� �ٲ� ƽ�� ���.
}

void CControlHelpPet::FrameMove()
{
	DWORD dwElapseTick = timeGetTime() - m_dwOldFrameTick;


	switch( m_nAnimation ) // ������ �ִϸ��̼ǿ� ���� �۵�.
	{
	case HELPPET_ANI_NORMAL: // �׳� �Ĵ��Ĵ�.

		// �巡�� ����ó��.
		if( m_bPetDrag ) // Ȥ�� �巡�� ���̸�...
			m_dwFrameRate = 50;  // ������ �Ĵ��Ĵ�
		else
			m_dwFrameRate = 60;

		// �켱 ������ �������� ���Ѵ�.
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate ) % g_HelpPetAniFrame[m_nAnimation];

		// ���� ������ �ڵ����� ��������...
		if( !m_dwTalkStartTick && !m_bPetDrag ) // ���ϴ� �߿� �̵� ���ϰ�... // �巡�� �߿���.
		{
			if( m_PetRect.top < m_ClientRc.bottom-HELPPET_LIKE_HEIGHT )
			{
//				m_PetRect.top += 0.008f * dwElapseTick * ((float)(m_PetRect.top+1)*0.5f);
				m_PetRect.top += 0.08f * dwElapseTick;

				if( m_PetRect.top > m_ClientRc.bottom-HELPPET_LIKE_HEIGHT )
					m_PetRect.top = m_ClientRc.bottom-HELPPET_LIKE_HEIGHT;
			}

			if( m_nMoveFlag == -1 ) // �·� �����̴� ��.
			{
				m_nDir = 0;
				m_PetRect.left -= 0.04f * dwElapseTick;
			}
			else if( m_nMoveFlag == 1 ) // ��� �����̴� ��.
			{
				m_nDir = 1;
				m_PetRect.left += 0.04f * dwElapseTick;
			}
			else if( m_nMoveFlag == 0 ) // ���ڸ� ���� ���. ///�굵��Ȯ��
			{
				if( m_PetRect.top >= m_ClientRc.bottom-HELPPET_LIKE_HEIGHT && (rand()%1000) < 15 ) // ����. �ؿ� ������������.
//				if( m_PetRect.top >= m_ClientRc.bottom-HELPPET_LIKE_HEIGHT && (rand()%1000) < 500 ) // ����. �׽�Ʈ��
				{
					SetPetAnimation( HELPPET_ANI_SLEEPY );
				}
			}

			if( (rand()%1000) < 20 ) // ������ ���� ���� Ȯ��
			{
				m_nMoveFlag = (rand()%3) - 1;

				if( (rand()%1000) < 20 ) // ���ڸ��� ������ Ȯ���� �����ش�.
					m_nMoveFlag = 0;
			}
		}

		break;

	case HELPPET_ANI_FIND: // �� ��(����)�� ã�Ҵ�!
		// �켱 ������ �������� ���Ѵ�.
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate );

		if( m_nNowFrame >= g_HelpPetAniFrame[m_nAnimation] ) // �ѹ��� ��������..
			SetPetAnimation( HELPPET_ANI_NORMAL ); // �븻�� �����ش�.

		break;

	case HELPPET_ANI_THROW: // ������ ���� ���� ���.
		// �켱 ������ �������� ���Ѵ�.
		m_nNowFrame = 7; // ������ ����

		// ��ġ ó��.
		if( m_ThrowPower ) // �Ŀ��� �����ִٸ�...
		{
			// ����������.
			m_PetRect.left += m_ClickX * (m_ThrowPower*0.001f) * ( (int)dwElapseTick * 0.01f );
			m_PetRect.top += m_ClickY * (m_ThrowPower*0.001f) * ( (int)dwElapseTick * 0.01f );

			// �ٿ�� üũ.
			if( m_PetRect.left < 0 )
			{
				m_PetRect.left = 0;

				// ������ �ٲ��ش�.
				m_ClickX *= -1;
				m_BoundCount++; ///�굵��Ȯ��
			}
			if( m_PetRect.top < 0 )
			{
				m_PetRect.top = 0;
				// ������ �ٲ��ش�.
				m_ClickY *= -1;
				m_BoundCount++; ///�굵��Ȯ��
			}
			if( m_PetRect.left+m_PetRect.right > m_ClientRc.right )
			{
				m_PetRect.left = m_ClientRc.right - m_PetRect.right;
				// ������ �ٲ��ش�.
				m_ClickX *= -1;
				m_BoundCount++; ///�굵��Ȯ��
			}
			if( m_PetRect.top+m_PetRect.bottom > m_ClientRc.bottom )
			{
				m_PetRect.top = m_ClientRc.bottom - m_PetRect.bottom;
				// ������ �ٲ��ش�.
				m_ClickY *= -1;
				m_BoundCount++; ///�굵��Ȯ��
			}

			m_ThrowPower -= dwElapseTick; // ���� �� ����.
			if( m_ThrowPower < 0 ) // ���̳ʽ��� �ƴٸ�...
			{
				m_ThrowPower = 0;
				m_dwPetAniChageTick = timeGetTime(); // ��� �ð��� �ֱ� ���� �ʱ�ȭ.
			}
		}
		else // ���̻� �� ���� ����
		{
			if( timeGetTime() - m_dwPetAniChageTick < 1300 ) // �ణ ����� Ÿ���� �ش�.
			{
				// �������� �� ������ �ٲ۴�.
				if( (timeGetTime() - m_dwPetAniChageTick)%100 < 30 )
				{
					m_nDir ^= 1; // ���
				}
			}
			else // ȭ����.
			{
				// ƨ���� ���ڸ� ��������. ///�굵��Ȯ��
				char TempString[256];
				// IDS_PETHELP_BOUNDCOUNT : %d�� ƨ���!!
				sprintf( TempString, (char *)G_STRING(IDS_PETHELP_BOUNDCOUNT), m_BoundCount );
				ExcuteHelpTalk( TempString );

				SetPetAnimation( HELPPET_ANI_ANGRY );
			}
			/*
						else // ������ �������.
						{
							SetPetAnimation( HELPPET_ANI_NORMAL ); // �븻�� �����ش�.
						}
			*/
		}

		break;

	case HELPPET_ANI_ANGRY: // ȭ��. ///�굵��Ȯ��

		// �켱 ������ �������� ���Ѵ�.
		/*
				m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate ) % g_HelpPetAniFrame[m_nAnimation]; // ����

				if( timeGetTime() - m_dwPetAniChageTick > 2000 ) // �����ð��� ��������.
					SetPetAnimation( HELPPET_ANI_NORMAL ); // �븻�� �����ش�.
		*/
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate );

		if( m_nNowFrame >= g_HelpPetAniFrame[m_nAnimation] ) // �ѹ��� ��������..
			SetPetAnimation( HELPPET_ANI_NORMAL ); // �븻�� �����ش�.

		break;
	case HELPPET_ANI_SLEEPY: // ����

		// �켱 ������ �������� ���Ѵ�.
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate );

		if( m_bPetDrag ) // �������� �ٷ� ����.
		{
			SetPetAnimation( HELPPET_ANI_NORMAL ); // �븻�� �����ش�.
			if( m_nDir == 0 ) // ���� ���� ������.
				m_nMoveFlag = -1;
			else
				m_nMoveFlag = 1;

			break;
		}

		if( m_nSleepCount < 2 ) // ��� �ڰ��ϱ�����.
		{
			int LoopStart = 2; // �ݺ��Ǵ� ������ ����
			int LoopEnd = 12;  // �ݺ��Ǵ� ������ ����
			int LoopLength = LoopEnd - LoopStart;

			if( m_nNowFrame >= LoopEnd )
			{
				int k = m_nNowFrame - LoopEnd;
				m_nSleepCount = k / LoopLength;
				k %= LoopLength;
				m_nNowFrame = LoopStart + k;
			}
		}
		else // ���� �Ͼ��...
		{
			// ������ �������� �̾��ֱ� ���ؼ� ���
			int LoopStart = 2; // �ݺ��Ǵ� ������ ����
			int LoopEnd = 12;  // �ݺ��Ǵ� ������ ����
			int LoopLength = LoopEnd - LoopStart;

			m_nNowFrame = (     m_nNowFrame - ( LoopEnd+(LoopLength*m_nSleepCount) )    );

			if( m_nNowFrame >= g_HelpPetAniFrame[m_nAnimation] ) // �ѹ��� ��������..(�Ǵ� Ŭ���̳� �巡�� �ص� ����.)
			{
				SetPetAnimation( HELPPET_ANI_NORMAL ); // �븻�� �����ش�.
				if( m_nDir == 0 ) // ���� ���� ������.
					m_nMoveFlag = -1;
				else
					m_nMoveFlag = 1;

			}
		}

		break;

	case HELPPET_ANI_SEARCH: // �˻� ��. ///�굵��Ȯ��

		// �켱 ������ �������� ���Ѵ�.
		m_nNowFrame = ( ( timeGetTime() - m_dwPetAniChageTick ) / m_dwFrameRate ) % g_HelpPetAniFrame[m_nAnimation];

		break;

	}

	// ȭ�� �Ѿ�� ����.
	if( m_PetRect.left < 0 )
		m_PetRect.left = 0;
	if( m_PetRect.top < 0 )
		m_PetRect.top = 0;
	if( m_PetRect.left+m_PetRect.right > m_ClientRc.right )
		m_PetRect.left = m_ClientRc.right - m_PetRect.right;
	if( m_PetRect.top+m_PetRect.bottom > m_ClientRc.bottom )
		m_PetRect.top = m_ClientRc.bottom - m_PetRect.bottom;


//	m_PrevBtn.SetPosition(m_pBackSur->Xpos+180, m_pBackSur->Ypos+565 );

	CheckHelpCondition( HELP_CON_TYPE_A_LEVEL, g_pRoh->m_Level, FALSE ); // ������ ������ �ֱ������� üũ���ֱ� ���ؼ�...

	m_dwOldFrameTick = timeGetTime(); // ƽ ����.
}

void CControlHelpPet::Draw() // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
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

	// ������ ����Ʈ�� �����ϰ� ���Ӱ� ����Ʈ�� �����Ѵ�.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	//// �������� ��Ʈ���� ����.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	// ���°� ����.
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



	float FrameWidthRatio = 0.0f; // �� �������� ���� ����
	int FrameWidth = HELPPET_FRAME_WIDTH; // ���� �ִϸ��̼��� �� �ؽ��Ŀ����� ���� ������ ����.
	float tu0,tv0,tu1,tv1,tu2,tv2;
	DWORD color;

	//----------------�׸��� ���� / �� --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		//-----------------------TLVERTEX �� �̿��� ������ ���(ȭ���� ���� ��ǥ�� �Է��Ѵ�)
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

		color = RGBA_MAKE ( 255, 255, 255, 255 );

		// ������ �ڽ��� ����ؾ� �ϴ� ���.
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

			// ������ jpg ���
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

			// ��ư ���
			m_PrevBtn2.PosX = m_ContentsRect.left+(m_ContentsRect.right/2)-63;
			m_PrevBtn2.PosY = m_ContentsRect.top+305;
			m_PrevBtn2.Draw();

			m_NextBtn2.PosX = m_ContentsRect.left+(m_ContentsRect.right/2)+30;
			m_NextBtn2.PosY = m_ContentsRect.top+305;
			m_NextBtn2.Draw();

			m_CloseBtn2.PosX = m_ContentsRect.left+m_ContentsRect.right-35;
			m_CloseBtn2.PosY = m_ContentsRect.top+6;
			m_CloseBtn2.Draw();

			// ������ ��ȣ ���.
			sprintf( strTemp, "%d / %d", m_nNowContentsPage+1, m_ListContentsSurface.size() );
//			m_TextOutBox.SetString( strTemp, RGB(250,250,10), RGB(0,0,0) );
//			m_TextOutBox.SetString( strTemp, RGB(5,5,5) );
			m_TextOutBox.SetString_withOutLine( strTemp, RGB(5,5,5), RGB(5,5,5) );
			m_TextOutBox.Draw( m_ContentsRect.left+4, m_ContentsRect.top+308, (m_ContentsRect.right) );
		}

		// ���� â
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , TRUE);

		// �켱�� ��.
		// �����ӿ� ���� tu,tv ����.
		FrameWidthRatio = (1.0f/(float)FrameWidth); // �� �������� ���� ����
		tu0 = (float)(m_nNowFrame % FrameWidth) * FrameWidthRatio;
		tv0 = (float)(m_nNowFrame / FrameWidth) * FrameWidthRatio;

		// ������� �¿츦 �����´�.
		if( m_nDir == 0 ) // ����.
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
		tv2 = tv0+FrameWidthRatio; // ���ε� ���ζ� �Ȱ��� �����ٴ� �����Ͽ� �۾�.

		if( m_nAnimation == HELPPET_ANI_THROW ) // ������ ��� ����ó��
		{
			// �ؽ��� ��ǥ�� ������ ��۹�� ���� ȿ���� ����.
			/*
						float k = (float)(timeGetTime()-m_dwPetAniChageTick); // ������ ������ �ð��� ��������..

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

		if( m_nAnimation == HELPPET_ANI_THROW ) // ������ ��� ����ó��
		{
			// ȸ�� ��Ų��.
			float k = (float)( (timeGetTime()-m_dwPetAniChageTick)%314 ) * 0.01f * (float)(m_ThrowPower*0.01f) ; // ������ ������ �ð��� ��������..// ������ ���� �Բ� ��꿡 �־������ν� �̵����Ҷ� ȸ���� �ȵǰ� ����ش�.

			if( m_ThrowPower && !m_ClickX ) // ���󰡴� �������.
				k *= -1.0f; // �ݴ��

			D3DMATRIX m;
			D3DUtil_SetRotateZMatrix( m, k );

			D3DVECTOR TempVec;
			D3DVECTOR TempVec2 = D3DVECTOR( m_PetRect.left+(m_PetRect.right/2), m_PetRect.top+(m_PetRect.bottom/2), 0.0f ); // ���� �߽���ǥ.
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


		// �ؽ��ĸ� ã���ش�.
		switch( m_nAnimation ) // ������ �ִϸ��̼ǿ� ���� �۵�.
		{
		case HELPPET_ANI_NORMAL: // �׳� �Ĵ��Ĵ�.

			if( m_bPetDrag )
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation]+1 ); // �ٵչٵ� �ִϸ��̼�.
			else
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );

			break;
		case HELPPET_ANI_FIND: // �� ��(����)�� ã�Ҵ�!

			if( m_nNowFrame > 16 ) // �������� �Ѿ�� ���.
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation]+1 ); // ���� �༮����...
			else
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );

			break;
		case HELPPET_ANI_THROW: // �׳� �Ĵ��Ĵ��� ���� ����.
			lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );
			break;

		case HELPPET_ANI_ANGRY: // ȭ��. ///�굵��Ȯ��
			lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );
			break;

		case HELPPET_ANI_SLEEPY: // ����

			if( m_nNowFrame > 16 ) // �������� �Ѿ�� ���.
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation]+1 ); // ���� �༮����...
			else
				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[m_nAnimation] );

			break;

		case HELPPET_ANI_SEARCH: // �˻�.
			lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_AniIndex[HELPPET_ANI_NORMAL] ); // �ϴ� ������ �븻�� ����.
			break;

		}

		GET_D3DDEVICE()->SetTexture(0, lpTexture );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);


		// ��ǳ���� ���������ϴ� ���.
		if( m_Searching ) // �˻� ��. ������ �켱 ������ ����. ///�굵��Ȯ��
		{
			// ��ǳ���� ��ġ�� �ǽð����� ������ش�.(ȭ�� ������ ������ ��� ������ �ٲ��ֱ� ������...
			// �⺻�� ���� �»�� ����.
			m_SearchRect.left = m_PetRect.left-m_SearchRect.right-HELP_TALKBOX_PLUS_POSX; // �⺻ ��ġ.
			m_SearchRect.top = m_PetRect.top-m_SearchRect.bottom-HELP_TALKBOX_PLUS_POSY; // �⺻��ġ
			tu1 = 0.0f;
			tv1 = 0.0f;
			tu2 = 1.0f;
			tv2 = 1.0f;

			// �¿� ������� �ϴ��� üũ.
			if( m_SearchRect.left < 0 ) // �Ѿ�ٸ�...
			{
				// ����� �ݴ������� �Ѿ�ٸ� �װ� ��ũ��Ʈ �Է��� ������.
				m_SearchRect.left = m_PetRect.left+m_PetRect.right+HELP_TALKBOX_PLUS_POSX;

				tu1 = 1.0f;
				tu2 = 0.0f;
			}

			// ���� ������� �ϴ��� üũ.
			if( m_SearchRect.top < 0 ) // �Ѿ�ٸ�...
			{
				// ����� �ݴ������� �Ѿ�ٸ� �װ� ��ũ��Ʈ �Է��� ������.
				m_SearchRect.top = m_PetRect.top+m_PetRect.bottom+HELP_TALKBOX_PLUS_POSY;

				tv1 = 0.995f;
				tv2 = 0.0f;
			}

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

			if( m_Searching == 1 ) // �˻��� �Է� ���.
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

				if( tv1 > 0.0f ) // ���ϰ� ������ ���� �ణ �� ������ ���ش�.
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

				if( tv1 > 0.0f ) // ���ϰ� ������ ���� �ణ �� ������ ���ش�.
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
				m_SearchInput.Draw(); // ��ǥ ����� �־�����.

				m_SearchBtn.PosX = m_SearchRect.left+m_SearchRect.right-50;
				m_SearchBtn.PosY = m_SearchRect.top+TempRect.top;
				m_SearchBtn.Draw();

				m_CloseBtn.PosX = m_SearchRect.left+m_SearchRect.right-25;
				m_CloseBtn.PosY = m_SearchRect.top+TempRect.top-24;
				m_CloseBtn.Draw();
			}
			else if( m_Searching == 2 ) // �˻� ��� ���.
			{
				m_TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left,m_SearchRect.top+m_SearchRect.bottom,0), 1, color, 0, tu1, tv2 );
				m_TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left,m_SearchRect.top,0), 1, color, 0, tu1, tv1 );
				m_TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+m_SearchRect.right,m_SearchRect.top,0), 1, color, 0, tu2, tv1 );
				m_TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_SearchRect.left+m_SearchRect.right,m_SearchRect.top+m_SearchRect.bottom,0), 1, color, 0, tu2, tv2 );

				lpTexture = m_pIndexedTexture->GetSurfaceByIdx( m_InterfaceTexIndex[HELPPET_INTERFACE_TALKBOX] );
				GET_D3DDEVICE()->SetTexture(0, lpTexture );
				GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

				int ReverseY = 0; // ������ ������ ��ġ ������.

				if( tv1 > 0.0f ) // ���ϰ� ������ ���� �ణ �� ������ ���ش�.
				{
//					if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // �� ȭ�� �̻��� ���.
					ReverseY += 34;
//					else
//						ReverseY += 12;
				}

				// �˻� ��� �� ���.
				if( m_TotalResult )
				{
					// IDS_PETHELP_SEARCHRESULT : * �˻� ����Դϴ�. *
					sprintf( strTemp, (char *)G_STRING(IDS_PETHELP_SEARCHRESULT) );
					m_TextOutBox.SetString_withOutLine( strTemp, RGB(150,250,100), RGB(20,20,20) );
					m_TextOutBox.Draw( m_SearchRect.left+15, m_SearchRect.top+ReverseY+15 );

//					int ControlLineY = 30; // ���� ������ ��ư �� ������ �� ǥ�� �� ��ġ��.
					int ControlLineY = HELPPET_SEARCHRESULT_CONTROLPOSY; // ���� ������ ��ư �� ������ �� ǥ�� �� ��ġ��.

					// �˻� ��� ���.
					for( int k = 0 ; k < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++k )
					{
						if( m_NowViewResult+k < m_TotalResult ) // ������ �������� ��츦 ����ؼ�...
						{
							sprintf( strTemp, "%d. %s", m_NowViewResult+k+1, m_SearchResultText.m_Text[m_NowViewResult+k] );

							if( m_NowSelResult-1 == k ) // ���õ� ������
								m_TextOutBox.SetString_withOutLine( strTemp, RGB(50,50,50), RGB(250,250,50) );
							else
								m_TextOutBox.SetString_withOutLine( strTemp, RGB(250,250,250), RGB(100,100,100) );
							m_TextOutBox.Draw( m_SearchRect.left+6, m_SearchRect.top+HELPPET_SEARCHRESULT_SEL_START_Y+(k*HELPPET_SEARCHRESULT_SEL_LINE_HEIGHT)+ReverseY );

//							ControlLineY += 20; // ���μ��� ����. ���� ��Ű��� ����.
						}
					}

//					if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // ���� �������� ǥ���ؾ��ϴ� ���.
					{
//						if( m_NowViewResult > 0 ) // �� �� �������� �ƴ϶��.
						{
							// �� ���� ȭ��ǥ�� ǥ���Ѵ�.
							m_PrevBtn.PosX = m_SearchRect.left+(m_SearchRect.right/2)-63;
							m_PrevBtn.PosY = m_SearchRect.top+ControlLineY+ReverseY;
							m_PrevBtn.Draw();
						}

//						if( m_TotalResult - m_NowViewResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // �ڷ� ǥ���ؾ��� �������� �� �ִ� ���.
						{
							// �� ���� ȭ��ǥ�� ǥ���Ѵ�.
							m_NextBtn.PosX = m_SearchRect.left+(m_SearchRect.right/2)+30;
							m_NextBtn.PosY = m_SearchRect.top+ControlLineY+ReverseY;
							m_NextBtn.Draw();
						}

						// ���� �������� ǥ���Ѵ�.
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
		else if( m_dwTalkStartTick && (timeGetTime()-m_dwTalkStartTick > HELPPET_TALKBOX_VIEWTERM || m_nAnimation == HELPPET_ANI_ANGRY) ) // ã�� �� �ణ �ں��� ��ǳ���� ������ش�. ///�굵��Ȯ��
		{
			// ��ǳ���� ��ġ�� �ǽð����� ������ش�.(ȭ�� ������ ������ ��� ������ �ٲ��ֱ� ������...
			// �⺻�� ���� �»�� ����.
			m_TalksRect.left = m_PetRect.left-m_TalksRect.right-HELP_TALKBOX_PLUS_POSX; // �⺻ ��ġ.
			m_TalksRect.top = m_PetRect.top-m_TalksRect.bottom-HELP_TALKBOX_PLUS_POSY; // �⺻��ġ
			tu1 = 0.0f;
			tv1 = 0.0f;
			tu2 = 1.0f;
			tv2 = 1.0f;

			// �¿� ������� �ϴ��� üũ.
			if( m_TalksRect.left < 0 ) // �Ѿ�ٸ�...
			{
				// ����� �ݴ������� �Ѿ�ٸ� �װ� ��ũ��Ʈ �Է��� ������.
				m_TalksRect.left = m_PetRect.left+m_PetRect.right+HELP_TALKBOX_PLUS_POSX;

				tu1 = 1.0f;
				tu2 = 0.0f;
			}

			// ���� ������� �ϴ��� üũ.
			if( m_TalksRect.top < 0 ) // �Ѿ�ٸ�...
			{
				// ����� �ݴ������� �Ѿ�ٸ� �װ� ��ũ��Ʈ �Է��� ������.
				m_TalksRect.top = m_PetRect.top+m_PetRect.bottom+HELP_TALKBOX_PLUS_POSY;

				tv1 = 0.995f;
				tv2 = 0.0f;
			}

			if( m_pTextSurface )
			{
				m_pTextSurface->Xpos = m_TalksRect.left+(15);
				m_pTextSurface->Ypos = m_TalksRect.top+(10);

				if( tv1 > 0.0f ) // ���ϰ� ������ ���� �ణ �� ������ ���ش�.
				{
					// �� ��� �׸� �� �ؽ�Ʈ ��ġ�� ����������Ѵ�.
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

			// �ؽ�Ʈ ���ǽ��� ��ǳ�� ��ġ�� ���缭 ����ش�.
			int tempY = 0;
			if( m_pTextSurface )
			{
				g_pDisplay->Blt( m_pTextSurface->Xpos, m_pTextSurface->Ypos, m_pTextSurface );
				tempY = m_pTextSurface->Ypos-4;
			}

			m_CloseBtn.PosX = m_TalksRect.left+m_TalksRect.right-25;
			m_CloseBtn.PosY = tempY;
			m_CloseBtn.Draw();

			if( timeGetTime()-m_dwTalkStartTick > HELPPET_TALKBOX_CLOSETIME ) // �����ð��̻� ���������..
			{
				TerminateTalkBox();
			}
		}

		GET_D3DDEVICE()->EndScene();
	}

	// ���󺹱�.
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
	BOOL bInputEnable = FALSE; // �Է�â ��Ŀ�� ��ȯ üũ�� ///�Ϻ������ݰ����׼���

	if( m_SearchInput.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() ) // ä���Է�â�� ��Ŀ���� ���ִٸ�... ///�Ϻ������ݰ����׼���
	{
		m_SearchInput.SetEnable( FALSE ); // ������ ��Ŀ���� Ǯ�������.(ó���� �ָ��ؼ� �̷��� �����ϰ� ��)
		pCMyApp->ConvertImmStatus(); // �ؽ�Ʈ �Է� ��Ŀ���� ���� �̰� ������ϴ� ��. ///�Ϻ������ݰ����׼���
	}

	switch (msg)
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( IsInsidePet(x, y) ) // ���� Ŭ���� ���.
		{
			if( m_nAnimation != HELPPET_ANI_THROW )
			{
				m_ClickX = x; // Ŭ���� ��ġ�� ���.
				m_ClickY = y;
			}

			m_bPetDrag = TRUE;
			m_dwPetDragTick = timeGetTime();

			m_DragDistX = m_PetRect.left - x;
			m_DragDistY = m_PetRect.top - y;


			return 1;
		}

		if( m_Searching || m_dwTalkStartTick ) // �� ǳ�� ���̴� ����.
		{
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			if( IsInsideSearchBox(x,y) )
			{
				if( m_Searching == 1 )
				{
					m_SearchBtn.MsgProc(hWnd, msg, wParam, lParam);

					bInputEnable = m_SearchInput.GetEnable(); // ����صξ��ٰ�... ///�Ϻ������ݰ����׼���

					m_SearchInput.MsgProc( hWnd, msg, wParam, lParam ); // ��Ŀ���� ���� �Ѱ� ���ؼ�...

					if( bInputEnable == FALSE && m_SearchInput.GetEnable() ) // �����ִ� ���¿��� �����ٸ�... ///�Ϻ������ݰ����׼���
						pCMyApp->RestoreImmStatus(); // �̰� ���ָ� �Ϻ����� ���� ���� �ݰ� ���װ� ������� ������ �ʹ�.
				}
				else if( m_Searching == 2 )
				{
					m_PrevBtn.MsgProc(hWnd, msg, wParam, lParam);
					m_NextBtn.MsgProc(hWnd, msg, wParam, lParam);
				}

				return 1; // �� ǳ�� ���� Ŭ���Ѱ� �� �޼��� �����ϵ���...
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

		// ��� ���� �巡�� �÷��״� Ǯ���ش�.

		if( IsInsidePet(x, y) && m_dwPetDragTick && timeGetTime() - m_dwPetDragTick < 300 ) // �ݹ� ���Ÿ� Ŭ������ ����.
		{
			if( m_nAnimation == HELPPET_ANI_THROW ) // ���� ������ �߿� Ŭ���̶��..
			{
				//�굵��Ȯ��
				if( timeGetTime() - m_dwPetAniChageTick < 500 ) // �ణ ����� Ÿ���� �ش�. // �� �ð��ȿ� Ŭ���ϸ� �ѹ��� ƨ��� �ְ� ���ش�.
				{
					m_ClickX = m_PetRect.left+(m_PetRect.right/2) - x; // ������ ���� Ŭ���� ��ġ�� �ݴ��...
					m_ClickX *= 20; // �̵� �ӵ��� ���ؼ� ���� ó��
					m_ClickY = m_PetRect.top+(m_PetRect.bottom/2) - y; // ������ ���� Ŭ���� ��ġ�� �ݴ��...
					m_ClickY *= 20; // �̵� �ӵ��� ���ؼ� ���� ó��
					m_ThrowPower = 500; // �� ���� ƨ����ְ� ���ش�.
				}
			}
			else // ���� ������ ó��.
			{
				float Power = DistPlToPlAbs( (float)m_ClickX, (float)m_ClickY, (float)x, (float)y ); // ������ �Ÿ��� ���� ����Ѵ�.

				if( Power >= 30 ) // �����Ÿ� �̻� ������ �������� �� ��츸 ������� ����.
				{
					m_ClickX = x - m_ClickX; // ���� �������� ���ȴ�.
					m_ClickY = y - m_ClickY;

//					m_ThrowPower = Power * 10; // ���� �Ÿ�.
//					m_ThrowPower = (timeGetTime() - m_dwPetDragTick) * 10; // ���� �ð��� ���.
					m_ThrowPower = (Power*2) + (( 300-(timeGetTime() - m_dwPetDragTick) )*2); // ���� �Ÿ��� ���. ���� �ð��� �ݺ��.

					SetPetAnimation( HELPPET_ANI_THROW ); // ������ ���·�.
				}
				else if( m_nAnimation != HELPPET_ANI_THROW )
				{
					// �������� �ö󰡰� ����� ó���� ������.
					m_PetRect.top -= 40.0f;

					// �¿�ε� �ణ ������.
					m_PetRect.left += RandomNum( -35.0f, 35.0f );

					// ȭ�� �Ѿ�� ����.
					if( m_PetRect.left < 0 )
						m_PetRect.left = 0;
					if( m_PetRect.top < 0 )
						m_PetRect.top = 0;
					if( m_PetRect.left+m_PetRect.right > m_ClientRc.right )
						m_PetRect.left = m_ClientRc.right - m_PetRect.right;
					if( m_PetRect.top+m_PetRect.bottom > m_ClientRc.bottom )
						m_PetRect.top = m_ClientRc.bottom - m_PetRect.bottom;

					m_nDir ^= 1; // ���⵵ ���. ��¦ �������.
				}
			}

			m_bPetDrag = FALSE; // ���϶����� ���⼭�� �ѹ���.
			m_dwPetDragTick = 0;

			return 1;
		}

		m_bPetDrag = FALSE;
		m_dwPetDragTick = 0;

		if( m_Searching || m_dwTalkStartTick ) // �� ǳ�� ���̴� ����.
		{
			m_NowSelResult = 0; // �ʱ�ȭ.

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_CloseBtn.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				TerminateTalkBox();
				m_Searching = 0;

				m_CloseBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.

				return 1;
			}

			if( (m_NowSelResult = IsInsideSearchBox(x,y)) )
			{
				if( m_Searching == 1 )
				{
					m_SearchBtn.MsgProc(hWnd, msg, wParam, lParam);

					if( m_SearchBtn.GetState()==BTN_ACTION )
					{
						// �˻� ����.
						HelpSearch( m_SearchInput.GetStrBuf() );

						m_SearchBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.

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

						// ��ǳ�� ������ ����.
						CMultiText TempMultiText; // CAlphaPopUp�� PutString �� �� Ŭ������ �Լ��� �־ ���̴� ������ ��ȯ�ϱ� ��.
						int MinWidth = 0; // �ּ� ������ �����.
						CAlphaPopUp TempPopup; // ������ ����.
						TempPopup.Init(16); // ��Ʈ ������. m_TextOutBox �� ���� ��Ʈ ũ�⿩���Ѵ�.
						TempPopup.LoadRes(m_ClientRc.right, m_ClientRc.bottom );

						// IDS_PETHELP_SEARCHRESULT : * �˻� ����Դϴ�. *
						TempMultiText.AddString( (char *)G_STRING(IDS_PETHELP_SEARCHRESULT) );
						TempPopup.PutString( &TempMultiText );
						MinWidth = TempPopup.GetSurWidth();
						TempMultiText.Reset();

						for( int i = 0 ; i < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++i )
						{
							if( m_NowViewResult+i < m_TotalResult )
								TempMultiText.AddString( m_SearchResultText.m_Text[m_NowViewResult+i] ); // ó�� ȭ�鿡 �ش��ϴ� �ٵ��� �μ�Ʈ.
						}

						TempPopup.PutString( &TempMultiText );

						if( TempPopup.GetSurWidth() < MinWidth ) // �˻� ��� �Դϴ� ���ں��� ª�ٸ� �˻� ����Դϴ�. ���� ���̿� �����ش�.
							m_SearchRect.right = MinWidth+HELPPET_SEARCHRESULT_WIDTHSPACE; // ���� �� ��Ʈ���� �������� ����ش�. ���� �� Ŭ���� ��ư�� ���.
						else
							m_SearchRect.right = TempPopup.GetSurWidth()+HELPPET_SEARCHRESULT_WIDTHSPACE; // ���� �� ��Ʈ���� �������� ����ش�. ���� �� Ŭ���� ��ư�� ���.

						m_SearchRect.bottom = HELPPET_SEARCHRESULT_HEIGHT; // �׳� ���� ũ��� ����.
						/*
												if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // �� ȭ�� �̻��� ���.
													m_SearchRect.bottom = (HELPPET_SEARCHRESULT_VIEWINPAGE+1) * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE + 40; // ��ư ũ�⵵ ���.
												else // �� ȭ���� ���.
													m_SearchRect.bottom = m_TotalResult * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE; // �������� ����.
						*/

						m_PrevBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
						return 1;
					}

					m_NextBtn.MsgProc(hWnd, msg, wParam, lParam);
					if( m_NextBtn.GetState()==BTN_ACTION )
					{
						if( m_NowViewResult < m_TotalResult-HELPPET_SEARCHRESULT_VIEWINPAGE )
							m_NowViewResult += HELPPET_SEARCHRESULT_VIEWINPAGE;

						if( m_NowViewResult > m_TotalResult )
							m_NowViewResult = m_TotalResult;

						// ��ǳ�� ������ ����.
						CMultiText TempMultiText; // CAlphaPopUp�� PutString �� �� Ŭ������ �Լ��� �־ ���̴� ������ ��ȯ�ϱ� ��.
						int MinWidth = 0; // �ּ� ������ �����.
						CAlphaPopUp TempPopup; // ������ ����.
						TempPopup.Init(16); // ��Ʈ ������. m_TextOutBox �� ���� ��Ʈ ũ�⿩���Ѵ�.
						TempPopup.LoadRes( m_ClientRc.right, m_ClientRc.bottom );

						// IDS_PETHELP_SEARCHRESULT : * �˻� ����Դϴ�. *
						TempMultiText.AddString( (char *)G_STRING(IDS_PETHELP_SEARCHRESULT) );
						TempPopup.PutString( &TempMultiText );
						MinWidth = TempPopup.GetSurWidth();
						TempMultiText.Reset();

						for( int i = 0 ; i < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++i )
						{
							if( m_NowViewResult+i < m_TotalResult )
								TempMultiText.AddString( m_SearchResultText.m_Text[m_NowViewResult+i] ); // ó�� ȭ�鿡 �ش��ϴ� �ٵ��� �μ�Ʈ.
						}

						TempPopup.PutString( &TempMultiText );

						if( TempPopup.GetSurWidth() < MinWidth ) // �˻� ��� �Դϴ� ���ں��� ª�ٸ� �˻� ����Դϴ�. ���� ���̿� �����ش�.
							m_SearchRect.right = MinWidth+HELPPET_SEARCHRESULT_WIDTHSPACE; // ���� �� ��Ʈ���� �������� ����ش�. ���� �� Ŭ���� ��ư�� ���.
						else
							m_SearchRect.right = TempPopup.GetSurWidth()+HELPPET_SEARCHRESULT_WIDTHSPACE; // ���� �� ��Ʈ���� �������� ����ش�. ���� �� Ŭ���� ��ư�� ���.

						m_SearchRect.bottom = HELPPET_SEARCHRESULT_HEIGHT; // �׳� ���� ũ��� ����.
						/*
												if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // �� ȭ�� �̻��� ���.
													m_SearchRect.bottom = (HELPPET_SEARCHRESULT_VIEWINPAGE+1) * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE + 40; // ��ư ũ�⵵ ���.
												else // �� ȭ���� ���.
													m_SearchRect.bottom = m_TotalResult * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE; // �������� ����.
						*/

						m_NextBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.

						return 1;
					}

					// �������� Ŭ������ ��� ó��.
					if( m_NowSelResult > 0 ) // ������ ���� �ƴ� ��� -1 �̳� 0�� �ü��ִ�.
					{
						int k = m_NowViewResult+(m_NowSelResult-1);

						if( k < m_TotalResult ) // ���� �����϶���.
							ExcuteHelpContents( m_SearchResultIndex[k] ); // ������ �����ش�.
					}
				}

				return 1; // �� ǳ�� ���� Ŭ���Ѱ� �� �޼��� �����ϵ���...
			}
		}

		if( m_bContents )
		{
			m_bContentsDrag = FALSE;

			m_PrevBtn2.MsgProc(hWnd, msg, wParam, lParam);
			if( m_PrevBtn2.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				--m_nNowContentsPage;

				if( m_nNowContentsPage < 0 )
				{
					m_nNowContentsPage = 0;
				}

				m_PrevBtn2.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
				return 1;
			}

			m_NextBtn2.MsgProc(hWnd, msg, wParam, lParam);
			if( m_NextBtn2.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				++m_nNowContentsPage;

				if( m_nNowContentsPage > m_ListContentsSurface.size()-1 )
				{
					m_nNowContentsPage = m_ListContentsSurface.size()-1;
				}

				m_NextBtn2.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
				return 1;
			}

			m_CloseBtn2.MsgProc(hWnd, msg, wParam, lParam);
			if( m_CloseBtn2.GetState()==BTN_ACTION ) // �����ݱ� ��ư Ŭ����
			{
				TerminateContents();

				m_CloseBtn2.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.

				return 1;
			}
		}

		break;

	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_Searching || m_dwTalkStartTick ) // �� ǳ�� ���̴� ����.
		{
			m_NowSelResult = 0; // �ʱ�ȭ.

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

				// ȭ�� �Ѿ�� ����.
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

		if( m_bPetDrag && !m_ThrowPower ) // �巡�� ���̸�...
		{
			m_PetRect.left = x + m_DragDistX;
			m_PetRect.top = y + m_DragDistY;

			// ȭ�� �Ѿ�� ����.
			if( m_PetRect.left < 0 )
				m_PetRect.left = 0;
			if( m_PetRect.top < 0 )
				m_PetRect.top = 0;
			if( m_PetRect.left+m_PetRect.right > m_ClientRc.right )
				m_PetRect.left = m_ClientRc.right - m_PetRect.right;
			if( m_PetRect.top+m_PetRect.bottom > m_ClientRc.bottom )
				m_PetRect.top = m_ClientRc.bottom - m_PetRect.bottom;
		}
		else if( IsInsidePet(x, y) ) // �꿡 ���콺 ������.
		{
		}

		break;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( IsInsidePet(x, y) )
		{
			// ���� ������� ����Ǿ� ����
			if( g_dwClientCountry != CTRY_KOR )
			{
				// �˻� ����� ���ش�.
				m_Searching = 1;
				m_SearchRect.right = 200;
				m_SearchRect.bottom = 70;

				m_SearchInput.SetEnable( TRUE ); // �ٷ� �Է��� ���ֵ��� ����.
				pCMyApp->RestoreImmStatus(); ///�Ϻ������ݰ����׼���
			}

			return 1; // �� �޼��� �����ϵ���...
		}
		else if( m_dwTalkStartTick && timeGetTime()-m_dwTalkStartTick > HELPPET_TALKBOX_VIEWTERM
				 && IsInsideTalkBox(x,y) ) // ã�� �� �ణ �ں��� ��ǳ���� ������ش�.
		{
			TerminateTalkBox();
			return 1;
		}


		break;

	case WM_LBUTTONDBLCLK:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_dwTalkStartTick && IsInsideTalkBox(x, y) ) // ��ĭ�� ��縦 ����Ŭ���� ���.
		{
			// �� �������� �����ش�.

			if( ExcuteHelpContents(m_TalkIndex) == FALSE && m_TalkIndex != -1 )
			{
				// ������ �������� ���� ���.
				// IDS_PETHELP_NOWREADY : �� ������ ���� �غ� ���̾�.
				ExcuteHelpTalk( (char *)G_STRING(IDS_PETHELP_NOWREADY) );
				return 1; // �ؿ� TerminateTalkBox ���ϱ� ���� ĵ��.
			}

			// ���� Ŭ�������� ���� ���� ���ش�.
			TerminateTalkBox();

			return 1;
		}
//		else if( IsInsidePet(x, y) ) // �˻� ��� Ȱ��ȭ.
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
			if( m_Searching == 1 && m_SearchInput.GetEnable() ) // ã�� �Է��� ���͸�...
			{
				// �˻� ����.
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
		Result = -1; // �ڽ� �ȿ��� ����.

		// ������ ���� ���� Ȯ��.
		// ���� ������� �ϴ��� üũ.
		int ReverseY = 0;
		if( m_SearchRect.top < 0 ) // �Ѿ�ٸ�...
		{
			ReverseY += 34;
		}

		int NowCheckY = 0;

		for( int k = 0 ; k < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++k )
		{
			NowCheckY = m_SearchRect.top+HELPPET_SEARCHRESULT_SEL_START_Y+(k*HELPPET_SEARCHRESULT_SEL_LINE_HEIGHT)+ReverseY; // �̹��� üũ�� �������� y ��ġ.

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


int CControlHelpPet::CheckHelpCondition( int ConType, int Arg, BOOL bForced ) // ��ϵ� �������� ������ ���鼭 �����ϴ� �༮�� �ִ��� üũ�Ѵ�.
{
	// ĵ�� ���ǵ� ���� üũ.
	if( !bForced ) // ���� üũ ���ǵ� �ִ�.
	{
		if( m_nAnimation == HELPPET_ANI_FIND || timeGetTime()-m_dwOldConCheckTick < 60000 ) // ���ø� �˻�. ���� �ֱ�θ� �˻�.
			return -1;
	}
	else if( ConType != HELP_CON_TYPE_F_SERVER ) // ���� üũ��� �ص� ǥ�� �߿� �ȵ���. (��������� ����)
	{
		if( m_dwTalkStartTick && timeGetTime()-m_dwTalkStartTick <= HELPPET_TALKBOX_CLOSETIME ) // ��ǳ���� ǥ�����̸�...
			return -1;
	}

	int i = 0;
	int len = 0;
	int TempInt1 = 0;
	int TempInt2 = 0;
	char *arg = NULL; // ���� ������.
	char strTemp2[50];

	sHelpData *TempHelp;
	list<sHelpData*>::iterator prHelp;
	for( prHelp = m_HelpManager.m_listHelp.begin(); prHelp != m_HelpManager.m_listHelp.end(); ++prHelp )
	{
		TempHelp = *prHelp;

		// ���� ���� üũ.
		switch( TempHelp->m_ExceptionCode )
		{
		case HELP_EXCEP_CODE_ONLY_ACHERON: // ���ɷп����� ����.

			if (g_SvrType != ST_ADULT_ONLY) // ���ɷ��� �ƴϸ�.
				continue; // ��ŵ.
			break;
		}


		// ��Ÿ�� üũ.
		if( TempHelp->m_ActivateTick && TempHelp->m_CoolTime > 0 && ConType != HELP_CON_TYPE_F_SERVER ) // �������� ������ ���� ���� �̹Ƿ� ��Ÿ�� üũ���ϰ� ���� 07-09-03
		{
			if( timeGetTime()-TempHelp->m_ActivateTick < (TempHelp->m_CoolTime*1000) ) // ���� ��Ÿ���� �ȳ������.
			{
				continue; // ��ŵ
			}
			else // ��Ÿ���� �������.
			{
				TempHelp->m_ActivateTick = 0; // �ʱ�ȭ ���ش�.
			}
		}

		if( ConType == HELP_CON_TYPE_F_SERVER ) // ���������� ���� ����(���� Ÿ�԰� ������� ����.)
		{
			if( Arg == TempHelp->m_Index ) // �����κ��� ���� �ε����� ����´�.
			{
				ExcuteHelpTalk( TempHelp );
				return TempHelp->m_Index; // �ε����� �������ش�. ���� ã�� �༮�� �켱�� �Ѵ�. ��, �켱 ������ ���̷��� ���� �÷���..
			}
		}
		else if( TempHelp->m_ConditionType == ConType ) // ���� ������ ���� �༮�̸�...
		{
			// ���� üũ.
			switch( TempHelp->m_ConditionType )
			{
			case HELP_CON_TYPE_F_LEVELUP: // ���� ���� ����.
				if( atoi(TempHelp->m_strConArg) == Arg ) // ���ڸ� �ܼ� ���� �ϳ��� �޴´�.
				{
					ExcuteHelpTalk( TempHelp );
					return TempHelp->m_Index; // �ε����� �������ش�. ���� ã�� �༮�� �켱�� �Ѵ�. ��, �켱 ������ ���̷��� ���� �÷���..
				}
				break;
			case HELP_CON_TYPE_A_LEVEL: // ������ ���� üũ ����.

				// �� �༮�� ���ڰ� �ΰ��ӿ� ����.
				sscanf( TempHelp->m_strConArg, "%d %d", &TempInt1, &TempInt2 );

				if( g_pRoh->m_Level >= TempInt1 && g_pRoh->m_Level <= TempInt2 )
				{
					ExcuteHelpTalk( TempHelp );
					return TempHelp->m_Index; // �ε����� �������ش�. ���� ã�� �༮�� �켱�� �Ѵ�. ��, �켱 ������ ���̷��� ���� �÷���..
				}

				break;
			case HELP_CON_TYPE_F_ITEM:

				if( !TempHelp->m_strConArg )
					break;

				arg = AnyOneArg( TempHelp->m_strConArg, strTemp2 ); // ����
				TempInt1 = atoi( strTemp2 );

				if( TempInt1 > 100 ) // �ʹ� ũ�� ���� �߸� �Էµ� ������ ����.
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
						return TempHelp->m_Index; // �ε����� �������ش�. ���� ã�� �༮�� �켱�� �Ѵ�. ��, �켱 ������ ���̷��� ���� �÷���..
					}
				}


				break;
			/*          // �������� �����Ű�°� �ش� Ÿ�Ը� �Ǵ°� �ƴ϶� �� Ÿ���� �����ϵ��� �����ߴ�. ��, ���� Ÿ�� ������ ���ٸ� Ŭ���̾�Ʈ üũ ������ ���� ����ϴµ��� ���̰� ���ڵ� ���õȴ�.
						case HELP_CON_TYPE_F_SERVER:
							if( atoi(TempHelp->m_strConArg) == Arg ) // ���ڸ� �ܼ� ���� �ϳ��� �޴´�.
							{
								ExcuteHelpTalk( TempHelp );
								return TempHelp->m_Index; // �ε����� �������ش�. ���� ã�� �༮�� �켱�� �Ѵ�. ��, �켱 ������ ���̷��� ���� �÷���..
							}
							break;
			*/
			case HELP_CON_TYPE_F_NPC_IN:

				if( atoi(TempHelp->m_strConArg) == Arg ) // ���ڸ� �ܼ� ���� �ϳ��� �޴´�.
				{
					ExcuteHelpTalk( TempHelp );
					return TempHelp->m_Index; // �ε����� �������ش�. ���� ã�� �༮�� �켱�� �Ѵ�. ��, �켱 ������ ���̷��� ���� �÷���..
				}
				break;
			}

		}
	}


	m_dwOldConCheckTick = timeGetTime();

	return -1;
}

BOOL CControlHelpPet::ExcuteHelpTalk( int HelpIndex ) // ��縦 �ߵ� ��Ų��.
{
	sHelpData *TempHelp = NULL;
	list<sHelpData*>::iterator prHelp;
	for( prHelp = m_HelpManager.m_listHelp.begin(); prHelp != m_HelpManager.m_listHelp.end(); ++prHelp )
	{
		if( (*prHelp)->m_Index == HelpIndex ) // ���� �༮�̸�...
		{
			TempHelp = *prHelp;

			break;
		}
	}

	if( !TempHelp )
		return FALSE;

	return ExcuteHelpTalk( TempHelp );
}

BOOL CControlHelpPet::ExcuteHelpTalk( sHelpData *TempHelp ) // ��縦 �ߵ� ��Ų��.
{
	if( !TempHelp )
		return FALSE;

	m_Searching = 0; // �˻� ���̾����� �˻��� ������ش�.
	
	m_TalksText.Reset(); // �ʱ�ȭ�� �־��ش�.

	if( g_pRoh->m_Chaotic_Grade ) // �̰� 0�� �ƴϸ� ī�����´�.
	{
		if( TempHelp->m_strChaoticTalk && strlen(TempHelp->m_strChaoticTalk) )
		{
			int PrevStringPoint = 0; // ������ ��Ʈ���� ������ ��ġ.
			int StrReadPt = 0;
			char *strTemp = NULL;

			int ContantsTotalLen = strlen(TempHelp->m_strChaoticTalk);

			while( StrReadPt < ContantsTotalLen ) // ��ü ��Ʈ���� ���鼭...
			{
				if( TempHelp->m_strChaoticTalk[StrReadPt] == '\\' && TempHelp->m_strChaoticTalk[StrReadPt+1] == 'n' ) // ���� ���ڰ� ���� ��쿡 �� ������ �����.
				{
					int Len = StrReadPt-PrevStringPoint;
					SAFE_DELETE_ARRAY( strTemp );
					strTemp = new char[Len+1];

					if( strTemp )
					{
						strncpy( strTemp, &TempHelp->m_strChaoticTalk[PrevStringPoint], Len );
						strTemp[Len] = '\0';

						m_TalksText.AddString( strTemp );

						StrReadPt += 2; // �̰� ���� ���ںе� �ѱ��.

						PrevStringPoint = StrReadPt; // ������ ���� ��ġ�� ����صд�.

						continue; // �ѹ� �����ش�.
					}
				}

				if( StrReadPt >= ContantsTotalLen ) // ������ ���ϰ�� �� ó��.
				{
					int Len = ContantsTotalLen-PrevStringPoint;
					SAFE_DELETE_ARRAY( strTemp );
					strTemp = new char[Len+1];

					if( strTemp )
					{
						strncpy( strTemp, &TempHelp->m_strChaoticTalk[PrevStringPoint], Len );
						strTemp[Len] = '\0';

						m_TalksText.AddString( strTemp );

						PrevStringPoint = ContantsTotalLen; // ������ ���� ��ġ�� ����صд�.

						continue; // �ѹ� �����ش�.
					}
				}
			}
		}
	}
	else // �Ϲ� ����.
	{
		if( TempHelp->m_strNormalTalk && strlen(TempHelp->m_strNormalTalk) )
		{
			int PrevStringPoint = 0; // ������ ��Ʈ���� ������ ��ġ.
			int StrReadPt = 0;
			char *strTemp = NULL;

			int ContantsTotalLen = strlen(TempHelp->m_strNormalTalk);

			while( StrReadPt < ContantsTotalLen ) // ��ü ��Ʈ���� ���鼭...
			{
				if( TempHelp->m_strNormalTalk[StrReadPt] == '\\' && TempHelp->m_strNormalTalk[StrReadPt+1] == 'n' ) // ���� ���ڰ� ���� ��쿡 �� ������ �����.
				{
					int Len = StrReadPt-PrevStringPoint;
					SAFE_DELETE_ARRAY( strTemp );
					strTemp = new char[Len+1];

					if( strTemp )
					{
						strncpy( strTemp, &TempHelp->m_strNormalTalk[PrevStringPoint], Len );
						strTemp[Len] = '\0';

						m_TalksText.AddString( strTemp );

						StrReadPt += 2; // �̰� ���� ���ںе� �ѱ��.

						PrevStringPoint = StrReadPt; // ������ ���� ��ġ�� ����صд�.

						continue; // �ѹ� �����ش�.
					}
				}

				// ������ �ƴϸ� ����ó��.
				if( TempHelp->m_strNormalTalk[StrReadPt] < 0 || TempHelp->m_strNormalTalk[StrReadPt] > 127 ) // �켱 ���� ���ڰ� 2����Ʈ �ڵ������� Ȯ��
				{
					// 2 ����Ʈ ������ ���.
					StrReadPt += 2; // 2 ����Ʈ�ϱ� ���� ���� ����Ʋ�� 2�������Ѽ� üũ. (Ÿ�̹��� ���⼭ �ϴ� ������ �Ʒ� ���� ���ϱ� ����.)
				}
				else // 1����Ʈ �ڵ�
				{
					StrReadPt += 1;
				}

				if( StrReadPt >= ContantsTotalLen ) // ������ ���ϰ�� �� ó��.
				{
					int Len = ContantsTotalLen-PrevStringPoint;
					SAFE_DELETE_ARRAY( strTemp );
					strTemp = new char[Len+1];

					if( strTemp )
					{
						strncpy( strTemp, &TempHelp->m_strNormalTalk[PrevStringPoint], Len );
						strTemp[Len] = '\0';

						m_TalksText.AddString( strTemp );

						PrevStringPoint = ContantsTotalLen; // ������ ���� ��ġ�� ����صд�.

						continue; // �ѹ� �����ش�.
					}
				}
			}
		}
	}

	if( m_TalksText.m_LineCount <= 0 ) // �ؽ�Ʈ�� ���� ���..
		return FALSE;

	// �ʿ��� �簢�� ������ ���.
	int GreatestCount=0; // ����ū ���ڿ��� ������ ��´�
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

	// ����ū ���ڿ��� ������ ��´�
	ZeroMemory( &m_TalksRect, sizeof(RECT) );
	for(int i = 0; i < m_TalksText.m_LineCount; i++)
	{
		// �Է¹��� �ؽ�Ʈ�� ������ ���鼭 ����� ���ڿ� üũ
		int len = strlen(m_TalksText.m_Text[i]);
		// ���ڿ��� ���� �����Ǿ��°�?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// ��Ʈ ������ ��ȯ
			GetTextExtentPoint32(hdc, m_TalksText.m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextWidth = size.cx;
		};
	}

	if (g_bFixedWidthFont)
	{
		// ����� ���ڿ��� ���μ��� ���ؼ� ���ǽ� ����
		TextSurWidth = GreatestCount*FontWidth;
		TextSurHeight = m_TalksText.m_LineCount*HELP_USE_LINE_HEIGHT;
	}
	else
	{
		TextSurWidth = TextWidth;
		TextSurHeight = m_TalksText.m_LineCount*HELP_USE_LINE_HEIGHT;

	}

	SAFE_DELETE(m_pTextSurface);

	// ���� ũ��� ���ǽ� ����
	g_pDisplay->CreateSurface( &m_pTextSurface, TextSurWidth, TextSurHeight );
	if( !m_pTextSurface )
		return FALSE;

	m_pTextSurface->SetWidth( TextSurWidth ); // �̰� �� �����Ǿ�� �Ѵ�.
	m_pTextSurface->SetHeight( TextSurHeight );


	m_TalksRect.right = TextSurWidth+HELP_TALKBOX_XPLUS_SIZE+20; // ����� ����صд�. ��ǳ�� ������ ����ؼ� ���� �˳��ϰ� �Ѵ�.
	m_TalksRect.bottom = TextSurHeight+HELP_TALKBOX_YPLUS_SIZE;

	// fillcolor�� �̿��� textsurface Ŭ�����ϱ�
	m_pTextSurface->Clear();

	// ���� ���ڿ� ����� �����ؼ� DrawText
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

		//-- Lyul ���� -- Shadow : 1
//		m_pTextSurface->DrawText(m_Font, m_TalksText.m_Text[i], Xpos, Ypos,
//			m_TalksText.m_TextBackColor[i], m_TalksText.m_TextColor[i], 1 ,RGB(10, 10, 10));
		//-- Lyul ���� -- Normal : 0
		m_pTextSurface->DrawText( m_Font, m_TalksText.m_Text[i], Xpos, Ypos,
								  m_TalksText.m_TextBackColor[i], m_TalksText.m_TextColor[i], 0);

		Ypos += HELP_USE_LINE_HEIGHT;
	}
	m_pTextSurface->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	SetPetAnimation( HELPPET_ANI_FIND ); // ã�Ҵ� �ִϸ��̼����� �ٲ��ش�.

	// ���������� ��Ÿ�� ������ ���ش�.
	TempHelp->m_ActivateTick = m_dwTalkStartTick = timeGetTime(); // ��ǳ�� ���� ƽ�� ����.

	m_TalkIndex = TempHelp->m_Index;

	return TRUE;
}

BOOL CControlHelpPet::ExcuteHelpTalk( const char *strTalk ) // ���� ��縦 �Է��ϴ� ���...
{
	m_Searching = 0; // �˻� ���̾����� �˻��� ������ش�.

	m_TalksText.Reset(); // �ʱ�ȭ�� �־��ش�.

	if( strTalk )
	{
		int PrevStringPoint = 0; // ������ ��Ʈ���� ������ ��ġ.
		int StrReadPt = 0;
		char *strTemp = NULL;

		int ContantsTotalLen = strlen(strTalk);

		while( StrReadPt < ContantsTotalLen ) // ��ü ��Ʈ���� ���鼭...
		{
			if( strTalk[StrReadPt] == '\\' && strTalk[StrReadPt+1] == 'n' ) // ���� ���ڰ� ���� ��쿡 �� ������ �����.
			{
				int Len = StrReadPt-PrevStringPoint;
				SAFE_DELETE_ARRAY( strTemp );
				strTemp = new char[Len+1];

				if( strTemp )
				{
					strncpy( strTemp, &strTalk[PrevStringPoint], Len );
					strTemp[Len] = '\0';

					m_TalksText.AddString( strTemp );

					StrReadPt += 2; // �̰� ���� ���ںе� �ѱ��.

					PrevStringPoint = StrReadPt; // ������ ���� ��ġ�� ����صд�.

					continue; // �ѹ� �����ش�.
				}
			}

			// ������ �ƴϸ� ����ó��.
			if( strTalk[StrReadPt] < 0 || strTalk[StrReadPt] > 127 ) // �켱 ���� ���ڰ� 2����Ʈ �ڵ������� Ȯ��
			{
				// 2 ����Ʈ ������ ���.
				StrReadPt += 2; // 2 ����Ʈ�ϱ� ���� ���� ����Ʋ�� 2�������Ѽ� üũ. (Ÿ�̹��� ���⼭ �ϴ� ������ �Ʒ� ���� ���ϱ� ����.)
			}
			else // 1����Ʈ �ڵ�
			{
				StrReadPt += 1;
			}


			if( StrReadPt >= ContantsTotalLen ) // ������ ���ϰ�� �� ó��.
			{
				int Len = ContantsTotalLen-PrevStringPoint;
				SAFE_DELETE_ARRAY( strTemp );
				strTemp = new char[Len+1];

				if( strTemp )
				{
					strncpy( strTemp, &strTalk[PrevStringPoint], Len );
					strTemp[Len] = '\0';

					m_TalksText.AddString( strTemp );

					PrevStringPoint = ContantsTotalLen; // ������ ���� ��ġ�� ����صд�.

					continue; // �ѹ� �����ش�.
				}
			}
		}
	}

	// �ʿ��� �簢�� ������ ���.
	int GreatestCount=0; // ����ū ���ڿ��� ������ ��´�
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

	// ����ū ���ڿ��� ������ ��´�
	ZeroMemory( &m_TalksRect, sizeof(RECT) );
	for(int i = 0; i < m_TalksText.m_LineCount; i++)
	{
		// �Է¹��� �ؽ�Ʈ�� ������ ���鼭 ����� ���ڿ� üũ
		int len = strlen(m_TalksText.m_Text[i]);
		// ���ڿ��� ���� �����Ǿ��°�?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// ��Ʈ ������ ��ȯ
			GetTextExtentPoint32(hdc, m_TalksText.m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextWidth = size.cx;
		};
	}

	if (g_bFixedWidthFont)
	{
		// ����� ���ڿ��� ���μ��� ���ؼ� ���ǽ� ����
		TextSurWidth = GreatestCount*FontWidth;
		TextSurHeight = m_TalksText.m_LineCount*HELP_USE_LINE_HEIGHT;
	}
	else
	{
		TextSurWidth = TextWidth;
		TextSurHeight = m_TalksText.m_LineCount*HELP_USE_LINE_HEIGHT;

	}

	SAFE_DELETE(m_pTextSurface);

	// ���� ũ��� ���ǽ� ����
	g_pDisplay->CreateSurface( &m_pTextSurface, TextSurWidth, TextSurHeight );
	if( !m_pTextSurface )
		return FALSE;

	m_TalksRect.right = TextSurWidth+HELP_TALKBOX_XPLUS_SIZE + 20; // ����� ����صд�. ��ǳ�� ������ ����ؼ� ���� �˳��ϰ� �Ѵ�.
	m_TalksRect.bottom = TextSurHeight+HELP_TALKBOX_YPLUS_SIZE;

	// fillcolor�� �̿��� textsurface Ŭ�����ϱ�
	m_pTextSurface->Clear();

	// ���� ���ڿ� ����� �����ؼ� DrawText
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

		//-- Lyul ���� -- Shadow : 1
//		m_pTextSurface->DrawText(m_Font, m_TalksText.m_Text[i], Xpos, Ypos,
//			m_TalksText.m_TextBackColor[i], m_TalksText.m_TextColor[i], 1 ,RGB(10, 10, 10));
		//-- Lyul ���� -- Normal : 0
		m_pTextSurface->DrawText( m_Font, m_TalksText.m_Text[i], Xpos, Ypos,
								  m_TalksText.m_TextBackColor[i], m_TalksText.m_TextColor[i], 0);

		Ypos += HELP_USE_LINE_HEIGHT;
	}
	m_pTextSurface->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

//	SetPetState( HELPPET_STATE_FIND ); // ã�Ҵ� �������ͽ��� �ٲ��ش�.
	SetPetAnimation( HELPPET_ANI_FIND ); // ã�Ҵ� �ִϸ��̼����� �ٲ��ش�.

	// ���������� ��Ÿ�� ������ ���ش�.
	m_dwTalkStartTick = timeGetTime(); // ��ǳ�� ���� ƽ�� ����.
	m_TalkIndex = -1;

	return TRUE;
}



BOOL CControlHelpPet::ExcuteHelpContents( int HelpIndex ) // �ڼ��� ���� �����ֱ⸦ �ߵ� ��Ų��.
{
	if( HelpIndex < 0 )
		return FALSE;

	TerminateContents(); // �ʱ�ȭ�� ����.
	m_Searching = 0; // �˻� ���̿����� �˻� ���.

	// �ϴ� �ѹ� ����ϰ� �����.
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
		if( TempHelp->m_Index == HelpIndex ) // ���� �༮�̸�...
		{
			if( TempHelp->m_listImage.size() <= 0 ) // �̹����� ����� �ȵ� ����.
				return FALSE;

			// JPG �׸����� �ε��ؼ� ����Ʈ�� �־��ش�.
			m_ListContentsSurface.reserve( TempHelp->m_listImage.size() ); // ���� ���� ����.

			int k = 0;
			for( i = 0 ; i < TempHelp->m_listImage.size() ; ++i )
			{
				sprintf( strTemp, "./interface/HelpPet/Details/%s", TempHelp->m_listImage[k] ); // �������.
				g_pDisplay->CreateSurfaceFromJPG( &TempSurface, strTemp );
				m_ListContentsSurface.push_back( TempSurface ); // ���̸� ���̶� ����?

				++k;
			}

			m_bContents = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

void CControlHelpPet::TerminateContents() // �����ִ� ������ �κ��� �ʱ�ȭ ���ִ� �༮.
{
	m_nNowContentsPage = 0;
	m_bContents = FALSE;
}

void CControlHelpPet::TerminateTalkBox() // ��ǳ���� �����.
{
	ZeroMemory( &m_TalksRect, sizeof(RECT) );
	SAFE_DELETE( m_pTextSurface );

	m_dwTalkStartTick = 0; // ��ǳ���� ���ش�.
	m_TalkIndex = -1;
}

BOOL CControlHelpPet::HelpSearch( char *SearchWord ) // ���� �˻� ���.
{
	m_SearchResultText.Reset();
	m_SearchResultIndex.clear(); // �ϴ� �� �����.

	sHelpData *TempHelp = NULL;
	list<sHelpData*>::iterator prHelp;
	for( prHelp = m_HelpManager.m_listHelp.begin(); prHelp != m_HelpManager.m_listHelp.end(); ++prHelp )
	{
		TempHelp = *prHelp;

		bool bSearchAtThis = FALSE; // �� �����ۿ��� ������ �Ǿ����� ����. ���� �˻�� ��ŵ�ϱ� ���� üũ.
		int ItemNameLen = 0;
		int SearchLen = strlen( SearchWord );
		int j = 0;

		if( TempHelp->m_strSearchTitle && strlen(TempHelp->m_strSearchTitle) ) // �˻��� ������ �������� �˻�����.
		{
			if( TempHelp->m_strSearchWord1 && strlen(TempHelp->m_strSearchWord1) ) // �� 1 ����� üũ.
			{
				ItemNameLen = strlen( TempHelp->m_strSearchWord1 );

				for( j = 0 ; j <= ItemNameLen-SearchLen ; ++j )
				{
					if( strncmp(&TempHelp->m_strSearchWord1[j],SearchWord,SearchLen) == 0 ) // ���� �̸��� ������...
					{
						m_SearchResultText.AddString( TempHelp->m_strSearchTitle );
						m_SearchResultIndex.push_back( TempHelp->m_Index );

						bSearchAtThis = TRUE;

						break; // �� �˻��� üũ�� ������ �ѱ��.
					}
				}
			}

			if( !bSearchAtThis && TempHelp->m_strSearchWord2 && strlen(TempHelp->m_strSearchWord2) ) // �� 1 �˻���� ��ã�� ���. �� 2 �˻�� üũ�Ѵ�.
			{
				ItemNameLen = strlen( TempHelp->m_strSearchWord2 );

				for( j = 0 ; j <= ItemNameLen-SearchLen ; ++j )
				{
					if( strncmp(&TempHelp->m_strSearchWord2[j],SearchWord,SearchLen) == 0 ) // ���� �̸��� ������...
					{
						m_SearchResultText.AddString( TempHelp->m_strSearchTitle );
						m_SearchResultIndex.push_back( TempHelp->m_Index );

						bSearchAtThis = TRUE;

						break; // �� �˻��� üũ�� ������ �ѱ��.
					}
				}
			}

			if( !bSearchAtThis && TempHelp->m_strSearchWord3 && strlen(TempHelp->m_strSearchWord3) ) // �� 1 �˻���� ��ã�� ���. �� 2 �˻�� üũ�Ѵ�.
			{
				ItemNameLen = strlen( TempHelp->m_strSearchWord3 );

				for( j = 0 ; j <= ItemNameLen-SearchLen ; ++j )
				{
					if( strncmp(&TempHelp->m_strSearchWord3[j],SearchWord,SearchLen) == 0 ) // ���� �̸��� ������...
					{
						m_SearchResultText.AddString( TempHelp->m_strSearchTitle );
						m_SearchResultIndex.push_back( TempHelp->m_Index );

						bSearchAtThis = TRUE;

						break; // �� �˻��� üũ�� ������ �ѱ��.
					}
				}
			}
		}
	}

	// ��� ǥ�ÿ� ��ġ�� ����.
	m_Searching = 2; // �˻� ��� ǥ�� ���·� �ٲ۴�.
	m_TotalResult = m_SearchResultText.m_LineCount; // �� ������ ����.
	m_NowViewResult = 0; // �̰� �ʱ�ȭ.

	// ������ ���.
	CMultiText TempMultiText; // CAlphaPopUp�� PutString �� �� Ŭ������ �Լ��� �־ ���̴� ������ ��ȯ�ϱ� ��.
	int MinWidth = 0; // �ּ� ������ �����.
	CAlphaPopUp TempPopup; // ������ ����.
	TempPopup.Init(16); // ��Ʈ ������. m_TextOutBox �� ���� ��Ʈ ũ�⿩���Ѵ�.
	TempPopup.LoadRes( m_ClientRc.right, m_ClientRc.bottom );

	if( !m_SearchResultText.m_LineCount )
	{
		// IDS_PETHELP_SEARCHFAILED : �˻�� �ش��ϴ� ����� �����ϴ�.
		ExcuteHelpTalk( (char *)G_STRING(IDS_PETHELP_SEARCHFAILED) );
		return FALSE;
	}
	else
	{
		// IDS_PETHELP_SEARCHRESULT : * �˻� ����Դϴ�. *
		TempMultiText.AddString( (char *)G_STRING(IDS_PETHELP_SEARCHRESULT) );

		TempPopup.PutString( &TempMultiText );
		MinWidth = TempPopup.GetSurWidth();
		TempMultiText.Reset();

		for( int i = 0 ; i < HELPPET_SEARCHRESULT_VIEWINPAGE ; ++i )
		{
			if( i < m_TotalResult )
				TempMultiText.AddString( m_SearchResultText.m_Text[i] ); // ó�� ȭ�鿡 �ش��ϴ� �ٵ��� �μ�Ʈ.
		}
	}


	TempPopup.PutString( &TempMultiText );

	if( TempPopup.GetSurWidth() < MinWidth ) // �˻� ��� �Դϴ� ���ں��� ª�ٸ� �˻� ����Դϴ�. ���� ���̿� �����ش�.
		m_SearchRect.right = MinWidth+HELPPET_SEARCHRESULT_WIDTHSPACE; // ���� �� ��Ʈ���� �������� ����ش�. ���� �� Ŭ���� ��ư�� ���.
	else
		m_SearchRect.right = TempPopup.GetSurWidth()+HELPPET_SEARCHRESULT_WIDTHSPACE; // ���� �� ��Ʈ���� �������� ����ش�. ���� �� Ŭ���� ��ư�� ���.

	m_SearchRect.bottom = HELPPET_SEARCHRESULT_HEIGHT; // �׳� ���� ũ��� ����.
	/*
		if( m_TotalResult > HELPPET_SEARCHRESULT_VIEWINPAGE ) // �� ȭ�� �̻��� ���.
			m_SearchRect.bottom = (HELPPET_SEARCHRESULT_VIEWINPAGE+1) * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE + 40; // ��ư ũ�⵵ ���.
		else // �� ȭ���� ���.
			m_SearchRect.bottom = m_TotalResult * HELP_USE_LINE_HEIGHT + HELPPET_SEARCHRESULT_HEIGHTSPACE; // �������� ����.
	*/

	if( !m_SearchResultIndex.size() )
	{
		return FALSE; // ã�� ����� ����.
	}


	return TRUE;
}

