#ifndef _DRAG_PROGRESS_H_
#define _DRAG_PROGRESS_H_

#include "Config.h"

#include "ddutil.h"

class CSurface;

class CDragProgress
{
public:
	CSurface* m_pProgress;        // 전면(활성화)이미지
	CSurface* m_pProgress_gray;   // 후면(비활성화)이미지
	CSurface* m_pDragBar_Nor;
	CSurface* m_pDragBar_Over;
	CSurface* m_pDragBar_Click;
	CSurface* m_pDragBar_Dis;

	char m_Fn_Progress[255];
	char m_Fn_Progress_gray[255];
	char m_Fn_DragBar[255];
	char m_Fn_DragBar_Nor[255];
	char m_Fn_DragBar_Over[255];
	char m_Fn_DragBar_Click[255];
	char m_Fn_DragBar_Dis[255];


	int  m_CurBarLevel;
	int  m_CurBarState;
	BOOL m_bVisible;
	BOOL m_bBarClick;

public:
	CDragProgress();
	~CDragProgress();

	void Init(char* , char* );
	void LoadRes(int xpos, int ypos, int pw=-1, int ph=-1, int dw=-1, int dh=-1);
	void DeleteRes();
	void Draw();
	void DrawProgress();
	void DrawBar();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	BOOL IsInside(CSurface* surface, int x, int y) ;
	void SetPos(int x, int y);
};

#endif // _DRAG_PROGRESS_H_
