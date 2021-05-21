#include "stdafx.h"



#include <string.h>
#include <direct.h>
#include "main.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "Sprite.h"


extern int g_EffectTextr[];


///////////////////////////////////////////////////////////////////////////////////////
// -= 스프라이트 그림 파일의 준비 =-
// 스프라이트로 쓸 그림파일들의 파일명을 똑같이 일치시키고 뒤에 두자리 번호를 붙인다.
// 예: aaaa00.bmp, aaaa01.bmp ~ aaaa99.bmp
// 총 그림 갯수를 로드시 직접 입력한다.

void CSprite::LoadRes( const char *ImageFileName, int NumOfImage, DWORD DelayTick, DWORD ColorKey ) // 파일명에 경로 포함.확장지 미포함 이어야함에 유의.
{
	if( !g_pDisplay )
		return;

	Init(); // 변수들을 한번 초기화.

	m_NumOfImage = NumOfImage;
	m_DelayTick = DelayTick;

	SAFE_DELETE_ARRAY( m_ppImageSurface ); // 혹시 모르니까 안전하게 메모리해제 검사.
	m_ppImageSurface = new CSurface*[NumOfImage];

	if( !m_ppImageSurface )
	{
		MessageBox( NULL, "Memory Low! In CSprite::LoadRes", "Error", MB_OK );

		return;
	}

	char strTemp[256] = {0,};

	for( int i = 0 ; i < m_NumOfImage ; ++i )
	{
		sprintf( strTemp, "%s%02d.bmp", ImageFileName, i ); // 현재 로드할 파일의 번호를 붙여 최종 경로를 만든다.

		if( g_pDisplay->CreateSurfaceFromBitmap( &m_ppImageSurface[i], strTemp ) != DD_OK )
		{
			// 이미지 로딩 실패면...
			MessageBox( NULL, "CreateSurfaceFromBitmap Failed! In CSprite::LoadRes", "Error", MB_OK );

			return;
		}

		if( m_ppImageSurface[i] && ColorKey != -1 ) // 컬리키 인자가 -1이면 투명처리 안하도록...
		{
			m_ppImageSurface[i]->SetColorKey( ColorKey );
		}

	}
}

int CSprite::Update( DWORD ElapsedTick )
{
	if( !g_pDisplay || !m_ppImageSurface )
		return -1;

	if( m_DelayTick <= 0 || m_NowFrame >= m_NumOfImage ) // 넘어가는 프레임 딜레이가 음수나 0은 안된다. 또한 1회 플레이 타입의 1회 플레이가 끝났을때(현재 프레임이 총 프레임보다 크면 그렇게 판단) 도 더이상 안들어간다.
		return -1; // 이경우 프레임을 증가시키지 않는다.

	if( m_bAniLock ) // 애니메이션 진행을 강제로 멈추는 녀석.
		return -1;

	m_RemainderTick += ElapsedTick; // 누적..

	while( m_RemainderTick > m_DelayTick ) // 프레임을 증가시켜야 할 만큼 커졌으면...(무한 루프 안돌게 수치 조절 잘하자.)
	{
		if( m_bReverseAni ) // 역순 애니메이션 모드의 여부.
		{
			--m_NowFrame;
			if( m_NowFrame < 0 ) // 정상 프레임을 넘겼으면...
			{
				switch( m_AniType )
				{
				case SPRITE_ANITYPE_ONEPLAY: // 별도로 처리가 필요없다. 현재 프레임이 전체 프레임을 넘어가면 자동으로 멈춘다.
					break;

				case SPRITE_ANITYPE_LOOP:

					m_NowFrame = m_NumOfImage-1; // 마지막으로 돌려놓는다.

					break;

				case SPRITE_ANITYPE_ONESTOP:

					m_NowFrame = 0; // 처음 프레임으로 셋팅해주고.
					m_bAniLock = TRUE; // 더이상 안돌게 설정.

					break;

				case SPRITE_ANITYPE_REV_LOOP:

					m_NowFrame = 0; // 정상 범위로 셋팅을 해줘야한다.
					m_bReverseAni = 0; // 리버스모드를 토글 시킨다.

					break;
				}
			}
		}
		else
		{
			++m_NowFrame;
			if( m_NowFrame >= m_NumOfImage ) // 끝 프레임을 넘겼으면...
			{
				switch( m_AniType )
				{
				case SPRITE_ANITYPE_ONEPLAY: // 별도로 처리가 필요없다. 현재 프레임이 전체 프레임을 넘어가면 자동으로 멈춘다.
					break;

				case SPRITE_ANITYPE_LOOP:

					m_NowFrame = 0; // 처음으로 돌려놓는다.

					break;

				case SPRITE_ANITYPE_ONESTOP:

					m_NowFrame = m_NumOfImage-1; // 마지막 프레임으로 셋팅해주고.
					m_bAniLock = TRUE; // 더이상 안돌게 설정.

					break;

				case SPRITE_ANITYPE_REV_LOOP:

					m_NowFrame = m_NumOfImage-1; // 정상 범위로 셋팅을 해줘야한다.
					m_bReverseAni = 1; // 리버스모드를 토글 시킨다.

					break;
				}
			}
		}

		m_RemainderTick -= m_DelayTick;
	}

	return m_NowFrame;
}

int CSprite::UpdateFast( DWORD ElapsedTick )
{
	if( !g_pDisplay || !m_ppImageSurface )
		return -1;

	if( m_DelayTick <= 0 || m_NowFrame >= m_NumOfImage ) // 넘어가는 프레임 딜레이가 음수나 0은 안된다. 또한 1회 플레이 타입의 1회 플레이가 끝났을때(현재 프레임이 총 프레임보다 크면 그렇게 판단) 도 더이상 안들어간다.
		return -1; // 이경우 프레임을 증가시키지 않는다.

	if( m_bAniLock ) // 애니메이션 진행을 강제로 멈추는 녀석.
		return -1;

	m_RemainderTick += ElapsedTick; // 누적..

	int PlusFrame = m_RemainderTick / m_DelayTick;
	m_RemainderTick = m_RemainderTick % m_DelayTick;

	m_NowFrame += PlusFrame;

	if( m_NowFrame >= m_NumOfImage ) // 끝 프레임을 넘겼으면...
	{
		switch( m_AniType )
		{
		case SPRITE_ANITYPE_ONEPLAY:
			break; // 이 경우는 리턴으로...

		case SPRITE_ANITYPE_LOOP:

			m_NowFrame %= m_NumOfImage; // 처음으로 돌려놓는다.

			break;

		case SPRITE_ANITYPE_ONESTOP:

			m_NowFrame = m_NumOfImage-1; // 마지막 프레임으로 셋팅해주고.
			m_bAniLock = TRUE; // 더이상 안돌게 설정.

			break;

		case SPRITE_ANITYPE_REV_LOOP: // 패스트 업데이트는 리버스 루프타입을 지원 안함.
			return -1;
		}
	}

	return m_NowFrame;
}

void CSprite::Draw( int x, int y, int DrawWidth, int DrawHeight )
{
	if( !g_pDisplay || !m_ppImageSurface || m_NowFrame < 0 || m_NowFrame >= m_NumOfImage ) // 끝 프레임을 넘긴경우(즉 1회 플레이 타입에서 1회 플레이가 끝난경우)에도 출력안함.
		return;

	if( DrawWidth == -1 && DrawHeight == -1 ) // 사이즈 조정이 없을 시.
		g_pDisplay->Blt( x, y, m_ppImageSurface[m_NowFrame] );
	else // 사이즈 조정을 원할 시..(이경우 가로,세로가 다 입력되어있어야 함에 유의.
		g_pDisplay->Blt( x, y, x+DrawWidth, y+DrawHeight, m_ppImageSurface[m_NowFrame] );
}

void CSprite::DrawFrame( int Frame, int x, int y, int DrawWidth, int DrawHeight ) // 특정 프레임을 출력.
{
	if( !g_pDisplay || !m_ppImageSurface || m_NowFrame < 0 || m_NowFrame >= m_NumOfImage ) // 끝 프레임을 넘긴경우(즉 1회 플레이 타입에서 1회 플레이가 끝난경우)에도 출력안함.
		return;

	if( DrawWidth == -1 && DrawHeight == -1 ) // 사이즈 조정이 없을 시.
		g_pDisplay->Blt( x, y, m_ppImageSurface[Frame] );
	else // 사이즈 조정을 원할 시..(이경우 가로,세로가 다 입력되어있어야 함에 유의.
		g_pDisplay->Blt( x, y, x+DrawWidth, y+DrawHeight, m_ppImageSurface[Frame] );
}
