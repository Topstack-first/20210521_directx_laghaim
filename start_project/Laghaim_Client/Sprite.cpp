#include "stdafx.h"



#include <string.h>
#include <direct.h>
#include "main.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "Sprite.h"


extern int g_EffectTextr[];


///////////////////////////////////////////////////////////////////////////////////////
// -= ��������Ʈ �׸� ������ �غ� =-
// ��������Ʈ�� �� �׸����ϵ��� ���ϸ��� �Ȱ��� ��ġ��Ű�� �ڿ� ���ڸ� ��ȣ�� ���δ�.
// ��: aaaa00.bmp, aaaa01.bmp ~ aaaa99.bmp
// �� �׸� ������ �ε�� ���� �Է��Ѵ�.

void CSprite::LoadRes( const char *ImageFileName, int NumOfImage, DWORD DelayTick, DWORD ColorKey ) // ���ϸ� ��� ����.Ȯ���� ������ �̾���Կ� ����.
{
	if( !g_pDisplay )
		return;

	Init(); // �������� �ѹ� �ʱ�ȭ.

	m_NumOfImage = NumOfImage;
	m_DelayTick = DelayTick;

	SAFE_DELETE_ARRAY( m_ppImageSurface ); // Ȥ�� �𸣴ϱ� �����ϰ� �޸����� �˻�.
	m_ppImageSurface = new CSurface*[NumOfImage];

	if( !m_ppImageSurface )
	{
		MessageBox( NULL, "Memory Low! In CSprite::LoadRes", "Error", MB_OK );

		return;
	}

	char strTemp[256] = {0,};

	for( int i = 0 ; i < m_NumOfImage ; ++i )
	{
		sprintf( strTemp, "%s%02d.bmp", ImageFileName, i ); // ���� �ε��� ������ ��ȣ�� �ٿ� ���� ��θ� �����.

		if( g_pDisplay->CreateSurfaceFromBitmap( &m_ppImageSurface[i], strTemp ) != DD_OK )
		{
			// �̹��� �ε� ���и�...
			MessageBox( NULL, "CreateSurfaceFromBitmap Failed! In CSprite::LoadRes", "Error", MB_OK );

			return;
		}

		if( m_ppImageSurface[i] && ColorKey != -1 ) // �ø�Ű ���ڰ� -1�̸� ����ó�� ���ϵ���...
		{
			m_ppImageSurface[i]->SetColorKey( ColorKey );
		}

	}
}

int CSprite::Update( DWORD ElapsedTick )
{
	if( !g_pDisplay || !m_ppImageSurface )
		return -1;

	if( m_DelayTick <= 0 || m_NowFrame >= m_NumOfImage ) // �Ѿ�� ������ �����̰� ������ 0�� �ȵȴ�. ���� 1ȸ �÷��� Ÿ���� 1ȸ �÷��̰� ��������(���� �������� �� �����Ӻ��� ũ�� �׷��� �Ǵ�) �� ���̻� �ȵ���.
		return -1; // �̰�� �������� ������Ű�� �ʴ´�.

	if( m_bAniLock ) // �ִϸ��̼� ������ ������ ���ߴ� �༮.
		return -1;

	m_RemainderTick += ElapsedTick; // ����..

	while( m_RemainderTick > m_DelayTick ) // �������� �������Ѿ� �� ��ŭ Ŀ������...(���� ���� �ȵ��� ��ġ ���� ������.)
	{
		if( m_bReverseAni ) // ���� �ִϸ��̼� ����� ����.
		{
			--m_NowFrame;
			if( m_NowFrame < 0 ) // ���� �������� �Ѱ�����...
			{
				switch( m_AniType )
				{
				case SPRITE_ANITYPE_ONEPLAY: // ������ ó���� �ʿ����. ���� �������� ��ü �������� �Ѿ�� �ڵ����� �����.
					break;

				case SPRITE_ANITYPE_LOOP:

					m_NowFrame = m_NumOfImage-1; // ���������� �������´�.

					break;

				case SPRITE_ANITYPE_ONESTOP:

					m_NowFrame = 0; // ó�� ���������� �������ְ�.
					m_bAniLock = TRUE; // ���̻� �ȵ��� ����.

					break;

				case SPRITE_ANITYPE_REV_LOOP:

					m_NowFrame = 0; // ���� ������ ������ ������Ѵ�.
					m_bReverseAni = 0; // ��������带 ��� ��Ų��.

					break;
				}
			}
		}
		else
		{
			++m_NowFrame;
			if( m_NowFrame >= m_NumOfImage ) // �� �������� �Ѱ�����...
			{
				switch( m_AniType )
				{
				case SPRITE_ANITYPE_ONEPLAY: // ������ ó���� �ʿ����. ���� �������� ��ü �������� �Ѿ�� �ڵ����� �����.
					break;

				case SPRITE_ANITYPE_LOOP:

					m_NowFrame = 0; // ó������ �������´�.

					break;

				case SPRITE_ANITYPE_ONESTOP:

					m_NowFrame = m_NumOfImage-1; // ������ ���������� �������ְ�.
					m_bAniLock = TRUE; // ���̻� �ȵ��� ����.

					break;

				case SPRITE_ANITYPE_REV_LOOP:

					m_NowFrame = m_NumOfImage-1; // ���� ������ ������ ������Ѵ�.
					m_bReverseAni = 1; // ��������带 ��� ��Ų��.

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

	if( m_DelayTick <= 0 || m_NowFrame >= m_NumOfImage ) // �Ѿ�� ������ �����̰� ������ 0�� �ȵȴ�. ���� 1ȸ �÷��� Ÿ���� 1ȸ �÷��̰� ��������(���� �������� �� �����Ӻ��� ũ�� �׷��� �Ǵ�) �� ���̻� �ȵ���.
		return -1; // �̰�� �������� ������Ű�� �ʴ´�.

	if( m_bAniLock ) // �ִϸ��̼� ������ ������ ���ߴ� �༮.
		return -1;

	m_RemainderTick += ElapsedTick; // ����..

	int PlusFrame = m_RemainderTick / m_DelayTick;
	m_RemainderTick = m_RemainderTick % m_DelayTick;

	m_NowFrame += PlusFrame;

	if( m_NowFrame >= m_NumOfImage ) // �� �������� �Ѱ�����...
	{
		switch( m_AniType )
		{
		case SPRITE_ANITYPE_ONEPLAY:
			break; // �� ���� ��������...

		case SPRITE_ANITYPE_LOOP:

			m_NowFrame %= m_NumOfImage; // ó������ �������´�.

			break;

		case SPRITE_ANITYPE_ONESTOP:

			m_NowFrame = m_NumOfImage-1; // ������ ���������� �������ְ�.
			m_bAniLock = TRUE; // ���̻� �ȵ��� ����.

			break;

		case SPRITE_ANITYPE_REV_LOOP: // �н�Ʈ ������Ʈ�� ������ ����Ÿ���� ���� ����.
			return -1;
		}
	}

	return m_NowFrame;
}

void CSprite::Draw( int x, int y, int DrawWidth, int DrawHeight )
{
	if( !g_pDisplay || !m_ppImageSurface || m_NowFrame < 0 || m_NowFrame >= m_NumOfImage ) // �� �������� �ѱ���(�� 1ȸ �÷��� Ÿ�Կ��� 1ȸ �÷��̰� �������)���� ��¾���.
		return;

	if( DrawWidth == -1 && DrawHeight == -1 ) // ������ ������ ���� ��.
		g_pDisplay->Blt( x, y, m_ppImageSurface[m_NowFrame] );
	else // ������ ������ ���� ��..(�̰�� ����,���ΰ� �� �ԷµǾ��־�� �Կ� ����.
		g_pDisplay->Blt( x, y, x+DrawWidth, y+DrawHeight, m_ppImageSurface[m_NowFrame] );
}

void CSprite::DrawFrame( int Frame, int x, int y, int DrawWidth, int DrawHeight ) // Ư�� �������� ���.
{
	if( !g_pDisplay || !m_ppImageSurface || m_NowFrame < 0 || m_NowFrame >= m_NumOfImage ) // �� �������� �ѱ���(�� 1ȸ �÷��� Ÿ�Կ��� 1ȸ �÷��̰� �������)���� ��¾���.
		return;

	if( DrawWidth == -1 && DrawHeight == -1 ) // ������ ������ ���� ��.
		g_pDisplay->Blt( x, y, m_ppImageSurface[Frame] );
	else // ������ ������ ���� ��..(�̰�� ����,���ΰ� �� �ԷµǾ��־�� �Կ� ����.
		g_pDisplay->Blt( x, y, x+DrawWidth, y+DrawHeight, m_ppImageSurface[Frame] );
}
