#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <assert.h>
#include <mmsystem.h>
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "DropStar.h"
#include "LoadEffectSound.h"
#include "CapSyong.h"
#include "GUtil.h"
// nate 2004 - 7 : Image Manager
#include "ImageDefine.h"


extern DWORD g_dwBaseTime;
unsigned short g_DropStarIndex[];

extern int g_EffectTextr[];
extern D3DMATRIX g_matView;
extern D3DMATRIX g_invView;

#define STAR_AREA	16	// ���� ����� ����
#define STAR_WIDTH	1	// �� �ʺ�
#define STAR_Y		(-5.0f)	// ���� ���۵Ǵ� ����
#define STAR_HEIGHT	(20.0f) // �ִ�� �������� �������� ����
#define STAR_SPEED	(0.01f) // 1/1000 �� ���� �������� �ӵ�
#define STAR_CREATE	50		// �� �ϳ��� �����Ǵ� �ð� ms

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropStar::CDropStar()
{
	memset(m_StarX, 0, sizeof(m_StarX));
	memset(m_StarY, 0, sizeof(m_StarY));
	memset(m_StarZ, 0, sizeof(m_StarZ));

	m_lastTime = 0;
	for (int i = 0; i < DROPSTAR_COUNT; i++)
		m_StarY0[i] = 10000.0f; // ���� ���� ��ġ���� �Ʒ��� ��������.
}

CDropStar::~CDropStar()
{}

void CDropStar::Render(float x, float y, float z)
{
	// �ʹ� �ð��� ���� ������ �ð� �ʱ�ȭ...
	if (g_dwBaseTime > m_lastTime + 500)
		m_lastTime = g_dwBaseTime;

	int star_num = 0;
	DWORD pastTime = g_dwBaseTime - m_lastTime;
	while (g_dwBaseTime >= m_lastTime + STAR_CREATE)
	{
		star_num++;
		m_lastTime += STAR_CREATE;
	}

	int star_count = 0;
	for (int i = 0; i < DROPSTAR_COUNT; i++)
	{
		if (m_StarY0[i] < 9999.0f)
		{
			// ���� �ֳ׿�.
			m_StarY[i] -= STAR_SPEED * pastTime;
			if (m_StarY0[i] - m_StarY[i] > STAR_HEIGHT) // ���� ���ٳ�.. �Ѥ�;
				m_StarY0[i] = 10000.0f;
			else
			{
				int uv = rand() % 4;
				D3DVECTOR starVector;
				starVector.x = m_StarX[i];
				starVector.y = m_StarY[i];
				starVector.z = m_StarZ[i];
				D3DMath_VectorMatrixMultiply(starVector, starVector, g_matView);
				m_StarVertex[star_count*4+0] = D3DLVERTEX(D3DVECTOR(starVector.x + STAR_WIDTH,
											   starVector.y + STAR_WIDTH,
											   starVector.z),
											   0xffffffff, 0L, uv*0.25f, 1);
				m_StarVertex[star_count*4+1] = D3DLVERTEX(D3DVECTOR(starVector.x + STAR_WIDTH,
											   starVector.y - STAR_WIDTH,
											   starVector.z),
											   0xffffffff, 0L, uv*0.25f, 0);
				m_StarVertex[star_count*4+2] = D3DLVERTEX(D3DVECTOR(starVector.x - STAR_WIDTH,
											   starVector.y + STAR_WIDTH,
											   starVector.z),
											   0xffffffff, 0L, (uv+1)*0.25f, 1);
				m_StarVertex[star_count*4+3] = D3DLVERTEX(D3DVECTOR(starVector.x - STAR_WIDTH,
											   starVector.y - STAR_WIDTH,
											   starVector.z),
											   0xffffffff, 0L, (uv+1)*0.25f, 0);
				NKMath_LVertexMatrixMultiply(m_StarVertex[star_count*4+0], m_StarVertex[star_count*4+0], g_invView);
				NKMath_LVertexMatrixMultiply(m_StarVertex[star_count*4+1], m_StarVertex[star_count*4+1], g_invView);
				NKMath_LVertexMatrixMultiply(m_StarVertex[star_count*4+2], m_StarVertex[star_count*4+2], g_invView);
				NKMath_LVertexMatrixMultiply(m_StarVertex[star_count*4+3], m_StarVertex[star_count*4+3], g_invView);
				star_count++;
			}
		}
		else
		{
			// ���� ���׿�.
			if (star_num > 0)
			{
				star_num--;
				m_StarY0[i] = m_StarY[i] = y + STAR_Y;
				m_StarX[i] = x + (rand() % STAR_AREA) - (STAR_AREA / 2);
				m_StarZ[i] = z + (rand() % STAR_AREA) - (STAR_AREA / 2);
			}
		}
	}
	if (star_count > 0)
	{
		if( !GET_D3DDEVICE() )
			return;

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

		LPDIRECTDRAWSURFACE7 starTexture = NULL;
		if( g_pCapsyongTexture )
			starTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_STAR_1]);

		if (GET_D3DDEVICE()->SetTexture(0, starTexture) != DD_OK)
			GET_D3DDEVICE()->SetTexture(0, NULL);

		GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
											   m_StarVertex, star_count*4, g_DropStarIndex, star_count*6, NULL );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	}
}