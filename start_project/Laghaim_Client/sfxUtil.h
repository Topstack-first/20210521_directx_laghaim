#ifndef _SFX_UTIL_H_
#define _SFX_UTIL_H_

#include "Config.h"

void BillBoard(D3DMATRIX* matWorld, D3DVECTOR target, bool onlyRotateYvoid, float rxBillBoard);

int RandomNum(int iMin, int iMax);
float RandomNum(float fMin, float fMax);

D3DVECTOR PointOnCurve(D3DVECTOR start, D3DVECTOR control1, D3DVECTOR control2, D3DVECTOR end, float time);

#endif // _SFX_UTIL_H_


