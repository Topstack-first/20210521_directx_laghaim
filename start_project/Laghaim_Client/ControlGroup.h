#ifndef _CONTROL_GROUP_H_
#define _CONTROL_GROUP_H_

#include "Config.h"

#include "ddutil.h"

#include <vector>
using namespace std;

typedef struct _stPoset
{
	int m_stPosX;
	int m_stPosY;

	_stPoset() {}

	_stPoset(int n_x, int n_y)
	{
		m_stPosX = n_x;
		m_stPosY = n_y;
	}

} BTPS, *PBTPS;

class CCheckbutton;
class CCheckGroup
{
public:
	static CCheckGroup* Create(int nbcnt = 2, char* groupname = NULL);

	~CCheckGroup();
public:

	// set function
	void SetPosition( int x, int y )
	{
		m_nPosX = x, m_nPosY = y;
	}
	void SetbuttonPos( PBTPS pbt, int nx = 0, int ny = 0, unsigned int nw = 0, unsigned int nh = 0);
	void SetFileName( char* );
	void SetVisible(bool bVisible)
	{
		m_bVisible = bVisible;
	}
	void SetBtSet(int nNum, bool bEnable);
	// get function
	int  GetBtSet();
	// Logic function
	bool IsInside( int, int );
	// resource
	void LoadRes( int w = -1, int h = -1 );
	void DeleteRes();
	// render
	void Draw();
	void Draw( int x, int y, BOOL bChecked = FALSE );
	void DrawToBlt(bool bChecked = false);
	// message
	void MsgProc( HWND, UINT, WPARAM, LPARAM );

private:
	int				m_nPosX, m_nPosY;		// 그룹 영역 시작 위치
	int				m_nbtnum;
	unsigned int	m_uiwidth, m_uiheight;	// 그룹 영역 폭, 높이
	bool			m_bVisible;				// 그룹 영역내 컨트롤들이 보이는지...
	bool			m_bCheckStatus[3];		// 버튼 MAX 3개 까지

	CCheckbutton* m_pbt;					// 체크, 기타 버튼들..

	char	m_szGroupName[128];				// 그룹 이름

	CCheckGroup(int nbcnt = 2, char* groupname = NULL);		// 생성자...
};
#endif // _CONTROL_GROUP_
