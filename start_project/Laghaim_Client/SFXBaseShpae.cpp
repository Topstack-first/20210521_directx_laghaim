#include "stdafx.h"
/*************************************************************************************************
	SFX - 2002. 3. 30 ~ 2003. 2. 30
	made by : Nice Guy
*************************************************************************************************/
#include "sfx.h"
#include "main.h"



/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CTriPlane Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CTriPlane::CTriPlane()
{
	m_TriPlane = NULL;
	max_triplane_vertexs = 0;

	D3DUtil_SetIdentityMatrix(matWorld);
	D3DUtil_InitMaterial(mtrl, 1.0f, 1.0f, 1.0f, 1.0f);
}
CTriPlane::~CTriPlane()
{
	if( m_TriPlane )
	{
		delete [] m_TriPlane;
		m_TriPlane = NULL;
	}
}


void CTriPlane::Create(float width, float height, bool bothFaces)
{
	D3DVECTOR		normal = D3DVECTOR( 0.0f, 1.0f, 0.0f );
	float			triwidth = width * 0.5f;
	float			triheight = height * 0.5f;


	if( bothFaces )
	{
		max_triplane_vertexs = 4;
		m_TriPlane = new D3DLVERTEX[max_triplane_vertexs];

		m_TriPlane[0] = D3DLVERTEX( D3DVECTOR( -triwidth, 0.0f, -triheight ), 0xffffffff, 0, 0.0f, 1.0f );
		m_TriPlane[1] = D3DLVERTEX( D3DVECTOR( -triwidth, 0.0f, triheight ), 0xffffffff, 0, 0.0f, 0.0f );
		m_TriPlane[2] = D3DLVERTEX( D3DVECTOR( triwidth, 0.0f, -triheight ), 0xffffffff, 0, 1.0f, 1.0f );
		m_TriPlane[3] = D3DLVERTEX( D3DVECTOR( -triwidth, 0.0f, -triheight ), 0xffffffff, 0, 0.0f, 1.0f );
	}
	else
	{
		max_triplane_vertexs = 3;
		m_TriPlane = new D3DLVERTEX[max_triplane_vertexs];

		m_TriPlane[0] = D3DLVERTEX( D3DVECTOR( -triwidth, 0.0f, -triheight ), 0xffffffff, 0, 0.0f, 1.0f );
		m_TriPlane[1] = D3DLVERTEX( D3DVECTOR( -triwidth, 0.0f, triheight ), 0xffffffff, 0, 0.0f, 0.0f );
		m_TriPlane[2] = D3DLVERTEX( D3DVECTOR( triwidth, 0.0f, -triheight ), 0xffffffff, 0, 1.0f, 1.0f );
	}
	return;
}

void CTriPlane::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	GET_D3DDEVICE()->SetMaterial( &mtrl );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_TriPlane, max_triplane_vertexs, 0 );
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CRectPlane Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CRectPlane::CRectPlane()
{
	m_RectPlane = NULL;
	max_rectplane_vertexs = 0;

	D3DUtil_SetIdentityMatrix(matWorld);
	D3DUtil_InitMaterial(mtrl, 1.0f, 1.0f, 1.0f, 1.0f);
}
CRectPlane::~CRectPlane()
{
	if( m_RectPlane )
	{
		delete [] m_RectPlane;
		m_RectPlane = NULL;
	}
}


void CRectPlane::Create(float width, float height, bool bothFaces)
{
	D3DVECTOR		normal = D3DVECTOR( 0.0f, 1.0f, 0.0f );
	float			rectwidth = width * 0.5f;
	float			rectheight = height * 0.5f;


	if( bothFaces )
	{
		max_rectplane_vertexs = 6;
		m_RectPlane = new D3DLVERTEX[max_rectplane_vertexs];

		m_RectPlane[0] = D3DLVERTEX( D3DVECTOR( -rectwidth, 0.0f, -rectheight ), 0xffffffff, 0, 0.0f, 1.0f );
		m_RectPlane[1] = D3DLVERTEX( D3DVECTOR( -rectwidth, 0.0f, rectheight ), 0xffffffff, 0, 0.0f, 0.0f );
		m_RectPlane[2] = D3DLVERTEX( D3DVECTOR( rectwidth, 0.0f, -rectheight ), 0xffffffff, 0, 1.0f, 1.0f );
		m_RectPlane[3] = D3DLVERTEX( D3DVECTOR( rectwidth, 0.0f, rectheight ), 0xffffffff, 0, 1.0f, 0.0f );
		m_RectPlane[4] = D3DLVERTEX( D3DVECTOR( -rectwidth, 0.0f, -rectheight ), 0xffffffff, 0, 0.0f, 1.0f );
		m_RectPlane[5] = D3DLVERTEX( D3DVECTOR( -rectwidth, 0.0f, rectheight ), 0xffffffff, 0, 0.0f, 0.0f );
	}
	else
	{
		max_rectplane_vertexs = 4;
		m_RectPlane = new D3DLVERTEX[max_rectplane_vertexs];

		m_RectPlane[0] = D3DLVERTEX( D3DVECTOR( -rectwidth, -rectheight, 0.0f ), 0xffffffff, 0, 0.0f, 1.0f );
		m_RectPlane[1] = D3DLVERTEX( D3DVECTOR( -rectwidth, rectheight, 0.0f ), 0xffffffff, 0, 0.0f, 0.0f );
		m_RectPlane[2] = D3DLVERTEX( D3DVECTOR( rectwidth, -rectheight, 0.0f ), 0xffffffff, 0, 1.0f, 1.0f );
		m_RectPlane[3] = D3DLVERTEX( D3DVECTOR( rectwidth, rectheight, 0.0f ), 0xffffffff, 0, 1.0f, 0.0f );
	}
	return;
}

void CRectPlane::Create(int frame, float width, float height, float cap, bool bTemp)
{
	if( g_SvrType != ST_ADULT_ONLY)
		return;

	D3DVECTOR		normal = D3DVECTOR( 0.0f, 1.0f, 0.0f );
	float			rectwidth = width * 0.5f;
	float			rectheight = height * 0.5f;

	max_rectplane_vertexs = 4;
	m_RectPlane = new D3DLVERTEX[max_rectplane_vertexs];

	m_RectPlane[0] = D3DLVERTEX( D3DVECTOR( -rectwidth, -rectheight, 0.0f ), 0xffffffff, 0, cap*frame, 0 );
	m_RectPlane[1] = D3DLVERTEX( D3DVECTOR( -rectwidth, rectheight, 0.0f ), 0xffffffff, 0, cap*frame+cap, 0 );
	m_RectPlane[2] = D3DLVERTEX( D3DVECTOR( rectwidth, -rectheight, 0.0f ), 0xffffffff, 0, cap*frame, 1 );
	m_RectPlane[3] = D3DLVERTEX( D3DVECTOR( rectwidth, rectheight, 0.0f ), 0xffffffff, 0, cap*frame+cap, 1 );
}

void CRectPlane::Create(float width, float height, float distance, bool bothFaces)
{
	D3DVECTOR		normal = D3DVECTOR( 0.0f, 1.0f, 0.0f );
	float			rectwidth = width * 0.5f;
	float			rectheight = height * 0.5f;


	if( bothFaces )
	{
		max_rectplane_vertexs = 6;
		m_RectPlane = new D3DLVERTEX[max_rectplane_vertexs];

		m_RectPlane[0] = D3DLVERTEX( D3DVECTOR( -rectwidth, 0.0f, 0.0f + distance ), 0xffffffff, 0, 0.0f, 1.0f );
		m_RectPlane[1] = D3DLVERTEX( D3DVECTOR( -rectwidth, 0.0f, rectheight + distance ), 0xffffffff, 0, 0.0f, 0.0f );
		m_RectPlane[2] = D3DLVERTEX( D3DVECTOR( rectwidth, 0.0f, 0.0f + distance ), 0xffffffff, 0, 1.0f, 1.0f );
		m_RectPlane[3] = D3DLVERTEX( D3DVECTOR( rectwidth, 0.0f, rectheight + distance ), 0xffffffff, 0, 1.0f, 0.0f );
		m_RectPlane[4] = D3DLVERTEX( D3DVECTOR( -rectwidth, 0.0f, 0.0f + distance ), 0xffffffff, 0, 0.0f, 1.0f );
		m_RectPlane[5] = D3DLVERTEX( D3DVECTOR( -rectwidth, 0.0f, rectheight + distance ), 0xffffffff, 0, 0.0f, 0.0f );
	}
	else
	{
		max_rectplane_vertexs = 4;
		m_RectPlane = new D3DLVERTEX[max_rectplane_vertexs];

		m_RectPlane[0] = D3DLVERTEX( D3DVECTOR( -rectwidth, 0.0f + distance, 0.0f ), 0xffffffff, 0, 0.0f, 1.0f );
		m_RectPlane[1] = D3DLVERTEX( D3DVECTOR( -rectwidth, rectheight + distance, 0.0f ), 0xffffffff, 0, 0.0f, 0.0f );
		m_RectPlane[2] = D3DLVERTEX( D3DVECTOR( rectwidth, 0.0f + distance, 0.0f ), 0xffffffff, 0, 1.0f, 1.0f );
		m_RectPlane[3] = D3DLVERTEX( D3DVECTOR( rectwidth, rectheight + distance, 0.0f ), 0xffffffff, 0, 1.0f, 0.0f );
	}
	return;
}

void CRectPlane::ReCreate(float zangle)
{
	D3DMATRIX		matRz;
	D3DVECTOR		vSrcPos, vDestPos;

	D3DUtil_SetRotateZMatrix(matRz, zangle);

	for( int i = 0; i < 4; i++ )
	{
		vSrcPos = D3DVECTOR( m_RectPlane[i].x, m_RectPlane[i].y, m_RectPlane[i].z );

		D3DMath_VectorMatrixMultiply(vDestPos, vSrcPos, matRz);

		m_RectPlane[i].x = vDestPos.x;
		m_RectPlane[i].y = vDestPos.y;
		m_RectPlane[i].z = vDestPos.z;
	}

	return;
}

void CRectPlane::ChangeVertexInfo( int Index, D3DLVERTEX vertex ) // 버택스 정보를 조정해주는 함수.(06-03-09 원석) ///페어리 시 추가
{
	if( m_RectPlane && Index < 6 )
	{
		m_RectPlane[Index] = vertex;
	}
}

void CRectPlane::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	GET_D3DDEVICE()->SetMaterial( &mtrl );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_RectPlane, max_rectplane_vertexs, 0 );
	return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CRoundPlane Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CRoundPlane::CRoundPlane()
{
	m_RoundPlane = NULL;
	max_roundplane_vertexs = 0;

	D3DUtil_SetIdentityMatrix(matWorld);
	D3DUtil_InitMaterial(mtrl, 1.0f, 1.0f, 1.0f, 1.0f);
}
CRoundPlane::~CRoundPlane()
{
	if( m_RoundPlane )
	{
		delete [] m_RoundPlane;
		m_RoundPlane = NULL;
	}
}


void CRoundPlane::Create(float radius, float depth, int division)
{
	D3DVECTOR		normal = D3DVECTOR( 0.0f, 1.0f, 0.0f );
	float			x, z, angle;
	float			step = g_2_PI / division;
	int				numVertex = 0;


	max_roundplane_vertexs = division + 2;
	m_RoundPlane = new D3DLVERTEX[max_roundplane_vertexs];

	m_RoundPlane[0] = D3DLVERTEX( D3DVECTOR( 0.0f, depth, 0.0f ), 0xffffffff, 0, 0.0f, 0.5f );

	for(angle = 0.0f; angle < g_2_PI+0.0001f; angle += step)
	{
		x = radius * (float)sin(angle);
		z = radius * (float)cos(angle);

		numVertex++;
		m_RoundPlane[numVertex] = D3DLVERTEX( D3DVECTOR( x, 0.0f, z ), 0xffffffff, 0, 1.0f, 1.0f );
	}
	return;
}


void CRoundPlane::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	GET_D3DDEVICE()->SetMaterial( &mtrl );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLEFAN, D3DFVF_LVERTEX,
									m_RoundPlane, max_roundplane_vertexs, 0 );
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CCylinderPlane Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CCylinderPlane::CCylinderPlane()
{
	m_CylinderPlane = NULL;
	max_cylinderplane_vertexs = 0;

	D3DUtil_SetIdentityMatrix(matWorld);
	D3DUtil_InitMaterial(mtrl, 1.0f, 1.0f, 1.0f, 1.0f);
}
CCylinderPlane::~CCylinderPlane()
{
	if( m_CylinderPlane )
	{
		delete [] m_CylinderPlane;
		m_CylinderPlane = NULL;
	}
}

void CCylinderPlane::Create(float topRadius, float bottomRadius, float depth, int division, bool bothFaces, float pattern_tu)
{
	float			angle = 0.0f;
	float			x, y, z;
	float			xx, zz;
	float			step = g_2_PI / division;
	int				numVertex = 0;
	float			tu, tv;


	if( bothFaces )
	{
		max_cylinderplane_vertexs = (division * 4) + 4;
		m_CylinderPlane = new D3DLVERTEX[max_cylinderplane_vertexs];

		for( int twofaces = 0; twofaces < 2; twofaces++ )
		{
			y = depth / 2.0f;

			for(angle = 0.0f; angle < g_2_PI-0.001f; angle += step)
//			for(angle = 0.0f; angle <= g_PI; angle += step)
			{
				if( twofaces == 0 )
				{
					x = bottomRadius * (float)sin(angle);
					z = bottomRadius * (float)cos(angle);
				}
				else
				{
					x = topRadius * (float)sin(angle);
					z = topRadius * (float)cos(angle);
				}


				tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//				tu = (angle / step) * (step / g_2_PI) ;
				tv = 1.0f;
				m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, -y, z ), 0xffffffff, 0, tu, tv );

				if( twofaces == 0 )
				{
					xx = topRadius * (float)sin(angle);
					zz = topRadius * (float)cos(angle);
				}
				else
				{
					xx = bottomRadius * (float)sin(angle);
					zz = bottomRadius * (float)cos(angle);
				}

				tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//				tu = (angle / step) * (step / g_2_PI);
				tv = 0.0f;
				m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, y, zz ), 0xffffffff, 0, tu, tv );
			}

			m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
			m_CylinderPlane[numVertex].tu = pattern_tu;
			m_CylinderPlane[numVertex++].tv = 1.0f;
			m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
			m_CylinderPlane[numVertex].tu = pattern_tu;
			m_CylinderPlane[numVertex++].tv = 0.0f;
			depth *= -1;
		}
	}
	else
	{
		max_cylinderplane_vertexs = (division * 2) + 2;
		m_CylinderPlane = new D3DLVERTEX[max_cylinderplane_vertexs];

		for(angle = 0.0f; angle < g_2_PI+0.0001f; angle += step)
		{
			x = bottomRadius * (float)sin(angle);
			z = bottomRadius * (float)cos(angle);
			y = depth / 2.0f;

			tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//			tu = (angle / step) * (step / g_2_PI) ;
			tv = 1.0f;
			m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, -y, z ), 0xffffffff, 0, tu, tv );

			xx = topRadius * (float)sin(angle);
			zz = topRadius * (float)cos(angle);

			tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//			tu = (angle / step) * (step / g_2_PI) ;
			tv = 0.0f;
			m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, y, zz ), 0xffffffff, 0, tu, tv );
		}
	}
	return;
}

void CCylinderPlane::Create(float topRadius, float bottomRadius, float depth, int division, float distance, bool bothFaces, float pattern_tu)
{
	float			angle = 0.0f;
	float			x, y, z;
	float			xx, zz;
	float			step = g_2_PI / division;
	int				numVertex = 0;
	float			tu, tv;


	if( bothFaces )
	{
		max_cylinderplane_vertexs = (division * 4) + 4;
		m_CylinderPlane = new D3DLVERTEX[max_cylinderplane_vertexs];

		for( int twofaces = 0; twofaces < 2; twofaces++ )
		{
			y = depth;

			for(angle = 0.0f; angle < g_2_PI-0.001f; angle += step)
//			for(angle = 0.0f; angle <= g_PI; angle += step)
			{
				if( twofaces == 0 )
				{
					tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//					tu = (angle / step) * (step / g_2_PI) ;
					tv = 1.0f;

					x = bottomRadius * (float)sin(angle);
					z = bottomRadius * (float)cos(angle);

					m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, 0.0f + distance, z ), 0xffffffff, 0, tu, tv );
				}
				else
				{
					tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//					tu = (angle / step) * (step / g_2_PI) ;
					tv = 0.0f;

					x = topRadius * (float)sin(angle);
					z = topRadius * (float)cos(angle);

					m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, y + distance, z ), 0xffffffff, 0, tu, tv );
				}

				if( twofaces == 0 )
				{
					tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//					tu = (angle / step) * (step / g_2_PI);
					tv = 0.0f;

					xx = topRadius * (float)sin(angle);
					zz = topRadius * (float)cos(angle);

					m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, y + distance, zz ), 0xffffffff, 0, tu, tv );
				}
				else
				{
					tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//					tu = (angle / step) * (step / g_2_PI);
					tv = 1.0f;

					xx = bottomRadius * (float)sin(angle);
					zz = bottomRadius * (float)cos(angle);

					m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, 0.0f + distance, zz ), 0xffffffff, 0, tu, tv );
				}
			}

			if( twofaces == 0 )
			{
				m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
				m_CylinderPlane[numVertex].tu = pattern_tu;
				m_CylinderPlane[numVertex++].tv = 1.0f;
				m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
				m_CylinderPlane[numVertex].tu = pattern_tu;
				m_CylinderPlane[numVertex++].tv = 0.0f;
			}
			else
			{
				m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
				m_CylinderPlane[numVertex].tu = pattern_tu;
				m_CylinderPlane[numVertex++].tv = 0.0f;
				m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
				m_CylinderPlane[numVertex].tu = pattern_tu;
				m_CylinderPlane[numVertex++].tv = 1.0f;

			}
		}
	}
	else
	{
		max_cylinderplane_vertexs = (division * 2) + 2;
		m_CylinderPlane = new D3DLVERTEX[max_cylinderplane_vertexs];

		for(angle = 0.0f; angle < g_2_PI+0.0001f; angle += step)
		{
			x = bottomRadius * (float)sin(angle);
			z = bottomRadius * (float)cos(angle);
			y = depth;

			tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//			tu = (angle / step) * (step / g_2_PI) ;
			tv = 1.0f;
			m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, 0.0f + distance, z ), 0xffffffff, 0, tu, tv );

			xx = topRadius * (float)sin(angle);
			zz = topRadius * (float)cos(angle);

			tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//			tu = (angle / step) * (step / g_2_PI) ;
			tv = 0.0f;
			m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, y + distance, zz ), 0xffffffff, 0, tu, tv );
		}
	}
	return;
}


void CCylinderPlane::ReCreate(float topRadius, float bottomRadius, float depth, int division, float distance, float pattern_tu)
{
	float			angle = 0.0f;
	float			x, y, z;
	float			xx, zz;
	float			step = g_2_PI / division;
	int				numVertex = 0;
	float			tu, tv;

	for( int twofaces = 0; twofaces < 2; twofaces++ )
	{
		y = depth;

		for(angle = 0.0f; angle < g_2_PI-0.001f; angle += step)
		{
			if( twofaces == 0 )
			{
				tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//				tu = (angle / step) * (step / g_2_PI) ;
				tv = 1.0f;

				x = bottomRadius * (float)sin(angle);
				z = bottomRadius * (float)cos(angle);

				m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, z, 0.0f + distance ), 0xffffffff, 0, tu, tv );
			}
			else
			{
				tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//				tu = (angle / step) * (step / g_2_PI) ;
				tv = 0.0f;

				x = topRadius * (float)sin(angle);
				z = topRadius * (float)cos(angle);

				m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, z, y + distance ), 0xffffffff, 0, tu, tv );
			}

			if( twofaces == 0 )
			{
				tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//				tu = (angle / step) * (step / g_2_PI);
				tv = 0.0f;

				xx = topRadius * (float)sin(angle);
				zz = topRadius * (float)cos(angle);

				m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, zz, y + distance ), 0xffffffff, 0, tu, tv );
			}
			else
			{
				tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
//				tu = (angle / step) * (step / g_2_PI);
				tv = 1.0f;

				xx = bottomRadius * (float)sin(angle);
				zz = bottomRadius * (float)cos(angle);

				m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, zz, 0.0f + distance ), 0xffffffff, 0, tu, tv );
			}
		}

		if( twofaces == 0 )
		{
			m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
			m_CylinderPlane[numVertex].tu = pattern_tu;
			m_CylinderPlane[numVertex++].tv = 1.0f;
			m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
			m_CylinderPlane[numVertex].tu = pattern_tu;
			m_CylinderPlane[numVertex++].tv = 0.0f;
		}
		else
		{
			m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
			m_CylinderPlane[numVertex].tu = pattern_tu;
			m_CylinderPlane[numVertex++].tv = 0.0f;
			m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
			m_CylinderPlane[numVertex].tu = pattern_tu;
			m_CylinderPlane[numVertex++].tv = 1.0f;

		}
	}
	return;
}


void CCylinderPlane::SkyCreate(float topRadius, float bottomRadius, float depth, int division, float distance, float pattern_tu)
{
	float			angle = 0.0f;
	float			x, y, z;
	float			xx, zz;
	float			step = g_2_PI / division;
	int				numVertex = 0;
	float			tu, tv;


	max_cylinderplane_vertexs = (division * 2) + 2;
	m_CylinderPlane = new D3DLVERTEX[max_cylinderplane_vertexs];

	/*	for(angle = 0.0f; angle < g_2_PI+0.0001f; angle += step)
		{
			x = bottomRadius * (float)sin(angle);
			z = bottomRadius * (float)cos(angle);
			y = depth;

	//		tu = (angle / step) * (step / g_2_PI) ;
			tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
			tv = 1.0f;
			m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, 0.0f + distance, z ), 0xffffffff, 0, tu, tv );

			xx = topRadius * (float)sin(angle);
			zz = topRadius * (float)cos(angle);

	//		tu = (angle / step) * (step / g_2_PI) ;
			tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
			tv = 0.0f;
			m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, y + distance, zz ), 0xffffffff, 0, tu, tv );
		}
	*/

	for(angle = 0.0f; angle < g_2_PI+0.0001f; angle += step)
	{
		x = topRadius * (float)sin(angle);
		z = topRadius * (float)cos(angle);
		y = depth;

		tu = (angle / step) * ((step*pattern_tu) / g_2_PI) ;
		tv = 1.0f;

		m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, y + distance, z ), 0xffffffff, 0, tu, tv );

		tu = (angle / step) * ((step*pattern_tu) / g_2_PI);
		tv = 0.0f;

		xx = bottomRadius * (float)sin(angle);
		zz = bottomRadius * (float)cos(angle);

		m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, 0.0f + distance, zz ), 0xffffffff, 0, tu, tv );

//		m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
//		m_CylinderPlane[numVertex].tu = 1.0f; m_CylinderPlane[numVertex++].tv = 0.0f;
//		m_CylinderPlane[numVertex] = m_CylinderPlane[numVertex - (division*2)];
//		m_CylinderPlane[numVertex].tu = 1.0f; m_CylinderPlane[numVertex++].tv = 1.0f;
	}
	return;

}


void CCylinderPlane::Create2(float innerRadius, float outerRadius, float depth, int division, int revolution, float gapStep, float radiusStep, float heightStep, float pattern_tu)
{
	float			angle = 0.0f;
	float			x, y, z;
	float			xx, zz;
	float			gap = 0.0f;
	float			step = g_2_PI / division;
	int				numVertex = 0;
	float			tu, tv;

	float heightbottom = 0.0f;
	float heightop = 0.0f;

	max_cylinderplane_vertexs = (division * 2 * revolution) + 2;
	m_CylinderPlane = new D3DLVERTEX[max_cylinderplane_vertexs];

	for(angle = 0.0f; angle < (g_2_PI * revolution) + 0.0001f; angle += step)
//	for(angle = 0.0f; angle <= g_PI; angle += step)
	{
		x = outerRadius * (float)sin(angle);
		z = outerRadius * (float)cos(angle);
		y = depth / 2.0f;
		heightbottom -= heightStep;

		tu = (angle / step) * ((step*pattern_tu) / (g_2_PI * revolution) ) ;
		tv = 1.0f;
//		m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, -y + gap, z ), 0xffffffff, 0, tu, tv );
		m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -x, -y + gap + heightbottom, z ), 0xffffffff, 0, tu, tv );

		outerRadius += radiusStep;

		xx = innerRadius * (float)sin(angle);
		zz = innerRadius * (float)cos(angle);
		heightop += heightStep;

		tu = (angle / step) * ((step*pattern_tu) / (g_2_PI * revolution) );
		tv = 0.0f;
//		m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, y + gap, zz ), 0xffffffff, 0, tu, tv );
		m_CylinderPlane[numVertex++] = D3DLVERTEX( D3DVECTOR( -xx, y + gap + heightop, zz ), 0xffffffff, 0, tu, tv );

		innerRadius += radiusStep;

		gap += gapStep;
	}
	return;
}


void CCylinderPlane::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	GET_D3DDEVICE()->SetMaterial( &mtrl );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_CylinderPlane, max_cylinderplane_vertexs, 0 );
	return;
}

void CCylinderPlane::Render2()
{
	if( !GET_D3DDEVICE() )
		return;

	GET_D3DDEVICE()->SetMaterial( &mtrl );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_CylinderPlane, max_cylinderplane_vertexs, 0 );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_CW );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_CylinderPlane, max_cylinderplane_vertexs, 0 );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);
	return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CSphere Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CSphere::CSphere()
{
	m_Sphere = NULL;
	max_sphere_vertexs = 0;

	D3DUtil_SetIdentityMatrix(matWorld);
	D3DUtil_InitMaterial(mtrl, 1.0f, 1.0f, 1.0f, 1.0f);
}
CSphere::~CSphere()
{
	SAFE_DELETE_ARRAY( m_Sphere );
}

void CSphere::Create()
{
	// Choose a tesselation level
	DWORD dwNumSphereRings    = 15;
	DWORD dwNumSphereSegments = 15;

	// Allocate space for the sphere
	max_sphere_vertexs = 2 * dwNumSphereRings * (dwNumSphereSegments+1);
	m_Sphere = new D3DLVERTEX[max_sphere_vertexs];
	D3DLVERTEX* vtx = m_Sphere;

	// Establish constants used in sphere generation
	//FLOAT fDeltaRingAngle = ( g_PI / dwNumSphereRings / 2.0f );
	//FLOAT fDeltaSegAngle  = ( 2.0f * g_PI / dwNumSphereSegments / 2.0f);

	FLOAT fDeltaRingAngle = ( g_PI / dwNumSphereRings );
	FLOAT fDeltaSegAngle  = ( g_2_PI / dwNumSphereSegments );

	// Generate the group of rings for the sphere
	for( DWORD ring = 0; ring < dwNumSphereRings; ring++ )
	{
		FLOAT r0 = sinf( (ring+0) * fDeltaRingAngle );
		FLOAT r1 = sinf( (ring+1) * fDeltaRingAngle );
		FLOAT y0 = cosf( (ring+0) * fDeltaRingAngle );
		FLOAT y1 = cosf( (ring+1) * fDeltaRingAngle );

		// Generate the group of segments for the current ring
		for( DWORD seg = 0; seg < (dwNumSphereSegments+1); seg++ )
		{
			FLOAT x0 = r0 * sinf( seg * fDeltaSegAngle );
			FLOAT z0 = r0 * cosf( seg * fDeltaSegAngle );
			FLOAT x1 = r1 * sinf( seg * fDeltaSegAngle );
			FLOAT z1 = r1 * cosf( seg * fDeltaSegAngle );

			if( dwNumSphereSegments == seg )
				x0 = x1 = 0.0f;

			// Add two vertices to the strip which makes up the sphere
			(*vtx++) = D3DLVERTEX( D3DVECTOR(x0,y0,z0), 0xffffffff, 0,
								   -((FLOAT)seg)/dwNumSphereSegments,
								   (ring+0)/(FLOAT)dwNumSphereRings );

			(*vtx++) = D3DLVERTEX( D3DVECTOR(x1,y1,z1), 0xffffffff, 0,
								   -((FLOAT)seg)/dwNumSphereSegments,
								   (ring+1)/(FLOAT)dwNumSphereRings );
		}

	}
	return;
}

void CSphere::SkyCreate(int division, float ring_TileTex, float seg_TileTex)
{
	// Choose a tesselation level
	DWORD dwNumSphereRings    = 15;
	DWORD dwNumSphereSegments = 15;

	// Allocate space for the sphere
	max_sphere_vertexs = 2 * dwNumSphereRings * (dwNumSphereSegments+1);
	m_Sphere = new D3DLVERTEX[max_sphere_vertexs];
	D3DLVERTEX* vtx = m_Sphere;

	FLOAT fDeltaRingAngle = 0.0f;
	FLOAT fDeltaSegAngle  = 0.0f;

	if( division == FULL_SPHERE )
	{
		fDeltaRingAngle = ( g_PI / dwNumSphereRings );
		fDeltaSegAngle  = ( g_2_PI / dwNumSphereSegments );
	}
	else if( division == HALF_OF_HEIGTH_SPHERE )
	{
		fDeltaRingAngle = ( g_PI / dwNumSphereRings / 2.0f );
		fDeltaSegAngle  = ( g_2_PI / dwNumSphereSegments );
	}
	else if( division == HALF_OF_WIDTH_SPHERE )
	{
		fDeltaRingAngle = ( g_PI / dwNumSphereRings );
		fDeltaSegAngle = ( g_PI / dwNumSphereRings );
	}

	// Establish constants used in sphere generation
//	FLOAT fDeltaRingAngle = ( g_PI / dwNumSphereRings / 2.0f );
//	FLOAT fDeltaSegAngle  = ( 2.0f * g_PI / dwNumSphereSegments / 2.0f);
//	FLOAT fDeltaSegAngle  = ( g_PI / dwNumSphereSegments / 2.0f);

	// Generate the group of rings for the sphere
	for( DWORD ring = 0; ring < dwNumSphereRings; ring++ )
	{
		FLOAT r0 = sinf( (ring+0) * fDeltaRingAngle );
		FLOAT r1 = sinf( (ring+1) * fDeltaRingAngle );
		FLOAT y0 = cosf( (ring+0) * fDeltaRingAngle );
		FLOAT y1 = cosf( (ring+1) * fDeltaRingAngle );

		// Generate the group of segments for the current ring
		for( DWORD seg = 0; seg < (dwNumSphereSegments+1); seg++ )
		{
			FLOAT x0 = r0 * sinf( seg * fDeltaSegAngle );
			FLOAT z0 = r0 * cosf( seg * fDeltaSegAngle );
			FLOAT x1 = r1 * sinf( seg * fDeltaSegAngle );
			FLOAT z1 = r1 * cosf( seg * fDeltaSegAngle );

			if( dwNumSphereSegments == seg )
				x0 = x1 = 0.0f;

			(*vtx++) = D3DLVERTEX( D3DVECTOR(x0,y0,z0), 0xffffffff, 0,
								   (-((FLOAT)seg)/dwNumSphereSegments) * seg_TileTex,
								   ((ring+0)/(FLOAT)dwNumSphereRings) * ring_TileTex );

			(*vtx++) = D3DLVERTEX( D3DVECTOR(x1,y1,z1), 0xffffffff, 0,
								   (-((FLOAT)seg)/dwNumSphereSegments) * seg_TileTex,
								   ((ring+1)/(FLOAT)dwNumSphereRings) * ring_TileTex );
		}

	}
	return;
}

void CSphere::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	GET_D3DDEVICE()->SetMaterial( &mtrl );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_Sphere, max_sphere_vertexs, 0 );
	return;
}


void CSphere::SkyRender()
{
	if( !GET_D3DDEVICE() )
		return;

	GET_D3DDEVICE()->SetMaterial( &mtrl );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_CW );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_Sphere, max_sphere_vertexs, 0 );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);
	return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CTailPlane Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CTailPlane::CTailPlane()
{
	m_TailPlane = NULL;
	max_tailplane_vertexs = 0;
	max_tailpos_vectors = 0;
	m_headsize = 0;
	m_tailsize = 0;
	m_pattern_tu = 0;
	m_bIsFade = TRUE;

	D3DUtil_SetIdentityMatrix(m_matWorld);
}
CTailPlane::~CTailPlane()
{
	if( m_TailPlane )
	{
		delete [] m_TailPlane;
		m_TailPlane = NULL;
	}

}

void CTailPlane::Create(int division, float head_size, float tail_size, float pattern_tu, bool bIsFade)
{
	m_headsize = head_size;
	m_tailsize = tail_size;
	m_pattern_tu = pattern_tu;

	max_tailplane_vertexs = (division * 2) + 2;
	max_tailpos_vectors = division+1;
	m_TailPlane = new D3DLVERTEX[max_tailplane_vertexs];

	// [12/22/2008 D.K ] : memory leak.
	ZeroMemory(m_TailPlane, max_tailplane_vertexs * sizeof( LPD3DLVERTEX ) );

	m_bIsFade = bIsFade;

	return;
}


void CTailPlane::AddTailPos(LPD3DVECTOR TailPos, int *CurTailPosNum, D3DVECTOR addPos, D3DVECTOR viewVec, float org_red, float org_green, float org_blue)
{
	if( CurTailPosNum == NULL )
		return;

	D3DVECTOR	TempPos;
	DWORD		color;
	float		red, green, blue, alpha;
	float		tu, tv;
	int			numVertex = 0;
	int			tailpos = 0;


	(*CurTailPosNum)++;

	if( max_tailpos_vectors < (*CurTailPosNum) )
	{
		(*CurTailPosNum) = max_tailpos_vectors;

		for( tailpos = 1; tailpos < (*CurTailPosNum); tailpos++)
		{
			TempPos = TailPos[tailpos];
			TailPos[tailpos-1] = TempPos;
		}
	}
	TailPos[(*CurTailPosNum)-1] = addPos;


	D3DVECTOR	vDirVec;
	D3DVECTOR	vRight;

	int emptyPosNum = max_tailpos_vectors - (*CurTailPosNum);

	for( tailpos = 0; tailpos < (*CurTailPosNum); tailpos++ )
	{
		if( tailpos == (*CurTailPosNum)-1 && tailpos != 0 )
			vDirVec = (TailPos[tailpos-1]) - (TailPos[tailpos]);
		else
			vDirVec = (TailPos[tailpos]) - (TailPos[tailpos+1]);

		vDirVec = Normalize(vDirVec);

		vRight = CrossProduct( viewVec, vDirVec );
		vRight = Normalize(vRight);

		float all_rate = m_headsize - m_tailsize;
		float ratio_size = (all_rate / max_tailpos_vectors) * tailpos;
		vRight *= (m_headsize - (all_rate-ratio_size));

		alpha = ((1.0f / (*CurTailPosNum)) * tailpos);
		alpha = (1.0f - alpha);

		if( !m_bIsFade )
			alpha = 0.0f;

		red = org_red - (org_red * alpha);
		green = org_green - (org_green * alpha);
		blue = org_blue - (org_blue * alpha);
//		color = D3DRGBA(red * 0.7f, green * 0.7f, blue * 0.7f, (1.0f - alpha) * 0.7f);
		color = D3DRGBA(red, green, blue, (1.0f - alpha));

//		tu = ( (1.0f * m_pattern_tu) / (*CurTailPosNum) ) * tailpos;		// 늘어나기
		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * (tailpos+emptyPosNum+1);	// 튀어나오기
		tv = 0.0f;

		m_TailPlane[numVertex++] = D3DLVERTEX( (TailPos[tailpos]) + vRight, color, color, tu, tv );

		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * (tailpos+emptyPosNum+1);	// 튀어나오기
		tv = 1.0f;

		m_TailPlane[numVertex++] = D3DLVERTEX( (TailPos[tailpos]) - vRight, color, color, tu, tv );
	}

	return;
}

void CTailPlane::SubTailPos(LPD3DVECTOR TailPos, int *CurTailPosNum, D3DVECTOR viewVec, float org_red, float org_green, float org_blue)
{
	if( CurTailPosNum == NULL )
		return;

	D3DVECTOR	TempPos;
	DWORD		color;
	float		red, green, blue, alpha;
	float		tu, tv;
	int			numVertex = 0;
	int			tailpos = 0;

	if( 1 > (*CurTailPosNum) )
		return;

	for( tailpos = 1; tailpos < (*CurTailPosNum); tailpos++)
	{
		TempPos = TailPos[tailpos];
		TailPos[tailpos-1] = TempPos;
	}

	(*CurTailPosNum)--;


	D3DVECTOR	vDirVec;
	D3DVECTOR	vRight;

	int emptyPosNum = max_tailpos_vectors - (*CurTailPosNum);

	for( tailpos = 0; tailpos < *CurTailPosNum; tailpos++ )
	{
		if( tailpos == (*CurTailPosNum)-1 && tailpos != 0 )
			vDirVec = (TailPos[tailpos-1]) - (TailPos[tailpos]);
		else
			vDirVec = TailPos[tailpos] - TailPos[tailpos+1];

		vDirVec = Normalize(vDirVec);

		vRight = CrossProduct( viewVec, vDirVec );
		vRight = Normalize(vRight);

		float all_rate = m_headsize - m_tailsize;
		float ratio_size = (all_rate / max_tailpos_vectors) * tailpos;
		vRight *= (m_tailsize + ratio_size);

		alpha = ((1.0f / (*CurTailPosNum)) * tailpos);
		alpha = (1.0f - alpha);

		if( !m_bIsFade )
			alpha = 0.0f;

		red = org_red - (org_red * alpha);
		green = org_green - (org_green * alpha);
		blue = org_blue - (org_blue * alpha);
//		color = D3DRGBA(red * 0.7f, green * 0.7f, blue * 0.7f, (1.0f - alpha) * 0.7f);
		color = D3DRGBA(red, green, blue, (1.0f - alpha));

//		tu = ( (1.0f * m_pattern_tu) / (*CurTailPosNum)) * tailpos;		// 줄어들기
		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * tailpos;	// 점차사라지기
		tv = 0.0f;

		m_TailPlane[numVertex++] = D3DLVERTEX( TailPos[tailpos] + vRight, color, color, tu, tv );

		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * tailpos;	// 점차사라지기
		tv = 1.0f;

		m_TailPlane[numVertex++] = D3DLVERTEX( TailPos[tailpos] - vRight, color, color, tu, tv );
	}

	return;
}


void CTailPlane::ReSetAddTailPos(LPD3DVECTOR TailPos, int *CurTailPosNum, D3DVECTOR viewVec, float org_red, float org_green, float org_blue)
{
	if( CurTailPosNum == NULL )
		return;

	DWORD		color;
	float		red, green, blue, alpha;
	float		tu, tv;
	int			numVertex = 0;
	int			tailpos = 0;


	D3DVECTOR	vDirVec;
	D3DVECTOR	vRight;

	int emptyPosNum = max_tailpos_vectors - (*CurTailPosNum);

	for( tailpos = 0; tailpos < (*CurTailPosNum); tailpos++ )
	{
		if( tailpos == (*CurTailPosNum)-1 && tailpos != 0 )
			vDirVec = (TailPos[tailpos-1]) - (TailPos[tailpos]);
		else
			vDirVec = (TailPos[tailpos]) - (TailPos[tailpos+1]);

		vDirVec = Normalize(vDirVec);

		vRight = CrossProduct( viewVec, vDirVec );
		vRight = Normalize(vRight);

		float all_rate = m_headsize - m_tailsize;
		float ratio_size = (all_rate / max_tailpos_vectors) * tailpos;
		vRight *= (m_headsize - (all_rate-ratio_size));				// AddTail 유지
//		vRight *= (m_tailsize + ratio_size);						// SubTail 유지

		alpha = ((1.0f / (*CurTailPosNum)) * tailpos);
		alpha = (1.0f - alpha);

		if( !m_bIsFade )
			alpha = 0.0f;

		red = org_red - (org_red * alpha);
		green = org_green - (org_green * alpha);
		blue = org_blue - (org_blue * alpha);
		color = D3DRGBA(red, green, blue, (1.0f - alpha));

		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * (tailpos+emptyPosNum+1);	// AddTail 유지
//		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * tailpos;					// SubTail 유지
		tv = 0.0f;

		m_TailPlane[numVertex++] = D3DLVERTEX( (TailPos[tailpos]) + vRight, color, color, tu, tv );

		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * (tailpos+emptyPosNum+1);	// AddTail 유지
//		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * tailpos;					// SubTail 유지
		tv = 1.0f;

		m_TailPlane[numVertex++] = D3DLVERTEX( (TailPos[tailpos]) - vRight, color, color, tu, tv );
	}

	return;
}

void CTailPlane::ReSetSubTailPos(LPD3DVECTOR TailPos, int *CurTailPosNum, D3DVECTOR viewVec, float org_red, float org_green, float org_blue)
{
	if( CurTailPosNum == NULL )
		return;

	DWORD		color;
	float		red, green, blue, alpha;
	float		tu, tv;
	int			numVertex = 0;
	int			tailpos = 0;


	D3DVECTOR	vDirVec;
	D3DVECTOR	vRight;

	int emptyPosNum = max_tailpos_vectors - (*CurTailPosNum);

	for( tailpos = 0; tailpos < (*CurTailPosNum); tailpos++ )
	{
		if( tailpos == (*CurTailPosNum)-1 && tailpos != 0 )
			vDirVec = (TailPos[tailpos-1]) - (TailPos[tailpos]);
		else
			vDirVec = (TailPos[tailpos]) - (TailPos[tailpos+1]);

		vDirVec = Normalize(vDirVec);

		vRight = CrossProduct( viewVec, vDirVec );
		vRight = Normalize(vRight);

		float all_rate = m_headsize - m_tailsize;
		float ratio_size = (all_rate / max_tailpos_vectors) * tailpos;
//		vRight *= (m_headsize - (all_rate-ratio_size));				// AddTail 유지
		vRight *= (m_tailsize + ratio_size);						// SubTail 유지

		alpha = ((1.0f / (*CurTailPosNum)) * tailpos);
		alpha = (1.0f - alpha);

		if( !m_bIsFade )
			alpha = 0.0f;

		red = org_red - (org_red * alpha);
		green = org_green - (org_green * alpha);
		blue = org_blue - (org_blue * alpha);
		color = D3DRGBA(red, green, blue, (1.0f - alpha));

//		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * (tailpos+emptyPosNum+1);	// AddTail 유지
		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * tailpos;					// SubTail 유지
		tv = 0.0f;

		m_TailPlane[numVertex++] = D3DLVERTEX( (TailPos[tailpos]) + vRight, color, color, tu, tv );

//		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * (tailpos+emptyPosNum+1);	// AddTail 유지
		tu = ( (1.0f * m_pattern_tu) / max_tailpos_vectors ) * tailpos;					// SubTail 유지
		tv = 1.0f;

		m_TailPlane[numVertex++] = D3DLVERTEX( (TailPos[tailpos]) - vRight, color, color, tu, tv );
	}

	return;
}


void CTailPlane::Render(int *CurTailPosNum)
{
	if( !GET_D3DDEVICE() || CurTailPosNum == NULL )
		return;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &m_matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_TailPlane, ((*CurTailPosNum)*2), 0 );

	return;
}


