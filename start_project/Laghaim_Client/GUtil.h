#ifndef _GUTIL_H_
#define _GUTIL_H_

#include "Config.h"

class CNkCharacter;

float DistPlToPl(CNkCharacter *pCha1, CNkCharacter *pCha2);		// 플레이어간에 거리를 리턴한다.
float DistPlToPl(float from_x, float from_z, float to_x, float to_z);
float DistPlToPlAbs(float from_x, float from_z, float to_x, float to_z);
void  GetDirectionElement(float radDir, float &x, float &z);
float Dist3PTo3P(float from_x, float from_y, float from_z, float to_x, float to_y, float to_z);
float DistVecToVec(D3DVECTOR &vFrom, D3DVECTOR &vTo);
bool get_normal_vector(D3DVECTOR &vRes, D3DVECTOR &vFrom, D3DVECTOR &vTo);
D3DLVERTEX *MakeLVertexRect(D3DLVERTEX *pLvertex, float fWidth, float fHeight, BOOL bHalf);

#endif // _GUTIL_H_