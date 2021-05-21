#include "stdafx.h"

#define D3D_OVERLOADS

#include <d3d.h>
#include <assert.h>
#include "main.h"
#include "Frustum.h"
#include "Nk2DFrame.h"
#include "NkMob.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "D3DMath.h"
#include "DirtSnd.h"
#include "Land.h"
#include "ClrAlloc.h"
#include "LoadEffectSound.h"
#include "uimgr.h"

#include "EventScriptActor.h"








void Convert640Pos( RECT *Rect ); // 640*480 해상도 기준의 좌표를 현재 해상도로 컨버팅시켜준다.
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////
// 액션 함수들 선언 R은 렌더링 함수. M은 메시지 프로시져 함수.
void SA_TutorialStart_R( DWORD StartTick );	 // 0 : 튜토리얼 시작 부분의 이벤트 씬
int SA_TutorialStart_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	 // 0 : 튜토리얼 시작 부분의 이벤트 씬

void SA_TutorialMove_R( DWORD StartTick );	 // 1 : 튜토리얼 중 이동 실습.
int SA_TutorialMove_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	 // 0 : 튜토리얼 시작 부분의 이벤트 씬

void SA_TutorialRotate_R( DWORD StartTick );	 // 2 : 튜토리얼 중 회전 실습.
int SA_TutorialRotate_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	 // 0 : 튜토리얼 시작 부분의 이벤트 씬

void CEventScriptActor::ActionSetting() // 액션 함수들을 등록해주는 함수.
{
	AddAction( SA_TutorialStart_R, SA_TutorialStart_M );   // 0 : 튜토리얼 시작 부분의 이벤트 씬
	AddAction( SA_TutorialMove_R, SA_TutorialMove_M );    // 1 : 튜토리얼 중 이동 실습.
	AddAction( SA_TutorialRotate_R, SA_TutorialRotate_M );  // 2 : 튜토리얼 중 회전 실습.
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CEventScriptActor::LoadScript( const char* FileName ) // 스크립트 및 텍스쳐를 로드.
{

	char strTemp[256];

	Init();

	if( !m_ScriptMgr.LoadScript( FileName ) ) // 스크립트를 읽어온다.
	{
		MessageBox( NULL, "Script Load Failed! Script File do not find!", "Warring", MB_OK );
		return FALSE;
	}

	// 필요한 텍스쳐 갯수를 계산한다.
	m_TextureNum = 1; // 대사창 그래픽.

	sSceneData *pScene;
	list<sSceneData*>::iterator prScene;
	for( prScene = m_ScriptMgr.m_listScene.begin(); prScene != m_ScriptMgr.m_listScene.end(); ++prScene )
	{
		pScene = *prScene;

		m_TextureNum += pScene->m_listImageOut.size(); // 각 씬마다 들어있는 이미지 갯수를 누적시킨다.
	}

	m_pIndexedTexture = new IndexedTexture( m_TextureNum );
	m_pIndexedTexture->SetTexturePath( "data/EventScript/textr/" );

	// 대사창 이미지 로드 작업
	if( !m_ScriptMgr.m_strBoxFileName ) // 없을 수도 있다. 없으면 곤란하므로 지정시키자.
	{
		MessageBox( NULL, "Script Load Failed! Please Set Talk Box File!", "Warring", MB_OK );
		return FALSE;
	}
	m_pIndexedTexture->CreateTextureFromFile( m_ScriptMgr.m_strBoxFileName ); // 항상 0번은 대사창.


	// 씬별 이미지 로드.
	for( prScene = m_ScriptMgr.m_listScene.begin(); prScene != m_ScriptMgr.m_listScene.end(); ++prScene )
	{
		pScene = *prScene;

		sImageOutInfo *pImageOut;
		list<sImageOutInfo*>::iterator prImageOut;
		for( prImageOut = pScene->m_listImageOut.begin(); prImageOut != pScene->m_listImageOut.end(); ++prImageOut )
		{
			pImageOut = *prImageOut;

			if( pImageOut->m_strFileName )
			{
				pImageOut->m_ContainerIndex = m_pIndexedTexture->CreateTextureFromFile( pImageOut->m_strFileName ); // 인덱스를 따로 기억.
				if( pImageOut->m_ContainerIndex < 0 ) // 로드가 제대로 안된 경우다.
				{
					sprintf( strTemp, "Script Load Failed! Image %s!", pImageOut->m_strFileName );
					MessageBox( NULL, strTemp, "Warring", MB_OK );
				}
			}
			else // 파일 지정이 잘 못 된경우.
			{
				sprintf( strTemp, "Script Load Failed! %d Scene`s Image wrong!", pScene->m_Index );
				MessageBox( NULL, strTemp, "Warring", MB_OK );
			}
		}
	}

	m_pIndexedTexture->RestoreAllTextures( GET_D3DDEVICE() );


	// 각 대사들의 서피스는 씬 전환시 만들어보자.(혹시 느리면 여기로 옮기자.)

	return TRUE;
}

void CEventScriptActor::SceneChange( int SceneIndex, BOOL bNotCheckFade ) // 씬 전환 처리. 대사를 서피스로 만들고 웨이트 타임이나 웨이트 액션등을 처리해준다.
{
	if( !g_pDisplay )
	{
		pCMyApp->m_pUIMgr->EndEventScene();
		return;
	}

	// 이전 씬에서의 페이드 아웃 설정이 있는지 체크한다. (종료 체크 전에 해야한다.)
	if( !bNotCheckFade && m_pNowScene && m_pNowScene->m_FadeOutSpeed ) // 페이드 아웃이 되야하는 시간으로 목적을 바꿨다.(ms 단위)
	{
		m_Fading = m_pNowScene->m_FadeOutSpeed*100; // ms 단위를 틱으로 바꾼다. -는 페이드 인임을 표시.
		m_ScriptState = EVENT_SCRIPT_STATE_FADING; // 페이딩 모드로 전환.
		m_pNowScene->m_SceneStartTick = timeGetTime(); // 페이딩 시작 시간을 기억하기 위해 사용.
		m_ReserveNextScent = SceneIndex; // 여기서 페이드 아웃이 끝나고 진행될 씬 번호를 기억해둔다.

		return; // 여기서 중단 시켜서 페이드를 진행시킨다.
	}

	if( SceneIndex == -1 ) // 스크립트 종료 체크.
	{
		pCMyApp->m_pUIMgr->EndEventScene();
		return;
	}

	// 우선은 해당 인덱스의 씬을 찾는다.
	m_pNowScene = NULL;
	sSceneData *pTempScene;
	list<sSceneData*>::iterator prScene;
	for( prScene = m_ScriptMgr.m_listScene.begin(); prScene != m_ScriptMgr.m_listScene.end(); ++prScene )
	{
		pTempScene = *prScene;

		if( pTempScene->m_Index == SceneIndex ) // 같은 씬을 찾았으면...
		{
			m_pNowScene = pTempScene;
			break;
		}
	}

	if( !m_pNowScene ) // 이게 NULL 이면 해당 인덱스의 씬이 없었던거다.
	{
		char strTemp[256];
		sprintf( strTemp, "Scene Change Failed! can`t find %d", SceneIndex );
		MessageBox( NULL, strTemp, "EventScript Error", MB_OK );
		return;
	}


	// 영역 셋팅.
	if( m_pNowScene->m_BoxArea.left >= 0 ) // 있다면..
		m_BoxArea.left = m_pNowScene->m_BoxArea.left;
	else // 없다면.
		m_BoxArea.left = m_ScriptMgr.m_BoxArea.left;

	if( m_pNowScene->m_BoxArea.top >= 0 ) // 있다면..
		m_BoxArea.top = m_pNowScene->m_BoxArea.top;
	else // 없다면.
		m_BoxArea.top = m_ScriptMgr.m_BoxArea.top;

	if( m_pNowScene->m_BoxArea.right >= 0 ) // 있다면..
		m_BoxArea.right = m_pNowScene->m_BoxArea.right;
	else // 없다면.
		m_BoxArea.right = m_ScriptMgr.m_BoxArea.right;

	if( m_pNowScene->m_BoxArea.bottom >= 0 ) // 있다면..
		m_BoxArea.bottom = m_pNowScene->m_BoxArea.bottom;
	else // 없다면.
		m_BoxArea.bottom = m_ScriptMgr.m_BoxArea.bottom;

	Convert640Pos( &m_BoxArea ); // 640*480 해상도 기준의 좌표를 현재 해상도로 컨버팅시켜준다.



	if( m_pNowScene->m_TextArea.left >= 0 ) // 있다면..
		m_TextArea.left = m_pNowScene->m_TextArea.left;
	else // 없다면.
		m_TextArea.left = m_ScriptMgr.m_TextArea.left;

	if( m_pNowScene->m_TextArea.top >= 0 ) // 있다면..
		m_TextArea.top = m_pNowScene->m_TextArea.top;
	else // 없다면.
		m_TextArea.top = m_ScriptMgr.m_TextArea.top;

	if( m_pNowScene->m_TextArea.right >= 0 ) // 있다면..
		m_TextArea.right = m_pNowScene->m_TextArea.right;
	else // 없다면.
		m_TextArea.right = m_ScriptMgr.m_TextArea.right;

	if( m_pNowScene->m_TextArea.bottom >= 0 ) // 있다면..
		m_TextArea.bottom = m_pNowScene->m_TextArea.bottom;
	else // 없다면.
		m_TextArea.bottom = m_ScriptMgr.m_TextArea.bottom;

	Convert640Pos( &m_TextArea ); // 640*480 해상도 기준의 좌표를 현재 해상도로 컨버팅시켜준다.

	// 기존 메모리 잡은거 초기화
	for( int i = 0 ; i < MAX_SCRIPT_TEXTLINE ; i++ )
	{
		SAFE_DELETE_ARRAY( m_ContantsText[i] );
		SAFE_DELETE( m_pContantsOutSurf[i] );
	}


	// 여기서 텍스트 서피스등을 만들어 놓는다.
	// 우선은 컨텐츠 스트링.
	m_ContantsLineCount = 0; // 라인수 초기화.
	if( m_pNowScene->m_strContents )
	{
		SIZE	StringSize;
		HDC		hdmDC = CreateCompatibleDC(NULL);
		HFONT	holdFont = (HFONT)SelectObject(hdmDC,m_Font);

		int PrevStringPoint = 0; // 이전에 스트링을 끊었던 위치.

		int StrReadPt = 0;

		int ContantsTotalLen = strlen(m_pNowScene->m_strContents);

		while( StrReadPt < ContantsTotalLen ) // 전체 스트링을 돌면서...
		{
			if( m_pNowScene->m_strContents[StrReadPt] == '\\' && m_pNowScene->m_strContents[StrReadPt+1] == 'n' ) // 개행 문자가 있을 경우는 무조건 새 라인을 만들면된다.
			{
				if( StrReadPt+2 >= ContantsTotalLen ) // 혹시 맨 마지막 개행이면 무시해준다. (괜히 공백줄이 들어가므로)
				{
					// 앞쪽 스트링은 하나 만들어주고...
					int Len = StrReadPt-PrevStringPoint;
					m_ContantsText[m_ContantsLineCount] = new char[Len+1];

					if( m_ContantsText[m_ContantsLineCount] )
					{
						strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
						m_ContantsText[m_ContantsLineCount][Len] = '\0';

						StrReadPt += 2; // 이건 개행 문자분도 넘긴다.

						PrevStringPoint = StrReadPt; // 이전에 끊은 위치를 기억해둔다.
						m_ContantsLineCount++;
						if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
						{
							return;
						}
					}

					break; // 여기서 중단해준다.
				}

				int Len = StrReadPt-PrevStringPoint;
				m_ContantsText[m_ContantsLineCount] = new char[Len+1];

				if( m_ContantsText[m_ContantsLineCount] )
				{
					strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
					m_ContantsText[m_ContantsLineCount][Len] = '\0';

					StrReadPt += 2; // 이건 개행 문자분도 넘긴다.

					PrevStringPoint = StrReadPt; // 이전에 끊은 위치를 기억해둔다.
					m_ContantsLineCount++;
					if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
					{
						return;
					}

					continue; // 한번 끊어준다.
				}
			}

			// 일단 개행문자가 아니면...이번 문자를 이어서 출력한다면 한줄을 넘어가는지를 체크해본다.
			if( m_pNowScene->m_strContents[StrReadPt] < 0 || m_pNowScene->m_strContents[StrReadPt] > 127 ) // 우선 현재 문자가 2바이트 코드인지를 확인
			{
				// 2 바이트 문자일 경우.
				StrReadPt += 2; // 2 바이트니까 다음 읽을 포인틀르 2증가시켜서 체크. (타이밍을 여기서 하는 이유는 아래 길이 구하기 때문.)
				int iLength = StrReadPt - PrevStringPoint;
				GetTextExtentPoint( hdmDC, &m_pNowScene->m_strContents[PrevStringPoint], iLength, &StringSize ); // 이번에 끊어야 하는 줄의 가로 길이를 구해본다.

				if( StringSize.cx > m_TextArea.right ) // 문장이 텍스트 출력 길이를 넘으면...
				{
					// 이전 글자를 기준으로 끊어준다.
					StrReadPt -= 2; // 위치를 다시 전 글자로 돌려준다.

					int Len = StrReadPt-PrevStringPoint;
					m_ContantsText[m_ContantsLineCount] = new char[Len+1];

					if( m_ContantsText[m_ContantsLineCount] )
					{
						strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
						m_ContantsText[m_ContantsLineCount][Len] = '\0';

						PrevStringPoint = StrReadPt; // 이전에 끊은 위치를 기억해둔다.
						m_ContantsLineCount++;
						if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
						{
							return;
						}

						continue; // 한번 끊어준다.
					}
				}
			}
			else // 1바이트 코드
			{
				StrReadPt += 1;
				int iLength = StrReadPt - PrevStringPoint;
				GetTextExtentPoint( hdmDC, &m_pNowScene->m_strContents[PrevStringPoint], iLength, &StringSize ); // 이번에 끊어야 하는 줄의 가로 길이를 구해본다.

				if( StringSize.cx > m_TextArea.right ) // 문장이 텍스트 출력 길이를 넘으면...
				{
					// 이전 글자를 기준으로 끊어준다.
					StrReadPt -= 1; // 위치를 다시 전 글자로 돌려준다.

					int Len = StrReadPt-PrevStringPoint;
					m_ContantsText[m_ContantsLineCount] = new char[Len+1];

					if( m_ContantsText[m_ContantsLineCount] )
					{
						strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
						m_ContantsText[m_ContantsLineCount][Len] = '\0';

						PrevStringPoint = StrReadPt; // 이전에 끊은 위치를 기억해둔다.
						m_ContantsLineCount++;
						if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
						{
							return;
						}

						continue; // 한번 끊어준다.
					}
				}
			}

			if( StrReadPt >= ContantsTotalLen ) // 마지막 줄일경우 의 처리.
			{
				int Len = ContantsTotalLen-PrevStringPoint;
				m_ContantsText[m_ContantsLineCount] = new char[Len+1];

				if( m_ContantsText[m_ContantsLineCount] )
				{
					strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
					m_ContantsText[m_ContantsLineCount][Len] = '\0';

					PrevStringPoint = ContantsTotalLen; // 이전에 끊은 위치를 기억해둔다.
					m_ContantsLineCount++;
					if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
					{
						return;
					}

					continue; // 한번 끊어준다.
				}
			}

		}


		// 줄 끊어놓은 것들로 서피스를 만들자.
		for( int i = 0 ; i < m_ContantsLineCount ; ++i )
		{
			if( m_ContantsText[i] && strlen(m_ContantsText[i]) ) // 혹시 개행문자만 들어가있다거나 하는경우 때문에 체크.
			{
				g_pDisplay->CreateSurfaceFromText( &m_pContantsOutSurf[i], m_Font, m_ContantsText[i], RGB(0, 0, 0), RGB(255, 255, 255) );
				if( m_pContantsOutSurf[i] )
					m_pContantsOutSurf[i]->SetColorKey(RGB(0, 0, 0));
			}
		}

		SelectObject(hdmDC,holdFont);
		DeleteDC(hdmDC);
	}

	// 표시 페이지 정보 셋팅
//	m_NowPageNum = 0;
//	m_StopLine = 0;
	m_NowViewLine = 0;
	m_dwStopTick = 0;
	m_OnePageLine = ( (m_TextArea.bottom-SCRIPT_TEXT_SPACE_TOP) / SCRIPT_USE_LINE_HEIGHT ); // 한 페이지의 표시 가능 라인
	m_OnePageLine--; // 페이지 넘김 표시용 줄을 고려한다.
	/*
		if( m_OnePageLine <= 0 )
			m_TotalPageNum = 1;
		else
			m_TotalPageNum = m_ContantsLineCount / m_OnePageLine; // 선택지 라인을 고려함에 유의.

		if( m_ContantsLineCount % m_OnePageLine > 0 ) // 나머지가 있다면..
			m_TotalPageNum++;
	*/



	// 출력 위치 초기화.
	m_NowPrintChar = 0;
	m_NowPrintLine = 0;


	//////////////////////////////////////////////////////////////
	// 선택지 서피스를 만든다.
	// 일단 지우고.
	SAFE_DELETE( m_pSelectionOn3Surf );
	SAFE_DELETE( m_pSelectionOn2Surf );
	SAFE_DELETE( m_pSelectionOn1Surf );
	SAFE_DELETE( m_pSelection3Surf );
	SAFE_DELETE( m_pSelection2Surf );
	SAFE_DELETE( m_pSelection1Surf );
	SAFE_DELETE( m_pClickSurf );

	m_SelectionLineCount = 0;

	// 페이지 넘김용 클릭 서피스.
	g_pDisplay->CreateSurfaceFromText( &m_pClickSurf, m_Font, "CLICK!", RGB(0, 0, 0), RGB(0, 255, 0) );
	if( m_pClickSurf )
		m_pClickSurf->SetColorKey(RGB(0, 0, 0));
	g_pDisplay->CreateSurfaceFromText( &m_pClickOnSurf, m_Font, "CLICK!", RGB(0, 0, 0), RGB(255, 255, 0) );
	if( m_pClickOnSurf )
		m_pClickOnSurf->SetColorKey(RGB(0, 0, 0));


	// 우선은 선택지 1
	if( m_pNowScene->m_strSelection1 && strlen(m_pNowScene->m_strSelection1) )
	{
		// 특수한 선택지 인지를 체크한다.
		if( strncmp(m_pNowScene->m_strSelection1,"<ENTER>",7) == 0 ) // 클릭으로 넘기기.
		{
			/*
						g_pDisplay->CreateSurfaceFromText( &m_pSelection1Surf, m_Font, "CLICK!", RGB(0, 0, 0), RGB(0, 255, 0) );
						if( m_pSelection1Surf )
							m_pSelection1Surf->SetColorKey(RGB(0, 0, 0));
						g_pDisplay->CreateSurfaceFromText( &m_pSelectionOn1Surf, m_Font, "CLICK!", RGB(0, 0, 0), RGB(255, 255, 0) );
						if( m_pSelectionOn1Surf )
							m_pSelectionOn1Surf->SetColorKey(RGB(0, 0, 0));
			*/
		}
		else if( strncmp(m_pNowScene->m_strSelection1,"<NEXT>",6) == 0 ) // 자동 넘기기
		{
			// 그냥 서피스 안 만들고 넘어간다.
		}
		else if( strncmp(m_pNowScene->m_strSelection1,"<END>",5) == 0 ) // 스크립트 종료.
		{
			// 그냥 서피스 안 만들고 넘어간다.
		}
		else
		{
			g_pDisplay->CreateSurfaceFromText( &m_pSelection1Surf, m_Font, m_pNowScene->m_strSelection1, RGB(0, 0, 0), RGB(255, 255, 50) );
			if( m_pSelection1Surf )
				m_pSelection1Surf->SetColorKey(RGB(0, 0, 0));
			g_pDisplay->CreateSurfaceFromText( &m_pSelectionOn1Surf, m_Font, m_pNowScene->m_strSelection1, RGB(0, 0, 0), RGB(50, 255, 50) );
			if( m_pSelectionOn1Surf )
				m_pSelectionOn1Surf->SetColorKey(RGB(0, 0, 0));
		}
		m_SelectionLineCount++;
	}

	if( m_pNowScene->m_strSelection2 && strlen(m_pNowScene->m_strSelection2) )
	{
		g_pDisplay->CreateSurfaceFromText( &m_pSelection2Surf, m_Font, m_pNowScene->m_strSelection2, RGB(0, 0, 0), RGB(255, 255, 50) );
		if( m_pSelection2Surf )
			m_pSelection2Surf->SetColorKey(RGB(0, 0, 0));
		g_pDisplay->CreateSurfaceFromText( &m_pSelectionOn2Surf, m_Font, m_pNowScene->m_strSelection2, RGB(0, 0, 0), RGB(50, 255, 50) );
		if( m_pSelectionOn2Surf )
			m_pSelectionOn2Surf->SetColorKey(RGB(0, 0, 0));

		m_SelectionLineCount++;
	}

	if( m_pNowScene->m_strSelection3 && strlen(m_pNowScene->m_strSelection3) )
	{
		g_pDisplay->CreateSurfaceFromText( &m_pSelection3Surf, m_Font, m_pNowScene->m_strSelection3, RGB(0, 0, 0), RGB(255, 255, 50) );
		if( m_pSelection3Surf )
			m_pSelection3Surf->SetColorKey(RGB(0, 0, 0));
		g_pDisplay->CreateSurfaceFromText( &m_pSelectionOn3Surf, m_Font, m_pNowScene->m_strSelection3, RGB(0, 0, 0), RGB(50, 255, 50) );
		if( m_pSelectionOn3Surf )
			m_pSelectionOn3Surf->SetColorKey(RGB(0, 0, 0));

		m_SelectionLineCount++;
	}

	/////////////////////////////////////////////////////////////



	// 타임 대기 체크.
	if( m_pNowScene->m_WaitTime > 0 )
	{
		m_Waiting = timeGetTime() + (m_pNowScene->m_WaitTime*1000); // 멈춤이 풀릴 시간을 기억.
	}

	// 다음으론 대기액션 체크
	if( m_pNowScene->m_WaitAction >= 0 && m_Action.ReadyAction(m_pNowScene->m_WaitAction) )
	{
		/*
				if( m_pNowScene->m_WaitAction >= m_ActionFuncNum ) // 등록이 안된 액션 함수가 온 경우다.
				{
					char strTemp[256];
					sprintf( strTemp, "%d Action Number is wrong!", m_pNowScene->m_WaitAction );
					MessageBox( NULL, strTemp, "Warring", MB_OK );

					return;
				}
		*/
		m_ScriptState = EVENT_SCRIPT_STATE_WAITACTION; // 대기 액션이나 페이드가 없으면 바로 실행으로...
		m_pNowScene->m_SceneStartTick = timeGetTime(); // 씬이 바뀐 시간을 기록.

		return; // 여기서 중단 시켜서 액션을 진행시킨다.
	}


	// 페이드인 체크. (만약 페이드 인이 있다면 페이드인이 끝나는 시점에 타임대기와 액션대기를 한번 더 체크한다.)
	if( m_pNowScene->m_FadeInSpeed ) // 페이드 인이 되야하는 시간으로 목적을 바꿨다.(ms 단위)
	{
		m_Fading = -m_pNowScene->m_FadeInSpeed*100; // ms 단위를 틱으로 바꾼다. -는 페이드 인임을 표시.
		m_ScriptState = EVENT_SCRIPT_STATE_FADING; // 페이딩 모드로 전환.
		m_pNowScene->m_SceneStartTick = timeGetTime(); // 페이딩 시작 시간을 기억하기 위해 사용.

		return; // 여기서 중단 시켜서 페이드를 진행시킨다.
	}

	m_ScriptState = EVENT_SCRIPT_STATE_RUN; // 대기 액션이나 페이드가 없으면 바로 실행으로...
	m_pNowScene->m_SceneStartTick = timeGetTime(); // 씬이 바뀐 시간을 기록.


}

void CEventScriptActor::RunScript() // 스크립트를 구동
{
	if( m_Waiting ) // 이게 0이 아니면 대기시간이 있는거다.
	{
		if( timeGetTime() >= m_Waiting ) // 대기할 틱 시간까지 스킵
		{
			m_Waiting = 0;
		}

		m_pNowScene->m_SceneStartTick = timeGetTime(); // 진행이 안되게 시간을 잡아준다.

		return; // 출력도 안한다.
	}

	switch( m_ScriptState )
	{
	case EVENT_SCRIPT_STATE_RUN: // 실행 중
	case EVENT_SCRIPT_STATE_WAITCLICK: // 선택 대기중
	case EVENT_SCRIPT_STATE_PAGEWAIT: // 페이지 넘기기 대기중.

		RenderScript(); // 그냥 함수 하나로 묶었다.

		break;

	case EVENT_SCRIPT_STATE_WAITACTION: // 이벤트 연출 대기중

		m_Action.m_pFunc->Render();

		break;

	case EVENT_SCRIPT_STATE_FADING: // 씬 페이딩

		if( m_Fading < 0 ) // 페이드 인 중.
		{
			DWORD Elapsed = timeGetTime()-m_pNowScene->m_SceneStartTick;
			int Fading = abs(m_Fading); // 양수로 바꾸고.

			Fading -= Elapsed; // 경과시간을 뺀다.

			int Alpha = 255-(int)(255.0f * ( (float)Fading/((float)m_pNowScene->m_FadeInSpeed*100.0f) ) );
			if( Alpha < 0 )
				Alpha = 0;
			if( Alpha > 255 )
				Alpha = 255;

			DWORD FadeColor = RGBA_MAKE( 0, 0, 0, Alpha );

			pCMyApp->m_pFxSet->RenderFadeScreen( FadeColor );

			if( Fading <= 0 ) // 페이드 인 종료 조건 체크.
			{
				m_Fading = 0;
//				m_pNowScene->m_SceneStartTick = timeGetTime(); // 씬의 시작 시간을 페이드인 종료 시점으로 셋팅해준다.
//				m_ScriptState = EVENT_SCRIPT_STATE_RUN;

				return;
			}
		}
		else if( m_Fading > 0 ) // 페이드 아웃 중.
		{
			DWORD Elapsed = timeGetTime()-m_pNowScene->m_SceneStartTick;
			int Fading = abs(m_Fading); // 양수로 바꾸고.

			Fading -= Elapsed; // 경과시간을 뺀다.

			int Alpha = (int)(255.0f * ( (float)Fading/((float)m_pNowScene->m_FadeOutSpeed*100.0f) ) );
			if( Alpha < 0 )
				Alpha = 0;
			if( Alpha > 255 )
				Alpha = 255;

			DWORD FadeColor = RGBA_MAKE( 0, 0, 0, Alpha );


			pCMyApp->m_pFxSet->RenderFadeScreen( FadeColor );

			if( Fading <= 0 ) // 페이드 아웃 종료 조건 체크.
			{
				m_Fading = 0;
				SceneChange( m_ReserveNextScent, TRUE ); // 다음 씬으로 이동시켜 준다. 두번째 인자를 줘서 다시 페이드 아웃을 반복하지 않도록 만든다.
				return;
			}
		}
		else // 0이면 종료. 다시 RUN 스테이트로 바꿔준다.
		{
			m_ScriptState = EVENT_SCRIPT_STATE_RUN;
			m_pNowScene->m_SceneStartTick = timeGetTime(); // 씬의 시작 시간을 페이드인 종료 시점으로 셋팅해준다.

			// 페이드 인이 있었을 경우 페이드 인이 끝난후 다시 한번 타임대기와 액션대기를 체크해준다.
			// 타임 대기 체크.
			if(	m_pNowScene->m_WaitTime > 0 )
			{
				m_Waiting = timeGetTime() + (m_pNowScene->m_WaitTime*1000); // 멈춤이 풀릴 시간을 기억.
			}

			// 다음으론 대기액션 체크
			if( m_pNowScene->m_WaitAction >= 0 )
			{
//				if( !SetWaitAction(m_pNowScene->m_WaitAction) )
				{
					// 만약 실패했다면...

				}
			}
		}

		break;
	}
}

void CEventScriptActor::RenderScript() // 스크립트 출력.
{
	if( !g_pDisplay )
		return;

	if( !m_pNowScene ) // 혹시라도 없다면...
		return;

	DWORD dwElapsedTick = timeGetTime() - m_pNowScene->m_SceneStartTick; // 씬이 시작된후 진행된 수간을 구한다.

	int i = 0;
	D3DTLVERTEX TLVec[4];
	DWORD color;
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;
	DWORD  dwTextureState[3];

	// 상태값 보존.
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

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
//	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
//	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


	// 대사창을 출력
	color = RGBA_MAKE( 255, 255, 255, 255 );

	TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_BoxArea.left,m_BoxArea.top+m_BoxArea.bottom,0), 1, color, 0, 0, 1.0f );
	TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_BoxArea.left,m_BoxArea.top,0), 1, color, 0, 0, 0 );
	TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_BoxArea.left+m_BoxArea.right,m_BoxArea.top,0), 1, color, 0, 1.0f, 0 );
	TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_BoxArea.left+m_BoxArea.right,m_BoxArea.top+m_BoxArea.bottom,0), 1, color, 0, 1.0f, 1.0f );

	GET_D3DDEVICE()->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx(0) ); // 처음 녀석이 대사창
	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);

	// 그다음 이미지 리스트 출력
	RECT rect;
	sImageOutInfo *pImageOut;
	list<sImageOutInfo*>::iterator prImageOut; // 역순으로 체크해서 찍어야할듯.
	for( prImageOut = m_pNowScene->m_listImageOut.begin(); prImageOut != m_pNowScene->m_listImageOut.end(); ++prImageOut )
	{
		pImageOut = *prImageOut;

		// 전 이미지를 다 검사하자.
		if( dwElapsedTick >= pImageOut->m_CreateTime && (pImageOut->m_CreateTime>=pImageOut->m_DelTime || dwElapsedTick <= pImageOut->m_DelTime+(pImageOut->m_FadeOutSpeed*100))  ) // 출력해야하는 시간대이면...(만약 시작과 종료 타임이 같거나 종료타임이 더 적으면 얘는 안 사라지는 타입이다.)
		{
			rect.left = pImageOut->m_PosX;
			rect.top = pImageOut->m_PosY;
			rect.right = pImageOut->m_Width;
			rect.bottom = pImageOut->m_Height;

			Convert640Pos( &rect );

			int Alpha = 255;

			if( pImageOut->m_FadeInSpeed > 0 && dwElapsedTick >= pImageOut->m_CreateTime
					&& dwElapsedTick <= pImageOut->m_CreateTime+(pImageOut->m_FadeInSpeed*100)  ) // 페이드 인 타이밍.
			{
				Alpha = (int)( 255.0f*((float)(dwElapsedTick-pImageOut->m_CreateTime)/((float)pImageOut->m_FadeInSpeed*100.0f)) );
			}
			else if( pImageOut->m_FadeOutSpeed > 0 && pImageOut->m_DelTime > pImageOut->m_CreateTime
					 && dwElapsedTick >= pImageOut->m_DelTime && dwElapsedTick <= pImageOut->m_DelTime+(pImageOut->m_FadeOutSpeed*100) ) // 페이드 아웃 타이밍.
			{
				Alpha = (int)(255.0f - ( 255.0f * ( (float)(dwElapsedTick-pImageOut->m_DelTime)/((float)pImageOut->m_FadeOutSpeed*100.0f)) ) );
			}

			if( Alpha < 0 )
				Alpha = 0;
			if( Alpha > 255 )
				Alpha = 255;
			color = RGBA_MAKE( 255, 255, 255, Alpha );

			TLVec[0] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.top+rect.bottom,0), 1, color, 0, 0, 1.0f );
			TLVec[1] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.top,0), 1, color, 0, 0, 0 );
			TLVec[2] = D3DTLVERTEX( D3DVECTOR(rect.left+rect.right,rect.top,0), 1, color, 0, 1.0f, 0 );
			TLVec[3] = D3DTLVERTEX( D3DVECTOR(rect.left+rect.right,rect.top+rect.bottom,0), 1, color, 0, 1.0f, 1.0f );

			GET_D3DDEVICE()->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx(pImageOut->m_ContainerIndex) ); // 처음 녀석이 대사창
			GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
		}
	}


	// 그다음 텍스트 대사 출력.(텍스트를 가장 마지막에 출력하는 이유는 꼼수로 대사창 바꾸기를 가능하게 하기 위해서)
	// 우선 중심 대사.
	// 현재 출력해야할 문자와 라인을 구한다.
	int PrintedLen = 0; // 지금까지 출력된 글자수.(누적시킨다.)
	int PrintedLine = 0;

	// 대사 출력을 페이지 별로 나눠서 해준다.
	for( i = 0 ; i < m_NowPrintLine ; ++i ) // 현재 출력중인 줄 전의 줄 들.
	{
		if( i >= m_NowViewLine ) // 보여지는 라인부터 출력. 출력하지 않아도 앞의 글자수 누적은 시켜야한다.
		{
			if( m_pContantsOutSurf[i] )
				g_pDisplay->Blt( m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT, m_TextArea.top+SCRIPT_TEXT_SPACE_TOP+(PrintedLine*SCRIPT_USE_LINE_HEIGHT), m_pContantsOutSurf[i] );

			++PrintedLine;
		}

		PrintedLen += strlen( m_ContantsText[i] ); // 출력된 글자수를 누적시킨다.
	}

	// 현재 출력 중인 줄의 출력.
	if( m_ScriptState == EVENT_SCRIPT_STATE_RUN && m_NowPrintLine < m_ContantsLineCount ) // 현제 페이지 라인까지만... // 최대 줄수 이상이라면 전부다 출력된거다. 이 경우 마지막 줄도 위에서 출력된다.
	{
		// 이전 계산된 위치로 출력해보자.
		SIZE FontSize;
		if( m_pContantsOutSurf[m_NowPrintLine] )
			getFontSize( &FontSize, m_Font, (LPCTSTR)m_pContantsOutSurf[m_NowPrintLine], m_NowPrintChar ); // 현재 출력할 위치까지만...
		else
		{
			FontSize.cx = 0;
			FontSize.cy = SCRIPT_USE_FONT_SIZE;
		}

		RECT DrawLineArea;
		DrawLineArea.left = 0;
		DrawLineArea.top = 0;
		DrawLineArea.right = FontSize.cx;
		DrawLineArea.bottom = FontSize.cy;


		if( m_pContantsOutSurf[m_NowPrintLine] )
			g_pDisplay->Blt( m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT, m_TextArea.top+SCRIPT_TEXT_SPACE_TOP+( (m_NowPrintLine-m_NowViewLine)*SCRIPT_USE_LINE_HEIGHT ), m_pContantsOutSurf[m_NowPrintLine], &DrawLineArea );

		if( m_pNowScene->m_TextSpeed > 0 ) // 이게 만약 0이상일때만 글자당 출력한다.
		{
			DWORD ElapsedPerLine = dwElapsedTick - ( PrintedLen * m_pNowScene->m_TextSpeed ); // 이 라인이 시작된 타임을 구한다.
			m_NowPrintChar = ElapsedPerLine / m_pNowScene->m_TextSpeed;

			// 개행 판단.
			if( m_NowPrintChar > strlen(m_ContantsText[m_NowPrintLine]) ) // 만약 출력을 다했다면..
			{
				m_NowPrintLine++; // 라인도 증가.
				m_NowPrintChar = 0;

				if( m_NowPrintLine >= m_ContantsLineCount ) // 다 출력이면 클릭대기로...
				{
					m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
					m_NowPrintChar = -1; // 다 출력됐음을 표시.
					m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
				}
				else // 페이지 넘김을 확인.
				{
					if( (m_NowPrintLine-m_NowViewLine) >= m_OnePageLine ) // 우선 페이지를 넘겨야하는지 여부를  확인한다.
					{
						// 페이지를 넘겨야 하는 경우.
						m_dwStopTick = timeGetTime();
						m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
					}
					else if( m_ContantsLineCount - m_NowPrintLine <= m_OnePageLine ) // 출력 막바지라면...
					{
						if( (m_NowPrintLine-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // 추가로 선택지를 합했을때 페이지를 넘겨야하는지 확인해본다.(선택지 라인수에 +1)함에 유의.(왜냐면 m_OnePageLine에 한라인 미리 고려되었있다.)
						{
							// 이 경우도 페이지를 넘겨준다.
							m_dwStopTick = timeGetTime();
							m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
						}
					}
				}

				/*
				if( (m_NowPrintLine-m_NowViewLine) >= m_OnePageLine ) // 우선 페이지를 넘겨야하는지 여부를  확인한다.
				{
					// 페이지를 넘겨야 하는 경우.
					m_dwStopTick = timeGetTime();
					m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
				}
				else if( m_ContantsLineCount - m_NowPrintLine <= m_OnePageLine ) // 출력 막바지라면...
				{
					if( (m_NowPrintLine-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // 추가로 선택지를 합했을때 페이지를 넘겨야하는지 확인해본다.(선택지 라인수에 +1)함에 유의.(왜냐면 m_OnePageLine에 한라인 미리 고려되었있다.)
					{
						// 이 경우도 페이지를 넘겨준다.
						m_dwStopTick = timeGetTime();
						m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
					}
					else if( m_NowPrintLine >= m_ContantsLineCount ) // 다 출력이면 클릭대기로...
					{
						m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
						m_NowPrintChar = -1; // 다 출력됐음을 표시.
						m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
					}
				}
				*/
			}
		}
		else // 만약 스피드 지정이 제대로 안되어있다면.
		{
			m_NowPrintLine++;

			if( (m_NowPrintLine-m_NowViewLine) > m_OnePageLine ) // 우선 페이지를 넘겨야하는지 여부를  확인한다.
			{
				// 페이지를 넘겨야 하는 경우.
				m_dwStopTick = timeGetTime();
				m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
			}
			else if( m_ContantsLineCount - m_NowPrintLine <= m_OnePageLine ) // 출력 막바지라면...
			{
				if( (m_NowPrintLine-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // 추가로 선택지를 합했을때 페이지를 넘겨야하는지 확인해본다.(선택지 라인수에 +1)함에 유의.(왜냐면 m_OnePageLine에 한라인 미리 고려되었있다.)
				{
					// 이 경우도 페이지를 넘겨준다.
					m_dwStopTick = timeGetTime();
					m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
				}
				else if( m_NowPrintLine >= m_ContantsLineCount ) // 다 출력이면 클릭대기로...
				{
					m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
					m_NowPrintChar = -1; // 다 출력됐음을 표시.
					m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
				}
			}
		}
	}
	else // 라인이 다출력된 상태에서..
	{
		// 이 경우가 어떤 경우 인지는 모르겠는데. 페이지 넘김 체크안해서 막 건너띄는 경우가 있더라. 그래서 막음.
		if( m_ScriptState == EVENT_SCRIPT_STATE_RUN ) // 만약 대기중으로 안넘어갔다면....이런 경우도 있다.
		{
			if( (m_NowPrintLine-m_NowViewLine) > m_OnePageLine ) // 우선 페이지를 넘겨야하는지 여부를  확인한다.
			{
				// 페이지를 넘겨야 하는 경우.
				m_dwStopTick = timeGetTime();
				m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
			}
			else if( m_ContantsLineCount - m_NowPrintLine <= m_OnePageLine ) // 출력 막바지라면...
			{
				if( (m_NowPrintLine-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // 추가로 선택지를 합했을때 페이지를 넘겨야하는지 확인해본다.(선택지 라인수에 +1)함에 유의.(왜냐면 m_OnePageLine에 한라인 미리 고려되었있다.)
				{
					// 이 경우도 페이지를 넘겨준다.
					m_dwStopTick = timeGetTime();
					m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
				}
				else if( m_NowPrintLine >= m_ContantsLineCount ) // 다 출력이면 클릭대기로...
				{
					m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
					m_NowPrintChar = -1; // 다 출력됐음을 표시.
					m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
				}
			}
		}
	}

	/*  // 라인을 페이지 식으로 나누어 줄수있도록 수정.
		for( i = 0 ; i < m_NowPrintLine ; ++i ) // 현재 출력중인 줄 전의 줄 들.
		{
			if( m_pContantsOutSurf[i] )
				g_pDisplay->Blt( m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT, m_TextArea.top+SCRIPT_TEXT_SPACE_TOP+(i*SCRIPT_USE_LINE_HEIGHT), m_pContantsOutSurf[i] );

			PrintedLen += strlen( m_ContantsText[i] ); // 출력된 글자수를 누적시킨다.
		}

		// 현재 출력 중인 줄의 출력.
		if( m_NowPrintLine < m_ContantsLineCount ) // 최대 줄수 이상이라면 전부다 출력된거다. 이 경우 마지막 줄도 위에서 출력된다.
		{
			// 이전 계산된 위치로 출력해보자.
			SIZE FontSize;
			getFontSize( &FontSize, m_Font, (LPCTSTR)m_pContantsOutSurf[m_NowPrintLine], m_NowPrintChar ); // 현재 출력할 위치까지만...

			RECT DrawLineArea;
			DrawLineArea.left = 0;
			DrawLineArea.top = 0;
			DrawLineArea.right = FontSize.cx;
			DrawLineArea.bottom = FontSize.cy;


			if( m_pContantsOutSurf[m_NowPrintLine] )
				g_pDisplay->Blt( m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT, m_TextArea.top+SCRIPT_TEXT_SPACE_TOP+(m_NowPrintLine*SCRIPT_USE_LINE_HEIGHT), m_pContantsOutSurf[m_NowPrintLine], &DrawLineArea );

			if( m_pNowScene->m_TextSpeed > 0 ) // 이게 만약 0이상일때만 글자당 출력한다.
			{
				DWORD ElapsedPerLine = dwElapsedTick - ( PrintedLen * m_pNowScene->m_TextSpeed ); // 이 라인이 시작된 타임을 구한다.
				m_NowPrintChar = ElapsedPerLine / m_pNowScene->m_TextSpeed;

				// 개행 판단.
				if( m_NowPrintChar > strlen(m_ContantsText[m_NowPrintLine]) ) // 만약 출력을 다했다면..
				{
					m_NowPrintLine++; // 라인도 증가.
					m_NowPrintChar = 0;

					if( m_NowPrintLine >= m_ContantsLineCount )
					{
						m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
						m_NowPrintChar = -1; // 다 출력됐음을 표시.
						m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
					}
				}
			}
			else // 만약 스피드 지정이 제대로 안되어있다면.
			{
				// 바로 전부 출력되게 한다.
				m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
				m_NowPrintChar = -1; // 다 출력됐음을 표시.
				m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
			}
		}
		else // 라인이 다출력된 상태에서..
		{
			if( m_ScriptState == EVENT_SCRIPT_STATE_RUN ) // 만약 대기중으로 안넘어갔다면....이런 경우도 있다.
			{
				m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
				m_NowPrintChar = -1; // 다 출력됐음을 표시.
				m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
			}
		}
	*/

	if( m_ScriptState == EVENT_SCRIPT_STATE_PAGEWAIT )
	{
		/*		if( m_pNowScene->m_TextSpeed > 0 ) // 이게 만약 0이상일때는 자동으로 넘겨준다.
				{
					if( timeGetTime()-m_dwStopTick > 1000 ) // 약간 기다렸다가 넘겨준다.
					{
						m_NowViewLine = m_NowPrintLine; // 현재까지 출력했던 라인 +1 된 위치다.
						m_pNowScene->m_SceneStartTick += timeGetTime()-m_dwStopTick; // 기다린 만큼 틱을 되돌려준다.

						m_ScriptState = EVENT_SCRIPT_STATE_RUN;
					}
				}
				else // 글자들 즉시 출력 의 경우 클릭으로 넘어가게 해준다.
				{
		*/			// 우 하단에 클릭이 깜빡이게 한다.
		if( m_pClickSurf && m_pClickOnSurf ) // 가장 아래것 부터 검사.
		{
			m_pClickSurf->Xpos = m_TextArea.left+m_TextArea.right-SCRIPT_TEXT_SPACE_LEFT-50;
			m_pClickSurf->Ypos = m_TextArea.top+m_TextArea.bottom-SCRIPT_TEXT_SPACE_LEFT-SCRIPT_USE_LINE_HEIGHT;

			if( (timeGetTime()/200) % 2 ) // 깜빡이게...
			{
				g_pDisplay->Blt( m_pClickSurf->Xpos, m_pClickSurf->Ypos, m_pClickOnSurf );
			}
			else
			{
				g_pDisplay->Blt( m_pClickSurf->Xpos, m_pClickSurf->Ypos, m_pClickSurf );
			}
		}
//		}
	}

	// 선택지 출력.
	if( m_ScriptState == EVENT_SCRIPT_STATE_WAITCLICK )
	{
		// 특수 선택지 체크.
		if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<ENTER>",7) == 0 ) // 클릭으로 넘기기.
		{
			// 우 하단에 클릭이 깜빡이게 한다.
			if( m_pClickSurf && m_pClickOnSurf ) // 가장 아래것 부터 검사.
			{
				m_pClickSurf->Xpos = m_TextArea.left+m_TextArea.right-SCRIPT_TEXT_SPACE_LEFT-50;
				m_pClickSurf->Ypos = m_TextArea.top+m_TextArea.bottom-SCRIPT_TEXT_SPACE_LEFT-SCRIPT_USE_LINE_HEIGHT;

				if( (timeGetTime()/200) % 2 ) // 깜빡이게...
				{
					g_pDisplay->Blt( m_pClickSurf->Xpos, m_pClickSurf->Ypos, m_pClickOnSurf );
				}
				else
				{
					g_pDisplay->Blt( m_pClickSurf->Xpos, m_pClickSurf->Ypos, m_pClickSurf );
				}

			}
		}
		else if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<NEXT>",6) == 0 ) // 자동 넘기기
		{
//			Sleep( 2000 ); // 바로 넘어가면 너무 빠르니까 약간 쉬어준다.
			SceneChange( m_pNowScene->m_LinkSel1 ); // 다음 씬으로.
		}
		else if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<END>",5) == 0 ) // 스크립트 종료.
		{
//			Sleep( 3000 ); // 바로 넘어가면 너무 빠르니까 약간 쉬어준다.
			SceneChange( -1 ); //스크립트 종료
		}
		else // 특수 선택지가 아닐때 일반 선택지를 출력한다.
		{
			int OutSelY = m_TextArea.bottom-SCRIPT_TEXT_SPACE_TOP-SCRIPT_USE_LINE_HEIGHT; // 출력위치 누적.

			if( m_pSelection3Surf && m_pSelectionOn3Surf ) // 가장 아래것 부터 검사.
			{
				m_pSelection3Surf->Xpos = m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT;
				m_pSelection3Surf->Ypos = m_TextArea.top+OutSelY;

				if( m_NowMouseOnSel == 3 ) // 혹시 마우스가 올라간 상태인가 체크.
				{
					g_pDisplay->Blt( m_pSelection3Surf->Xpos, m_pSelection3Surf->Ypos, m_pSelectionOn3Surf );
				}
				else
				{
					g_pDisplay->Blt( m_pSelection3Surf->Xpos, m_pSelection3Surf->Ypos, m_pSelection3Surf );
				}

				OutSelY -= SCRIPT_USE_LINE_HEIGHT;
			}

			if( m_pSelection2Surf && m_pSelectionOn2Surf ) // 가장 아래것 부터 검사.
			{
				m_pSelection2Surf->Xpos = m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT;
				m_pSelection2Surf->Ypos = m_TextArea.top+OutSelY;

				if( m_NowMouseOnSel == 2 ) // 혹시 마우스가 올라간 상태인가 체크.
				{
					g_pDisplay->Blt( m_pSelection2Surf->Xpos, m_pSelection2Surf->Ypos, m_pSelectionOn2Surf );
				}
				else
				{
					g_pDisplay->Blt( m_pSelection2Surf->Xpos, m_pSelection2Surf->Ypos, m_pSelection2Surf );
				}

				OutSelY -= SCRIPT_USE_LINE_HEIGHT;
			}

			if( m_pSelection1Surf && m_pSelectionOn1Surf ) // 가장 아래것 부터 검사.
			{
				m_pSelection1Surf->Xpos = m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT;
				m_pSelection1Surf->Ypos = m_TextArea.top+OutSelY;

				if( m_NowMouseOnSel == 1 ) // 혹시 마우스가 올라간 상태인가 체크.
				{
					g_pDisplay->Blt( m_pSelection1Surf->Xpos, m_pSelection1Surf->Ypos, m_pSelectionOn1Surf );
				}
				else
				{
					g_pDisplay->Blt( m_pSelection1Surf->Xpos, m_pSelection1Surf->Ypos, m_pSelection1Surf );
				}

				OutSelY -= SCRIPT_USE_LINE_HEIGHT;
			}
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
}

BOOL CEventScriptActor::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) // 메세지 프로시져.
{
	int x, y;
	int result = 0;

	switch( m_ScriptState )
	{
	case EVENT_SCRIPT_STATE_RUN: // 실행 중

		// 클릭하면 글자 진행 스킵.
		if( msg == WM_LBUTTONDOWN )
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( IsInsideTalkBox(x,y) ) // 대사 창을 클릭했으면...
			{
				if( (m_ContantsLineCount-m_NowViewLine) > m_OnePageLine ) // 우선 페이지를 넘겨야하는지 여부를  확인한다.
				{
					// 페이지를 넘겨야 하는 경우.
//					m_NowPrintLine = (m_ContantsLineCount-m_NowViewLine)-1;
					m_NowPrintLine = m_NowViewLine+m_OnePageLine;
					m_dwStopTick = timeGetTime();
					m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
					return 1;
				}
				else
				{
					if( (m_ContantsLineCount-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // 추가로 선택지를 합했을때 페이지를 넘겨야하는지 확인해본다.(선택지 라인수에 +1)함에 유의.(왜냐면 m_OnePageLine에 한라인 미리 고려되었있다.)
					{
						// 이 경우도 페이지를 넘겨준다.
//						m_NowPrintLine = (m_ContantsLineCount-m_NowViewLine)+(m_SelectionLineCount-1)-1;
						m_NowPrintLine = m_NowViewLine+m_OnePageLine;
						m_dwStopTick = timeGetTime();
						m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)

						return 1;
					}
				}

				// 그 외의 경우는 바로 전부 출력되게 한다.
				m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
				m_NowPrintChar = -1; // 다 출력됐음을 표시.
				m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)


				/*
								if( (m_NowPrintLine-m_NowViewLine) >= m_OnePageLine ) // 마지막 페이지가 아닌경우.
								{
									m_dwStopTick = timeGetTime();
									m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
								}
								else // 마지막 페이지인 경우.
								{
									m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
									m_NowPrintChar = -1; // 다 출력됐음을 표시.
									m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
								}
				*/
				/*
								// 바로 전부 출력되게 한다.
								m_NowPrintLine = m_ContantsLineCount; // 이렇게 하면 다음 루프부터 전부 출력되고 더이상 여기로 안들어온다.
								m_NowPrintChar = -1; // 다 출력됐음을 표시.
								m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // 상태를 클릭 대기중으로 해준다.(출력은 변함없고 메시지 프로시져가 다르게 동작해준다.)
				*/
			}
		}

		break;

	case EVENT_SCRIPT_STATE_PAGEWAIT:

		if( msg == WM_LBUTTONDOWN )
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( IsInsideTalkBox(x,y) /*&& m_pNowScene->m_TextSpeed <= 0*/ ) // 대사 창을 클릭했으면...  (글자 흐를땐 자동으로 넘어간다.)
			{
				m_NowViewLine = m_NowPrintLine; // 현재까지 출력했던 라인 +1 된 위치다.
				m_pNowScene->m_SceneStartTick += timeGetTime()-m_dwStopTick; // 기다린 만큼 틱을 되돌려준다.

				m_ScriptState = EVENT_SCRIPT_STATE_RUN;
			}
		}

		break;

	case EVENT_SCRIPT_STATE_WAITACTION: // 이벤트 연출 대기중

//		result = ActionFunc[m_pNowScene->m_WaitAction].MsgProcFunc( hWnd, msg, wParam, lParam );
		result = m_Action.m_pFunc->MsgProc( hWnd, msg, wParam, lParam );
		/*
				if( result <= 0 ) // 0 이하면 종료.
				{
					m_ScriptState = EVENT_SCRIPT_STATE_RUN; // 씬 실행으로...
					m_pNowScene->m_SceneStartTick = timeGetTime(); // 씬이 바뀐 시간을 기록.
				}
		*/
		if( m_Action.m_pFunc->EndCheck() ) // 종료 조건 체크.
		{
			m_Action.ReleaseAction( m_pNowScene->m_WaitAction ); // 메모리 해제를 해준다.

			m_ScriptState = EVENT_SCRIPT_STATE_RUN; // 씬 실행으로...
			m_pNowScene->m_SceneStartTick = timeGetTime(); // 씬이 바뀐 시간을 기록.
		}

		return result;


	case EVENT_SCRIPT_STATE_WAITCLICK: // 선택 대기중

		// 임시 테스트로 클릭하면 무조건 1번 선택지 씬으로 넘기게 처리.
		// 클릭하면 글자 진행 스킵.
		if( msg == WM_MOUSEMOVE )
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			m_NowMouseOnSel = IsInsideSelection(x,y); // 선택지 위로 롤 오버 체크. 선택지 체크.
		}
		else if( msg == WM_LBUTTONDOWN )
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<ENTER>",7) == 0 ) // 클릭으로 넘기기.
			{
				if( IsInsideTalkBox(x,y) ) // 대사 창을 클릭했으면...
				{
					SceneChange( m_pNowScene->m_LinkSel1 ); // 임시 테스트.
				}
			}
			/*
						else if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<END>",5) == 0 ) // 클릭으로 종료.
						{
			//				if( IsInsideTalkBox(x,y) ) // 대사 창을 클릭했으면...
							{
								SceneChange( -1 ); // 임시 테스트.
							}
						}
			*/
			else // 아니면 선택지 체크
			{
				m_NowMouseOnSel = IsInsideSelection(x,y); // 선택지 위로 롤 오버 체크. 선택지 체크.
				if( m_NowMouseOnSel > 0 ) // 선택지를 클릭했으면...
				{
					if( m_NowMouseOnSel == 1 )
						SceneChange( m_pNowScene->m_LinkSel1 ); // 1번 선택지 링크를 따라서
					else if( m_NowMouseOnSel == 2 )
						SceneChange( m_pNowScene->m_LinkSel2 ); // 2번 선택지 링크를 따라서
					else if( m_NowMouseOnSel == 3 )
						SceneChange( m_pNowScene->m_LinkSel3 ); // 3번 선택지 링크를 따라서
				}
			}
		}

		break;

	case EVENT_SCRIPT_STATE_FADING: // 씬 페이딩
		break;
	}

	return TRUE; // 다른 메세지를 안 받아준다.
}

BOOL CEventScriptActor::IsInsideTalkBox(int x, int y)
{
	if ( ( ( x > m_BoxArea.left) && ( x < (m_BoxArea.left + m_BoxArea.right)) )
			&& ( ( y >m_BoxArea.top ) && ( y < (m_BoxArea.top + m_BoxArea.bottom) ) ) )
		return true;
	else
		return false;
}

int CEventScriptActor::IsInsideSelection(int x, int y)
{
	// 선택지 1 체크.
	if( m_pSelection1Surf )
	{
		if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<ENTER>",7) == 0 ) // 클릭으로 넘기기.
		{
		}
		else // 아닐때만 체크
		{
			if ( ( ( x > m_pSelection1Surf->Xpos) && ( x < (m_pSelection1Surf->Xpos + m_pSelection1Surf->GetWidth())) )
					&& ( ( y >m_pSelection1Surf->Ypos ) && ( y < (m_pSelection1Surf->Ypos + m_pSelection1Surf->GetHeight()) ) ) )
				return 1;
		}
	}
	// 선택지 2 체크.
	if( m_pSelection2Surf )
	{
		if ( ( ( x > m_pSelection2Surf->Xpos) && ( x < (m_pSelection2Surf->Xpos + m_pSelection2Surf->GetWidth())) )
				&& ( ( y >m_pSelection2Surf->Ypos ) && ( y < (m_pSelection2Surf->Ypos + m_pSelection2Surf->GetHeight()) ) ) )
			return 2;
	}

	// 선택지 3 체크.
	if( m_pSelection3Surf )
	{
		if ( ( ( x > m_pSelection3Surf->Xpos) && ( x < (m_pSelection3Surf->Xpos + m_pSelection3Surf->GetWidth())) )
				&& ( ( y >m_pSelection3Surf->Ypos ) && ( y < (m_pSelection3Surf->Ypos + m_pSelection3Surf->GetHeight()) ) ) )
			return 3;
	}

	return 0;
}

void Convert640Pos( RECT *Rect ) // 640*480 해상도 기준의 좌표를 현재 해상도로 컨버팅시켜준다.
{
	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);


	Rect->left = (int)( (float)Rect->left * 0.745f );
	Rect->top = (int)( (float)Rect->top * 0.745f );
	Rect->right = (int)( (float)Rect->right * 0.745f );
	Rect->bottom = (int)( (float)Rect->bottom * 0.745f );
}











/*

////////////////////////////////////////////////////////////////////////////////////////////////////////
// 액션 함수들 정의 R은 렌더링 함수. M은 메시지 프로시져 함수.
void SA_TutorialStart_R( DWORD StartTick )	 // 0 : 튜토리얼 시작 부분의 이벤트 씬
{
	if( !pCMyApp || !pCMyApp->m_pUIMgr || !pCMyApp->m_pUIMgr->m_pEventScript || !pCMyApp->m_pUIMgr->m_pEventScript->m_pNowScene )
		return -1;

	sSceneData pNowScene = pCMyApp->m_pUIMgr->m_pEventScript->m_pNowScene;
	DWORD Elapsed = timeGetTime() - pNowScene->m_SceneStartTick;


	CNkMob MobWolf;
	MobWolf.SetInfo(23,0);
	CNkMob MobTiger;
	MobWolf.SetInfo(24,1);



	if( Elapsed < 10000 ) // 이 씬의 초반. 몹에게 포위당하는 페이즈
	{


	}
	else if( Elapsed >= 10000 && Elapsed < 15000 ) // 몹이 전멸 당하는 페이즈
	{

	}
	else // 교관이 다가옴.
	{

	}


	return;
}

int SA_TutorialStart_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )	 // 0 : 튜토리얼 시작 부분의 이벤트 씬
{
	if( !pCMyApp || !pCMyApp->m_pUIMgr || !pCMyApp->m_pUIMgr->m_pEventScript || !pCMyApp->m_pUIMgr->m_pEventScript->m_pNowScene )
		return -1;

	sSceneData pNowScene = pCMyApp->m_pUIMgr->m_pEventScript->m_pNowScene;
	// 이 액션은 자동 연출 뿐이므로 일정 시간이 되면 종료시켜준다.
	if( timeGetTime() - pNowScene->m_SceneStartTick > 20000 )
		return 0;

	return 1;
}



void SA_TutorialMove_R( DWORD StartTick )	 // 1 : 튜토리얼 중 이동 실습.
{

	return;
}

int SA_TutorialMove_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )	 // 0 : 튜토리얼 시작 부분의 이벤트 씬
{
	return 1;
}



void SA_TutorialRotate_R( DWORD StartTick )	 // 2 : 튜토리얼 중 회전 실습.
{

	return;
}

int SA_TutorialRotate_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )	 // 0 : 튜토리얼 시작 부분의 이벤트 씬
{
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

*/