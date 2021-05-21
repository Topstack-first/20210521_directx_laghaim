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








void Convert640Pos( RECT *Rect ); // 640*480 �ػ� ������ ��ǥ�� ���� �ػ󵵷� �����ý����ش�.
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////
// �׼� �Լ��� ���� R�� ������ �Լ�. M�� �޽��� ���ν��� �Լ�.
void SA_TutorialStart_R( DWORD StartTick );	 // 0 : Ʃ�丮�� ���� �κ��� �̺�Ʈ ��
int SA_TutorialStart_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	 // 0 : Ʃ�丮�� ���� �κ��� �̺�Ʈ ��

void SA_TutorialMove_R( DWORD StartTick );	 // 1 : Ʃ�丮�� �� �̵� �ǽ�.
int SA_TutorialMove_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	 // 0 : Ʃ�丮�� ���� �κ��� �̺�Ʈ ��

void SA_TutorialRotate_R( DWORD StartTick );	 // 2 : Ʃ�丮�� �� ȸ�� �ǽ�.
int SA_TutorialRotate_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	 // 0 : Ʃ�丮�� ���� �κ��� �̺�Ʈ ��

void CEventScriptActor::ActionSetting() // �׼� �Լ����� ������ִ� �Լ�.
{
	AddAction( SA_TutorialStart_R, SA_TutorialStart_M );   // 0 : Ʃ�丮�� ���� �κ��� �̺�Ʈ ��
	AddAction( SA_TutorialMove_R, SA_TutorialMove_M );    // 1 : Ʃ�丮�� �� �̵� �ǽ�.
	AddAction( SA_TutorialRotate_R, SA_TutorialRotate_M );  // 2 : Ʃ�丮�� �� ȸ�� �ǽ�.
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CEventScriptActor::LoadScript( const char* FileName ) // ��ũ��Ʈ �� �ؽ��ĸ� �ε�.
{

	char strTemp[256];

	Init();

	if( !m_ScriptMgr.LoadScript( FileName ) ) // ��ũ��Ʈ�� �о�´�.
	{
		MessageBox( NULL, "Script Load Failed! Script File do not find!", "Warring", MB_OK );
		return FALSE;
	}

	// �ʿ��� �ؽ��� ������ ����Ѵ�.
	m_TextureNum = 1; // ���â �׷���.

	sSceneData *pScene;
	list<sSceneData*>::iterator prScene;
	for( prScene = m_ScriptMgr.m_listScene.begin(); prScene != m_ScriptMgr.m_listScene.end(); ++prScene )
	{
		pScene = *prScene;

		m_TextureNum += pScene->m_listImageOut.size(); // �� ������ ����ִ� �̹��� ������ ������Ų��.
	}

	m_pIndexedTexture = new IndexedTexture( m_TextureNum );
	m_pIndexedTexture->SetTexturePath( "data/EventScript/textr/" );

	// ���â �̹��� �ε� �۾�
	if( !m_ScriptMgr.m_strBoxFileName ) // ���� ���� �ִ�. ������ ����ϹǷ� ������Ű��.
	{
		MessageBox( NULL, "Script Load Failed! Please Set Talk Box File!", "Warring", MB_OK );
		return FALSE;
	}
	m_pIndexedTexture->CreateTextureFromFile( m_ScriptMgr.m_strBoxFileName ); // �׻� 0���� ���â.


	// ���� �̹��� �ε�.
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
				pImageOut->m_ContainerIndex = m_pIndexedTexture->CreateTextureFromFile( pImageOut->m_strFileName ); // �ε����� ���� ���.
				if( pImageOut->m_ContainerIndex < 0 ) // �ε尡 ����� �ȵ� ����.
				{
					sprintf( strTemp, "Script Load Failed! Image %s!", pImageOut->m_strFileName );
					MessageBox( NULL, strTemp, "Warring", MB_OK );
				}
			}
			else // ���� ������ �� �� �Ȱ��.
			{
				sprintf( strTemp, "Script Load Failed! %d Scene`s Image wrong!", pScene->m_Index );
				MessageBox( NULL, strTemp, "Warring", MB_OK );
			}
		}
	}

	m_pIndexedTexture->RestoreAllTextures( GET_D3DDEVICE() );


	// �� ������ ���ǽ��� �� ��ȯ�� ������.(Ȥ�� ������ ����� �ű���.)

	return TRUE;
}

void CEventScriptActor::SceneChange( int SceneIndex, BOOL bNotCheckFade ) // �� ��ȯ ó��. ��縦 ���ǽ��� ����� ����Ʈ Ÿ���̳� ����Ʈ �׼ǵ��� ó�����ش�.
{
	if( !g_pDisplay )
	{
		pCMyApp->m_pUIMgr->EndEventScene();
		return;
	}

	// ���� �������� ���̵� �ƿ� ������ �ִ��� üũ�Ѵ�. (���� üũ ���� �ؾ��Ѵ�.)
	if( !bNotCheckFade && m_pNowScene && m_pNowScene->m_FadeOutSpeed ) // ���̵� �ƿ��� �Ǿ��ϴ� �ð����� ������ �ٲ��.(ms ����)
	{
		m_Fading = m_pNowScene->m_FadeOutSpeed*100; // ms ������ ƽ���� �ٲ۴�. -�� ���̵� ������ ǥ��.
		m_ScriptState = EVENT_SCRIPT_STATE_FADING; // ���̵� ���� ��ȯ.
		m_pNowScene->m_SceneStartTick = timeGetTime(); // ���̵� ���� �ð��� ����ϱ� ���� ���.
		m_ReserveNextScent = SceneIndex; // ���⼭ ���̵� �ƿ��� ������ ����� �� ��ȣ�� ����صд�.

		return; // ���⼭ �ߴ� ���Ѽ� ���̵带 �����Ų��.
	}

	if( SceneIndex == -1 ) // ��ũ��Ʈ ���� üũ.
	{
		pCMyApp->m_pUIMgr->EndEventScene();
		return;
	}

	// �켱�� �ش� �ε����� ���� ã�´�.
	m_pNowScene = NULL;
	sSceneData *pTempScene;
	list<sSceneData*>::iterator prScene;
	for( prScene = m_ScriptMgr.m_listScene.begin(); prScene != m_ScriptMgr.m_listScene.end(); ++prScene )
	{
		pTempScene = *prScene;

		if( pTempScene->m_Index == SceneIndex ) // ���� ���� ã������...
		{
			m_pNowScene = pTempScene;
			break;
		}
	}

	if( !m_pNowScene ) // �̰� NULL �̸� �ش� �ε����� ���� �������Ŵ�.
	{
		char strTemp[256];
		sprintf( strTemp, "Scene Change Failed! can`t find %d", SceneIndex );
		MessageBox( NULL, strTemp, "EventScript Error", MB_OK );
		return;
	}


	// ���� ����.
	if( m_pNowScene->m_BoxArea.left >= 0 ) // �ִٸ�..
		m_BoxArea.left = m_pNowScene->m_BoxArea.left;
	else // ���ٸ�.
		m_BoxArea.left = m_ScriptMgr.m_BoxArea.left;

	if( m_pNowScene->m_BoxArea.top >= 0 ) // �ִٸ�..
		m_BoxArea.top = m_pNowScene->m_BoxArea.top;
	else // ���ٸ�.
		m_BoxArea.top = m_ScriptMgr.m_BoxArea.top;

	if( m_pNowScene->m_BoxArea.right >= 0 ) // �ִٸ�..
		m_BoxArea.right = m_pNowScene->m_BoxArea.right;
	else // ���ٸ�.
		m_BoxArea.right = m_ScriptMgr.m_BoxArea.right;

	if( m_pNowScene->m_BoxArea.bottom >= 0 ) // �ִٸ�..
		m_BoxArea.bottom = m_pNowScene->m_BoxArea.bottom;
	else // ���ٸ�.
		m_BoxArea.bottom = m_ScriptMgr.m_BoxArea.bottom;

	Convert640Pos( &m_BoxArea ); // 640*480 �ػ� ������ ��ǥ�� ���� �ػ󵵷� �����ý����ش�.



	if( m_pNowScene->m_TextArea.left >= 0 ) // �ִٸ�..
		m_TextArea.left = m_pNowScene->m_TextArea.left;
	else // ���ٸ�.
		m_TextArea.left = m_ScriptMgr.m_TextArea.left;

	if( m_pNowScene->m_TextArea.top >= 0 ) // �ִٸ�..
		m_TextArea.top = m_pNowScene->m_TextArea.top;
	else // ���ٸ�.
		m_TextArea.top = m_ScriptMgr.m_TextArea.top;

	if( m_pNowScene->m_TextArea.right >= 0 ) // �ִٸ�..
		m_TextArea.right = m_pNowScene->m_TextArea.right;
	else // ���ٸ�.
		m_TextArea.right = m_ScriptMgr.m_TextArea.right;

	if( m_pNowScene->m_TextArea.bottom >= 0 ) // �ִٸ�..
		m_TextArea.bottom = m_pNowScene->m_TextArea.bottom;
	else // ���ٸ�.
		m_TextArea.bottom = m_ScriptMgr.m_TextArea.bottom;

	Convert640Pos( &m_TextArea ); // 640*480 �ػ� ������ ��ǥ�� ���� �ػ󵵷� �����ý����ش�.

	// ���� �޸� ������ �ʱ�ȭ
	for( int i = 0 ; i < MAX_SCRIPT_TEXTLINE ; i++ )
	{
		SAFE_DELETE_ARRAY( m_ContantsText[i] );
		SAFE_DELETE( m_pContantsOutSurf[i] );
	}


	// ���⼭ �ؽ�Ʈ ���ǽ����� ����� ���´�.
	// �켱�� ������ ��Ʈ��.
	m_ContantsLineCount = 0; // ���μ� �ʱ�ȭ.
	if( m_pNowScene->m_strContents )
	{
		SIZE	StringSize;
		HDC		hdmDC = CreateCompatibleDC(NULL);
		HFONT	holdFont = (HFONT)SelectObject(hdmDC,m_Font);

		int PrevStringPoint = 0; // ������ ��Ʈ���� ������ ��ġ.

		int StrReadPt = 0;

		int ContantsTotalLen = strlen(m_pNowScene->m_strContents);

		while( StrReadPt < ContantsTotalLen ) // ��ü ��Ʈ���� ���鼭...
		{
			if( m_pNowScene->m_strContents[StrReadPt] == '\\' && m_pNowScene->m_strContents[StrReadPt+1] == 'n' ) // ���� ���ڰ� ���� ���� ������ �� ������ �����ȴ�.
			{
				if( StrReadPt+2 >= ContantsTotalLen ) // Ȥ�� �� ������ �����̸� �������ش�. (���� �������� ���Ƿ�)
				{
					// ���� ��Ʈ���� �ϳ� ������ְ�...
					int Len = StrReadPt-PrevStringPoint;
					m_ContantsText[m_ContantsLineCount] = new char[Len+1];

					if( m_ContantsText[m_ContantsLineCount] )
					{
						strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
						m_ContantsText[m_ContantsLineCount][Len] = '\0';

						StrReadPt += 2; // �̰� ���� ���ںе� �ѱ��.

						PrevStringPoint = StrReadPt; // ������ ���� ��ġ�� ����صд�.
						m_ContantsLineCount++;
						if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
						{
							return;
						}
					}

					break; // ���⼭ �ߴ����ش�.
				}

				int Len = StrReadPt-PrevStringPoint;
				m_ContantsText[m_ContantsLineCount] = new char[Len+1];

				if( m_ContantsText[m_ContantsLineCount] )
				{
					strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
					m_ContantsText[m_ContantsLineCount][Len] = '\0';

					StrReadPt += 2; // �̰� ���� ���ںе� �ѱ��.

					PrevStringPoint = StrReadPt; // ������ ���� ��ġ�� ����صд�.
					m_ContantsLineCount++;
					if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
					{
						return;
					}

					continue; // �ѹ� �����ش�.
				}
			}

			// �ϴ� ���๮�ڰ� �ƴϸ�...�̹� ���ڸ� �̾ ����Ѵٸ� ������ �Ѿ������ üũ�غ���.
			if( m_pNowScene->m_strContents[StrReadPt] < 0 || m_pNowScene->m_strContents[StrReadPt] > 127 ) // �켱 ���� ���ڰ� 2����Ʈ �ڵ������� Ȯ��
			{
				// 2 ����Ʈ ������ ���.
				StrReadPt += 2; // 2 ����Ʈ�ϱ� ���� ���� ����Ʋ�� 2�������Ѽ� üũ. (Ÿ�̹��� ���⼭ �ϴ� ������ �Ʒ� ���� ���ϱ� ����.)
				int iLength = StrReadPt - PrevStringPoint;
				GetTextExtentPoint( hdmDC, &m_pNowScene->m_strContents[PrevStringPoint], iLength, &StringSize ); // �̹��� ����� �ϴ� ���� ���� ���̸� ���غ���.

				if( StringSize.cx > m_TextArea.right ) // ������ �ؽ�Ʈ ��� ���̸� ������...
				{
					// ���� ���ڸ� �������� �����ش�.
					StrReadPt -= 2; // ��ġ�� �ٽ� �� ���ڷ� �����ش�.

					int Len = StrReadPt-PrevStringPoint;
					m_ContantsText[m_ContantsLineCount] = new char[Len+1];

					if( m_ContantsText[m_ContantsLineCount] )
					{
						strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
						m_ContantsText[m_ContantsLineCount][Len] = '\0';

						PrevStringPoint = StrReadPt; // ������ ���� ��ġ�� ����صд�.
						m_ContantsLineCount++;
						if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
						{
							return;
						}

						continue; // �ѹ� �����ش�.
					}
				}
			}
			else // 1����Ʈ �ڵ�
			{
				StrReadPt += 1;
				int iLength = StrReadPt - PrevStringPoint;
				GetTextExtentPoint( hdmDC, &m_pNowScene->m_strContents[PrevStringPoint], iLength, &StringSize ); // �̹��� ����� �ϴ� ���� ���� ���̸� ���غ���.

				if( StringSize.cx > m_TextArea.right ) // ������ �ؽ�Ʈ ��� ���̸� ������...
				{
					// ���� ���ڸ� �������� �����ش�.
					StrReadPt -= 1; // ��ġ�� �ٽ� �� ���ڷ� �����ش�.

					int Len = StrReadPt-PrevStringPoint;
					m_ContantsText[m_ContantsLineCount] = new char[Len+1];

					if( m_ContantsText[m_ContantsLineCount] )
					{
						strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
						m_ContantsText[m_ContantsLineCount][Len] = '\0';

						PrevStringPoint = StrReadPt; // ������ ���� ��ġ�� ����صд�.
						m_ContantsLineCount++;
						if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
						{
							return;
						}

						continue; // �ѹ� �����ش�.
					}
				}
			}

			if( StrReadPt >= ContantsTotalLen ) // ������ ���ϰ�� �� ó��.
			{
				int Len = ContantsTotalLen-PrevStringPoint;
				m_ContantsText[m_ContantsLineCount] = new char[Len+1];

				if( m_ContantsText[m_ContantsLineCount] )
				{
					strncpy( m_ContantsText[m_ContantsLineCount], &m_pNowScene->m_strContents[PrevStringPoint], Len );
					m_ContantsText[m_ContantsLineCount][Len] = '\0';

					PrevStringPoint = ContantsTotalLen; // ������ ���� ��ġ�� ����صд�.
					m_ContantsLineCount++;
					if( m_ContantsLineCount >= MAX_SCRIPT_TEXTLINE )
					{
						return;
					}

					continue; // �ѹ� �����ش�.
				}
			}

		}


		// �� ������� �͵�� ���ǽ��� ������.
		for( int i = 0 ; i < m_ContantsLineCount ; ++i )
		{
			if( m_ContantsText[i] && strlen(m_ContantsText[i]) ) // Ȥ�� ���๮�ڸ� ���ִٰų� �ϴ°�� ������ üũ.
			{
				g_pDisplay->CreateSurfaceFromText( &m_pContantsOutSurf[i], m_Font, m_ContantsText[i], RGB(0, 0, 0), RGB(255, 255, 255) );
				if( m_pContantsOutSurf[i] )
					m_pContantsOutSurf[i]->SetColorKey(RGB(0, 0, 0));
			}
		}

		SelectObject(hdmDC,holdFont);
		DeleteDC(hdmDC);
	}

	// ǥ�� ������ ���� ����
//	m_NowPageNum = 0;
//	m_StopLine = 0;
	m_NowViewLine = 0;
	m_dwStopTick = 0;
	m_OnePageLine = ( (m_TextArea.bottom-SCRIPT_TEXT_SPACE_TOP) / SCRIPT_USE_LINE_HEIGHT ); // �� �������� ǥ�� ���� ����
	m_OnePageLine--; // ������ �ѱ� ǥ�ÿ� ���� ����Ѵ�.
	/*
		if( m_OnePageLine <= 0 )
			m_TotalPageNum = 1;
		else
			m_TotalPageNum = m_ContantsLineCount / m_OnePageLine; // ������ ������ ����Կ� ����.

		if( m_ContantsLineCount % m_OnePageLine > 0 ) // �������� �ִٸ�..
			m_TotalPageNum++;
	*/



	// ��� ��ġ �ʱ�ȭ.
	m_NowPrintChar = 0;
	m_NowPrintLine = 0;


	//////////////////////////////////////////////////////////////
	// ������ ���ǽ��� �����.
	// �ϴ� �����.
	SAFE_DELETE( m_pSelectionOn3Surf );
	SAFE_DELETE( m_pSelectionOn2Surf );
	SAFE_DELETE( m_pSelectionOn1Surf );
	SAFE_DELETE( m_pSelection3Surf );
	SAFE_DELETE( m_pSelection2Surf );
	SAFE_DELETE( m_pSelection1Surf );
	SAFE_DELETE( m_pClickSurf );

	m_SelectionLineCount = 0;

	// ������ �ѱ�� Ŭ�� ���ǽ�.
	g_pDisplay->CreateSurfaceFromText( &m_pClickSurf, m_Font, "CLICK!", RGB(0, 0, 0), RGB(0, 255, 0) );
	if( m_pClickSurf )
		m_pClickSurf->SetColorKey(RGB(0, 0, 0));
	g_pDisplay->CreateSurfaceFromText( &m_pClickOnSurf, m_Font, "CLICK!", RGB(0, 0, 0), RGB(255, 255, 0) );
	if( m_pClickOnSurf )
		m_pClickOnSurf->SetColorKey(RGB(0, 0, 0));


	// �켱�� ������ 1
	if( m_pNowScene->m_strSelection1 && strlen(m_pNowScene->m_strSelection1) )
	{
		// Ư���� ������ ������ üũ�Ѵ�.
		if( strncmp(m_pNowScene->m_strSelection1,"<ENTER>",7) == 0 ) // Ŭ������ �ѱ��.
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
		else if( strncmp(m_pNowScene->m_strSelection1,"<NEXT>",6) == 0 ) // �ڵ� �ѱ��
		{
			// �׳� ���ǽ� �� ����� �Ѿ��.
		}
		else if( strncmp(m_pNowScene->m_strSelection1,"<END>",5) == 0 ) // ��ũ��Ʈ ����.
		{
			// �׳� ���ǽ� �� ����� �Ѿ��.
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



	// Ÿ�� ��� üũ.
	if( m_pNowScene->m_WaitTime > 0 )
	{
		m_Waiting = timeGetTime() + (m_pNowScene->m_WaitTime*1000); // ������ Ǯ�� �ð��� ���.
	}

	// �������� ���׼� üũ
	if( m_pNowScene->m_WaitAction >= 0 && m_Action.ReadyAction(m_pNowScene->m_WaitAction) )
	{
		/*
				if( m_pNowScene->m_WaitAction >= m_ActionFuncNum ) // ����� �ȵ� �׼� �Լ��� �� ����.
				{
					char strTemp[256];
					sprintf( strTemp, "%d Action Number is wrong!", m_pNowScene->m_WaitAction );
					MessageBox( NULL, strTemp, "Warring", MB_OK );

					return;
				}
		*/
		m_ScriptState = EVENT_SCRIPT_STATE_WAITACTION; // ��� �׼��̳� ���̵尡 ������ �ٷ� ��������...
		m_pNowScene->m_SceneStartTick = timeGetTime(); // ���� �ٲ� �ð��� ���.

		return; // ���⼭ �ߴ� ���Ѽ� �׼��� �����Ų��.
	}


	// ���̵��� üũ. (���� ���̵� ���� �ִٸ� ���̵����� ������ ������ Ÿ�Ӵ��� �׼Ǵ�⸦ �ѹ� �� üũ�Ѵ�.)
	if( m_pNowScene->m_FadeInSpeed ) // ���̵� ���� �Ǿ��ϴ� �ð����� ������ �ٲ��.(ms ����)
	{
		m_Fading = -m_pNowScene->m_FadeInSpeed*100; // ms ������ ƽ���� �ٲ۴�. -�� ���̵� ������ ǥ��.
		m_ScriptState = EVENT_SCRIPT_STATE_FADING; // ���̵� ���� ��ȯ.
		m_pNowScene->m_SceneStartTick = timeGetTime(); // ���̵� ���� �ð��� ����ϱ� ���� ���.

		return; // ���⼭ �ߴ� ���Ѽ� ���̵带 �����Ų��.
	}

	m_ScriptState = EVENT_SCRIPT_STATE_RUN; // ��� �׼��̳� ���̵尡 ������ �ٷ� ��������...
	m_pNowScene->m_SceneStartTick = timeGetTime(); // ���� �ٲ� �ð��� ���.


}

void CEventScriptActor::RunScript() // ��ũ��Ʈ�� ����
{
	if( m_Waiting ) // �̰� 0�� �ƴϸ� ���ð��� �ִ°Ŵ�.
	{
		if( timeGetTime() >= m_Waiting ) // ����� ƽ �ð����� ��ŵ
		{
			m_Waiting = 0;
		}

		m_pNowScene->m_SceneStartTick = timeGetTime(); // ������ �ȵǰ� �ð��� ����ش�.

		return; // ��µ� ���Ѵ�.
	}

	switch( m_ScriptState )
	{
	case EVENT_SCRIPT_STATE_RUN: // ���� ��
	case EVENT_SCRIPT_STATE_WAITCLICK: // ���� �����
	case EVENT_SCRIPT_STATE_PAGEWAIT: // ������ �ѱ�� �����.

		RenderScript(); // �׳� �Լ� �ϳ��� ������.

		break;

	case EVENT_SCRIPT_STATE_WAITACTION: // �̺�Ʈ ���� �����

		m_Action.m_pFunc->Render();

		break;

	case EVENT_SCRIPT_STATE_FADING: // �� ���̵�

		if( m_Fading < 0 ) // ���̵� �� ��.
		{
			DWORD Elapsed = timeGetTime()-m_pNowScene->m_SceneStartTick;
			int Fading = abs(m_Fading); // ����� �ٲٰ�.

			Fading -= Elapsed; // ����ð��� ����.

			int Alpha = 255-(int)(255.0f * ( (float)Fading/((float)m_pNowScene->m_FadeInSpeed*100.0f) ) );
			if( Alpha < 0 )
				Alpha = 0;
			if( Alpha > 255 )
				Alpha = 255;

			DWORD FadeColor = RGBA_MAKE( 0, 0, 0, Alpha );

			pCMyApp->m_pFxSet->RenderFadeScreen( FadeColor );

			if( Fading <= 0 ) // ���̵� �� ���� ���� üũ.
			{
				m_Fading = 0;
//				m_pNowScene->m_SceneStartTick = timeGetTime(); // ���� ���� �ð��� ���̵��� ���� �������� �������ش�.
//				m_ScriptState = EVENT_SCRIPT_STATE_RUN;

				return;
			}
		}
		else if( m_Fading > 0 ) // ���̵� �ƿ� ��.
		{
			DWORD Elapsed = timeGetTime()-m_pNowScene->m_SceneStartTick;
			int Fading = abs(m_Fading); // ����� �ٲٰ�.

			Fading -= Elapsed; // ����ð��� ����.

			int Alpha = (int)(255.0f * ( (float)Fading/((float)m_pNowScene->m_FadeOutSpeed*100.0f) ) );
			if( Alpha < 0 )
				Alpha = 0;
			if( Alpha > 255 )
				Alpha = 255;

			DWORD FadeColor = RGBA_MAKE( 0, 0, 0, Alpha );


			pCMyApp->m_pFxSet->RenderFadeScreen( FadeColor );

			if( Fading <= 0 ) // ���̵� �ƿ� ���� ���� üũ.
			{
				m_Fading = 0;
				SceneChange( m_ReserveNextScent, TRUE ); // ���� ������ �̵����� �ش�. �ι�° ���ڸ� �༭ �ٽ� ���̵� �ƿ��� �ݺ����� �ʵ��� �����.
				return;
			}
		}
		else // 0�̸� ����. �ٽ� RUN ������Ʈ�� �ٲ��ش�.
		{
			m_ScriptState = EVENT_SCRIPT_STATE_RUN;
			m_pNowScene->m_SceneStartTick = timeGetTime(); // ���� ���� �ð��� ���̵��� ���� �������� �������ش�.

			// ���̵� ���� �־��� ��� ���̵� ���� ������ �ٽ� �ѹ� Ÿ�Ӵ��� �׼Ǵ�⸦ üũ���ش�.
			// Ÿ�� ��� üũ.
			if(	m_pNowScene->m_WaitTime > 0 )
			{
				m_Waiting = timeGetTime() + (m_pNowScene->m_WaitTime*1000); // ������ Ǯ�� �ð��� ���.
			}

			// �������� ���׼� üũ
			if( m_pNowScene->m_WaitAction >= 0 )
			{
//				if( !SetWaitAction(m_pNowScene->m_WaitAction) )
				{
					// ���� �����ߴٸ�...

				}
			}
		}

		break;
	}
}

void CEventScriptActor::RenderScript() // ��ũ��Ʈ ���.
{
	if( !g_pDisplay )
		return;

	if( !m_pNowScene ) // Ȥ�ö� ���ٸ�...
		return;

	DWORD dwElapsedTick = timeGetTime() - m_pNowScene->m_SceneStartTick; // ���� ���۵��� ����� ������ ���Ѵ�.

	int i = 0;
	D3DTLVERTEX TLVec[4];
	DWORD color;
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;
	DWORD  dwTextureState[3];

	// ���°� ����.
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


	// ���â�� ���
	color = RGBA_MAKE( 255, 255, 255, 255 );

	TLVec[0] = D3DTLVERTEX( D3DVECTOR(m_BoxArea.left,m_BoxArea.top+m_BoxArea.bottom,0), 1, color, 0, 0, 1.0f );
	TLVec[1] = D3DTLVERTEX( D3DVECTOR(m_BoxArea.left,m_BoxArea.top,0), 1, color, 0, 0, 0 );
	TLVec[2] = D3DTLVERTEX( D3DVECTOR(m_BoxArea.left+m_BoxArea.right,m_BoxArea.top,0), 1, color, 0, 1.0f, 0 );
	TLVec[3] = D3DTLVERTEX( D3DVECTOR(m_BoxArea.left+m_BoxArea.right,m_BoxArea.top+m_BoxArea.bottom,0), 1, color, 0, 1.0f, 1.0f );

	GET_D3DDEVICE()->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx(0) ); // ó�� �༮�� ���â
	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);

	// �״��� �̹��� ����Ʈ ���
	RECT rect;
	sImageOutInfo *pImageOut;
	list<sImageOutInfo*>::iterator prImageOut; // �������� üũ�ؼ� �����ҵ�.
	for( prImageOut = m_pNowScene->m_listImageOut.begin(); prImageOut != m_pNowScene->m_listImageOut.end(); ++prImageOut )
	{
		pImageOut = *prImageOut;

		// �� �̹����� �� �˻�����.
		if( dwElapsedTick >= pImageOut->m_CreateTime && (pImageOut->m_CreateTime>=pImageOut->m_DelTime || dwElapsedTick <= pImageOut->m_DelTime+(pImageOut->m_FadeOutSpeed*100))  ) // ����ؾ��ϴ� �ð����̸�...(���� ���۰� ���� Ÿ���� ���ų� ����Ÿ���� �� ������ ��� �� ������� Ÿ���̴�.)
		{
			rect.left = pImageOut->m_PosX;
			rect.top = pImageOut->m_PosY;
			rect.right = pImageOut->m_Width;
			rect.bottom = pImageOut->m_Height;

			Convert640Pos( &rect );

			int Alpha = 255;

			if( pImageOut->m_FadeInSpeed > 0 && dwElapsedTick >= pImageOut->m_CreateTime
					&& dwElapsedTick <= pImageOut->m_CreateTime+(pImageOut->m_FadeInSpeed*100)  ) // ���̵� �� Ÿ�̹�.
			{
				Alpha = (int)( 255.0f*((float)(dwElapsedTick-pImageOut->m_CreateTime)/((float)pImageOut->m_FadeInSpeed*100.0f)) );
			}
			else if( pImageOut->m_FadeOutSpeed > 0 && pImageOut->m_DelTime > pImageOut->m_CreateTime
					 && dwElapsedTick >= pImageOut->m_DelTime && dwElapsedTick <= pImageOut->m_DelTime+(pImageOut->m_FadeOutSpeed*100) ) // ���̵� �ƿ� Ÿ�̹�.
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

			GET_D3DDEVICE()->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx(pImageOut->m_ContainerIndex) ); // ó�� �༮�� ���â
			GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
		}
	}


	// �״��� �ؽ�Ʈ ��� ���.(�ؽ�Ʈ�� ���� �������� ����ϴ� ������ �ļ��� ���â �ٲٱ⸦ �����ϰ� �ϱ� ���ؼ�)
	// �켱 �߽� ���.
	// ���� ����ؾ��� ���ڿ� ������ ���Ѵ�.
	int PrintedLen = 0; // ���ݱ��� ��µ� ���ڼ�.(������Ų��.)
	int PrintedLine = 0;

	// ��� ����� ������ ���� ������ ���ش�.
	for( i = 0 ; i < m_NowPrintLine ; ++i ) // ���� ������� �� ���� �� ��.
	{
		if( i >= m_NowViewLine ) // �������� ���κ��� ���. ������� �ʾƵ� ���� ���ڼ� ������ ���Ѿ��Ѵ�.
		{
			if( m_pContantsOutSurf[i] )
				g_pDisplay->Blt( m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT, m_TextArea.top+SCRIPT_TEXT_SPACE_TOP+(PrintedLine*SCRIPT_USE_LINE_HEIGHT), m_pContantsOutSurf[i] );

			++PrintedLine;
		}

		PrintedLen += strlen( m_ContantsText[i] ); // ��µ� ���ڼ��� ������Ų��.
	}

	// ���� ��� ���� ���� ���.
	if( m_ScriptState == EVENT_SCRIPT_STATE_RUN && m_NowPrintLine < m_ContantsLineCount ) // ���� ������ ���α�����... // �ִ� �ټ� �̻��̶�� ���δ� ��µȰŴ�. �� ��� ������ �ٵ� ������ ��µȴ�.
	{
		// ���� ���� ��ġ�� ����غ���.
		SIZE FontSize;
		if( m_pContantsOutSurf[m_NowPrintLine] )
			getFontSize( &FontSize, m_Font, (LPCTSTR)m_pContantsOutSurf[m_NowPrintLine], m_NowPrintChar ); // ���� ����� ��ġ������...
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

		if( m_pNowScene->m_TextSpeed > 0 ) // �̰� ���� 0�̻��϶��� ���ڴ� ����Ѵ�.
		{
			DWORD ElapsedPerLine = dwElapsedTick - ( PrintedLen * m_pNowScene->m_TextSpeed ); // �� ������ ���۵� Ÿ���� ���Ѵ�.
			m_NowPrintChar = ElapsedPerLine / m_pNowScene->m_TextSpeed;

			// ���� �Ǵ�.
			if( m_NowPrintChar > strlen(m_ContantsText[m_NowPrintLine]) ) // ���� ����� ���ߴٸ�..
			{
				m_NowPrintLine++; // ���ε� ����.
				m_NowPrintChar = 0;

				if( m_NowPrintLine >= m_ContantsLineCount ) // �� ����̸� Ŭ������...
				{
					m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
					m_NowPrintChar = -1; // �� ��µ����� ǥ��.
					m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
				}
				else // ������ �ѱ��� Ȯ��.
				{
					if( (m_NowPrintLine-m_NowViewLine) >= m_OnePageLine ) // �켱 �������� �Ѱܾ��ϴ��� ���θ�  Ȯ���Ѵ�.
					{
						// �������� �Ѱܾ� �ϴ� ���.
						m_dwStopTick = timeGetTime();
						m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
					}
					else if( m_ContantsLineCount - m_NowPrintLine <= m_OnePageLine ) // ��� ���������...
					{
						if( (m_NowPrintLine-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // �߰��� �������� �������� �������� �Ѱܾ��ϴ��� Ȯ���غ���.(������ ���μ��� +1)�Կ� ����.(�ֳĸ� m_OnePageLine�� �Ѷ��� �̸� ����Ǿ��ִ�.)
						{
							// �� ��쵵 �������� �Ѱ��ش�.
							m_dwStopTick = timeGetTime();
							m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
						}
					}
				}

				/*
				if( (m_NowPrintLine-m_NowViewLine) >= m_OnePageLine ) // �켱 �������� �Ѱܾ��ϴ��� ���θ�  Ȯ���Ѵ�.
				{
					// �������� �Ѱܾ� �ϴ� ���.
					m_dwStopTick = timeGetTime();
					m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
				}
				else if( m_ContantsLineCount - m_NowPrintLine <= m_OnePageLine ) // ��� ���������...
				{
					if( (m_NowPrintLine-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // �߰��� �������� �������� �������� �Ѱܾ��ϴ��� Ȯ���غ���.(������ ���μ��� +1)�Կ� ����.(�ֳĸ� m_OnePageLine�� �Ѷ��� �̸� ����Ǿ��ִ�.)
					{
						// �� ��쵵 �������� �Ѱ��ش�.
						m_dwStopTick = timeGetTime();
						m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
					}
					else if( m_NowPrintLine >= m_ContantsLineCount ) // �� ����̸� Ŭ������...
					{
						m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
						m_NowPrintChar = -1; // �� ��µ����� ǥ��.
						m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
					}
				}
				*/
			}
		}
		else // ���� ���ǵ� ������ ����� �ȵǾ��ִٸ�.
		{
			m_NowPrintLine++;

			if( (m_NowPrintLine-m_NowViewLine) > m_OnePageLine ) // �켱 �������� �Ѱܾ��ϴ��� ���θ�  Ȯ���Ѵ�.
			{
				// �������� �Ѱܾ� �ϴ� ���.
				m_dwStopTick = timeGetTime();
				m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
			}
			else if( m_ContantsLineCount - m_NowPrintLine <= m_OnePageLine ) // ��� ���������...
			{
				if( (m_NowPrintLine-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // �߰��� �������� �������� �������� �Ѱܾ��ϴ��� Ȯ���غ���.(������ ���μ��� +1)�Կ� ����.(�ֳĸ� m_OnePageLine�� �Ѷ��� �̸� ����Ǿ��ִ�.)
				{
					// �� ��쵵 �������� �Ѱ��ش�.
					m_dwStopTick = timeGetTime();
					m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
				}
				else if( m_NowPrintLine >= m_ContantsLineCount ) // �� ����̸� Ŭ������...
				{
					m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
					m_NowPrintChar = -1; // �� ��µ����� ǥ��.
					m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
				}
			}
		}
	}
	else // ������ ����µ� ���¿���..
	{
		// �� ��찡 � ��� ������ �𸣰ڴµ�. ������ �ѱ� üũ���ؼ� �� �ǳʶ�� ��찡 �ִ���. �׷��� ����.
		if( m_ScriptState == EVENT_SCRIPT_STATE_RUN ) // ���� ��������� �ȳѾ�ٸ�....�̷� ��쵵 �ִ�.
		{
			if( (m_NowPrintLine-m_NowViewLine) > m_OnePageLine ) // �켱 �������� �Ѱܾ��ϴ��� ���θ�  Ȯ���Ѵ�.
			{
				// �������� �Ѱܾ� �ϴ� ���.
				m_dwStopTick = timeGetTime();
				m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
			}
			else if( m_ContantsLineCount - m_NowPrintLine <= m_OnePageLine ) // ��� ���������...
			{
				if( (m_NowPrintLine-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // �߰��� �������� �������� �������� �Ѱܾ��ϴ��� Ȯ���غ���.(������ ���μ��� +1)�Կ� ����.(�ֳĸ� m_OnePageLine�� �Ѷ��� �̸� ����Ǿ��ִ�.)
				{
					// �� ��쵵 �������� �Ѱ��ش�.
					m_dwStopTick = timeGetTime();
					m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
				}
				else if( m_NowPrintLine >= m_ContantsLineCount ) // �� ����̸� Ŭ������...
				{
					m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
					m_NowPrintChar = -1; // �� ��µ����� ǥ��.
					m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
				}
			}
		}
	}

	/*  // ������ ������ ������ ������ �ټ��ֵ��� ����.
		for( i = 0 ; i < m_NowPrintLine ; ++i ) // ���� ������� �� ���� �� ��.
		{
			if( m_pContantsOutSurf[i] )
				g_pDisplay->Blt( m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT, m_TextArea.top+SCRIPT_TEXT_SPACE_TOP+(i*SCRIPT_USE_LINE_HEIGHT), m_pContantsOutSurf[i] );

			PrintedLen += strlen( m_ContantsText[i] ); // ��µ� ���ڼ��� ������Ų��.
		}

		// ���� ��� ���� ���� ���.
		if( m_NowPrintLine < m_ContantsLineCount ) // �ִ� �ټ� �̻��̶�� ���δ� ��µȰŴ�. �� ��� ������ �ٵ� ������ ��µȴ�.
		{
			// ���� ���� ��ġ�� ����غ���.
			SIZE FontSize;
			getFontSize( &FontSize, m_Font, (LPCTSTR)m_pContantsOutSurf[m_NowPrintLine], m_NowPrintChar ); // ���� ����� ��ġ������...

			RECT DrawLineArea;
			DrawLineArea.left = 0;
			DrawLineArea.top = 0;
			DrawLineArea.right = FontSize.cx;
			DrawLineArea.bottom = FontSize.cy;


			if( m_pContantsOutSurf[m_NowPrintLine] )
				g_pDisplay->Blt( m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT, m_TextArea.top+SCRIPT_TEXT_SPACE_TOP+(m_NowPrintLine*SCRIPT_USE_LINE_HEIGHT), m_pContantsOutSurf[m_NowPrintLine], &DrawLineArea );

			if( m_pNowScene->m_TextSpeed > 0 ) // �̰� ���� 0�̻��϶��� ���ڴ� ����Ѵ�.
			{
				DWORD ElapsedPerLine = dwElapsedTick - ( PrintedLen * m_pNowScene->m_TextSpeed ); // �� ������ ���۵� Ÿ���� ���Ѵ�.
				m_NowPrintChar = ElapsedPerLine / m_pNowScene->m_TextSpeed;

				// ���� �Ǵ�.
				if( m_NowPrintChar > strlen(m_ContantsText[m_NowPrintLine]) ) // ���� ����� ���ߴٸ�..
				{
					m_NowPrintLine++; // ���ε� ����.
					m_NowPrintChar = 0;

					if( m_NowPrintLine >= m_ContantsLineCount )
					{
						m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
						m_NowPrintChar = -1; // �� ��µ����� ǥ��.
						m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
					}
				}
			}
			else // ���� ���ǵ� ������ ����� �ȵǾ��ִٸ�.
			{
				// �ٷ� ���� ��µǰ� �Ѵ�.
				m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
				m_NowPrintChar = -1; // �� ��µ����� ǥ��.
				m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
			}
		}
		else // ������ ����µ� ���¿���..
		{
			if( m_ScriptState == EVENT_SCRIPT_STATE_RUN ) // ���� ��������� �ȳѾ�ٸ�....�̷� ��쵵 �ִ�.
			{
				m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
				m_NowPrintChar = -1; // �� ��µ����� ǥ��.
				m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
			}
		}
	*/

	if( m_ScriptState == EVENT_SCRIPT_STATE_PAGEWAIT )
	{
		/*		if( m_pNowScene->m_TextSpeed > 0 ) // �̰� ���� 0�̻��϶��� �ڵ����� �Ѱ��ش�.
				{
					if( timeGetTime()-m_dwStopTick > 1000 ) // �ణ ��ٷȴٰ� �Ѱ��ش�.
					{
						m_NowViewLine = m_NowPrintLine; // ������� ����ߴ� ���� +1 �� ��ġ��.
						m_pNowScene->m_SceneStartTick += timeGetTime()-m_dwStopTick; // ��ٸ� ��ŭ ƽ�� �ǵ����ش�.

						m_ScriptState = EVENT_SCRIPT_STATE_RUN;
					}
				}
				else // ���ڵ� ��� ��� �� ��� Ŭ������ �Ѿ�� ���ش�.
				{
		*/			// �� �ϴܿ� Ŭ���� �����̰� �Ѵ�.
		if( m_pClickSurf && m_pClickOnSurf ) // ���� �Ʒ��� ���� �˻�.
		{
			m_pClickSurf->Xpos = m_TextArea.left+m_TextArea.right-SCRIPT_TEXT_SPACE_LEFT-50;
			m_pClickSurf->Ypos = m_TextArea.top+m_TextArea.bottom-SCRIPT_TEXT_SPACE_LEFT-SCRIPT_USE_LINE_HEIGHT;

			if( (timeGetTime()/200) % 2 ) // �����̰�...
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

	// ������ ���.
	if( m_ScriptState == EVENT_SCRIPT_STATE_WAITCLICK )
	{
		// Ư�� ������ üũ.
		if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<ENTER>",7) == 0 ) // Ŭ������ �ѱ��.
		{
			// �� �ϴܿ� Ŭ���� �����̰� �Ѵ�.
			if( m_pClickSurf && m_pClickOnSurf ) // ���� �Ʒ��� ���� �˻�.
			{
				m_pClickSurf->Xpos = m_TextArea.left+m_TextArea.right-SCRIPT_TEXT_SPACE_LEFT-50;
				m_pClickSurf->Ypos = m_TextArea.top+m_TextArea.bottom-SCRIPT_TEXT_SPACE_LEFT-SCRIPT_USE_LINE_HEIGHT;

				if( (timeGetTime()/200) % 2 ) // �����̰�...
				{
					g_pDisplay->Blt( m_pClickSurf->Xpos, m_pClickSurf->Ypos, m_pClickOnSurf );
				}
				else
				{
					g_pDisplay->Blt( m_pClickSurf->Xpos, m_pClickSurf->Ypos, m_pClickSurf );
				}

			}
		}
		else if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<NEXT>",6) == 0 ) // �ڵ� �ѱ��
		{
//			Sleep( 2000 ); // �ٷ� �Ѿ�� �ʹ� �����ϱ� �ణ �����ش�.
			SceneChange( m_pNowScene->m_LinkSel1 ); // ���� ������.
		}
		else if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<END>",5) == 0 ) // ��ũ��Ʈ ����.
		{
//			Sleep( 3000 ); // �ٷ� �Ѿ�� �ʹ� �����ϱ� �ణ �����ش�.
			SceneChange( -1 ); //��ũ��Ʈ ����
		}
		else // Ư�� �������� �ƴҶ� �Ϲ� �������� ����Ѵ�.
		{
			int OutSelY = m_TextArea.bottom-SCRIPT_TEXT_SPACE_TOP-SCRIPT_USE_LINE_HEIGHT; // �����ġ ����.

			if( m_pSelection3Surf && m_pSelectionOn3Surf ) // ���� �Ʒ��� ���� �˻�.
			{
				m_pSelection3Surf->Xpos = m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT;
				m_pSelection3Surf->Ypos = m_TextArea.top+OutSelY;

				if( m_NowMouseOnSel == 3 ) // Ȥ�� ���콺�� �ö� �����ΰ� üũ.
				{
					g_pDisplay->Blt( m_pSelection3Surf->Xpos, m_pSelection3Surf->Ypos, m_pSelectionOn3Surf );
				}
				else
				{
					g_pDisplay->Blt( m_pSelection3Surf->Xpos, m_pSelection3Surf->Ypos, m_pSelection3Surf );
				}

				OutSelY -= SCRIPT_USE_LINE_HEIGHT;
			}

			if( m_pSelection2Surf && m_pSelectionOn2Surf ) // ���� �Ʒ��� ���� �˻�.
			{
				m_pSelection2Surf->Xpos = m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT;
				m_pSelection2Surf->Ypos = m_TextArea.top+OutSelY;

				if( m_NowMouseOnSel == 2 ) // Ȥ�� ���콺�� �ö� �����ΰ� üũ.
				{
					g_pDisplay->Blt( m_pSelection2Surf->Xpos, m_pSelection2Surf->Ypos, m_pSelectionOn2Surf );
				}
				else
				{
					g_pDisplay->Blt( m_pSelection2Surf->Xpos, m_pSelection2Surf->Ypos, m_pSelection2Surf );
				}

				OutSelY -= SCRIPT_USE_LINE_HEIGHT;
			}

			if( m_pSelection1Surf && m_pSelectionOn1Surf ) // ���� �Ʒ��� ���� �˻�.
			{
				m_pSelection1Surf->Xpos = m_TextArea.left+SCRIPT_TEXT_SPACE_LEFT;
				m_pSelection1Surf->Ypos = m_TextArea.top+OutSelY;

				if( m_NowMouseOnSel == 1 ) // Ȥ�� ���콺�� �ö� �����ΰ� üũ.
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

BOOL CEventScriptActor::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) // �޼��� ���ν���.
{
	int x, y;
	int result = 0;

	switch( m_ScriptState )
	{
	case EVENT_SCRIPT_STATE_RUN: // ���� ��

		// Ŭ���ϸ� ���� ���� ��ŵ.
		if( msg == WM_LBUTTONDOWN )
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( IsInsideTalkBox(x,y) ) // ��� â�� Ŭ��������...
			{
				if( (m_ContantsLineCount-m_NowViewLine) > m_OnePageLine ) // �켱 �������� �Ѱܾ��ϴ��� ���θ�  Ȯ���Ѵ�.
				{
					// �������� �Ѱܾ� �ϴ� ���.
//					m_NowPrintLine = (m_ContantsLineCount-m_NowViewLine)-1;
					m_NowPrintLine = m_NowViewLine+m_OnePageLine;
					m_dwStopTick = timeGetTime();
					m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
					return 1;
				}
				else
				{
					if( (m_ContantsLineCount-m_NowViewLine)+(m_SelectionLineCount-1) > m_OnePageLine ) // �߰��� �������� �������� �������� �Ѱܾ��ϴ��� Ȯ���غ���.(������ ���μ��� +1)�Կ� ����.(�ֳĸ� m_OnePageLine�� �Ѷ��� �̸� ����Ǿ��ִ�.)
					{
						// �� ��쵵 �������� �Ѱ��ش�.
//						m_NowPrintLine = (m_ContantsLineCount-m_NowViewLine)+(m_SelectionLineCount-1)-1;
						m_NowPrintLine = m_NowViewLine+m_OnePageLine;
						m_dwStopTick = timeGetTime();
						m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)

						return 1;
					}
				}

				// �� ���� ���� �ٷ� ���� ��µǰ� �Ѵ�.
				m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
				m_NowPrintChar = -1; // �� ��µ����� ǥ��.
				m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)


				/*
								if( (m_NowPrintLine-m_NowViewLine) >= m_OnePageLine ) // ������ �������� �ƴѰ��.
								{
									m_dwStopTick = timeGetTime();
									m_ScriptState = EVENT_SCRIPT_STATE_PAGEWAIT; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
								}
								else // ������ �������� ���.
								{
									m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
									m_NowPrintChar = -1; // �� ��µ����� ǥ��.
									m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
								}
				*/
				/*
								// �ٷ� ���� ��µǰ� �Ѵ�.
								m_NowPrintLine = m_ContantsLineCount; // �̷��� �ϸ� ���� �������� ���� ��µǰ� ���̻� ����� �ȵ��´�.
								m_NowPrintChar = -1; // �� ��µ����� ǥ��.
								m_ScriptState = EVENT_SCRIPT_STATE_WAITCLICK; // ���¸� Ŭ�� ��������� ���ش�.(����� ���Ծ��� �޽��� ���ν����� �ٸ��� �������ش�.)
				*/
			}
		}

		break;

	case EVENT_SCRIPT_STATE_PAGEWAIT:

		if( msg == WM_LBUTTONDOWN )
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( IsInsideTalkBox(x,y) /*&& m_pNowScene->m_TextSpeed <= 0*/ ) // ��� â�� Ŭ��������...  (���� �带�� �ڵ����� �Ѿ��.)
			{
				m_NowViewLine = m_NowPrintLine; // ������� ����ߴ� ���� +1 �� ��ġ��.
				m_pNowScene->m_SceneStartTick += timeGetTime()-m_dwStopTick; // ��ٸ� ��ŭ ƽ�� �ǵ����ش�.

				m_ScriptState = EVENT_SCRIPT_STATE_RUN;
			}
		}

		break;

	case EVENT_SCRIPT_STATE_WAITACTION: // �̺�Ʈ ���� �����

//		result = ActionFunc[m_pNowScene->m_WaitAction].MsgProcFunc( hWnd, msg, wParam, lParam );
		result = m_Action.m_pFunc->MsgProc( hWnd, msg, wParam, lParam );
		/*
				if( result <= 0 ) // 0 ���ϸ� ����.
				{
					m_ScriptState = EVENT_SCRIPT_STATE_RUN; // �� ��������...
					m_pNowScene->m_SceneStartTick = timeGetTime(); // ���� �ٲ� �ð��� ���.
				}
		*/
		if( m_Action.m_pFunc->EndCheck() ) // ���� ���� üũ.
		{
			m_Action.ReleaseAction( m_pNowScene->m_WaitAction ); // �޸� ������ ���ش�.

			m_ScriptState = EVENT_SCRIPT_STATE_RUN; // �� ��������...
			m_pNowScene->m_SceneStartTick = timeGetTime(); // ���� �ٲ� �ð��� ���.
		}

		return result;


	case EVENT_SCRIPT_STATE_WAITCLICK: // ���� �����

		// �ӽ� �׽�Ʈ�� Ŭ���ϸ� ������ 1�� ������ ������ �ѱ�� ó��.
		// Ŭ���ϸ� ���� ���� ��ŵ.
		if( msg == WM_MOUSEMOVE )
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			m_NowMouseOnSel = IsInsideSelection(x,y); // ������ ���� �� ���� üũ. ������ üũ.
		}
		else if( msg == WM_LBUTTONDOWN )
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<ENTER>",7) == 0 ) // Ŭ������ �ѱ��.
			{
				if( IsInsideTalkBox(x,y) ) // ��� â�� Ŭ��������...
				{
					SceneChange( m_pNowScene->m_LinkSel1 ); // �ӽ� �׽�Ʈ.
				}
			}
			/*
						else if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<END>",5) == 0 ) // Ŭ������ ����.
						{
			//				if( IsInsideTalkBox(x,y) ) // ��� â�� Ŭ��������...
							{
								SceneChange( -1 ); // �ӽ� �׽�Ʈ.
							}
						}
			*/
			else // �ƴϸ� ������ üũ
			{
				m_NowMouseOnSel = IsInsideSelection(x,y); // ������ ���� �� ���� üũ. ������ üũ.
				if( m_NowMouseOnSel > 0 ) // �������� Ŭ��������...
				{
					if( m_NowMouseOnSel == 1 )
						SceneChange( m_pNowScene->m_LinkSel1 ); // 1�� ������ ��ũ�� ����
					else if( m_NowMouseOnSel == 2 )
						SceneChange( m_pNowScene->m_LinkSel2 ); // 2�� ������ ��ũ�� ����
					else if( m_NowMouseOnSel == 3 )
						SceneChange( m_pNowScene->m_LinkSel3 ); // 3�� ������ ��ũ�� ����
				}
			}
		}

		break;

	case EVENT_SCRIPT_STATE_FADING: // �� ���̵�
		break;
	}

	return TRUE; // �ٸ� �޼����� �� �޾��ش�.
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
	// ������ 1 üũ.
	if( m_pSelection1Surf )
	{
		if( m_pNowScene->m_strSelection1 && strncmp(m_pNowScene->m_strSelection1,"<ENTER>",7) == 0 ) // Ŭ������ �ѱ��.
		{
		}
		else // �ƴҶ��� üũ
		{
			if ( ( ( x > m_pSelection1Surf->Xpos) && ( x < (m_pSelection1Surf->Xpos + m_pSelection1Surf->GetWidth())) )
					&& ( ( y >m_pSelection1Surf->Ypos ) && ( y < (m_pSelection1Surf->Ypos + m_pSelection1Surf->GetHeight()) ) ) )
				return 1;
		}
	}
	// ������ 2 üũ.
	if( m_pSelection2Surf )
	{
		if ( ( ( x > m_pSelection2Surf->Xpos) && ( x < (m_pSelection2Surf->Xpos + m_pSelection2Surf->GetWidth())) )
				&& ( ( y >m_pSelection2Surf->Ypos ) && ( y < (m_pSelection2Surf->Ypos + m_pSelection2Surf->GetHeight()) ) ) )
			return 2;
	}

	// ������ 3 üũ.
	if( m_pSelection3Surf )
	{
		if ( ( ( x > m_pSelection3Surf->Xpos) && ( x < (m_pSelection3Surf->Xpos + m_pSelection3Surf->GetWidth())) )
				&& ( ( y >m_pSelection3Surf->Ypos ) && ( y < (m_pSelection3Surf->Ypos + m_pSelection3Surf->GetHeight()) ) ) )
			return 3;
	}

	return 0;
}

void Convert640Pos( RECT *Rect ) // 640*480 �ػ� ������ ��ǥ�� ���� �ػ󵵷� �����ý����ش�.
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
// �׼� �Լ��� ���� R�� ������ �Լ�. M�� �޽��� ���ν��� �Լ�.
void SA_TutorialStart_R( DWORD StartTick )	 // 0 : Ʃ�丮�� ���� �κ��� �̺�Ʈ ��
{
	if( !pCMyApp || !pCMyApp->m_pUIMgr || !pCMyApp->m_pUIMgr->m_pEventScript || !pCMyApp->m_pUIMgr->m_pEventScript->m_pNowScene )
		return -1;

	sSceneData pNowScene = pCMyApp->m_pUIMgr->m_pEventScript->m_pNowScene;
	DWORD Elapsed = timeGetTime() - pNowScene->m_SceneStartTick;


	CNkMob MobWolf;
	MobWolf.SetInfo(23,0);
	CNkMob MobTiger;
	MobWolf.SetInfo(24,1);



	if( Elapsed < 10000 ) // �� ���� �ʹ�. ������ �������ϴ� ������
	{


	}
	else if( Elapsed >= 10000 && Elapsed < 15000 ) // ���� ���� ���ϴ� ������
	{

	}
	else // ������ �ٰ���.
	{

	}


	return;
}

int SA_TutorialStart_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )	 // 0 : Ʃ�丮�� ���� �κ��� �̺�Ʈ ��
{
	if( !pCMyApp || !pCMyApp->m_pUIMgr || !pCMyApp->m_pUIMgr->m_pEventScript || !pCMyApp->m_pUIMgr->m_pEventScript->m_pNowScene )
		return -1;

	sSceneData pNowScene = pCMyApp->m_pUIMgr->m_pEventScript->m_pNowScene;
	// �� �׼��� �ڵ� ���� ���̹Ƿ� ���� �ð��� �Ǹ� ��������ش�.
	if( timeGetTime() - pNowScene->m_SceneStartTick > 20000 )
		return 0;

	return 1;
}



void SA_TutorialMove_R( DWORD StartTick )	 // 1 : Ʃ�丮�� �� �̵� �ǽ�.
{

	return;
}

int SA_TutorialMove_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )	 // 0 : Ʃ�丮�� ���� �κ��� �̺�Ʈ ��
{
	return 1;
}



void SA_TutorialRotate_R( DWORD StartTick )	 // 2 : Ʃ�丮�� �� ȸ�� �ǽ�.
{

	return;
}

int SA_TutorialRotate_M( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )	 // 0 : Ʃ�丮�� ���� �κ��� �̺�Ʈ ��
{
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

*/