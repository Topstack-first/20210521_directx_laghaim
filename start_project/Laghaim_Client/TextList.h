#ifndef _TEXTLIST_H_
#define _TEXTLIST_H_

#include "Config.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

/************************************************************************/
/* include                                                                     */
/************************************************************************/

#include <map>
#include <algorithm>
#include <list>

#include <D3DUtil.h>

using namespace std;

/************************************************************************/
/* #define                                                                     */
/************************************************************************/
#define TBOX_IDX (m_iter->first)
#define TBOX_LIST (m_iter->second)

class CTextOutBox;

typedef std::map<int, CTextOutBox* > BTEXT_LIST;
/************************************************************************/
/* CTextList                                                                     */
/************************************************************************/
class CTextList
{
protected:

	float	m_fBasisX, m_fBasisY;

	int		m_nSelectNum;

	float	m_fBoxWidth;
	float	m_fHeight;

	bool	m_bIsColLine;

	COLORREF m_color;

	int		 m_nfontsize;

	BTEXT_LIST				m_BTextList;
	BTEXT_LIST::iterator	m_iter;


public:
	CTextList();
	virtual ~CTextList();

	void DeleteRes();
	void Init(int font_size, COLORREF font_color, int pos_x, int pos_y, int width = -1, int height = -1);
	void Restore();
	void Draw(int nPage);
	void DrawLine(int nLine, int nX, int nY, BOOL bOver);

	void DrawLine(int nLine, int nX, int nY, int nMin, int nMax, int nPos, BOOL bOver);
	void DelLine(int nLine);

	BOOL InputProcess(int x, int y, int nPage);

	void AddTBox(int nBoxNum);
	void AllDelTBox();

	void SetText(int nTextIdx, char * szText);
	char* GetText(int nTextIdx);
	void RemoveText(int nTextIdx);
	void AllRemoveText();

	void SetAllSelect();
	void SetAllUnSelect();

	float GetBoxWidth()
	{
		return m_fBoxWidth;
	}
	int   GetSelectNum();
	void  SetSelect(int nTextIdx, bool fFlag, COLORREF c_select = RGB(0, 0, 0), COLORREF c_unselect = RGB(0, 0, 0));
	void  SetSelectOther(int nTextIdx, COLORREF color = RGB(0, 0, 0));


	void SetBasisPos(float fBasisX, float fBasisY);

};
#endif // _TEXTLIST_H_
