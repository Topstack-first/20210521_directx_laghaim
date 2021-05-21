#include "stdafx.h"


#include "ComboBox.h"
#include "headers.h"

extern HFONT G_GetFont(int h, int w,const char  * const fontname);


void CComboBox::LoadRes( char *ListViewBtnName, char *SelectedBackGroundName, char *ListBackGroundName )
// ������ ������ �̹��� �ε� ���� �̷�������Ѵ�.
{
	if( !g_pDisplay )
		return;

	char TempString1[100];

	if( ListViewBtnName == NULL ) // ����Ʈ �ٿ� ��ư�� ��� ���ϸ� ������ ������ ����Ʈ�� �ٸ� �׸��� ���ý����ش�.
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
	else // �޸� ����.
		MessageBox( NULL, "Not enough memory! in ComboBox LoadRes", "Error", MB_OK );

	if( m_pSelectedBackGround && SelectedBackGroundName == NULL ) // �̹��� ������ ������ ����� ���缭 ������ �簢 �ڽ��� ����.
	{
		g_pDisplay->CreateSurface( &m_pSelectedBackGround, m_ItemWidth+m_X_Draw_Plus+m_X_Draw_Plus, m_ItemHeight+m_Y_Draw_Plus+m_Y_Draw_Plus ); // �� �༮�� ������� �̸� �˼��ִ�.

		m_pSelectedBackGround->SetWidth( m_ItemWidth+m_X_Draw_Plus+m_X_Draw_Plus ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
		m_pSelectedBackGround->SetHeight( m_ItemHeight+m_Y_Draw_Plus+m_Y_Draw_Plus );

		// fillcolor�� �̿��� textsurface Ŭ�����ϱ�
		m_pSelectedBackGround->Clear( RGB(0,0,0) );
	}
	else
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pSelectedBackGround, SelectedBackGroundName, 0, 0 ); // �̷��� �����ϸ� m_dwWidth,m_dwHeight ���� ����� �Է��صд�.

		if( m_pSelectedBackGround )
			m_pSelectedBackGround->SetColorKey( RGB(0, 0, 0) );
		else
		{
			sprintf( TempString1, "%s File Not Found", SelectedBackGroundName );
			MessageBox( NULL, TempString1, "Error", MB_OK );
			return;
		}
	}

	if( m_pListBackGround && ListBackGroundName == NULL ) // �̹��� ������ ������ ����� ���缭 ������ ������ �簢 �ڽ��� ����.
	{
		g_pDisplay->CreateSurface( &m_pListBackGround, m_ItemWidth, 100 ); // ���� ������� ���Ƿ�..(������ �Էµ� ������ ������ �𸣴� ������� ��������� ����ؾ��Ѵ�.)

		m_pListBackGround->SetWidth( m_ItemWidth+m_X_Draw_Plus+m_X_Draw_Plus ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
		m_pListBackGround->SetHeight( m_ItemHeight+m_Y_Draw_Plus+m_Y_Draw_Plus );

		// fillcolor�� �̿��� textsurface Ŭ�����ϱ�
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
	Init(); // �̾ȿ� ����� ��ƾ �� �ִ�.
}


int CComboBox::InsertItem( char* ItemText, int FontSize, DWORD TextColor, DWORD OverColor, DWORD BackColor )
{
	if( !g_pDisplay )
		return -1;
	if( m_InsertedCount >= MAX_COMBO_LIST ) // ����Ʈ �Է� �ѵ��� �ɷ�����...
	{
		MessageBox( NULL, "I have exceeded the limit I can put in the combo box.", "Combo Box Error", MB_OK );
		return -1;
	}

	DWORD TransColor = RGB( 0, 0, 0 ); // Clear �Լ��� 16��Ʈ �ػ� ������� ���׶����� ������� 0�� ����Ѵ�.
	HFONT Font = G_GetFont( FontSize, 0, "����ü" ); // �̰� "����ü" �κ��� �����ϸ� �ȴ�. �׳� �˾Ƽ� �� ������ ���� ��Ʈ�� �ε��ϴ���.

	SAFE_DELETE( m_pListItem[m_InsertedCount] );

	g_pDisplay->CreateSurface( &m_pListItem[m_InsertedCount], m_ItemWidth, m_ItemHeight );

	if( m_pListItem[m_InsertedCount] )
	{
		m_pListItem[m_InsertedCount]->SetWidth( m_ItemWidth ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
		m_pListItem[m_InsertedCount]->SetHeight( m_ItemHeight );
		m_pListItem[m_InsertedCount]->Clear( TransColor ); // �ѹ� ��������� ����ϰ� �����. // Clear�Լ� 16��Ʈ���� ���۵��Ѵ�. Ŭ���� ���� ���� -_-
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
		m_pListOverItem[m_InsertedCount]->SetWidth( m_ItemWidth ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
		m_pListOverItem[m_InsertedCount]->SetHeight( m_ItemHeight );
		m_pListOverItem[m_InsertedCount]->Clear( TransColor ); // �ѹ� ��������� ����ϰ� �����. // Clear�Լ� 16��Ʈ���� ���۵��Ѵ�. Ŭ���� ���� ���� -_-
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

int CComboBox::InsertItemByImage( char* ImageFileName ) // �̹����� ����.
{
	if( m_InsertedCount >= MAX_COMBO_LIST ) // ����Ʈ �Է� �ѵ��� �ɷ�����...
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

	// Init()���� �ʱ�ȭ �ϸ� �ȵȴ�.(��������� �����Ǿ�� �ϴϱ�.)
	for( i = 0 ; i < m_InsertedCount ; ++i )
	{
		SAFE_DELETE( m_pListItem[i] );
		SAFE_DELETE( m_pListOverItem[i] );
	}

	m_InsertedCount = 0;
}

int CComboBox::IsInsideList( int x, int y )
// ����Ʈ Ŭ����...����� Ŭ���ߴ����� ����(�̼��ý� 0����, �迭�󿡼� 0�� ����Ʈ �������̹Ƿ� ����Ʈ���� �������� �ʴ´�.)
{
	if( m_ItemHeight == 0 ) // �� ��� �����ϱ� �׳� ����...
		return 0;

	if(   x > m_pListBackGround->Xpos
			&& x < m_pListBackGround->Xpos+(int)m_pListBackGround->GetWidth()
			&& y > m_pListBackGround->Ypos
			&& y < m_pListBackGround->Ypos+(int)m_pListBackGround->GetHeight()  ) // �� ��ư ��ġ�� ũ��� ���� üũ. �̷��� ���� ��Ȯ�ϰ� ���ðͰ���.
	{
		// ���° �������� ���õǾ����� �� ����Ѵ�.
		for( int i = 1 ; i < m_InsertedCount ; ++i ) // 0���� �����Կ� ����.
		{
			if( m_pListItem[i] )
			{
				if(   x > m_pListItem[i]->Xpos
						&& x < m_pListItem[i]->Xpos+(int)m_pListItem[i]->GetWidth()
						&& y > m_pListItem[i]->Ypos
						&& y < m_pListItem[i]->Ypos+(int)m_pListItem[i]->GetHeight()  ) // �� ��ư ��ġ�� ũ��� ���� üũ. �̷��� ���� ��Ȯ�ϰ� ���ðͰ���.
					return i;
			}
		}

		return -1; // ����Ʈ ������ �´µ� �ȿ� �������� ���� �ƴҰ��.( �ٸ� �޼��� ���ø� ���� �ʿ��ϴ� )
	}

	return 0;
}

LRESULT CComboBox::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);

	int SelItemNumber = 0; // ���õ� ������ ��ȣ �����.

	switch( msg )
	{
	case WM_LBUTTONDOWN :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // �̰� ���⼭ ����� ��ư �̹��� ��ȯ�� �����۵��Ѵ�.

		if( m_State == COMBO_STATE_SELECT ) // ����Ʈ���� ���� �����϶��� ó���ϴ� �༮��.
		{
			if( IsInsideList(x,y) ) // Ȥ�� ����Ʈ ������ Ŭ���ѰŸ� �ٸ� �޼����� ���ް� �ؾ��Ѵ�.(�ؿ� ��۹�ư ó�� WM_LBUTTONDOWN �ÿ� ������ ó���ϴ� ��� �ؿ��� �޾ƹ�����.
				return 1;
		}

		break;

	case WM_LBUTTONUP :

		if( m_State == COMBO_STATE_NORMAL ) // ����Ʈ���� ���� �����϶��� ó���ϴ� �༮��.
		{
			if( m_pListViewBtn )
				m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // �̰� ���⼭ ����� ��ư �̹��� ��ȯ�� �����۵��Ѵ�.

			if( m_pListViewBtn->GetState() == BTN_ACTION )
			{
				m_State = COMBO_STATE_ANIMATE;
				m_AnimateHeight = 0; // �̰� �ʱ�ȭ����� �ִϸ���Ʈ�� �ȴ�.
				return 1; // �ٸ� �޼��� ���ν����� ���ý�Ų��.
			}
		}
		else if( m_State == COMBO_STATE_SELECT ) // ����Ʈ���� ���� �����϶��� ó���ϴ� �༮��.
		{
			SelItemNumber = IsInsideList( x, y ); // ���õ� �������� ��ȣ�� �޾ƿ´�.

			if( SelItemNumber > 0 ) // ������ ����.
			{
				m_NowSelectIndex = SelItemNumber; // ���õ� ��ȣ�� ����
				m_State = COMBO_STATE_NORMAL;
				return 1; // �ٸ� �༮���� �޼��� ���ް� �ϱ�
			}
			else if( SelItemNumber == -1 ) // �������� ���þ������� ����Ʈ ����� ������ ���.
				return 1; // �̷��� �ؾ� �ٸ� �༮���� WM_MOUSEMOVE �� �۵� ���ϰ� �������ִ�.
			else // ������ ����.
				m_State = COMBO_STATE_NORMAL;

		}

		break;

	case WM_MOUSEMOVE :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // ��ư �׸� �ٲ�� �� �������� �̰� �ʿ��ѵ�.

		if( m_State == COMBO_STATE_SELECT ) // ����Ʈ���� ���� �����϶��� ó���ϴ� �༮��.
		{
			SelItemNumber = IsInsideList( x, y ); // ���õ� �������� ��ȣ�� �޾ƿ´�.

			if( SelItemNumber ) // 0�̸� ������ ����.
			{
				m_NowOverIndex = SelItemNumber; // ���콺�� �ö��ִ� ��ȣ�� ����
				return 1; // �ٸ� �༮���� �޼��� ���ް� �ϱ�
			}
		}

		break;

	default :
		break;
	}

	return 0; // ���� 0�̸� �ٸ� �޼��� ���ν����� �Ѱ��شٰ� ���� �ȴ�.
}

int CComboBox::IsInsideListImage( int x, int y )
// ����Ʈ Ŭ����...����� Ŭ���ߴ����� ����(�̼��ý� 0����, �迭�󿡼� 0�� ����Ʈ �������̹Ƿ� ����Ʈ���� �������� �ʴ´�.)
{
	if( m_ItemHeight == 0 ) // �� ��� �����ϱ� �׳� ����...
		return 0;

	if(   x > m_pListBackGround->Xpos
			&& x < m_pListBackGround->Xpos+(int)m_pListBackGround->GetWidth()
			&& y > m_pListBackGround->Ypos
			&& y < m_pListBackGround->Ypos+(int)m_pListBackGround->GetHeight()  ) // �� ��ư ��ġ�� ũ��� ���� üũ. �̷��� ���� ��Ȯ�ϰ� ���ðͰ���.
	{

		// ���° �������� ���õǾ����� �� ����Ѵ�.
		for( int i = 0 ; i < m_InsertedCount ; ++i ) // 0���� �����Կ� ����.
		{
			if( m_pListItem[i] )
			{
				if(   x > m_pListItem[i]->Xpos
						&& x < m_pListItem[i]->Xpos+(int)m_pListItem[i]->GetWidth()
						&& y > m_pListItem[i]->Ypos
						&& y < m_pListItem[i]->Ypos+(int)m_pListItem[i]->GetHeight()  ) // �� ��ư ��ġ�� ũ��� ���� üũ. �̷��� ���� ��Ȯ�ϰ� ���ðͰ���.
					return i;
			}
		}

		return -1; // ����Ʈ ������ �´µ� �ȿ� �������� ���� �ƴҰ��.( �ٸ� �޼��� ���ø� ���� �ʿ��ϴ� )
	}

	return 0;
}

LRESULT CComboBox::MsgProcImage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);

	int SelItemNumber = 0; // ���õ� ������ ��ȣ �����.

	switch( msg )
	{
	case WM_LBUTTONDOWN :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // �̰� ���⼭ ����� ��ư �̹��� ��ȯ�� �����۵��Ѵ�.

		if( m_State == COMBO_STATE_SELECT ) // ����Ʈ���� ���� �����϶��� ó���ϴ� �༮��.
		{
			if( IsInsideListImage(x,y) != -1 ) // Ȥ�� ����Ʈ ������ Ŭ���ѰŸ� �ٸ� �޼����� ���ް� �ؾ��Ѵ�.(�ؿ� ��۹�ư ó�� WM_LBUTTONDOWN �ÿ� ������ ó���ϴ� ��� �ؿ��� �޾ƹ�����.
				return 1;
		}

		break;

	case WM_LBUTTONUP :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // �̰� ���⼭ ����� ��ư �̹��� ��ȯ�� �����۵��Ѵ�.
		if( m_State == COMBO_STATE_NORMAL ) // ����Ʈ���� ���� �����϶��� ó���ϴ� �༮��.
		{

			if( m_pListViewBtn->GetState() == BTN_ACTION )
			{
				m_State = COMBO_STATE_ANIMATE;
				m_AnimateHeight = 0; // �̰� �ʱ�ȭ����� �ִϸ���Ʈ�� �ȴ�.
				return 1; // �ٸ� �޼��� ���ν����� ���ý�Ų��.
			}
		}
		else if( m_State == COMBO_STATE_SELECT ) // ����Ʈ���� ���� �����϶��� ó���ϴ� �༮��.
		{
			SelItemNumber = IsInsideListImage( x, y ); // ���õ� �������� ��ȣ�� �޾ƿ´�.

			if( SelItemNumber >= 0 ) // ������ ����.
			{
				m_NowSelectIndex = SelItemNumber; // ���õ� ��ȣ�� ����
				m_State = COMBO_STATE_NORMAL;
				return 1; // �ٸ� �༮���� �޼��� ���ް� �ϱ�
			}
			else if( SelItemNumber == -1 ) // �������� ���þ������� ����Ʈ ����� ������ ���.
				return 1; // �̷��� �ؾ� �ٸ� �༮���� WM_MOUSEMOVE �� �۵� ���ϰ� �������ִ�.
			else // ������ ����.
				m_State = COMBO_STATE_NORMAL;

		}

		break;

	case WM_MOUSEMOVE :

		if( m_pListViewBtn )
			m_pListViewBtn->MsgProc( hWnd, msg, wParam, lParam ); // ��ư �׸� �ٲ�� �� �������� �̰� �ʿ��ѵ�.

		if( m_State == COMBO_STATE_SELECT ) // ����Ʈ���� ���� �����϶��� ó���ϴ� �༮��.
		{
			SelItemNumber = IsInsideListImage( x, y ); // ���õ� �������� ��ȣ�� �޾ƿ´�.

			if( SelItemNumber >= 0 ) // 0�̸� ������ ����.
			{
				m_NowOverIndex = SelItemNumber; // ���콺�� �ö��ִ� ��ȣ�� ����
				return 1; // �ٸ� �༮���� �޼��� ���ް� �ϱ�
			}
		}

		break;

	default :
		break;
	}

	return 0; // ���� 0�̸� �ٸ� �޼��� ���ν����� �Ѱ��شٰ� ���� �ȴ�.
}

void CComboBox::DrawImage() // ����Ʈ�� x��ǥ ����ġ�� ���ڷ� �޴´�.(���� �׵θ� ����ϴ� ����ġ)
{
	if( !g_pDisplay )
		return;

	if( m_dwElapsedTick == 0 ) // ó�� ������..
		m_dwElapsedTick = timeGetTime(); // ó���� �ް� ����.

	m_dwElapsedTick = timeGetTime() - m_dwElapsedTick; // ���� �����ӿ��� ������ �ð�.

	if( m_pSelectedBackGround ) // ���õ� �������� ����ִ� ���. �׻� ���.
		g_pDisplay->Blt( m_DrawPosX, m_DrawPosY, m_pSelectedBackGround );

	if( m_pListViewBtn ) // ����Ʈ �ٿ�(����Ʈ ���̱�)��ư. �׻� �߷�.
	{
		if( m_State == COMBO_STATE_DISABLE ) // ��Ȱ�� ����϶�...
			m_pListViewBtn->Draw( m_nBtnPosX, m_nBtnPosY, TRUE ); // �� Ȱ�� �׸����� ���.
		else
			m_pListViewBtn->Draw( m_nBtnPosX, m_nBtnPosY);
	}

	if( m_State == COMBO_STATE_NORMAL || m_State == COMBO_STATE_DISABLE ) // �Ϲݸ�� ���(��Ȱ���϶��� ���õ� �������� �׷����Ѵ�.)
	{
		// ���õ� �������� �׸���.
		if( m_NowSelectIndex >= 0 && m_pListItem[m_NowSelectIndex] )
			g_pDisplay->Blt( m_DrawPosX+m_X_Draw_Plus, m_DrawPosY+m_Y_Draw_Plus, m_pListItem[m_NowSelectIndex] );
	}
	else if( m_State == COMBO_STATE_SELECT ) // ������ ���(����Ʈ�� �׸���.)
	{
		if( m_pListBackGround ) // ����Ʈ�� ���.
		{
			m_pListBackGround->Xpos = m_DrawPosX+m_X_Draw_Plus;
			m_pListBackGround->Ypos = m_DrawPosY+m_pSelectedBackGround->GetHeight(); // ��� ��ġ�� ��ü�� ������ �����صд�.(��ҿ� ���� �ȵǴ���.) ����üũ�ÿ� ����.
			g_pDisplay->Blt( m_pListBackGround->Xpos, m_pListBackGround->Ypos, m_pListBackGround );
		}

		for( int i = 0 ; i < m_InsertedCount ; ++i ) // 0���� �����Կ� ����.
		{
			if( m_pListItem[i] )
			{
				m_pListItem[i]->Xpos = m_pListBackGround->Xpos+1; // �� �ȼ������� ���ǻ� ����.(����Ʈ ��濡�� �׵θ��� �����������ϱ�.)
				m_pListItem[i]->Ypos = m_pListBackGround->Ypos+1+((i)*m_ItemHeight); // ��� ��ġ�� ��ü�� ������ �����صд�.(��ҿ� ���� �ȵǴ���.) ����üũ�ÿ� ����.

				if( i == m_NowOverIndex ) // ���콺�� �÷����ִ� �༮�̸� �ٸ� �̹����� ���
					g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListOverItem[i] );
				else
					g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListItem[i] );
			}
		}
	}
	else if( m_State == COMBO_STATE_ANIMATE ) // ����Ʈ ���� ���.(��游 �����Ѵ�.)
	{
		if( m_pListBackGround ) // ����Ʈ�� ����� �ְ�
		{
			if( ((int)m_AnimateHeight < (int)m_pListBackGround->GetHeight()) )// ���� �� ������ �ȵǾ�����...
				m_AnimateHeight += ((float)m_dwElapsedTick * 0.001f) * ( /*(float)m_pListBackGround->GetHeight() **/ 250.0f );
			else // ������ ��������...
			{
				m_AnimateHeight = (float)m_pListBackGround->GetHeight(); // ������ ��¶����� ��Ȯ�ϰ� ������Ѵ�.
				m_State = COMBO_STATE_SELECT; // ���� ���� �ٲ�д�.(�ٲٰ� �ѹ� �� ������ ���� ������ ����.)
			}

			m_pListBackGround->Xpos = m_DrawPosX+m_X_Draw_Plus;
			m_pListBackGround->Ypos = m_DrawPosY+m_pSelectedBackGround->GetHeight(); // ��� ��ġ�� ��ü�� ������ �����صд�.(��ҿ� ���� �ȵǴ���.) ����üũ�ÿ� ����.

			g_pDisplay->Blt( m_pListBackGround->Xpos, m_pListBackGround->Ypos
							 , m_pListBackGround->Xpos+m_pListBackGround->GetWidth()
							 , m_pListBackGround->Ypos+(int)m_AnimateHeight, m_pListBackGround );

			for( int i = 0 ; i < m_InsertedCount ; ++i ) // 0���� �����Կ� ����.
			{
				if( m_pListItem[i] )
				{
					m_pListItem[i]->Xpos = m_pListBackGround->Xpos+1; // �� �ȼ������� ���ǻ� ����.(����Ʈ ��濡�� �׵θ��� �����������ϱ�.)
					m_pListItem[i]->Ypos = m_pListBackGround->Ypos+1+((i)*m_ItemHeight); // ��� ��ġ�� ��ü�� ������ �����صд�.(��ҿ� ���� �ȵǴ���.) ����üũ�ÿ� ����.

					if( m_pListBackGround->Ypos+(int)m_AnimateHeight >= m_pListItem[i]->Ypos+(int)m_pListItem[i]->GetHeight() ) // ���� �� ���̱�.
						g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListItem[i] );
					else if( m_pListBackGround->Ypos+(int)m_AnimateHeight > m_pListItem[i]->Ypos ) // �Ϻ� ���̴� ��ġ�̸�...
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
		else // ���� ����� ������...
			m_State = COMBO_STATE_SELECT; // �ٷ� ���� ����...
	}

	m_dwElapsedTick = timeGetTime(); // �ð��� ����صд�. ���⼭ �̰� ����� ���ʿ��� ���Ҷ� ����� ������.
}


void CComboBox::Draw() // ����Ʈ�� x��ǥ ����ġ�� ���ڷ� �޴´�.(���� �׵θ� ����ϴ� ����ġ)
{
	if( !g_pDisplay )
		return;

	if( m_dwElapsedTick == 0 ) // ó�� ������..
		m_dwElapsedTick = timeGetTime(); // ó���� �ް� ����.

	m_dwElapsedTick = timeGetTime() - m_dwElapsedTick; // ���� �����ӿ��� ������ �ð�.

	if( m_pSelectedBackGround ) // ���õ� �������� ����ִ� ���. �׻� ���.
		g_pDisplay->Blt( m_DrawPosX, m_DrawPosY, m_pSelectedBackGround );

	if( m_pListViewBtn ) // ����Ʈ �ٿ�(����Ʈ ���̱�)��ư. �׻� �߷�.
	{
		if( m_State == COMBO_STATE_DISABLE ) // ��Ȱ�� ����϶�...
			m_pListViewBtn->Draw( m_DrawPosX+m_pSelectedBackGround->GetWidth(), m_DrawPosY, TRUE ); // �� Ȱ�� �׸����� ���.
		else
			m_pListViewBtn->Draw( m_DrawPosX+m_pSelectedBackGround->GetWidth(), m_DrawPosY );
	}

	if( m_State == COMBO_STATE_NORMAL || m_State == COMBO_STATE_DISABLE ) // �Ϲݸ�� ���(��Ȱ���϶��� ���õ� �������� �׷����Ѵ�.)
	{
		// ���õ� �������� �׸���.
		if( m_NowSelectIndex >= 0 && m_pListItem[m_NowSelectIndex] )
			g_pDisplay->Blt( m_DrawPosX+m_X_Draw_Plus, m_DrawPosY+m_Y_Draw_Plus, m_pListItem[m_NowSelectIndex] );
	}
	else if( m_State == COMBO_STATE_SELECT ) // ������ ���(����Ʈ�� �׸���.)
	{
		if( m_pListBackGround ) // ����Ʈ�� ���.
		{
			m_pListBackGround->Xpos = m_DrawPosX+m_X_Draw_Plus;
			m_pListBackGround->Ypos = m_DrawPosY+m_pSelectedBackGround->GetHeight(); // ��� ��ġ�� ��ü�� ������ �����صд�.(��ҿ� ���� �ȵǴ���.) ����üũ�ÿ� ����.
			g_pDisplay->Blt( m_pListBackGround->Xpos, m_pListBackGround->Ypos, m_pListBackGround );
		}

		for( int i = 1 ; i < m_InsertedCount ; ++i ) // 0���� �����Կ� ����.
		{
			if( m_pListItem[i] )
			{
				m_pListItem[i]->Xpos = m_pListBackGround->Xpos+1; // �� �ȼ������� ���ǻ� ����.(����Ʈ ��濡�� �׵θ��� �����������ϱ�.)
				m_pListItem[i]->Ypos = m_pListBackGround->Ypos+1+((i-1)*m_ItemHeight); // ��� ��ġ�� ��ü�� ������ �����صд�.(��ҿ� ���� �ȵǴ���.) ����üũ�ÿ� ����.

				if( i == m_NowOverIndex ) // ���콺�� �÷����ִ� �༮�̸� �ٸ� �̹����� ���
					g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListOverItem[i] );
				else
					g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListItem[i] );
			}
		}
	}
	else if( m_State == COMBO_STATE_ANIMATE ) // ����Ʈ ���� ���.(��游 �����Ѵ�.)
	{
		if( m_pListBackGround ) // ����Ʈ�� ����� �ְ�
		{
			if( ((int)m_AnimateHeight < (int)m_pListBackGround->GetHeight()) )// ���� �� ������ �ȵǾ�����...
				m_AnimateHeight += ((float)m_dwElapsedTick * 0.001f) * ( /*(float)m_pListBackGround->GetHeight() **/ 250.0f );
			else // ������ ��������...
			{
				m_AnimateHeight = (float)m_pListBackGround->GetHeight(); // ������ ��¶����� ��Ȯ�ϰ� ������Ѵ�.
				m_State = COMBO_STATE_SELECT; // ���� ���� �ٲ�д�.(�ٲٰ� �ѹ� �� ������ ���� ������ ����.)
			}

			m_pListBackGround->Xpos = m_DrawPosX+m_X_Draw_Plus;
			m_pListBackGround->Ypos = m_DrawPosY+m_pSelectedBackGround->GetHeight(); // ��� ��ġ�� ��ü�� ������ �����صд�.(��ҿ� ���� �ȵǴ���.) ����üũ�ÿ� ����.

			g_pDisplay->Blt( m_pListBackGround->Xpos, m_pListBackGround->Ypos
							 , m_pListBackGround->Xpos+m_pListBackGround->GetWidth()
							 , m_pListBackGround->Ypos+(int)m_AnimateHeight, m_pListBackGround );

			for( int i = 1 ; i < m_InsertedCount ; ++i ) // 0���� �����Կ� ����.
			{
				if( m_pListItem[i] )
				{
					m_pListItem[i]->Xpos = m_pListBackGround->Xpos+1; // �� �ȼ������� ���ǻ� ����.(����Ʈ ��濡�� �׵θ��� �����������ϱ�.)
					m_pListItem[i]->Ypos = m_pListBackGround->Ypos+1+((i-1)*m_ItemHeight); // ��� ��ġ�� ��ü�� ������ �����صд�.(��ҿ� ���� �ȵǴ���.) ����üũ�ÿ� ����.

					if( m_pListBackGround->Ypos+(int)m_AnimateHeight >= m_pListItem[i]->Ypos+(int)m_pListItem[i]->GetHeight() ) // ���� �� ���̱�.
						g_pDisplay->Blt( m_pListItem[i]->Xpos, m_pListItem[i]->Ypos, m_pListItem[i] );
					else if( m_pListBackGround->Ypos+(int)m_AnimateHeight > m_pListItem[i]->Ypos ) // �Ϻ� ���̴� ��ġ�̸�...
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
		else // ���� ����� ������...
			m_State = COMBO_STATE_SELECT; // �ٷ� ���� ����...
	}

	m_dwElapsedTick = timeGetTime(); // �ð��� ����صд�. ���⼭ �̰� ����� ���ʿ��� ���Ҷ� ����� ������.
}
