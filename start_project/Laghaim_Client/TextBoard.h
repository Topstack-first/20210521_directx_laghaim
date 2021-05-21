#ifndef _TEXTBOARD_H_
#define _TEXTBOARD_H_

#include "Config.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

/************************************************************************/
/* include                                                                     */
/************************************************************************/

#include <map>
#include <algorithm>
#include <list>



using namespace std;
/************************************************************************/
/* #define                                                                     */
/************************************************************************/
#define TEXTLINE_IDX (m_iter->first)
#define TEXTLINE_LIST (m_iter->second)

#define TEXTLIST_RGB_NOR RGB(141, 195,   9) // RGB(152, 212, 4)
#define TEXTLIST_RGB_OVR RGB( 97, 249, 254) // RGB(152, 212, 4)
#define TEXTLIST_RGB_CLK RGB(254, 254, 254) // RGB(152, 212, 4)

class CTextList;

typedef std::map<int, CTextList * > TLINE_LIST;
/************************************************************************/
/* CTextBoard                                                                     */
/************************************************************************/
// 선택할수 있는 텍스트 박스
// 최하위 클래스로는 CTextBox가 있고 그것을 한줄로 묶은(세로로) CTextList가 있다.
// CTextBoard는 그 List들을 묶어 한번에 표시한 클래스이다.

class CTextBoard
{
protected:
	float	m_fBasisX, m_fBasisY;

	TLINE_LIST::iterator	m_iter;
	TLINE_LIST				m_TLineList;

	int		m_nNowSelectLine;

	float	m_fHeight;

	int		m_nLineBoxNum;		// 텍스트보드의 세로 라인수
	int		m_nColumnNum;		// 텍스트보드의 가로 라인수

	int		m_nNowPage;

	int		m_nTextSize;

public:
	CTextBoard();
	CTextBoard(int nTextSize);
	virtual ~CTextBoard();

	void DeleteRes();
	void Restore();
	void Draw(int nPage);
	void DrawLine(int nLine, int nX, int nY, BOOL bOver);
	void DrawClipLine(int nLine, int nX, int nY, int nMin, int nMax, int nPos, BOOL bOver);
	void Destroy();

	BOOL    IsInside(int x, int y);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void Clear();
	void Init(float fBasisX, float fBasisY, float fHeight, int nLineNum, int nColumnNum);

	void AddLine(int dwTextIdx, float fBoxWidth);
	void DelLine(int nLine);
	void AllDelLine();

	void SelectLine(int nSelectNum, bool bRelease);
	int  GetSelectLine();

	char * GetText(int nLineIdx, int nTextIdx);
	void SetText(int nLineIdx, int nTextIdx, char *szText);
	void RemoveLine(int nLineIdx);
	void AllRemoveLine();

	void SetBasisPos(float fBasisX, float fBasisY);

	void ExtendedBoardLine( int Plus ); ///071102 원석 수정 : 라인수가 엄청 많이 와서 기존 초기 수치를 넘을때 확장해주는 함수.
};
#endif // !defined(_TEXTBOARD_H_)
