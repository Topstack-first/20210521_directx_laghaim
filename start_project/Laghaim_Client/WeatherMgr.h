#ifndef _WEATHER_MGR_H_
#define _WEATHER_MGR_H_

#include "Config.h"

#define RAIN_MAX	700
#define RAIN_START	(80.0f)
#define RAIN_AREA	(100.0f)
#define RAIN_WIDTH	(0.1f)
#define RAIN_HEIGHT	(10.0f)
#define RAIN_SPEED	(8.0f)
#define RAIN_TIME	(50)	// 밀리세컨드...
#define RAIN_MOVE	(3.0f)

#define SNOW_WIDTH	(0.5f)	// SNOW_HEIGHT의 절반이 적당하다.
//#define SNOW_HEIGHT	(0.5f)
#define SNOW_SPEED	(2.0f)
#define SNOW_MOVE	(0.5f)

#define DROP_MAX	20
#define DROP_HEIGHT	(1.0f)
#define DROP_WIDTH	(0.7f)

#define MAX_VERTEX 4
#define MAX_INDEX 6

class CRains;
class IndexedTexture;


class CRainObj
{
public:
	CRainObj(float x, float z, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f);

	D3DVECTOR m_RainBottom;
	float m_PosY;
	CRainObj *m_Next;

	float m_fSpeed;
	float m_fSpd_x;
	float m_move;
};

class CRains
{
public:
	CRains();
	virtual ~CRains();

	void AddRain(int weight);
	void RemoveAllRains();
	void Drop();
	void Render(LPDIRECTDRAWSURFACE7 rainTexture, LPDIRECTDRAWSURFACE7 dropTexture,
				D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, int weight = 3, BOOL isSnow = FALSE);
	void Render2(LPDIRECTDRAWSURFACE7 rainTexture, LPDIRECTDRAWSURFACE7 dropTexture,
				 D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, int weight = 3, BOOL isSnow = FALSE);
	void Render3(LPDIRECTDRAWSURFACE7 rainTexture, LPDIRECTDRAWSURFACE7 dropTexture,
				 D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, int weight = 3, BOOL isSnow = FALSE);

	void SakuraSpeed(CRainObj* sakuraobj);

	CRainObj *m_RainList;
	int m_LastTime;

	D3DLVERTEX	m_RainVertex[RAIN_MAX*MAX_VERTEX];
	unsigned short m_RainIndex[RAIN_MAX*MAX_INDEX];
	D3DLVERTEX	m_DropVertex[DROP_MAX*MAX_VERTEX];
	unsigned short m_DropIndex[DROP_MAX*MAX_INDEX];

	// 임시 저장용.
	static D3DLVERTEX	m_tmpLvertex[MAX_VERTEX];
	static int			m_nInit;
	BOOL m_bLogin;
};

class CWeatherMgr
{
protected:
	// 비에 관련된 것.
	CRains		*m_pRain;
	int			m_nRainWeight;
	BOOL		m_bShowRain;
	BOOL		m_bShowSnow;

	int			m_nSakuraType;

	BOOL		m_bTonado;
	float		m_fTonadoAng;
	
	DWORD		m_dwBaseTime;

	// 임시 기억장소..
	D3DMATRIX	m_matWorld, m_matRotate, m_matView;

	int			m_nCurWeatherState;
	BOOL		m_bEnableWeatherSystem;
		
	BOOL m_bLogin;
public:
	CWeatherMgr();
	virtual ~CWeatherMgr();

	void		LoadRes();

	void		SetDevice() {};
	void		DeleteRes();

	BOOL		IsRaining()
	{
		return (m_bShowRain || m_bShowSnow);
	};

	void		StartSakura(int weight, BOOL bTonado = FALSE);
	void		StopSakura();
	void		RenderSakura();

	BOOL		IsSnowing()
	{
		return m_bShowSnow;
	};
	void		StartRain(int weight = 3);
	void		StopRain();

	void		StartSnow(int weight = 3, BOOL bTonado = FALSE);
	void		StopSnow();

	void		Render();
	void		RenderRain();
	void		RenderSnow();

	void		SetWeather(int weather);
	void		StopWeatherSystem();
	void		ResumeWeatherSystem();
	void		DungeonWeatherSystem();

	void		SetLogin(BOOL login)
	{
		m_bLogin = login;
		m_pRain->m_bLogin = login;
	};
};

#endif // _WEATHER_MGR_H_
