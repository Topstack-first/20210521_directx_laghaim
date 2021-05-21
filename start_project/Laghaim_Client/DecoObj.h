#ifndef _DECO_OBJ_H_
#define _DECO_OBJ_H_

#include "Config.h"

#include <list>
#include <iterator>
using namespace std;
#include "CapSyong.h"
#include "MemoryPoolBase.h"

class IndexedTexture;
class CLolos;
class CClrAlloc;


#define BLD_HOUSE		0
#define BLD_BLD			1
#define BLD_TREE		2
#define BLD_TABLE		3
#define BLD_CHAIR		4
#define BLD_ETC			5
#define BLD_EVENT_1		6
#define BLD_EVENT_2		7
#define BLD_SHOP		8
#define BLD_FLY			9
#define BLD_STAR		11
#define BLD_FLOOR		12	// [2007/11/5 theodoric] 바닥 속성 추가
#define BLD_TYPE_NUM	13

#define MAX_FIRE_PARTICLE 10
#define MAX_SMOKE_PARTICLE 10
#define MAX_SPRAY_PARTICLE 10
#define MAX_LIGHT_PARTICLE 10
#define MAX_TURN_PARTICLE 10
#define MAX_NKCHA_PARTICLE 10
#define MAX_NKMOB_PARTICLE 20

#define MAX_ELCTRIC_PARTICLE 60
#define MAX_CLYSTALGLOW 3
#define MAX_WATERFALL 1
#define MAX_WATERFALL_PARTICLE 200
#define MAX_PORTAL_BOSSRAID_PARTICLE 50
#define MAX_FIRESMOKE_PARTICLE 20

#define REDFIRE 0
#define BLUEFIRE 1
#define REDFIRE_ONETIME 2 // 한번 생겼다가 작아지며 꺼지는 불꽃 ///플레임스피어 때 추가

#define DECO_ALPHA_NONE	0
#define DECO_ALPHA_SEE	1
#define DECO_ALPHA_FAR	2

#define LOLO_CRYSTAL_TOWER	182
#define LOLO_CASTLE_FLAG	239
#define LOLO_OUTSIDE_DOOR	184
#define LOLO_CASTLE_GATE_OPEND	67
#define LOLO_CASTLE_GATE_CLOSED	233
#define LOLO_OUTSIDE_DOOR_CLOSED	183

//wan:2004-12
//Whitehorn Dungeon
#define LOLO_BOSS_GATE_CLOSED 80

typedef struct Deco_Particle
{
	int				start_time;
	int 			life_time;
	int 			cur_time;
	float			scale;
	float			fade;

	float			red;
	float			green;
	float			blue;

	D3DVECTOR		first_Pos;
	D3DVECTOR		cur_Pos;
	D3DVECTOR		direction;
	D3DVECTOR		gravity;

	int				texIndex;
	int				texFrame;

} Deco_Particle;



class CDecoObj : public MemoryPoolBase<CDecoObj>
{
	// 캐릭터와 관련된 effects
	list<int>		m_listEffect;
	void			InsertEffectNum(int num);

protected:
	CClrAlloc	*m_pClrAlloc;
	BOOL		m_bOpenedRoof;
public:
	CDecoObj(CClrAlloc *pClrAlloc=NULL);
	~CDecoObj();

	CLolos *m_pLolo;						// object 하나.
	int		m_nLoloId;

	float m_wx, m_wy, m_wz, m_dir;			// 월드공간 좌표와 방향
	D3DVECTOR	m_vBoundBox[8];
	WORD		m_wColorIdx;
	DWORD		m_dwColorPick;
	int			m_nCurFrame;
	float		m_fTimeElapsed;
	int			m_nFlyFrame;
	D3DVECTOR*	m_WalkPlaneVect;

	static D3DMATRIX	m_matWorld, m_matTrans, m_matRotate;
	static float		m_fAniRate;

	// Particle Effects...
	BOOL			m_LineEffectsLoad;
	void			DeleteEffectNum();

	Deco_Particle*	Fire_Particle;
	Deco_Particle*	Smoke_Particle;
	Deco_Particle*	Spray_Particle;
	Deco_Particle*	Turn_Particle;
	Deco_Particle*	Light_Particle;
	Deco_Particle*	Electric_Particle;
	Deco_Particle*	ClystalGlow;
	Deco_Particle*	Waterfall;
	Deco_Particle*	Waterfall_Particle;
	Deco_Particle*	PortalBossraid;
	Deco_Particle*	Firesmoke_Particle;

	void		CalBoundBox();

	void		OpenRoof()
	{
		m_bOpenedRoof = TRUE;
	};
	void		CloseRoof()
	{
		m_bOpenedRoof = FALSE;
	};
	BOOL		IsRoofOpend()
	{
		return m_bOpenedRoof;
	};
	BOOL		IsInside(float wx, float wz);
	BOOL		MustRenderLast()
	{
		return (m_pLolo->MustRenderLast());
	}

	HRESULT Render(BOOL bNoRoof = FALSE, int whichAlpha=DECO_ALPHA_NONE);
	HRESULT	RenderMatrixLolo(); //메트릭스 롤러 그리자
	// 멀리 있는 것 알파 블렌딩을 위한 Factor, 0.0f: 먼거, 1.0f: 가까운거
	HRESULT	RenderMatrix2009Lolo();
	float m_farFactor;
};

class CDecoObjSet
{
protected:
	IndexedTexture	*m_pIndexedTexture;
	char	*m_pLolosDir;

public:
	CLolos	*m_pLoloSet;					// 배열 ,,, 이중포인터가 아니다.

	char	**m_FileName;
	char	**m_NickName;
	int		*m_pnObjIdx;						// object 고유 넘버.
	int		m_nLoloCount;
	int		*m_pnObjType;						// 오브젝트의 종류.

	CDecoObjSet(IndexedTexture *pTexture);
	virtual ~CDecoObjSet();

	void SetLolosDir(char *dir_name);
	void ReadDataFile( char *file_name, BOOL bLoadWalkPlane = FALSE );
	char *GetSentence(char *dest, int n, FILE *fp);
	char *PGetSentence(char *dest, int n, PFILE *fp);
};

#endif // _DECO_OBJ_H_
