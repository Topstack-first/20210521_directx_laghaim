#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Config.h"

#define SPRITE_ANITYPE_ONEPLAY 0   // 애니메이션 타입. 1회 플레이
#define SPRITE_ANITYPE_LOOP    1   // 애니메이션 타입. 무한 루프
#define SPRITE_ANITYPE_ONESTOP 2   // 애니메이션 타입. 1회 플레이후 마지막 프레임에 멈춰있는타입.
#define SPRITE_ANITYPE_REV_LOOP    4   // 애니메이션 타입. 무한 루프 단 돌아가는 순서가 번걸아서 엇갈리며 간다.

///////////////////////////////////////////////////////////////////////////////////////////
// 제작작 : 이원석
// 제작일 : 05-09-29
// 내  용 :2D 스프라이트 애니메이션 관리 클래스.
class CSprite
{
public:
	CSprite()
	{
		m_ppImageSurface = NULL;

		Init();
	}

	~CSprite()
	{
		DeleteRes();
	}

	void Init()
	{
		m_NumOfImage = 0; // 프레임 수.

		m_DelayTick = 100; // 한 화면이 넘어가는데 지연시킬 시간. (0.0f로 초기화하면 안좋음에 유의.)
		m_RemainderTick = 0;

		m_NowFrame = 0; // 현제 출력 프레임.

//		m_DrawPosX = 0; // 실제 찍히는 좌표.
//		m_DrawPosY = 0;
		m_Width = 0;  // 읽어온 이미지의 가로 길이.
		m_Height = 0;

		m_AniType = 0; // 루프 여부등의 애니메이션 방식..

		m_bAniLock = FALSE;
		m_bReverseAni = FALSE;
	}

	void DeleteRes()
	{
		int i = 0;

		if( m_ppImageSurface )
		{
			for( i = 0 ; i < m_NumOfImage ; ++i )
			{
				SAFE_DELETE( m_ppImageSurface[i] );
			}
		}

		SAFE_DELETE_ARRAY( m_ppImageSurface );
	}

	BOOL IsAniEnd()
	{
		if( m_NowFrame >= m_NumOfImage )
			return TRUE;

		return FALSE;
	}

	void SetStart()
	{
		m_NowFrame = 0;
		m_bAniLock = FALSE; // 새롭게 시작시엔 락을 무조건 풀고 본다.
		m_RemainderTick = 0;
	}

	void LoadRes( const char *ImageFileName, int NumOfImage, DWORD DelayTick, DWORD ColorKey = -1 );
	int Update( DWORD ElapsedTick ); // 현재 프레임 번호 갱신. 현재 프레임번호를 리턴도 해준다.
	int UpdateFast( DWORD ElapsedTick ); // 좀 더 빠르게 갱신. 대신 리버스 루프 타입은 지원을 안한다.

	void Draw( int x, int y, int DrawWidth = -1, int DrawHeight = -1 );
	void DrawFrame( int Frame, int x, int y, int DrawWidth = -1, int DrawHeight = -1 ); // 특정 프레임을 출력.

	CSurface	**m_ppImageSurface; // 실제 이미지 서피스

	int m_Width;  // 읽어온 이미지의 가로 길이.
	int m_Height;

	int m_NumOfImage; // 프레임 수.
	DWORD m_DelayTick; // 한 화면이 넘어가는데 지연시킬 시간.
	int m_NowFrame; // 현제 출력 프레임.
	DWORD m_RemainderTick; // 한 갱신당 지나간 시간을 기억.(이걸 멤버로 가지고있어야 동시에 여러개 플레이할때 제대로 된다.)

	int m_AniType; // 애니메이션 타입.(0 - 1회 플레이, 1 - 무한 루프)

	BOOL m_bAniLock; // 애니메이션을 강제로 멈추기위한 플래그.
	BOOL m_bReverseAni; // 역순 애니메이션 중인지 플래그. 애니타입 SPRITE_ANITYPE_REV_LOOP 일때 사용한다.
};

#endif // SPRITE_H