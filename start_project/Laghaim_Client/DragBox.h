#ifndef _DRAG_BOX_H_
#define _DRAG_BOX_H_

#include "Config.h"


#include "Sheet.h"

class CSurface;
class CSheet;

class CDragBox
{
public:

	CDragBox();
	virtual ~CDragBox();

	CSurface *m_pDBarSurf;
	CSurface *m_pDBtnNormal;
	CSurface *m_pDBtnClick;

	char* m_BarFn;
	char* m_BtnNormalFn;
	char* m_BtnClickFn;


	int m_PosX;
	int m_PosY;
	int m_Width;

	int m_DBtnX;
	int m_DBtnY;
	int m_DBtnWidth;
	int m_DBtnHeight;

	int m_DBarX;
	int m_DBarY;
	int m_DBarWidth;
	int m_DBarHeight;

	BOOL m_bButtonDown;


	int m_MinPos;
	int m_MaxPos;
	int m_CurPos;
	int m_DisablePos;
	void Init(int pos_x, int pos_y, int width, char* BtnNormalFn, char* BtnClickFn, char*BarFn);
	void LoadRes();
	void DeleteRes();

	void SetValues(int min_pos, int max_pos, int cur_pos, int disable_pos = -1);
	void SetCurPos(int cur_pos, int disable_pos = -1) ;
	void MoveButton(int x, int y);
	void CalcPos();
	void CalcValues();
	int  GetCurPos()
	{
		return m_CurPos;
	}
	void Draw();
	////////////////////////////////////////////
	int m_DragBarXpos;			// DragBar의 x위치를 저장할 변수를 새로 만들었다
	void DrawBar();				// DragBar를 그린다
	//  Sheet클레스에서 CSetValuePage->SetDragBarPos(int x0, int x1, int x2, int x3, int x4) 함수를 호출
	//  5종류의 힘에대한 DragBar의 위치를 SetDragBar(int Xpos)함수를 통해서 구할수 있다..??
	void SetDragBar(int Xpos);
	///////////////////////////////////////////

	void SetPos(int x, int y)
	{
		m_PosX = x;
		m_PosY = y;
	}

	BOOL IsInside(int x, int y);

	int MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
				 LPARAM lParam );

};

#endif // _DRAG_BOX_H_
