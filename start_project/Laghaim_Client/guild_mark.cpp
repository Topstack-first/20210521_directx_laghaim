#include "stdafx.h"
#define D3D_OVERLOADS

#include "guild_mark.h"
#include "headers.h"
#include "IndexedTexture.h"
#include "main.h"
#include "D3DMath.h"



CGuildMark* CGuildMark::Create(int index, char* ImgName)
{
	return new CGuildMark(index, ImgName);
}

CGuildMark::CGuildMark(int index, char* ImgName)
{
	m_marksurface = new IndexedTexture(1);

	m_marksurface->SetTexturePath("data/guildmark/");

	m_texturenum = m_marksurface->CreateTextureFromFileNonePack(ImgName);
	//m_texturenum = m_marksurface->CreateTextureFromFile(ImgName);

	if(m_texturenum >= 0) m_marksurface->RestoreAllTextures(GET_D3DDEVICE());

	m_guildindex  = index;

}

void CGuildMark::Render(HWND hWnd, float nPosx, float nPosy)
{
	if( !GET_D3DDEVICE() )
		return;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;

	GET_D3DDEVICE()->GetViewport(&vpOld);

	RECT rect;

	GET_D3DDEVICE()->GetViewport(&vpNew);
	GetClientRect(hWnd, &rect);
	vpNew.dwWidth  = rect.right - rect.left ;

	int nHeight;
	nHeight = vpNew.dwWidth;
	if ( (nHeight - (rect.bottom-rect.top)) > 0)
		nHeight = rect.bottom - rect.top;
	vpNew.dwY = ( (rect.bottom - rect.top) - nHeight ) / 2;
	vpNew.dwHeight = nHeight;

	GET_D3DDEVICE()->SetViewport(&vpNew);

	TLVec[0] = D3DTLVERTEX( D3DVECTOR(nPosx - 0.5	  , nPosy - 0.5         , 0), 1, RGBA_MAKE( 255, 255, 255, 128), 0, 0, 0);
	TLVec[1] = D3DTLVERTEX( D3DVECTOR(nPosx - 0.5+16.f, nPosy - 0.5         , 0), 1, RGBA_MAKE( 255, 255, 255, 128), 0, 1, 0);
	TLVec[2] = D3DTLVERTEX( D3DVECTOR(nPosx - 0.5	  , nPosy - 0.5+16.f	, 0), 1, RGBA_MAKE( 255, 255, 255, 128), 0, 0, 1);
	TLVec[3] = D3DTLVERTEX( D3DVECTOR(nPosx - 0.5+16.f, nPosy - 0.5+16.f	, 0), 1, RGBA_MAKE( 255, 255, 255, 128), 0, 1, 1);


	LPDIRECTDRAWSURFACE7 lpTexture = m_marksurface->GetSurfaceByIdx(m_texturenum);

	GET_D3DDEVICE()->SetTexture(0, lpTexture);

	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

	GET_D3DDEVICE()->SetViewport(&vpOld);
}

CGuildMark::~CGuildMark()
{
	SAFE_DELETE(m_marksurface);
}
