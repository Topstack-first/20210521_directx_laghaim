#include "stdafx.h"
//-----------------------------------------------------------------------------
// File: D3DMath.cpp
//
// Desc: Shortcut macros and functions for using DX objects
//
// Copyright (c) 1997-1999 Microsoft Corporation. All rights reserved
//-----------------------------------------------------------------------------
#define D3D_OVERLOADS

#include <math.h>

#include "D3DMath.h"



//-----------------------------------------------------------------------------
// Name: D3DMath_MatrixMultiply()
// Desc: Does the matrix operation: [Q] = [A] * [B]. Note that the order of
//       this operation was changed from the previous version of the DXSDK.
//-----------------------------------------------------------------------------
VOID D3DMath_MatrixMultiply( D3DMATRIX& q, D3DMATRIX& a, D3DMATRIX& b )
{
	FLOAT* pA = (FLOAT*)&a;
	FLOAT* pB = (FLOAT*)&b;
	FLOAT  pM[16];

	ZeroMemory( pM, sizeof(D3DMATRIX) );

	for( WORD i=0; i<4; i++ )
		for( WORD j=0; j<4; j++ )
			for( WORD k=0; k<4; k++ )
				pM[4*i+j] +=  pA[4*i+k] * pB[4*k+j];

	memcpy( &q, pM, sizeof(D3DMATRIX) );
}

//-----------------------------------------------------------------------------
// Name: D3DMath_MatrixInvert()
// Desc: Does the matrix operation: [Q] = inv[A]. Note: this function only
//       works for matrices with [0 0 0 1] for the 4th column.
//-----------------------------------------------------------------------------
HRESULT D3DMath_MatrixInvert( D3DMATRIX& q, D3DMATRIX& a )
{
	if( fabs(a._44 - 1.0f) > .001f)
		return E_INVALIDARG;
	if( fabs(a._14) > .001f || fabs(a._24) > .001f || fabs(a._34) > .001f )
		return E_INVALIDARG;

	FLOAT fDetInv = 1.0f / ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
							 a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
							 a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

	q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
	q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
	q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
	q._14 = 0.0f;

	q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
	q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
	q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
	q._24 = 0.0f;

	q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
	q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
	q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
	q._34 = 0.0f;

	q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
	q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
	q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
	q._44 = 1.0f;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: D3DMath_VectorMatrixMultiply()
// Desc: Multiplies a vector by a matrix
//-----------------------------------------------------------------------------
HRESULT D3DMath_VectorMatrixMultiply( D3DVECTOR& vDest, D3DVECTOR& vSrc,
									  D3DMATRIX& mat)
{
	// 예외 사항 체크. 실제로 이녀석들에서 튕기는 경우가 있었기에 막았다.
	if( (&vDest) == NULL ) // &연산자 해서 체크해야함에 유의.
	{
		// 이게 없으면 도리가 없다. 그냥 실패.
		return E_INVALIDARG;
	}
	else if( (&vSrc) == NULL || (&mat) == NULL ) // 이것들이 없는 경우는 그냥 대체값 넣어주고 리턴시키자.연산시에 뻑나는 경우로 실제 이경우로 튕겼던 듯.
	{
		// 0.0f로 초기화.
		vDest.x = 0.0f;
		vDest.y = 0.0f;
		vDest.z = 0.0f;

		return E_INVALIDARG;
	}

	FLOAT x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
	FLOAT y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
	FLOAT z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
	FLOAT w = vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;

	if( fabs( w ) < g_EPSILON )
		return E_INVALIDARG;

	vDest.x = x/w;
	vDest.y = y/w;
	vDest.z = z/w;

	return S_OK;
}

HRESULT D3DMath_VectorMatrixMultiply2( D3DVECTOR& vDest, D3DVECTOR& vSrc,
									   D3DMATRIX& mat)
{
	// 예외 사항 체크. 실제로 이녀석들에서 튕기는 경우가 있었기에 막았다.
	if( (&vDest) == NULL ) // &연산자 해서 체크해야함에 유의.
	{
		// 이게 없으면 도리가 없다. 그냥 실패.
		return E_INVALIDARG;
	}
	else if( (&vSrc) == NULL || (&mat) == NULL ) // 이것들이 없는 경우는 그냥 대체값 넣어주고 리턴시키자.연산시에 뻑나는 경우로 실제 이경우로 튕겼던 듯.
	{
		// 0.0f로 초기화.
		vDest.x = 0.0f;
		vDest.y = 0.0f;
		vDest.z = 0.0f;

		return E_INVALIDARG;
	}

	FLOAT x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31;// + mat._41;
	FLOAT y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32;// + mat._42;
	FLOAT z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33;// + mat._43;
	FLOAT w = vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;

	if( fabs( w ) < g_EPSILON )
		return E_INVALIDARG;

	vDest.x = x/w;
	vDest.y = y/w;
	vDest.z = z/w;

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: D3DMath_VertexMatrixMultiply()
// Desc: Multiplies a vertex by a matrix
//-----------------------------------------------------------------------------
HRESULT D3DMath_VertexMatrixMultiply( D3DVERTEX& vDest, D3DVERTEX& vSrc,
									  D3DMATRIX& mat )
{
	// 예외 사항 체크. 실제로 이녀석들에서 튕기는 경우가 있었기에 막았다.
	if( (&vDest) == NULL ) // &연산자 해서 체크해야함에 유의.
	{
		// 이게 없으면 도리가 없다. 그냥 실패.
		return E_INVALIDARG;
	}
	else if( (&vSrc) == NULL || (&mat) == NULL ) // 이것들이 없는 경우는 그냥 대체값 넣어주고 리턴시키자.연산시에 뻑나는 경우로 실제 이경우로 튕겼던 듯.
	{
		// 0.0f로 초기화.
		vDest.x = 0.0f;
		vDest.y = 0.0f;
		vDest.z = 0.0f;

		return E_INVALIDARG;
	}

	HRESULT    hr;
	D3DVECTOR* pSrcVec  = (D3DVECTOR*)&vSrc.x;
	D3DVECTOR* pDestVec = (D3DVECTOR*)&vDest.x;

	// D3DVERTEX의 구조
	//
	if( SUCCEEDED( hr = D3DMath_VectorMatrixMultiply( *pDestVec, *pSrcVec,
						mat ) ) )
	{
		pSrcVec  = (D3DVECTOR*)&vSrc.nx;
		pDestVec = (D3DVECTOR*)&vDest.nx;
		hr = D3DMath_VectorMatrixMultiply2( *pDestVec, *pSrcVec, mat);
	}
	return hr;
}

HRESULT D3DMath_LVertexMatrixMultiply( D3DLVERTEX& vDest, D3DLVERTEX& vSrc,
									   D3DMATRIX& mat)
{
	// 예외 사항 체크. 실제로 이녀석들에서 튕기는 경우가 있었기에 막았다.
	if( (&vDest) == NULL ) // &연산자 해서 체크해야함에 유의.
	{
		// 이게 없으면 도리가 없다. 그냥 실패.
		return E_INVALIDARG;
	}
	else if( (&vSrc) == NULL || (&mat) == NULL ) // 이것들이 없는 경우는 그냥 대체값 넣어주고 리턴시키자.연산시에 뻑나는 경우로 실제 이경우로 튕겼던 듯.
	{
		// 0.0f로 초기화.
		vDest.x = 0.0f;
		vDest.y = 0.0f;
		vDest.z = 0.0f;

		return E_INVALIDARG;
	}

	D3DVECTOR* pSrcVec  = (D3DVECTOR*)&vSrc.x;
	D3DVECTOR* pDestVec = (D3DVECTOR*)&vDest.x;

	return D3DMath_VectorMatrixMultiply( *pDestVec, *pSrcVec, mat );
}

//-----------------------------------------------------------------------------
// Name: D3DMath_QuaternionFromRotation()
// Desc: Converts a normalized axis and angle to a unit quaternion.
//-----------------------------------------------------------------------------
VOID D3DMath_QuaternionFromRotation( FLOAT& x, FLOAT& y, FLOAT& z, FLOAT& w,
									 D3DVECTOR& v, FLOAT fTheta )
{
	x = sinf( fTheta/2.0f ) * v.x;
	y = sinf( fTheta/2.0f ) * v.y;
	z = sinf( fTheta/2.0f ) * v.z;
	w = cosf( fTheta/2.0f );
}



//-----------------------------------------------------------------------------
// Name: D3DMath_RotationFromQuaternion()
// Desc: Converts a normalized axis and angle to a unit quaternion.
//-----------------------------------------------------------------------------
VOID D3DMath_RotationFromQuaternion( D3DVECTOR& v, FLOAT& fTheta,
									 FLOAT x, FLOAT y, FLOAT z, FLOAT w )

{
	fTheta = acosf(w) * 2.0f;
	v.x    = x / sinf( fTheta/2.0f );
	v.y    = y / sinf( fTheta/2.0f );
	v.z    = z / sinf( fTheta/2.0f );
}



//-----------------------------------------------------------------------------
// Name: D3DMath_QuaternionFromAngles()
// Desc: Converts euler angles to a unit quaternion.
//-----------------------------------------------------------------------------
VOID D3DMath_QuaternionFromAngles( FLOAT& x, FLOAT& y, FLOAT& z, FLOAT& w,
								   FLOAT fYaw, FLOAT fPitch, FLOAT fRoll )

{
	FLOAT fSinYaw   = sinf( fYaw/2.0f );
	FLOAT fSinPitch = sinf( fPitch/2.0f );
	FLOAT fSinRoll  = sinf( fRoll/2.0f );
	FLOAT fCosYaw   = cosf( fYaw/2.0f );
	FLOAT fCosPitch = cosf( fPitch/2.0f );
	FLOAT fCosRoll  = cosf( fRoll/2.0f );

	x = fSinRoll * fCosPitch * fCosYaw - fCosRoll * fSinPitch * fSinYaw;
	y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
	z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
	w = fCosRoll * fCosPitch * fCosYaw + fSinRoll * fSinPitch * fSinYaw;
}




//-----------------------------------------------------------------------------
// Name: D3DMath_MatrixFromQuaternion()
// Desc: Converts a unit quaternion into a rotation matrix.
//-----------------------------------------------------------------------------
VOID D3DMath_MatrixFromQuaternion( D3DMATRIX& mat, FLOAT x, FLOAT y, FLOAT z,
								   FLOAT w )
{
	FLOAT xx = x*x;
	FLOAT yy = y*y;
	FLOAT zz = z*z;
	FLOAT xy = x*y;
	FLOAT xz = x*z;
	FLOAT yz = y*z;
	FLOAT wx = w*x;
	FLOAT wy = w*y;
	FLOAT wz = w*z;

	mat._11 = 1 - 2 * ( yy + zz );
	mat._12 =     2 * ( xy - wz );
	mat._13 =     2 * ( xz + wy );

	mat._21 =     2 * ( xy + wz );
	mat._22 = 1 - 2 * ( xx + zz );
	mat._23 =     2 * ( yz - wx );

	mat._31 =     2 * ( xz - wy );
	mat._32 =     2 * ( yz + wx );
	mat._33 = 1 - 2 * ( xx + yy );

	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._41 = mat._42 = mat._43 = 0.0f;
	mat._44 = 1.0f;
}



//-----------------------------------------------------------------------------
// Name: D3DMath_QuaternionFromMatrix()
// Desc: Converts a rotation matrix into a unit quaternion.
//-----------------------------------------------------------------------------
VOID D3DMath_QuaternionFromMatrix( FLOAT& x, FLOAT& y, FLOAT& z, FLOAT& w,
								   D3DMATRIX& mat )
{
	if( mat._11 + mat._22 + mat._33 > 0.0f )
	{
		FLOAT s = sqrtf( mat._11 + mat._22 + mat._33 + mat._44 );

		x = (mat._23-mat._32) / (2*s);
		y = (mat._31-mat._13) / (2*s);
		z = (mat._12-mat._21) / (2*s);
		w = 0.5f * s;
	}
	else
	{


	}
	FLOAT xx = x*x;
	FLOAT yy = y*y;
	FLOAT zz = z*z;
	FLOAT xy = x*y;
	FLOAT xz = x*z;
	FLOAT yz = y*z;
	FLOAT wx = w*x;
	FLOAT wy = w*y;
	FLOAT wz = w*z;

	mat._11 = 1 - 2 * ( yy + zz );
	mat._12 =     2 * ( xy - wz );
	mat._13 =     2 * ( xz + wy );

	mat._21 =     2 * ( xy + wz );
	mat._22 = 1 - 2 * ( xx + zz );
	mat._23 =     2 * ( yz - wx );

	mat._31 =     2 * ( xz - wy );
	mat._32 =     2 * ( yz + wx );
	mat._33 = 1 - 2 * ( xx + yy );

	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._41 = mat._42 = mat._43 = 0.0f;
	mat._44 = 1.0f;
}



//-----------------------------------------------------------------------------
// Name: D3DMath_QuaternionMultiply()
// Desc: Mulitples two quaternions together as in {Q} = {A} * {B}.
//-----------------------------------------------------------------------------
VOID D3DMath_QuaternionMultiply( FLOAT& Qx, FLOAT& Qy, FLOAT& Qz, FLOAT& Qw,
								 FLOAT Ax, FLOAT Ay, FLOAT Az, FLOAT Aw,
								 FLOAT Bx, FLOAT By, FLOAT Bz, FLOAT Bw )
{
	FLOAT Dx =  Ax*Bw + Ay*Bz - Az*By + Aw*Bx;
	FLOAT Dy = -Ax*Bz + Ay*Bw + Az*Bx + Aw*By;
	FLOAT Dz =  Ax*By - Ay*Bx + Az*Bw + Aw*Bz;
	FLOAT Dw = -Ax*Bx - Ay*By - Az*Bz + Aw*Bw;

	Qx = Dx;
	Qy = Dy;
	Qz = Dz;
	Qw = Dw;
}




//-----------------------------------------------------------------------------
// Name: D3DMath_SlerpQuaternions()
// Desc: Compute a quaternion which is the spherical linear interpolation
//       between two other quaternions by dvFraction.
//-----------------------------------------------------------------------------
VOID D3DMath_QuaternionSlerp( FLOAT& Qx, FLOAT& Qy, FLOAT& Qz, FLOAT& Qw,
							  FLOAT Ax, FLOAT Ay, FLOAT Az, FLOAT Aw,
							  FLOAT Bx, FLOAT By, FLOAT Bz, FLOAT Bw,
							  FLOAT fAlpha )
{
	// Compute dot product (equal to cosine of the angle between quaternions)
	FLOAT fCosTheta = Ax*Bx + Ay*By + Az*Bz + Aw*Bw;

	// Check angle to see if quaternions are in opposite hemispheres
	if( fCosTheta < 0.0f )
	{
		// If so, flip one of the quaterions
		fCosTheta = -fCosTheta;
		Bx = -Bx;
		By = -By;
		Bz = -Bz;
		Bw = -Bw;
	}

	// Set factors to do linear interpolation, as a special case where the
	// quaternions are close together.
	FLOAT fBeta = 1.0f - fAlpha;

	// If the quaternions aren't close, proceed with spherical interpolation
	if( 1.0f - fCosTheta > 0.001f )
	{
		FLOAT fTheta = acosf( fCosTheta );

		fBeta  = sinf( fTheta*fBeta ) / sinf( fTheta);
		fAlpha = sinf( fTheta*fAlpha ) / sinf( fTheta);
	}

	// Do the interpolation
	Qx = fBeta*Ax + fAlpha*Bx;
	Qy = fBeta*Ay + fAlpha*By;
	Qz = fBeta*Az + fAlpha*Bz;
	Qw = fBeta*Aw + fAlpha*Bw;
}


int Nk3DMath_CCW(int ax, int ay, int bx, int by, int cx, int cy)
{
	long res;

	res = (long) bx * cy - (long) ay * bx - (long) ax * cy - (long) by * cx
		  + (long) ax * by + (long) ay * cx;

	if (res > 0) return 1;
	else if (res < 0) return -1;
	else return 0;
}

int Nk3DMath_CCW(float ax, float ay, float bx, float by, int cx, int cy)
{
	long res;

	res =(long) (bx * cy - ay * bx - ax * cy - by * cx
				 + ax * by + ay * cx);

	if (res > 0) return 1;
	else if (res < 0) return -1;
	else return 0;
}


// 반시계방향으로 회전하는 함수
float Nk3dMath_Atan2(float y, float x)
{
	float temp;

	temp = (float)(0.0f - atan2((double) y, (double) x));
	return temp;
}


