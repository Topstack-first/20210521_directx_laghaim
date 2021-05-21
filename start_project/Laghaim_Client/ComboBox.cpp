#include "stdafx.h"


#include "ComboBox.h"
#include "headers.h"

extern HFONT G_GetFont(int h, int w,const char  * const fontname);


void CComboBox::LoadRes( char *ListViewBtnName, char *SelectedBackGroundName, char *ListBackGroundName )
// 사이즈 셋팅은 이미지 로드 전에 이루어져야한다.
{
	if( !g_pDisplay )
		return;

	char TempString1[100];

	if( ListViewBtnName == NULL ) // 리스트 다운 버튼의 경우 파일명 지정이 없으면 디폴트로 다른 그림을 선택시켜준다.
	{
		strcpy( TempString1, "cb_arrowdown" );
		ListViewBtnName = TempString1;
	}

	m_pListViewBtn = new CBasicButton();
	if( m_pListViewBtn )
	{
		m_pListViewBtn->SetFileName( ListViewBtnName );
		m_pListViewBtn->SetDisable( false );
		m_pListViewBtn->LoadRes();
	}
	else // 메모리 부족.
		MessageBox( NULL, "Not enough memory! in ComboBox LoadRes", "Error", MB_OK );

	if( m_pSelectedBackGround && SelectedBackGroundName == NULL ) // 이미지 지정이 없으면 사이즈에 맞춰서 검은색 사각 박스를 생성.
	{
		g_pDisplay->CreateSurface( &m_pSelectedBackGround, m_ItemWidth+m_X_Draw_Plus+m_X_Draw_Plus, m_ItemHeight+m_Y_Draw_Plus+m_Y_Draw_Plus ); // 이 녀석의 사이즈는 미리 알수있다.

		m_pSelectedBackGround->SetWidth( m_ItemWidth+m_X_Draw_Plus+m_X_Draw_Plus ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
		m_pSelectedBackGround->SetHeight( m_ItemHeight+m_Y_Draw_Plus+m_Y_Draw_Plus );

		// fillcolor를 이용한 textsurface 클리어하기
		m_pSelectedBackGround->Clear( RGB(0,0,0) );
	}
	else
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pSelectedBackGround, SelectedBackGroundName, 0, 0 ); // 이렇게 생성하면 m_dwWidth,m_dwHeight 값을 제대로 입력해둔다.

		if( m_pSelectedBackGround )
			m_pSelectedBackGround->SetColorKey( RGB(0, 0, 0) );
		else
		{
			sprintf( TempString1, "%s File Not Found", SelectedBackGroundName );
			MessageBox( NULL, TempString1, "Error", MB_OK );
			return;
		}
	}

	if( m_pListBackGround && ListBackGroundName == NULL ) // 이미지 지정이 없으면 사이즈에 맞춰서 검은색 반투명 사각 박스를 생성.
	{
		g_pDisplay->CreateSurface( &m_pListBackGround, m_ItemWidth, 100 ); // 세로 사이즈는 임의로..(어차피 입력될 아이템 갯수를 모르니 사이즈는 리사이즈로 출력해야한다.)

		m_pListBackGround->SetWidth( m_ItemWidth+m_X_Draw_Plus+m_X_Draw_Plus ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
		m_pListBackGround->SetHeight( m_ItemHeight+m_Y_Draw_Plus+m_Y_Draw_Plus );

		// fillcolor를 이용한 textsurface 클리어하기
		m_pListBackGround->Clear( RGB(0,0,0) );
	}
	else
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pListBackGround, ListBackGroundName, 0, 0 );

		if( m_pListBackGround )
			m_pListBackGround->SetColorKey( RGB(0, 0, 0) );
		else
		{
			sprintf( TempString1, "%s File Not Found", ListBackGroundName );
			MessageBox( NULL, TempString1, "Error", MB_OK );
			return;
		}
	}
}

void CComboBox::DeleteRes()
{
	Init(); // 이안에 지우는 루틴 다 있다.
}


int CComboBox::InsertItem( char* ItemText, int FontSize, DWORD TextColor, DWORD OverColor, DWORD BackColor )
{
	if( !g_pDisplay )
		return -1;
	if( m_InsertedCount >= MAX_COMBO_LIST ) // 리스트 입력 한도에 걸렸으면...
	{
		MessageBox( NULL, "I have exceeded the limit I can put in the combo box.", "Combo Box Error", MB_OK );
		return -1;
	}

	DWORD TransColor = RGB( 0, 0, 0 ); // Clear 함수의 16비트 해상도 고려안한 버그때문에 투명색은 0을 써야한다.
	HFONT Font = G_GetFont( FontSize, 0, "굴림체" ); // 이거 "굴림체" 부분은 무시하면 된다. 그냥 알아서 각 국가별 지정 폰트로 로드하더라.

	SAFE_DELETE( m_pListItem[m_InsertedCount] );

	g_pDisplay->CreateSurface( &m_pListItem[m_InsertedCount], m_ItemWidth, m_ItemHeight );

	if( m_pListItem[m_InsertedCount] )
	{
		m_pListItem[m_InsertedCount]->SetWidth( m_ItemWidth ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
		m_pListItem[m_InsertedCount]->SetHeight( m_ItemHeight );
		m_pListItem[m_InsertedCount]->Clear( TransColor ); // 한번 투명색으로 깔끔하게 지운다. // Clear함수 16비트에서 오작동한다. 클리어 하지 마라 -_-
		m_pListItem[m_InsertedCount]->DrawText( Font, ItemText, m_X_Draw_Plus, m_Y_Draw_Plus, BackColor, TextColor,0 );
		m_pListItem[m_InsertedCount]->SetColorKey( TransColor );
	}
	else
	{
		MessageBox( NULL, "Out of memory! in ComboBox InsertItem", "Error", MB_OK );
		return -1;
	}

	SAFE_DELETE( m_pListOverItem[m_InsertedCount] );

	g_pDisplay->CreateSurface( &m_pListOverItem[m_InsertedCount], m_ItemWidth, m_ItemHeight );

	if( m_pListOverItem[m_InsertedCount] )
	{
		m_pListOverItem[m_InsertedCount]->SetWidth( m_ItemWidth ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
		m_pListOverItem[m_InsertedCount]->SetHeight( m_ItemHeight );
		m_pListOverItem[m_InsertedCount]->Clear( TransColor ); // 한번 투명색으로 깔끔하게 지운다. // Clear함수 16비트에서 오작동한다. 클리어 하지 마라 -_-
		m_pListOverItem[m_InsertedCount]->DrawText( Font, ItemText, m_X_Draw_Plus, m_Y_Draw_Plus, BackColor, OverColor, 0 );
		m_pListOverItem[m_InsertedCount]->SetColorKey(TransColor);
	}
	else
	{
		MessageBox( NULL, "Out of memory! in ComboBox InsertItem", "Error", MB_OK );
		return -1;
	}

	++m_InsertedCount;

	return m_InsertedCount;
}

int CComboBox::InsertItemByImage( char* ImageFileName ) // 이미지로 삽입.
{
	if( m_InsertedCount >= MAX_COMBO_LIST ) // 리스트 입력 한도에 걸렸으면...
		MessageBox( NULL, "I have exceeded the limit I can put in the combo box.", "Combo Box Error", MB_OK );

	if( ImageFileName == NULL )
		return -1;

	char TempString1[100] = {0,};
	char TempString2[100] = {0,};

	//sprintf(TempString1, "%s_normal.bmp", ImageFileName);
	//sprintf(TempString2, "%s_Over.bmp", ImageFileName);

	strcat( TempString1, ImageFileName );
	strcat( TempString1, "_normal.bmp" );

	strcat( TempString2, ImageFileName );
	strcat( TempString2, "_Over.bmp" );

	SAFE_DELETE( m_pListItem[m_InsertedCount] );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pListItem[m_InsertedCount], TempString1, 0, 0 );
	if( m_pListItem[m_InsertedCount] )
		m_pListItem[m_InsertedCount]->SetColorKey( RGB(0, 0, 0) );
	else
	{
		sprintf( TempString2, "%s File Not Found", TempString1 );
		MessageBox( NULL, TempString2, "Error", MB_OK );
		return -1;
	}

	g_pDisplay->CreateSurfaceFromBitmap( &m_pListOverItem[m_InsertedCount], TempString2, 0, 0 );
	if( m_pListOverItem[m_InsertedCount] )
		m_pListOverItem[m_InsertedCount]->SetColorKey( RGB(0, 0, 0) );
	else
	{
		sprintf( TempString1, "%s File Not Found", TempString2 );
		MessageBox( NULL, TempString1, "Error", MB_OK );
		return -1;
	}

	++m_InsertedCount;

	return m_InsertedCount;
}

void CComboBox::DeleteAllItem()
{
	int i = 0;

	// Init()으로 초기화 하면 안된다.(사이즈등은 유지되어야 하니까.)
	for( i = 0 ; i < m_InsertedCount ; ++i )
	{
		SAFE_DELETE( m_pListItem[i] );
		SAFE_DELETE( m_pListOverItem[i] );
	}

	m_InsertedCount = 0;
}

int CComboBox::IsInsideList( int x, int y )
// 리스트 클릭시...몇번을 클릭했는지를 리턴(미선택시 0리턴, 배열상에서 0은 디폴트 아이템이므로 리스트에선 보여지지 않는다.)
{
	if( m_ItemHeight == 0 ) // 이 경우 뻑나니까 그냥 리턴...
		return 0;

	if(   x > m_pListBackGround->Xpos
			&& x < m_pListBackGround->Xpos+(int)m_pListBackGround->GetWidth()
			&& y > m_pListBackGround->Ypos
			&& y < m_pListBackGround->Ypos+(int)m_pListBackGround->GetHeight()  ) // 각 버튼 위치와 크기로 직접 체크. 이래야 좀더 정확하게 나올것같다.
	{
		// 몇번째 아이템이 선택되었는지 를 계산한다.
		for( int i = 1 ; i < m_InsertedCount ; ++i ) // 0번은 제외함에 유의.
		{
			if( m_pListItem[i] )
			{
				if(   x > m_pListItem[i]->Xpos
						&& x < m_pListItem[i]->Xpos+(int)m_pListItem[i]->GetWidth()
						&& y > m_pListItem[i]->Ypos
						&& y < m_pListItem[i]->Ypos+(int)m_pListItem[i]->GetHeight()  ) // 각 버튼 위치와 크기로 직접 체크. 이래야 좀더 정확하게 나올것같다.
					return i;
			}
		}

		return -1; // 리스트 영역은 맞는데 안에 아이템의 안은 아닐경우.( 다른 메세지 무시를 위해 필요하다 )
	}

	return 0;
}

LRESULT CComboBox::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);

	int SelItemNumber = 0; // 선택된 아이템 번호 저장용.

	switch( msg )
	{
	case WM_LBUTTONDOWN :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // 이건 여기서 해줘야 버튼 이미지 전환이 정상작동한다.

		if( m_State == COMBO_STATE_SELECT ) // 리스트에서 고르기 상태일때만 처리하는 녀석들.
		{
			if( IsInsideList(x,y) ) // 혹시 리스트 안쪽을 클린한거면 다른 메세지를 못받게 해야한다.(밑에 토글버튼 처럼 WM_LBUTTONDOWN 시에 뭔가를 처리하는 경우 밑에서 받아버린다.
				return 1;
		}

		break;

	case WM_LBUTTONUP :

		if( m_State == COMBO_STATE_NORMAL ) // 리스트에서 고르기 상태일때만 처리하는 녀석들.
		{
			if( m_pListViewBtn )
				m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // 이건 여기서 해줘야 버튼 이미지 전환이 정상작동한다.

			if( m_pListViewBtn->GetState() == BTN_ACTION )
			{
				m_State = COMBO_STATE_ANIMATE;
				m_AnimateHeight = 0; // 이걸 초기화해줘야 애니메이트가 된다.
				return 1; // 다른 메세지 프로시져를 무시시킨다.
			}
		}
		else if( m_State == COMBO_STATE_SELECT ) // 리스트에서 고르기 상태일때만 처리하는 녀석들.
		{
			SelItemNumber = IsInsideList( x, y ); // 선택된 아이템의 번호를 받아온다.

			if( SelItemNumber > 0 ) // 영역안 선택.
			{
				m_NowSelectIndex = SelItemNumber; // 선택된 번호를 갱신
				m_State = COMBO_STATE_NORMAL;
				return 1; // 다른 녀석들은 메세지 못받게 하기
			}
			else if( SelItemNumber == -1 ) // 아이템은 선택안했지만 리스트 배경은 선택한 경우.
				return 1; // 이렇게 해야 다른 녀석들의 WM_MOUSEMOVE 가 작동 못하게 막을수있다.
			else // 영역외 선택.
				m_State = COMBO_STATE_NORMAL;

		}

		break;

	case WM_MOUSEMOVE :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // 버튼 그림 바뀌는 것 때문에라도 이게 필요한듯.

		if( m_State == COMBO_STATE_SELECT ) // 리스트에서 고르기 상태일때만 처리하는 녀석들.
		{
			SelItemNumber = IsInsideList( x, y ); // 선택된 아이템의 번호를 받아온다.

			if( SelItemNumber ) // 0이면 영역외 선택.
			{
				m_NowOverIndex = SelItemNumber; // 마우스가 올라가있는 번호를 갱신
				return 1; // 다른 녀석들은 메세지 못받게 하기
			}
		}

		break;

	default :
		break;
	}

	return 0; // 리턴 0이면 다른 메세지 프로시져로 넘겨준다고 보면 된다.
}

int CComboBox::IsInsideListImage( int x, int y )
// 리스트 클릭시...몇번을 클릭했는지를 리턴(미선택시 0리턴, 배열상에서 0은 디폴트 아이템이므로 리스트에선 보여지지 않는다.)
{
	if( m_ItemHeight == 0 ) // 이 경우 뻑나니까 그냥 리턴...
		return 0;

	if(   x > m_pListBackGround->Xpos
			&& x < m_pListBackGround->Xpos+(int)m_pListBackGround->GetWidth()
			&& y > m_pListBackGround->Ypos
			&& y < m_pListBackGround->Ypos+(int)m_pListBackGround->GetHeight()  ) // 각 버튼 위치와 크기로 직접 체크. 이래야 좀더 정확하게 나올것같다.
	{

		// 몇번째 아이템이 선택되었는지 를 계산한다.
		for( int i = 0 ; i < m_InsertedCount ; ++i ) // 0번은 제외함에 유의.
		{
			if( m_pListItem[i] )
			{
				if(   x > m_pListItem[i]->Xpos
						&& x < m_pListItem[i]->Xpos+(int)m_pListItem[i]->GetWidth()
						&& y > m_pListItem[i]->Ypos
						&& y < m_pListItem[i]->Ypos+(int)m_pListItem[i]->GetHeight()  ) // 각 버튼 위치와 크기로 직접 체크. 이래야 좀더 정확하게 나올것같다.
					return i;
			}
		}

		return -1; // 리스트 영역은 맞는데 안에 아이템의 안은 아닐경우.( 다른 메세지 무시를 위해 필요하다 )
	}

	return 0;
}

LRESULT CComboBox::MsgProcImage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);

	int SelItemNumber = 0; // 선택된 아이템 번호 저장용.

	switch( msg )
	{
	case WM_LBUTTONDOWN :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // 이건 여기서 해줘야 버튼 이미지 전환이 정상작동한다.

		if( m_State == COMBO_STATE_SELECT ) // 리스트에서 고르기 상태일때만 처리하는 녀석들.
		{
			if( IsInsideListImage(x,y) != -1 ) // 혹시 리스트 안쪽을 클린한거면 다른 메세지를 못받게 해야한다.(밑에 토글버튼 처럼 WM_LBUTTONDOWN 시에 뭔가를 처리하는 경우 밑에서 받아버린다.
				return 1;
		}

		break;

	case WM_LBUTTONUP :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // 이건 여기서 해줘야 버튼 이미지 전환이 정상작동한다.
		if( m_State == COMBO_STATE_NORMAL ) // 리스트에서 고르기 상태일때만 처리하는 녀석들.
		{

			if( m_pListViewBtn->GetState() == BTN_ACTION )
			{
				m_State = COMBO_STATE_ANIMATE;
				m_AnimateHeight = 0; // 이걸 초기화해줘야 애니메이트가 된다.
				return 1; // 다른 메세지 프로시져를 무시시킨다.
			}
		}
		else if( m_State == COMBO_STATE_SELECT ) // 리스트에서 고르기 상태일때만 처리하는 녀석들.
		{
			SelItemNumber = IsInsideListImage( x, y ); // 선택된 아이템의 번호를 받아온다.

			if( SelItemNumber >= 0 ) // 영역안 선택.
			{
				m_NowSelectIndex = SelItemNumber; // 선택된 번호를 갱신
				m_State = COMBO_STATE_NORMAL;
				return 1; // 다른 녀석들은 메세지 못받게 하기
			}
			else if( SelItemNumber == -1 ) // 아이템은 선택안했지만 리스트 배경은 선택한 경우.
				return 1; // 이렇게 해야 다른 녀석들의 WM_MOUSEMOVE 가 작동 못하게 막을수있다.
			else // 영역외 선택.
				m_State = COMBO_STATE_NORMAL;

		}

		break;

	case WM_MOUSEMOVE :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // 버튼 그림 바뀌는 것 때문에라도 이게 필요한듯.

		if( m_State == COMBO_STATE_SELECT ) // 리스트에서 고르기 상태일때만 처리하는 녀석들.
		{
			SelItemNumber = IsInsideListImage( x, y ); // 선택된 아이템의 번호를 받아온다.

			if( SelItemNumber >= 0 ) // 0이면 영역외 선택.
			{
				m_NowOverIndex = SelItemNumber; // 마우스가 올라가있는 번호를 갱신
				return 1; // 다른 녀석들은 메세지 못받게 하기
			}
		}

		break;

	default :
		break;
	}

	return 0; // 리턴 0이면 다른 메세지 프로시져로 넘겨준다고 보면 된다.
}

void CComboBox::DrawImage() // 리스트의 x좌표 보정치를 인자로 받는다.(위쪽 테두리 고려하는 보정치)
{
	if( !g_pDisplay )
		return;

	if( m_dwElapsedTick == 0 ) // 처음 왔으면..
		m_dwElapsedTick = timeGetTime(); // 처음엔 받고 시작.

	m_dwElapsedTick = timeGetTime() - m_dwElapsedTick; // 이전 프레임에서 지나간 시간.

	if( m_pSelectedBackGround ) // 선택된 아이템을 찍어주는 배경. 항상 출력.
		g_pDisplay->Blt( m_DrawPosX, m_DrawPosY, m_pSelectedBackGround );

	if( m_pListViewBtn ) // 리스트 다운(리스트 보이기)버튼. 항상 추력.
	{
		if( m_State == COMBO_STATE_DISABLE ) // 비활성 모드일땐...
			m_pListViewBtn->Draw( m_nBtnPosX, m_nBtnPosY, TRUE ); // 비 활성 그림으로 출력.
		else
			m_pListViewBtn->Draw( m_nBtnPosX, m_nBtnPosY);
	}

	if( m_State == COMBO_STATE_NORMAL || m_State == COMBO_STATE_DISABLE ) // 일반모드 출력(비활성일때도 선택된 아이템은 그려야한다.)
	{
		// 선택된 아이템을 그린다.
		if( m_NowSelectIndex >= 0 && m_pListItem[m_NowSelectIndex] )
			g_pDisplay->Blt( m_DrawPosX+m_X_Draw_Plus, m_DrawPosY+m_Y_Draw_Plus, m_pListItem[m_NowSelectIndex] );
	}
	else if( m_State == COMBO_STATE_SELECT ) // 고르기모드 출력(리스트를 그린다.)
	{
		if( m_pListBackGround ) // 리스트의 배경.
		{
			m_pListBackGround->Xpos = m_DrawPosX+m_X_Draw_Plus;
			m_pListBackGround->Ypos = m_DrawPosY+m_pSelectedBackGround->GetHeight(); // 출력 위치를 객체의 변수에 저장해둔다.(평소엔 저장 안되더라.) 범위체크시에 쓴다.
			g_pDisplay->Blt( m_pListBackGround->Xpos, m_pListBackGround->Ypos, m_pListBackGround );
		}

		for( int i = 0 ; i < m_InsertedCount ; ++i ) // 0번은 제외함에 유의.
		{
			if( m_pListItem[i] )
			{
				m_pListItem[i]->Xpos = m_pListBackGround->Xpos+1; // 몇 픽셀정도는 예의상 보정.(리스트 배경에도 테두리가 있을수있으니까.)
				m_pListItem[i]->Ypos = m_pListBackGround->Ypos+1+((i)*m_ItemHeight); // 출력 위치를 객체의 변수에 저장해둔다.(평소엔 저장 안되더라.) 범위체크시에 쓴다.

				if( i == m_NowOverIndex ) // 마우스가 올려져있는 녀석이면 다른 이미지를 출력
					g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListOverItem[i] );
				else
					g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListItem[i] );
			}
		}
	}
	else if( m_State == COMBO_STATE_ANIMATE ) // 리스트 전개 모드.(배경만 전개한다.)
	{
		if( m_pListBackGround ) // 리스트의 배경이 있고
		{
			if( ((int)m_AnimateHeight < (int)m_pListBackGround->GetHeight()) )// 아직 다 전개가 안되었으면...
				m_AnimateHeight += ((float)m_dwElapsedTick * 0.001f) * ( /*(float)m_pListBackGround->GetHeight() **/ 250.0f );
			else // 전개가 끝났으면...
			{
				m_AnimateHeight = (float)m_pListBackGround->GetHeight(); // 마지막 출력때문에 정확하게 맞춰야한다.
				m_State = COMBO_STATE_SELECT; // 선택 모드로 바꿔둔다.(바꾸고 한번 더 찍어줘야 텀이 없음에 유의.)
			}

			m_pListBackGround->Xpos = m_DrawPosX+m_X_Draw_Plus;
			m_pListBackGround->Ypos = m_DrawPosY+m_pSelectedBackGround->GetHeight(); // 출력 위치를 객체의 변수에 저장해둔다.(평소엔 저장 안되더라.) 범위체크시에 쓴다.

			g_pDisplay->Blt( m_pListBackGround->Xpos, m_pListBackGround->Ypos
							 , m_pListBackGround->Xpos+m_pListBackGround->GetWidth()
							 , m_pListBackGround->Ypos+(int)m_AnimateHeight, m_pListBackGround );

			for( int i = 0 ; i < m_InsertedCount ; ++i ) // 0번은 제외함에 유의.
			{
				if( m_pListItem[i] )
				{
					m_pListItem[i]->Xpos = m_pListBackGround->Xpos+1; // 몇 픽셀정도는 예의상 보정.(리스트 배경에도 테두리가 있을수있으니까.)
					m_pListItem[i]->Ypos = m_pListBackGround->Ypos+1+((i)*m_ItemHeight); // 출력 위치를 객체의 변수에 저장해둔다.(평소엔 저장 안되더라.) 범위체크시에 쓴다.

					if( m_pListBackGround->Ypos+(int)m_AnimateHeight >= m_pListItem[i]->Ypos+(int)m_pListItem[i]->GetHeight() ) // 완전 다 보이기.
						g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListItem[i] );
					else if( m_pListBackGround->Ypos+(int)m_AnimateHeight > m_pListItem[i]->Ypos ) // 일부 보이는 위치이면...
					{
						g_pDisplay->Blt( m_pListItem[i]->Xpos
										 , m_pListItem[i]->Ypos
										 , m_pListItem[i]->Xpos+m_ItemWidth
										 , m_pListBackGround->Ypos+(int)m_AnimateHeight
										 , m_pListItem[i] );
					}
				}
			}
		}
		else // 만약 배경이 없으면...
			m_State = COMBO_STATE_SELECT; // 바로 선택 모드로...
	}

	m_dwElapsedTick = timeGetTime(); // 시간을 기억해둔다. 여기서 이걸 해줘야 위쪽에서 구할때 제대로 구해짐.
}


void CComboBox::Draw() // 리스트의 x좌표 보정치를 인자로 받는다.(위쪽 테두리 고려하는 보정치)
{
	if( !g_pDisplay )
		return;

	if( m_dwElapsedTick == 0 ) // 처음 왔으면..
		m_dwElapsedTick = timeGetTime(); // 처음엔 받고 시작.

	m_dwElapsedTick = timeGetTime() - m_dwElapsedTick; // 이전 프레임에서 지나간 시간.

	if( m_pSelectedBackGround ) // 선택된 아이템을 찍어주는 배경. 항상 출력.
		g_pDisplay->Blt( m_DrawPosX, m_DrawPosY, m_pSelectedBackGround );

	if( m_pListViewBtn ) // 리스트 다운(리스트 보이기)버튼. 항상 추력.
	{
		if( m_State == COMBO_STATE_DISABLE ) // 비활성 모드일땐...
			m_pListViewBtn->Draw( m_DrawPosX+m_pSelectedBackGround->GetWidth(), m_DrawPosY, TRUE ); // 비 활성 그림으로 출력.
		else
			m_pListViewBtn->Draw( m_DrawPosX+m_pSelectedBackGround->GetWidth(), m_DrawPosY );
	}

	if( m_State == COMBO_STATE_NORMAL || m_State == COMBO_STATE_DISABLE ) // 일반모드 출력(비활성일때도 선택된 아이템은 그려야한다.)
	{
		// 선택된 아이템을 그린다.
		if( m_NowSelectIndex >= 0 && m_pListItem[m_NowSelectIndex] )
			g_pDisplay->Blt( m_DrawPosX+m_X_Draw_Plus, m_DrawPosY+m_Y_Draw_Plus, m_pListItem[m_NowSelectIndex] );
	}
	else if( m_State == COMBO_STATE_SELECT ) // 고르기모드 출력(리스트를 그린다.)
	{
		if( m_pListBackGround ) // 리스트의 배경.
		{
			m_pListBackGround->Xpos = m_DrawPosX+m_X_Draw_Plus;
			m_pListBackGround->Ypos = m_DrawPosY+m_pSelectedBackGround->GetHeight(); // 출력 위치를 객체의 변수에 저장해둔다.(평소엔 저장 안되더라.) 범위체크시에 쓴다.
			g_pDisplay->Blt( m_pListBackGround->Xpos, m_pListBackGround->Ypos, m_pListBackGround );
		}

		for( int i = 1 ; i < m_InsertedCount ; ++i ) // 0번은 제외함에 유의.
		{
			if( m_pListItem[i] )
			{
				m_pListItem[i]->Xpos = m_pListBackGround->Xpos+1; // 몇 픽셀정도는 예의상 보정.(리스트 배경에도 테두리가 있을수있으니까.)
				m_pListItem[i]->Ypos = m_pListBackGround->Ypos+1+((i-1)*m_ItemHeight); // 출력 위치를 객체의 변수에 저장해둔다.(평소엔 저장 안되더라.) 범위체크시에 쓴다.

				if( i == m_NowOverIndex ) // 마우스가 올려져있는 녀석이면 다른 이미지를 출력
					g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListOverItem[i] );
				else
					g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListItem[i] );
			}
		}
	}
	else if( m_State == COMBO_STATE_ANIMATE ) // 리스트 전개 모드.(배경만 전개한다.)
	{
		if( m_pListBackGround ) // 리스트의 배경이 있고
		{
			if( ((int)m_AnimateHeight < (int)m_pListBackGround->GetHeight()) )// 아직 다 전개가 안되었으면...
				m_AnimateHeight += ((float)m_dwElapsedTick * 0.001f) * ( /*(float)m_pListBackGround->GetHeight() **/ 250.0f );
			else // 전개가 끝났으면...
			{
				m_AnimateHeight = (float)m_pListBackGround->GetHeight(); // 마지막 출력때문에 정확하게 맞춰야한다.
				m_State = COMBO_STATE_SELECT; // 선택 모드로 바꿔둔다.(바꾸고 한번 더 찍어줘야 텀이 없음에 유의.)
			}

			m_pListBackGround->Xpos = m_DrawPosX+m_X_Draw_Plus;
			m_pListBackGround->Ypos = m_DrawPosY+m_pSelectedBackGround->GetHeight(); // 출력 위치를 객체의 변수에 저장해둔다.(평소엔 저장 안되더라.) 범위체크시에 쓴다.

			g_pDisplay->Blt( m_pListBackGround->Xpos, m_pListBackGround->Ypos
							 , m_pListBackGround->Xpos+m_pListBackGround->GetWidth()
							 , m_pListBackGround->Ypos+(int)m_AnimateHeight, m_pListBackGround );

			for( int i = 1 ; i < m_InsertedCount ; ++i ) // 0번은 제외함에 유의.
			{
				if( m_pListItem[i] )
				{
					m_pListItem[i]->Xpos = m_pListBackGround->Xpos+1; // 몇 픽셀정도는 예의상 보정.(리스트 배경에도 테두리가 있을수있으니까.)
					m_pListItem[i]->Ypos = m_pListBackGround->Ypos+1+((i-1)*m_ItemHeight); // 출력 위치를 객체의 변수에 저장해둔다.(평소엔 저장 안되더라.) 범위체크시에 쓴다.

					if( m_pListBackGround->Ypos+(int)m_AnimateHeight >= m_pListItem[i]->Ypos+(int)m_pListItem[i]->GetHeight() ) // 완전 다 보이기.
						g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListItem[i] );
					else if( m_pListBackGround->Ypos+(int)m_AnimateHeight > m_pListItem[i]->Ypos ) // 일부 보이는 위치이면...
					{
						g_pDisplay->Blt( m_pListItem[i]->Xpos
										 , m_pListItem[i]->Ypos
										 , m_pListItem[i]->Xpos+m_ItemWidth
										 , m_pListBackGround->Ypos+(int)m_AnimateHeight
										 , m_pListItem[i] );
					}
				}
			}
		}
		else // 만약 배경이 없으면...
			m_State = COMBO_STATE_SELECT; // 바로 선택 모드로...
	}

	m_dwElapsedTick = timeGetTime(); // 시간을 기억해둔다. 여기서 이걸 해줘야 위쪽에서 구할때 제대로 구해짐.
}
