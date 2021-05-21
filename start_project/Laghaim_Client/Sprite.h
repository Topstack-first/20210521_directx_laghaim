#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Config.h"

#define SPRITE_ANITYPE_ONEPLAY 0   // �ִϸ��̼� Ÿ��. 1ȸ �÷���
#define SPRITE_ANITYPE_LOOP    1   // �ִϸ��̼� Ÿ��. ���� ����
#define SPRITE_ANITYPE_ONESTOP 2   // �ִϸ��̼� Ÿ��. 1ȸ �÷����� ������ �����ӿ� �����ִ�Ÿ��.
#define SPRITE_ANITYPE_REV_LOOP    4   // �ִϸ��̼� Ÿ��. ���� ���� �� ���ư��� ������ ���ɾƼ� �������� ����.

///////////////////////////////////////////////////////////////////////////////////////////
// ������ : �̿���
// ������ : 05-09-29
// ��  �� :2D ��������Ʈ �ִϸ��̼� ���� Ŭ����.
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
		m_NumOfImage = 0; // ������ ��.

		m_DelayTick = 100; // �� ȭ���� �Ѿ�µ� ������ų �ð�. (0.0f�� �ʱ�ȭ�ϸ� �������� ����.)
		m_RemainderTick = 0;

		m_NowFrame = 0; // ���� ��� ������.

//		m_DrawPosX = 0; // ���� ������ ��ǥ.
//		m_DrawPosY = 0;
		m_Width = 0;  // �о�� �̹����� ���� ����.
		m_Height = 0;

		m_AniType = 0; // ���� ���ε��� �ִϸ��̼� ���..

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
		m_bAniLock = FALSE; // ���Ӱ� ���۽ÿ� ���� ������ Ǯ�� ����.
		m_RemainderTick = 0;
	}

	void LoadRes( const char *ImageFileName, int NumOfImage, DWORD DelayTick, DWORD ColorKey = -1 );
	int Update( DWORD ElapsedTick ); // ���� ������ ��ȣ ����. ���� �����ӹ�ȣ�� ���ϵ� ���ش�.
	int UpdateFast( DWORD ElapsedTick ); // �� �� ������ ����. ��� ������ ���� Ÿ���� ������ ���Ѵ�.

	void Draw( int x, int y, int DrawWidth = -1, int DrawHeight = -1 );
	void DrawFrame( int Frame, int x, int y, int DrawWidth = -1, int DrawHeight = -1 ); // Ư�� �������� ���.

	CSurface	**m_ppImageSurface; // ���� �̹��� ���ǽ�

	int m_Width;  // �о�� �̹����� ���� ����.
	int m_Height;

	int m_NumOfImage; // ������ ��.
	DWORD m_DelayTick; // �� ȭ���� �Ѿ�µ� ������ų �ð�.
	int m_NowFrame; // ���� ��� ������.
	DWORD m_RemainderTick; // �� ���Ŵ� ������ �ð��� ���.(�̰� ����� �������־�� ���ÿ� ������ �÷����Ҷ� ����� �ȴ�.)

	int m_AniType; // �ִϸ��̼� Ÿ��.(0 - 1ȸ �÷���, 1 - ���� ����)

	BOOL m_bAniLock; // �ִϸ��̼��� ������ ���߱����� �÷���.
	BOOL m_bReverseAni; // ���� �ִϸ��̼� ������ �÷���. �ִ�Ÿ�� SPRITE_ANITYPE_REV_LOOP �϶� ����Ѵ�.
};

#endif // SPRITE_H