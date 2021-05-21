#include "stdafx.h"




#include "Scroll.h"
#include "headers.h"



//---------------------------------------------------------
//		 ���� �� ���� ó�� �ΰ��� ���ϸ��� ��ư ��������
//		 ������ ���ϸ��� ��α��� ���°� ������ �ʵȴ�
//----------------------------------------------------------
void CScroll::LoadRes( char *DecBtn, char *IncBtn, char *ScrollBar )
{
	if( !g_pDisplay )
		return;

	char TempString1[100] = {0,};
	char TempString2[100] = {0,};
	char TempString3[100] = {0,};

	Init();

	if( DecBtn == NULL || DecBtn[0] == '\0' ) // Ȥ�� ����Ʈ �׸��� ��û�ߴٸ�... ///�޴��������̽� �߿� ��������.
	{
		//		ZeroMemory( TempString, sizeof(char)*100 );
		strcpy( TempString1, "common/scroll_up_t01" );
		DecBtn = TempString1;
	}
	if( IncBtn == NULL || IncBtn[0] == '\0' ) // Ȥ�� ����Ʈ �׸��� ��û�ߴٸ�...
	{
		//		ZeroMemory( TempString, sizeof(char)*100 );
		strcpy( TempString2, "common/scroll_dn_t01" );
		IncBtn = TempString2;
	}
	if( ScrollBar == NULL || ScrollBar[0] == '\0' ) // Ȥ�� ����Ʈ �׸��� ��û�ߴٸ�...
	{
		//		ZeroMemory( TempString, sizeof(char)*100 );
		strcpy( TempString3, "interface/common/scroll_thumb_01.bmp" );
		ScrollBar = TempString3;
	}


	m_DecBtn = new CBasicButton();
	if( m_DecBtn )
	{
		m_DecBtn->SetFileName( DecBtn );
		//	m_DecBtn.SetSize( SCROLLBTN_W, SCROLLBTN_H );
		m_DecBtn->SetDisable( false );
		m_DecBtn->LoadRes();
	}
	else // �޸� ����.
	{
		MessageBox( NULL, "Out of memory! in Scroll LoadRes", "Error", MB_OK );
	}

	m_IncBtn = new CBasicButton();
	if( m_IncBtn )
	{
		m_IncBtn->SetFileName( IncBtn );
		//	m_IncBtn.SetSize( SCROLLBTN_W, SCROLLBTN_H );  // �̰� �Լ��� �߲ٴ�. ����� �ƴ϶� ������ִ� ��ġ �ƴѰ�? Ȯ������ ������ Ȯ�� �ʿ�!.
		m_IncBtn->SetDisable( false );
		m_IncBtn->LoadRes();
	}
	else // �޸� ����.
	{
		MessageBox( NULL, "Out of memory! in Scroll LoadRes", "Error", MB_OK );
	}

	g_pDisplay->CreateSurfaceFromBitmap( &ScrollBarSurface, ScrollBar, 0, 0 );

	if( ScrollBarSurface )
	{
		ScrollBarSurface->SetColorKey( RGB(0, 0, 0) );
	}
}

void CScroll::DeleteRes() ///��Ƽ���� �� �߰�.
{
	Init(); // �̾ȿ� ����� ��ƾ �� �ִ�.
}

int CScroll::CalculateBarYPos() // Scroll Bar�� ��ġ�� ����Ѵ�.( ���� ��ǥ�� �ƴ϶� ��ũ�ѹ� ��ǥ�� �������� �� ��ġ�ӿ� ����. )
{
	int ScrollAreaSizeY = DrawHeight - m_DecBtn->Height - m_IncBtn->Height - ScrollBarSurface->GetHeight(); // ��ũ�ѿ��� ��ư���� �� �̵����� ���� ���̰� ���´�.

	if( ScrollAreaSizeY <= 0 ) // ������ ������ ��ũ�� ���� ���̰� ��ũ�� ��ü���� �� ����̴�.
	{
		// �̰�� ���� ��ư���� ���� ���.(�Ʒ� ��ư�� ������������ �ִ�.)
		return m_DecBtn->Height;
	}

	if( (RangeMax-1) <= 0 ) // 0���� ������ �������ϱ� ���� ó��.
	{
		return 0; // �̷��� ���ָ� ��ũ�� ������ 0�̳� 1�ϰ��.(��, �̻��� ���� ���) ��ũ�� �ٰ� �������� �ʴ´�.
	}

	return (ScrollAreaSizeY *  NowPos) / (RangeMax-1);
}

void CScroll::IncPos()
{
	NowPos += MoveGap;

	if( NowPos > RangeMax-1 )
	{
		NowPos = RangeMax-1;
	}
}

void CScroll::DecPos()
{
	NowPos -= MoveGap;

	if( NowPos < 0 )
	{
		NowPos = 0;
	}
}

void CScroll::Draw()
{
	if( m_DecBtn )
	{
		m_DecBtn->Draw( DrawPosX, DrawPosY );
	}
	if( m_IncBtn )
	{
		m_IncBtn->Draw( DrawPosX, DrawPosY + DrawHeight - m_IncBtn->Height );
	}

	if( ScrollBarSurface )
	{
		g_pDisplay->Blt( DrawPosX, DrawPosY+m_DecBtn->Height+CalculateBarYPos()-1, ScrollBarSurface );
		//		g_pDisplay->Blt( DrawPosX, DrawPosY+m_DecBtn->Height+CalculateBarYPos()-1, DrawPosX + 100, DrawPosY+m_DecBtn->Height+CalculateBarYPos()-1+100, ScrollBarSurface );
	}
}

#define SCROLL_FLAG_Y 12
// Message ó��
LRESULT CScroll::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);

	static int OldY = 0; // �巡���� ���� ���콺 ��ǥ ���.

	switch( msg )
	{
	case 0x020A: // �ٸ��콺 �޼���

		if( IsInside(x,y) || IsInsideWheelArea(x, y) )
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				IncPos();
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				DecPos();
			}
			return S_OK;
		}

		break;

	case WM_LBUTTONDOWN :

		x = LOWORD(lParam);
		y = HIWORD(lParam);

		m_DecBtn->MsgProc( hWnd, msg, wParam, lParam );
		m_IncBtn->MsgProc( hWnd, msg, wParam, lParam );

		if( m_DecBtn->bDown ) // ���ȴٸ�...
		{
			NowPos -= MoveGap;

			if( NowPos < 0 )
			{
				NowPos = 0;
			}

			return S_OK; // �ٸ� �޼��� ���ν����� ���ý�Ų��.
		}

		if( m_IncBtn->bDown ) // ���ȴٸ�...
		{
			NowPos += MoveGap;

			if( NowPos > RangeMax-1 )
			{
				NowPos = RangeMax-1;
			}

			return S_OK; // �ٸ� �޼��� ���ν����� ���ý�Ų��.
		}

		if( IsInsideScrollBar(x,y) ) // �ٸ� Ŭ�� ���� ��.
		{
			// �巡��.
			m_bDraging = TRUE;

			return S_OK; // �ٸ� �༮���� �޼��� ���ް� �ϱ�
		}

		if( IsInsideScrollArea(x,y) ) // �ٿ� ��ư�� ������ ������ ��ũ���� ���� üũ.
		{
			// ����.
			SetPos( CalcPosFromY(y) ); // SetPos�� ���ָ� ���� ������ �ѹ��� �˻����ټ��ִ�.

			return S_OK; // �ٸ� �༮���� �޼��� ���ް� �ϱ�
		}
		/*
		// Scroll Btn ��..
		if (ret = IsInScrollBtnArea(x, y))
		{
		state = SCROLLBTN;
		if (ret == SCROLL_DEC_BTN)	DecrementPos ();
		else						IncrementPos ();
		}
		// Scroll Bar�� ������ �� ���� ������..
		else if (ret = IsInScrollBarArea(x, y))
		{
		if (ret == SCROLL_BAR_POLL)
		state = SCROLLBAR_POLL;
		else
		{
		state = SCROLLBAR_FLAT;
		SetBarPosToGo (y);
		}
		}
		*/
		break;



	case WM_LBUTTONUP :

		m_bDraging = FALSE; // ���콺 ���� �巡���� ������ ����.

		if( E_FAIL != m_DecBtn->MsgProc( hWnd, msg, wParam, lParam ) )
			return S_OK;
		/*
		if( m_DecBtn->GetState() == BTN_ACTION) // �̷��� üũ�ؾ��ϴµ�.
		{
		m_DecBtn->m_iBtnFlg = BTN_NORMAL;
		NowPos--;

		if( NowPos < 0 )
		{
		NowPos = 0;
		}

		return 1; // �ٸ� �޼��� ���ν����� ���ý�Ų��.
		}
		*/

		if( E_FAIL != m_IncBtn->MsgProc( hWnd, msg, wParam, lParam ) )
			return S_OK;
	/*
	if( m_IncBtn->GetState() == BTN_ACTION) // �̷��� üũ�ؾ��ϴµ�.
	{
	m_IncBtn->m_iBtnFlg = BTN_NORMAL;
	NowPos++;

	if( NowPos > RangeMax-1 )
	{
	NowPos = RangeMax-1;
	}

	return 1; // �ٸ� �޼��� ���ν����� ���ý�Ų��.
	}
	*/
	case WM_MOUSEMOVE :

		if(	m_bDraging ) // �巡�� ���̶��...
		{
			// �̵�.
			SetPos( CalcPosFromY(y) ); // SetPos�� ���ָ� ���� ������ �ѹ��� �˻����ټ��ִ�.
			/*
			if( y < OldY ) // �پ����� ����.
			{
			NowPos--;

			if( NowPos < 0 )
			{
			NowPos = 0;
			}
			}
			else if( y > OldY ) // �þ����� �Ʒ���
			{
			NowPos++;

			if( NowPos > RangeMax-1 )
			{
			NowPos = RangeMax-1;
			}
			}

			OldY = y;
			*/
			return S_OK; // �ٸ� �༮���� �޼��� ���ް� �ϱ�
		}

		m_DecBtn->MsgProc( hWnd, msg, wParam, lParam ); // ��ư �׸� �ٲ�� �� �������� �̰� �ʿ��ѵ�.
		m_IncBtn->MsgProc( hWnd, msg, wParam, lParam );
		/*
		int ret;

		if (state == SCROLLBAR_POLL)
		{
		// if (IsInScrollBarArea(x,y))
		SetBarPosToGo (y);
		}
		// Content(�����) ��..
		//wan:2004-12	���콺 ���� ����Ʈ ������ ����
		else if ((ret = IsInContentArea(x, y)) != -1)
		{
		state = CONTENT;
		if (ret < TotalLine) {
		if (SelectedLine >= 0) {
		delete m_pTextSurf[SelectedLine];
		g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[SelectedLine], Hfont, LineText[SelectedLine], RGB(0, 0, 1), TextColor);
		m_pTextSurf[SelectedLine]->SetColorKey(RGB(0, 0, 1));
		}
		SelectedLine = ret;
		delete m_pTextSurf[SelectedLine];
		g_pDisplay->CreateSurfaceFromText(&m_pTextSurf[SelectedLine], Hfont, LineText[SelectedLine], RGB(0, 0, 1), RGB(255, 142, 103));
		m_pTextSurf[SelectedLine]->SetColorKey(RGB(0, 0, 1));
		return ret;
		}
		}
		*/

		break;


	default :
		break;
	}
	/*
	// ���⼭ ��ư�� ���ȴ� ���� üũ�Ѵ�.(���⼭ �ϴ� ������ ������ �ִ� �� ���� üũ�ϱ� ���ؼ�)
	// ��ũ���� �̺�Ʈ üũ�� ���콺 ��ư UP�� �ƴ� Down�� �������� üũ���ش�.
	// �� ������ Down �� �ٸ� �޼��� ���ν����� �������ֱ� ����.
	// ���� ���� Up���� �ؾ��Ѵٸ� Down���� �޼������� ���콺 ��ǥ�� ��ũ����
	// ������ �ִ����� �˻��ؼ� ��ũ�� ���� �ִٸ� 1�� ����������Ѵ�.
	if( m_DecBtn->bDown ) // ���ȴٸ�...
	{
	NowPos--;

	if( NowPos < 0 )
	{
	NowPos = 0;
	}

	return 1; // �ٸ� �޼��� ���ν����� ���ý�Ų��.
	}
	else if( m_IncBtn->bDown ) // ���ȴٸ�...
	{
	NowPos++;

	if( NowPos > RangeMax-1 )
	{
	NowPos = RangeMax-1;
	}

	return 1; // �ٸ� �޼��� ���ν����� ���ý�Ų��.
	}
	*/
	return E_FAIL; // ���� 0�̸� �ٸ� �޼��� ���ν����� �Ѱ��شٰ� ���� �ȴ�.
}

bool CScroll::IsInside( int x, int y ) // ���� ��ü üũ��.
{
	if( !ScrollBarSurface )
		return false;

	if(   ( x > DrawPosX && x < DrawPosX + (int)ScrollBarSurface->GetWidth() ) && ( y > DrawPosY && y < DrawPosY + DrawHeight )    )
		return true;
	else
		return false;
}

bool CScroll::IsInsideScrollArea( int x, int y ) // ���� üũ��
{
	if( !ScrollBarSurface )
		return false;

	if(   ( x > DrawPosX && x < DrawPosX + (int)ScrollBarSurface->GetWidth() )
			&& ( y > DrawPosY+m_DecBtn->Height && y < DrawPosY+DrawHeight-m_IncBtn->Height )    )
	{
		return true;
	}
	else
		return false;
}

bool CScroll::IsInsideScrollBar( int x, int y ) // �巡�� üũ��.
{
	if( !ScrollBarSurface || !m_DecBtn )
		return false;

	int BarPosY = DrawPosY+m_DecBtn->Height+CalculateBarYPos()-1;

	if(   ( x > DrawPosX && x < DrawPosX + (int)ScrollBarSurface->GetWidth() )
			&& ( y > BarPosY && y < BarPosY + (int)ScrollBarSurface->GetHeight() )    )
	{
		return true;
	}
	else
		return false;
}

int CScroll::CalcPosFromY( int y ) // y ������ ���� Pos���� �����Ѵ�.( y�� ���� ��ǥ )
{
	if( !ScrollBarSurface || !m_IncBtn || !m_DecBtn )
		return false;

	if( y < DrawPosY + m_DecBtn->Height ) // ���� �� ó��
	{
		return 0;
	}
	if( y > DrawPosY + DrawHeight - m_IncBtn->Height ) // ���� �� ó��
	{
		return RangeMax-1;
	}

	int ScrollAreaHeight = DrawHeight - m_IncBtn->Height - m_DecBtn->Height - (int)ScrollBarSurface->GetHeight(); // ������ ���� ���� ����

	int PureY = y - DrawPosY - m_DecBtn->Height; // 0���� �����ϴ� ��ũ�� ������ Y ��ġ

	return (PureY * RangeMax) / ScrollAreaHeight;
}

void CScroll::SetWheelArea(int x, int y, int width, int height)
{
	WheelArea.left = x;
	WheelArea.right = x + width;
	WheelArea.top = y;
	WheelArea.bottom = y + height;
}

void CScroll::SetUseWheelArea(bool isUse)
{
	IsUseWheelArea = isUse;
}

bool CScroll::IsInsideWheelArea(int x, int y)
{
	if( IsUseWheelArea == false )
		return false;

	if( WheelArea.left < x && WheelArea.right > x &&
		WheelArea.top < y && WheelArea.bottom > y )
		return true;

	return false;
}
