#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "NkCharacter.h"
#include "GUtil.h"
#include "D3DMath.h"
#include <math.h>


float DistPlToPl(CNkCharacter *pCha1, CNkCharacter *pCha2)
{
	return sqrtf( ((pCha1->m_wx - pCha2->m_wx)*(pCha1->m_wx - pCha2->m_wx) + (pCha1->m_wz - pCha2->m_wz)*(pCha1->m_wz - pCha2->m_wz)));
}

float DistPlToPl(float from_x, float from_z, float to_x, float to_z)
{
	return sqrtf( (from_x - to_x)*(from_x - to_x) + (from_z-to_z)*(from_z-to_z) );
}

float DistPlToPlAbs(float from_x, float from_z, float to_x, float to_z)
{
	//return (float) ( fabs(from_x-to_x)+fabs(from_z-to_z) );				// 잘못된 dist
	return DistPlToPl(from_x, from_z, to_x, to_z);							// by.Sol9 [2012.06.15]
}

float Dist3PTo3P(float from_x, float from_y, float from_z, float to_x, float to_y, float to_z)
{
	return sqrtf( (from_x-to_x)*(from_x-to_x) + (from_y-to_y)*(from_y-to_y) + (from_z-to_z)*(from_z-to_z) );
}

float DistVecToVec(D3DVECTOR &vFrom, D3DVECTOR &vTo)
{
	return sqrtf((vFrom.x-vTo.x)*(vFrom.x-vTo.x)+(vFrom.y-vTo.y)*(vFrom.y-vTo.y)+(vFrom.z-vTo.z)*(vFrom.z-vTo.z));
}

void  GetDirectionElement(float radDir, float &x, float &z)
{
	float mag;			// w 는 밑변, h는 높이

	if (g_PI_DIV_2 - radDir <=g_EPSILON)
	{
		x = 0.0f;
		z = 1.0f;
		return;
	}
	else if (g_PI - radDir <= g_EPSILON)
	{
		x = -1.0f;
		z = 0.0f;
		return;
	}
	else if (g_PI + g_PI_DIV_2 - radDir <= g_EPSILON)
	{
		x = 0.0f;
		z = -1.0f;
		return;
	}
	else if (D3DMath_IsZero(radDir))
	{
		x = 1.0f;
		z = 0.0f;
		return;
	}
	else
		;

	x = 1.0f;
	z = tanf(radDir);

	mag = sqrtf(x*x+z*z);
	x = x / mag;
	z = z / mag;

	radDir = fmodf(radDir, g_2_PI);

	if (radDir < 0.0f)
		radDir += g_2_PI;

	// 어느 사분면에 있는지 알아본다.
	// 위의 계산값은 180도 이하에서의 계산값이다.
	if (radDir > 0.0f && radDir <= g_PI_DIV_2)
	{
		return;
	}
	else if (radDir > g_PI_DIV_2 && radDir < g_PI)  		// 결과는 음수, x 는 음수, z는 양수
	{
		x = 0.0f - x;
		z = 0.0f - z;
		return;
	}
	else if (radDir > g_PI && radDir < (g_PI+g_PI_DIV_2))  	// 둘다 음수가 되어야 한다.
	{
		x = 0.0f - x;
		z = 0.0f - z;
	}
	else
		;
}

void itocola(char *buf, int num)
{
	char temp[20];
	int str_count = 0;
	while (num > 0)
	{
		if (str_count > 0 && str_count % 3 == 0)
		{
			temp[str_count] = ',';
			str_count++;
		}
		int mod = num % 10;
		temp[str_count] = mod + '0';
		str_count++;
		num /= 10;
	}
	if (str_count == 0)
	{
		temp[str_count++] = '0';
	}
	for (int i = 0; i < str_count; i++)
		buf[i] = temp[str_count-i-1];
	buf[str_count] = '\0';
}

bool get_normal_vector(D3DVECTOR &vRes, D3DVECTOR &vFrom, D3DVECTOR &vTo)
{
	vRes.x = vTo.x - vFrom.x;
	vRes.y = vTo.y - vFrom.y;
	vRes.z = vTo.z - vFrom.z;

	vRes = Normalize(vRes);
	return true;
}

// ------------------------------------------------------------------------------
// MakeLVertexRect()
// Desc : 삼각형 두개를 이용한 사각형을 만든다.
// Param : Strip으로 그리기 때문에 pLvertex에는 4개의 버턱스가 미리 할당되어 있어야 한다.
// ------------------------------------------------------------------------------
D3DLVERTEX *MakeLVertexRect(D3DLVERTEX *pLvertex, float fWidth, float fHeight, BOOL bHalf)
{
	if (pLvertex == NULL)
		return NULL;

	if (fWidth < 0 || fHeight < 0)
	{
		MessageBox(NULL, "MakeLVertex", "경고", MB_OK);
		return NULL;
	}

	if (bHalf)
	{
		pLvertex[0] = D3DLVERTEX( D3DVECTOR(-fWidth, -fHeight, 0), 0xffffffff, 0, 0.0f, 1.0f );
		pLvertex[1] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffffff, 0, 0.0f, 0.0f );
		pLvertex[2] = D3DLVERTEX( D3DVECTOR( fWidth, -fHeight, 0), 0xffffffff, 0, 1.0f, 1.0f );
		pLvertex[3] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffffff, 0, 1.0f, 0.0f );
	}
	else
	{
		pLvertex[0] = D3DLVERTEX( D3DVECTOR(-fWidth, 0, 0), 0xffffffff, 0, 0.0f, 1.0f );
		pLvertex[1] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffffff, 0, 0.0f, 0.0f );
		pLvertex[2] = D3DLVERTEX( D3DVECTOR( fWidth, 0, 0), 0xffffffff, 0, 1.0f, 1.0f );
		pLvertex[3] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffffff, 0, 1.0f, 0.0f );
	}

	return pLvertex;
}

