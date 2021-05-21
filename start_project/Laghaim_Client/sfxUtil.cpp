#include "stdafx.h"
/*************************************************************************************************
	sfxUtil - 2002. 4. 3
	made by : Nice Guy
*************************************************************************************************/
#include "sfx.h"


int RandomNum(int iMin, int iMax)
{
	if(iMin == iMax)
		return(iMin);

	return ( (rand() % (abs(iMax-iMin)+1))+iMin );
}

float RandomNum(float fMin, float fMax)
{
	if (fMin == fMax)
		return(fMin);

	float fRandom = (float)rand() / (float)RAND_MAX;

	return( (fRandom * (float)fabs(fMax-fMin))+fMin );
}


void BillBoard(D3DMATRIX* matWorld, D3DVECTOR target, bool onlyRotateY, float rxBillBoard )
{
	D3DMATRIX		matRx, matRy;
	D3DVECTOR		vViewVec;
	D3DVECTOR		vPos_Target, vPos_World;

	float			rx = 0.0f, ry = 0.0f, rz = 0.0f;

	vPos_Target = D3DVECTOR( target.x, target.y, target.z );
	vPos_World = D3DVECTOR( 0.0f, 0.0f, 0.0f );

	vViewVec = vPos_Target - vPos_World;

	ry = atan2f(vViewVec.x, vViewVec.z);

	if( ! onlyRotateY )
		rx = atan2f(vViewVec.x*sinf(ry) + vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f);
	else
		rx = rxBillBoard;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(*matWorld, matRx, matRy);

//	sprintf(OutputTextBuffer, "x=%.3f, y=%.3f, z=%.3f", m_vPosition.x, m_vPosition.y, m_vPosition.z );
//	OutputText( 5, 15, OutputTextBuffer );

//	sprintf(OutputTextBuffer, "rx=%.3f   ry=%.3f", rx, ry);
//	OutputText( 5, 30, OutputTextBuffer );

}


/*
void BillBoard(D3DMATRIX* matWorld, D3DVECTOR target, bool onlyRotateY, float rxBillBoard)
{
	D3DMATRIX		matRx, matRy;
	D3DVECTOR		vViewVec;
	D3DVECTOR		vPos_Target, vPos_World;

	float			rx = 0.0f, ry = 0.0f;
	float			orgx, orgy, orgz;


	orgx = matWorld->_41;
	orgy = matWorld->_42;
	orgz = matWorld->_43;

	vPos_Target = D3DVECTOR( target.x, target.y, target.z );
	vPos_World = D3DVECTOR( orgx, orgy, orgz );

	vViewVec = vPos_Target - vPos_World;

	ry = atan2f(vViewVec.x, vViewVec.z);

	if( ! onlyRotateY )
		rx = atan2f(vViewVec.x*sinf(ry) + vViewVec.z*cosf(ry), -vViewVec.y) - (g_PI*0.5f);
	else
		rx = rxBillBoard;

	ry = ry + g_PI;

	if (ry >= g_2_PI)
		ry -= g_2_PI;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(*matWorld, matRx, matRy);

	matWorld->_41 = orgx;
	matWorld->_42 = orgy;
	matWorld->_43 = orgz;

//	sprintf(OutputTextBuffer, "x=%.3f, y=%.3f, z=%.3f", m_vPosition.x, m_vPosition.y, m_vPosition.z );
//	OutputText( 5, 15, OutputTextBuffer );

//	sprintf(OutputTextBuffer, "rx=%.3f   ry=%.3f", rx, ry);
//	OutputText( 5, 30, OutputTextBuffer );

} */

///////////////////////////////// POINT ON CURVE /////////////////////////////////
/////
/////	This function returns an XYZ point along the curve, depending on t (0 to 1)
/////
///////////////////////////////// POINT ON CURVE /////////////////////////////////

D3DVECTOR PointOnCurve(D3DVECTOR start, D3DVECTOR control1, D3DVECTOR control2, D3DVECTOR end, float time)
{
	float var1, var2, var3;
	D3DVECTOR vPoint = D3DVECTOR(0.0f, 0.0f, 0.0f);

	// Here is the juice of our tutorial.  Below is the equation for a 4 control point
	// bezier curve:
	// B(t) = P1 * ( 1 - t )^3 + P2 * 3 * t * ( 1 - t )^2 + P3 * 3 * t^2 * ( 1 - t ) + P4 * t^3
	// Yes I agree, this isn't the most intuitive equation, but it is pretty straight forward.
	// If you got up to Trig, you will notice that this is a polynomial.  That is what a curve is.
	// "t" is the time from 0 to 1.  You could also think of it as the distance along the
	// curve, because that is really what it is.  P1 - P4 are the 4 control points.
	// They each have an (x, y, z) associated with them.  You notice that there is a lot of
	// (1 - t) 's?  Well, to clean up our code we will try to contain some of these
	// repetitions into variables.  This helps our repeated computations as well.

	// Store the (1 - t) in a variable because it is used frequently
	var1 = 1 - time;

	// Store the (1 - t)^3 into a variable to cut down computation and create clean code
	var2 = var1 * var1 * var1;

	// Store the t^3 in a variable to cut down computation and create clean code
	var3 = time * time * time;

	// Now that we have some computation cut down, we just follow the equation above.
	// If you multiply and simplify the equation, you come up with what we have below.
	// If you don't see how we came to here from the equation, multiply the equation
	// out and it will become more clear.  I don't intend to go into any more detail
	// on the math of a bezier curve, because there are far better places out there
	// with graphical displays and tons of examples.  Look in our * Quick Notes *
	// for an EXCELLENT web site that does just this.  It derives everything and has
	// excellent visuals.  It's the best I have seen so far.
	vPoint.x = var2*start.x + 3*time*var1*var1*control1.x + 3*time*time*var1*control2.x + var3*end.x;
	vPoint.y = var2*start.y + 3*time*var1*var1*control1.y + 3*time*time*var1*control2.y + var3*end.y;
	vPoint.z = var2*start.z + 3*time*var1*var1*control1.z + 3*time*time*var1*control2.z + var3*end.z;

	// Now we should have the point on the curve, so let's return it.
	return(vPoint);
}


