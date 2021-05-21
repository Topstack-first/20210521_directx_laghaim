#include "stdafx.h"




#include "Scroll.h"
#include "headers.h"



//---------------------------------------------------------
//		 주의 할 점은 처음 두개의 파일명은 버튼 형식으로
//		 마지막 파일명은 경로까지 쓰는걸 잊으면 않된다
//----------------------------------------------------------
void CScroll::LoadRes( char *DecBtn, char *IncBtn, char *ScrollBar )
{
	if( !g_pDisplay )
		return;

	char TempString1[100] = {0,};
	char TempString2[100] = {0,};
	char TempString3[100] = {0,};

	Init();

	if( DecBtn == NULL || DecBtn[0] == '\0' ) // 혹시 디폴트 그림을 요청했다면... ///메뉴인터페이스 중에 수정했음.
	{
		//		ZeroMemory( TempString, sizeof(char)*100 );
		strcpy( TempString1, "common/scroll_up_t01" );
		DecBtn = TempString1;
	}
	if( IncBtn == NULL || IncBtn[0] == '\0' ) // 혹시 디폴트 그림을 요청했다면...
	{
		//		ZeroMemory( TempString, sizeof(char)*100 );
		strcpy( TempString2, "common/scroll_dn_t01" );
		IncBtn = TempString2;
	}
	if( ScrollBar == NULL || ScrollBar[0] == '\0' ) // 혹시 디폴트 그림을 요청했다면...
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
	else // 메모리 부족.
	{
		MessageBox( NULL, "Out of memory! in Scroll LoadRes", "Error", MB_OK );
	}

	m_IncBtn = new CBasicButton();
	if( m_IncBtn )
	{
		m_IncBtn->SetFileName( IncBtn );
		//	m_IncBtn.SetSize( SCROLLBTN_W, SCROLLBTN_H );  // 이거 함수명 삐꾸다. 사이즈가 아니라 출력해주는 위치 아닌가? 확실하지 않으니 확인 필요!.
		m_IncBtn->SetDisable( false );
		m_IncBtn->LoadRes();
	}
	else // 메모리 부족.
	{
		MessageBox( NULL, "Out of memory! in Scroll LoadRes", "Error", MB_OK );
	}

	g_pDisplay->CreateSurfaceFromBitmap( &ScrollBarSurface, ScrollBar, 0, 0 );

	if( ScrollBarSurface )
	{
		ScrollBarSurface->SetColorKey( RGB(0, 0, 0) );
	}
}

void CScroll::DeleteRes() ///파티개편 시 추가.
{
	Init(); // 이안에 지우는 루틴 다 있다.
}

int CScroll::CalculateBarYPos() // Scroll Bar의 위치를 계산한다.( 절대 좌표가 아니라 스크롤바 좌표를 기준으로 한 위치임에 유의. )
{
	int ScrollAreaSizeY = DrawHeight - m_DecBtn->Height - m_IncBtn->Height - ScrollBarSurface->GetHeight(); // 스크롤에서 버튼등을 뺀 이동가능 공간 길이가 나온다.

	if( ScrollAreaSizeY <= 0 ) // 음수가 나오면 스크롤 바의 길이가 스크롤 전체보다 긴 경우이다.
	{
		// 이경우 위쪽 버튼에만 맞춰 출력.(아래 버튼은 가려버릴수가 있다.)
		return m_DecBtn->Height;
	}

	if( (RangeMax-1) <= 0 ) // 0으로 나누면 에러나니까 예외 처리.
	{
		return 0; // 이렇게 해주면 스크롤 범위가 0이나 1일경우.(즉, 이상한 값일 경우) 스크롤 바가 움직이지 않는다.
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
// Message 처리
LRESULT CScroll::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = LOWORD(lParam);
	int	y = HIWORD(lParam);

	static int OldY = 0; // 드래깅을 위한 마우스 좌표 기억.

	switch( msg )
	{
	case 0x020A: // 휠마우스 메세지

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

		if( m_DecBtn->bDown ) // 눌렸다면...
		{
			NowPos -= MoveGap;

			if( NowPos < 0 )
			{
				NowPos = 0;
			}

			return S_OK; // 다른 메세지 프로시져를 무시시킨다.
		}

		if( m_IncBtn->bDown ) // 눌렸다면...
		{
			NowPos += MoveGap;

			if( NowPos > RangeMax-1 )
			{
				NowPos = RangeMax-1;
			}

			return S_OK; // 다른 메세지 프로시져를 무시시킨다.
		}

		if( IsInsideScrollBar(x,y) ) // 바를 클릭 했을 때.
		{
			// 드래깅.
			m_bDraging = TRUE;

			return S_OK; // 다른 녀석들은 메세지 못받게 하기
		}

		if( IsInsideScrollArea(x,y) ) // 바와 버튼을 제외한 나머지 스크롤의 영역 체크.
		{
			// 점프.
			SetPos( CalcPosFromY(y) ); // SetPos로 해주면 범위 오버를 한번더 검사해줄수있다.

			return S_OK; // 다른 녀석들은 메세지 못받게 하기
		}
		/*
		// Scroll Btn 안..
		if (ret = IsInScrollBtnArea(x, y))
		{
		state = SCROLLBTN;
		if (ret == SCROLL_DEC_BTN)	DecrementPos ();
		else						IncrementPos ();
		}
		// Scroll Bar가 움직일 수 공간 안인지..
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

		m_bDraging = FALSE; // 마우스 때면 드래깅은 무조건 중지.

		if( E_FAIL != m_DecBtn->MsgProc( hWnd, msg, wParam, lParam ) )
			return S_OK;
		/*
		if( m_DecBtn->GetState() == BTN_ACTION) // 이렇게 체크해야하는듯.
		{
		m_DecBtn->m_iBtnFlg = BTN_NORMAL;
		NowPos--;

		if( NowPos < 0 )
		{
		NowPos = 0;
		}

		return 1; // 다른 메세지 프로시져를 무시시킨다.
		}
		*/

		if( E_FAIL != m_IncBtn->MsgProc( hWnd, msg, wParam, lParam ) )
			return S_OK;
	/*
	if( m_IncBtn->GetState() == BTN_ACTION) // 이렇게 체크해야하는듯.
	{
	m_IncBtn->m_iBtnFlg = BTN_NORMAL;
	NowPos++;

	if( NowPos > RangeMax-1 )
	{
	NowPos = RangeMax-1;
	}

	return 1; // 다른 메세지 프로시져를 무시시킨다.
	}
	*/
	case WM_MOUSEMOVE :

		if(	m_bDraging ) // 드래깅 중이라면...
		{
			// 이동.
			SetPos( CalcPosFromY(y) ); // SetPos로 해주면 범위 오버를 한번더 검사해줄수있다.
			/*
			if( y < OldY ) // 줄었으면 위로.
			{
			NowPos--;

			if( NowPos < 0 )
			{
			NowPos = 0;
			}
			}
			else if( y > OldY ) // 늘었으면 아래로
			{
			NowPos++;

			if( NowPos > RangeMax-1 )
			{
			NowPos = RangeMax-1;
			}
			}

			OldY = y;
			*/
			return S_OK; // 다른 녀석들은 메세지 못받게 하기
		}

		m_DecBtn->MsgProc( hWnd, msg, wParam, lParam ); // 버튼 그림 바뀌는 것 때문에라도 이게 필요한듯.
		m_IncBtn->MsgProc( hWnd, msg, wParam, lParam );
		/*
		int ret;

		if (state == SCROLLBAR_POLL)
		{
		// if (IsInScrollBarArea(x,y))
		SetBarPosToGo (y);
		}
		// Content(내용글) 안..
		//wan:2004-12	마우스 위의 리스트 색상을 변경
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
	// 여기서 버튼이 눌렸는 지를 체크한다.(여기서 하는 이유는 누르고 있는 거 등을 체크하기 위해서)
	// 스크롤의 이벤트 체크를 마우스 버튼 UP이 아닌 Down을 기준으로 체크해준다.
	// 그 이유는 Down 시 다른 메세지 프로시져로 빠질수있기 때문.
	// 만약 굳이 Up으로 해야한다면 Down등의 메세지에서 마우스 좌표가 스크롤의
	// 범위에 있는지를 검사해서 스크롤 위에 있다면 1을 리턴해줘야한다.
	if( m_DecBtn->bDown ) // 눌렸다면...
	{
	NowPos--;

	if( NowPos < 0 )
	{
	NowPos = 0;
	}

	return 1; // 다른 메세지 프로시져를 무시시킨다.
	}
	else if( m_IncBtn->bDown ) // 눌렸다면...
	{
	NowPos++;

	if( NowPos > RangeMax-1 )
	{
	NowPos = RangeMax-1;
	}

	return 1; // 다른 메세지 프로시져를 무시시킨다.
	}
	*/
	return E_FAIL; // 리턴 0이면 다른 메세지 프로시져로 넘겨준다고 보면 된다.
}

bool CScroll::IsInside( int x, int y ) // 영역 전체 체크용.
{
	if( !ScrollBarSurface )
		return false;

	if(   ( x > DrawPosX && x < DrawPosX + (int)ScrollBarSurface->GetWidth() ) && ( y > DrawPosY && y < DrawPosY + DrawHeight )    )
		return true;
	else
		return false;
}

bool CScroll::IsInsideScrollArea( int x, int y ) // 점프 체크용
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

bool CScroll::IsInsideScrollBar( int x, int y ) // 드래그 체크용.
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

int CScroll::CalcPosFromY( int y ) // y 값으로 부터 Pos값을 산출한다.( y는 절대 좌표 )
{
	if( !ScrollBarSurface || !m_IncBtn || !m_DecBtn )
		return false;

	if( y < DrawPosY + m_DecBtn->Height ) // 범위 외 처리
	{
		return 0;
	}
	if( y > DrawPosY + DrawHeight - m_IncBtn->Height ) // 범위 외 처리
	{
		return RangeMax-1;
	}

	int ScrollAreaHeight = DrawHeight - m_IncBtn->Height - m_DecBtn->Height - (int)ScrollBarSurface->GetHeight(); // 순수한 여분 영역 길이

	int PureY = y - DrawPosY - m_DecBtn->Height; // 0부터 시작하는 스크롤 에서의 Y 위치

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
