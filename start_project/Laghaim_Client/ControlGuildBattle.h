#ifndef	_200706112040_Guild_battle_interface_pointDraw__
#define _200706112040_Guild_battle_interface_pointDraw__

#include "TextOutBox.h"

#define _MAX_NUMBER 3

class IndexedTexture;
class CTextOutBox;

class CGuildRankWarBattleCount
{
protected:
	typedef enum __imgType_
	{
		IT_NONE		= 0,
		IT_NUM		= 1,
		IT_READY	= 10,
		IT_START	= 11
	} IMGTYPE, CNT_STATE;



	int		m_tID_Numberic[12];

	int		m_befortime;
	int		m_nCount;

	float	m_alpha;
	float	m_scale;
	float   m_nFrameCount;

	bool	m_bActive;

	CNT_STATE	m_CountState;

	WORD				m_Idx[6];
	IndexedTexture*		m_pIDTexture;
	LPD3DTLVERTEX		m_pVertex;
public:

	CGuildRankWarBattleCount();
	~CGuildRankWarBattleCount();

	void	LoadResource();
	void	DeleteRes();

	bool	IsActive()
	{
		return m_bActive;
	}
	void	SetActive(bool active);
	void	ToggleActive()
	{
		SetActive( !IsActive() );
	}
	bool	IsInside(int x, int y);

	void	ReadyCount()
	{
		SetActive(TRUE);
	}
	void	StartCount(int count = 9);
	void	EndCount();
	void	StartBattle();

	void	OnStartCount();
	void	OnEndCount();

	void	SetVertex( IMGTYPE imgType ,float alpha = 1.0f, float scale = 1.0f );
	void	Render();
	void	Render( int TexID);

	HRESULT	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};

class CControlGuildBattle
{
protected:

	typedef enum __imgtype
	{
		BACKIMG = 0,
		NUMBERIMG
	} IMGTYP;

	typedef struct number_Info
	{
		byte	Num;
		int		XPos;
		int		YPos;
	};

	bool				m_bActive;

	CTextOutBox			m_ATeamName;
	CTextOutBox			m_BTeamName;

	number_Info			m_ATeamNum[_MAX_NUMBER];
	number_Info			m_BTeamNum[_MAX_NUMBER];

	int					m_texID_Back;
	int					m_texID_Numberic[10];
	int					m_wX;
	int					m_wY;

	WORD				m_Idx[6];
	IndexedTexture*		m_pIndexedTexture;
	LPD3DTLVERTEX		m_pVertex;



public:

	CControlGuildBattle();
	~CControlGuildBattle();

	void	LoadResource();
	void	DeleteRes();

	bool	IsActive()
	{
		return m_bActive;
	}
	void	SetActive(bool active)
	{
		m_bActive = active;
	}
	void	ToggleActive()
	{
		SetActive( !IsActive() );
	}

	void	SetTeamName(char* ATeamName, char* BTeamName );
	bool	IsInside(int x, int y);
	void	SetTeamKillcount(int ATeamKCount, int BTeamKCount);
	void	SetPosition();
	void	SetVertex( int x, int y, IMGTYP imgType );

	void	Render(int texID);
	void	Render();

	HRESULT	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};

#endif
