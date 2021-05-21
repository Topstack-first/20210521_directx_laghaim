#ifndef _CONTROL_PIECE_H_
#define _CONTROL_PIECE_H_

#include "Config.h"

#include "BasicButton.h"
#include "TextOutBox.h"
#include "AlphaPopup.h"
#include "NkCharacter.h"

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CMsgPopUp;
class CTextOutBox;


#define PETSHOP_SLOT_WIDTH		(27)// * m_ScreenWidth / BASE_X_SIZE)
#define PETSHOP_SLOT_HEIGHT		(27)// * m_ScreenHeight / BASE_Y_SIZE)

#define PETSHOP_PACK_NUM		7
#define PETSHOP_SLOT_X_NUM		6
#define PETSHOP_SLOT_Y_NUM		9

class CControlPeiceShop
{
public:
	CNkCharacter		*m_pNkCha;
	CSurface			*m_pSurfaceBack;
	CTextOutBox			*m_pMoneyText;
	CBasicButton		*m_pBtn_Close;

	// 상단의 버튼들
	CBasicButton		*m_pBtnTail, *m_pBtnWing, *m_pBtnLeg;
	CBasicButton		*m_pBtnHead, *m_pBtnBody, *m_pBtnHorn;
	CBasicButton		*m_pBtnItem;

	void				SetButtonDown(CBasicButton *pButton);


	int m_ScreenWidth, m_ScreenHeight;
	int m_iMainX, m_iMainY;
	int m_shopslotstart_x, m_shopslotstart_y;
	__int64 m_iCurExp;
	char m_MsgBuf[1024];

public:
	CControlPeiceShop();
	virtual ~CControlPeiceShop();

	void    SetNkCha(CNkCharacter *pNkCha);
	void	SetMyPetExp(__int64 a);
	void	SetPieceName();
	void    LoadRes();
	void	DeleteRes();
	CItem *	Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);

	// shop data
	int		m_nCurPackNum;
	CItem	*m_pOverItem;
	CItem	*m_SellItem[PETSHOP_PACK_NUM+1];
	int		m_Slot[PETSHOP_PACK_NUM][PETSHOP_SLOT_X_NUM][PETSHOP_SLOT_Y_NUM];
	// piece name of mypet
	CTextOutBox	*m_pPieceNameBox;

	// shop control
	void	AddItemsToShop(int type);
	void	AddItemToShop(int type, int vnum);
	BOOL	FindBlankSlot(int pack_num, int &x, int &y, int w, int h);
	int		GetSlotVnum(int x, int y, int *type = NULL, int *shape = NULL);
	void	RemoveShopItems();

	void	LoadShopItems();
	void	RestroeStr();
};

#endif // _CONTROL_PIECE_H_
