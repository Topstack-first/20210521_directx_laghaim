#ifndef _UI_ANIMLOLO_H_
#define _UI_ANIMLOLO_H_

#include <d3dtypes.h>
#include <ddraw.h>

class CLolos;
class IndexedTexture;

class UI_AnimLolo
{
public:
	UI_AnimLolo();
	~UI_AnimLolo();

public:
	void Init(const int width, const int height);
	void SetPos(const int posX, const int posY);
	void SetEyeAt(const D3DVECTOR& vEye, const D3DVECTOR& vAt);
	void LoadLolo(const std::string& fileName);
	void SetTexturePath(const std::string& path);
	void SetLolo(CLolos* pLolo);

	void Play(const bool loop);
	void Pause();
	void Stop();

	void Render();

	bool CheckRestore();
	HRESULT Restore();

private:	
	HRESULT	CreateDevices();
	void DeleteRes();

private:
	LPDIRECTDRAWSURFACE7 m_pddsBlt;
	LPDIRECTDRAWSURFACE7 m_pddsRenderTarget;
	LPDIRECTDRAWSURFACE7 m_pddsZBuffer;

	CLolos* m_pLolo;
	IndexedTexture* m_pTexture;

	D3DVECTOR m_vEyePt;
	D3DVECTOR m_vLookatPt;
	D3DVECTOR m_vUpVec;

	int	m_posX;
	int m_posY;
	int m_width;
	int m_height;

	int m_curFrame;
	int m_maxFrame;
	bool m_bPlaying;
	bool m_bLoop;
};

#endif
