#pragma once
#include <d3d9types.h>

#define D3DVIEWPORT7 D3DVIEWPORT9
#define D3DMATERIAL7 D3DMATERIAL9
#define LPDIRECT3DDEVICE7 LPDIRECT3DDEVICE9
#define LPDIRECT3D7 LPDIRECT3D9
#define D3DRENDERSTATE_AMBIENT D3DRS_AMBIENT
#define D3DRENDERSTATE_LIGHTING D3DRS_LIGHTING
#define D3DTRANSFORMSTATE_VIEW D3DTS_VIEW
#define D3DTRANSFORMSTATE_WORLD D3DTS_WORLD
#define D3DRGB(r,g,b) D3DCOLOR_COLORVALUE(r,g,b,1.0f)
#define D3DRGBA(r,g,b,a) D3DCOLOR_COLORVALUE(r,g,b,a)
#define D3DLIGHT7 D3DLIGHT9

D3DVALUE SquareMagnitude(const _D3DVECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}


D3DVALUE Magnitude(const _D3DVECTOR& v)
{
    return sqrt(SquareMagnitude(v));
}

D3DVECTOR Normalize(const _D3DVECTOR& v)
{
    _D3DVECTOR vv = v;
    float n = Magnitude(v);
    vv.x /= n;   vv.y /= n;   vv.z /= n;
    return vv;
}


D3DVECTOR D3DVECTOR_New(float a, float b, float c)
{
    D3DVECTOR result;
    result.x = a;
    result.y = b;
    result.z = c;
    return result;
}
D3DVECTOR D3DVECTOR_New(float a, float b, int c)
{
    D3DVECTOR result;
    result.x = a;
    result.y = b;
    result.z = c;
    return result;
}
D3DVECTOR D3DVECTOR_New(int a, float b, float c)
{
    D3DVECTOR result;
    result.x = a;
    result.y = b;
    result.z = c;
    return result;
}
D3DVECTOR D3DVECTOR_New(int a, float b, int c)
{
    D3DVECTOR result;
    result.x = a;
    result.y = b;
    result.z = c;
    return result;
}
D3DVECTOR D3DVECTOR_New(int a, int b, float c)
{
    D3DVECTOR result;
    result.x = a;
    result.y = b;
    result.z = c;
    return result;
}
D3DVECTOR D3DVECTOR_New(int a, int b, int c)
{
    D3DVECTOR result;
    result.x = a;
    result.y = b;
    result.z = c;
    return result;
}


/*
 * Transformed/lit vertices
 */
typedef struct _D3DTLVERTEX {
    union {
        D3DVALUE    sx;             /* Screen coordinates */
        D3DVALUE    dvSX;
    };
    union {
        D3DVALUE    sy;
        D3DVALUE    dvSY;
    };
    union {
        D3DVALUE    sz;
        D3DVALUE    dvSZ;
    };
    union {
        D3DVALUE    rhw;        /* Reciprocal of homogeneous w */
        D3DVALUE    dvRHW;
    };
    union {
        D3DCOLOR    color;          /* Vertex color */
        D3DCOLOR    dcColor;
    };
    union {
        D3DCOLOR    specular;       /* Specular component of vertex */
        D3DCOLOR    dcSpecular;
    };
    union {
        D3DVALUE    tu;             /* Texture coordinates */
        D3DVALUE    dvTU;
    };
    union {
        D3DVALUE    tv;
        D3DVALUE    dvTV;
    };
#if(DIRECT3D_VERSION >= 0x0500)
#if (defined __cplusplus) && (defined D3D_OVERLOADS)
    _D3DTLVERTEX() { }
    _D3DTLVERTEX(const D3DVECTOR& v, float _rhw,
        D3DCOLOR _color, D3DCOLOR _specular,
        float _tu, float _tv)
    {
        sx = v.x; sy = v.y; sz = v.z; rhw = _rhw;
        color = _color; specular = _specular;
        tu = _tu; tv = _tv;
    }
#endif
#endif /* DIRECT3D_VERSION >= 0x0500 */
} D3DTLVERTEX, * LPD3DTLVERTEX;
