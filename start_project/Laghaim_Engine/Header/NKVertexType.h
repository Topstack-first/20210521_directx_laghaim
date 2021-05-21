#ifndef _NKVERTEXTYPE_H_
#define _NKVERTEXTYPE_H_

#include <d3d.h>

//////////////////////////////////////////////////////////////////////////
typedef struct _NKVERTEXLT
{
public:
	union
	{
		struct
		{
			D3DVECTOR	p;
			DWORD		c;
			float		u, v;
		};
		struct
		{
			float		x, y, z;
			DWORD		c;
			float		u, v;
		};
	};

	_NKVERTEXLT();
	_NKVERTEXLT( const D3DVECTOR& Pos,  DWORD dwColor, float fu, float fv );
	_NKVERTEXLT( float fx, float fy, float fz, DWORD dwColor, float fu, float fv );

	void	Set( const D3DVECTOR& Pos, DWORD dwColor, float fu, float fv );
	void	Set( float fx, float fy, float fz, DWORD dwColor, float fu, float fv );
} NKVERTEXLT;

const DWORD	NKVERTEXLT_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

//////////////////////////////////////////////////////////////////////////
typedef struct _NKVERTEXT2
{
public:
	union
	{
		struct
		{
			D3DVECTOR	p;
			float		u1, v1;
			float		u2, v2;
		};
		struct
		{
			float		x, y, z;
			float		u1, v1;
			float		u2, v2;
		};
	};

	_NKVERTEXT2();
	_NKVERTEXT2( const D3DVECTOR& Pos, float fu1, float fv1, float fu2, float fv2 );
	_NKVERTEXT2( float fx, float fy, float fz, float fu1, float fv1, float fu2, float fv2 );

	void	Set( const D3DVECTOR& Pos, float fu1, float fv1, float fu2, float fv2 );
	void	Set( float fx, float fy, float fz, float fu1, float fv1, float fu2, float fv2 );
} NKVERTEXT2;

const DWORD	NKVERTEXT2_FVF = D3DFVF_XYZ | D3DFVF_TEX2;

//////////////////////////////////////////////////////////////////////////
inline NKVERTEXLT::_NKVERTEXLT()
{
}

inline NKVERTEXLT::_NKVERTEXLT( const D3DVECTOR& Pos,  DWORD dwColor, float fu, float fv )
{
	p = Pos;
	c = dwColor;
	u = fu;
	v = fv;
}

inline NKVERTEXLT::_NKVERTEXLT( float fx, float fy, float fz, DWORD dwColor, float fu, float fv )
{
	x = fx;
	y = fy;
	z = fz;
	c = dwColor;
	u = fu;
	v = fv;
}

inline void NKVERTEXLT::Set( const D3DVECTOR& Pos, DWORD dwColor, float fu, float fv )
{
	p = Pos;
	c = dwColor;
	u = fu;
	v = fv;
}

inline void NKVERTEXLT::Set( float fx, float fy, float fz, DWORD dwColor, float fu, float fv )
{
	x = fx;
	y = fy;
	z = fz;
	c = dwColor;
	u = fu;
	v = fv;
}

//////////////////////////////////////////////////////////////////////////
inline _NKVERTEXT2::_NKVERTEXT2()
{
}

inline _NKVERTEXT2::_NKVERTEXT2( const D3DVECTOR& Pos, float fu1, float fv1, float fu2, float fv2 )
{
	p = Pos;
	u1 = fu1;
	v1 = fv1;
	u2 = fu2;
	v2 = fv2;
}

inline _NKVERTEXT2::_NKVERTEXT2( float fx, float fy, float fz, float fu1, float fv1, float fu2, float fv2 )
{
	x = fx;
	y = fy;
	z = fz;
	u1 = fu1;
	v1 = fv1;
	u2 = fu2;
	v2 = fv2;
}

inline void NKVERTEXT2::Set( const D3DVECTOR& Pos, float fu1, float fv1, float fu2, float fv2 )
{
	p = Pos;
	u1 = fu1;
	v1 = fv1;
	u2 = fu2;
	v2 = fv2;
}

inline void NKVERTEXT2::Set( float fx, float fy, float fz, float fu1, float fv1, float fu2, float fv2 )
{
	x = fx;
	y = fy;
	z = fz;
	u1 = fu1;
	v1 = fv1;
	u2 = fu2;
	v2 = fv2;
}


#endif // _NKVERTEXTYPE_H_
