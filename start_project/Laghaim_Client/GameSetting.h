#ifndef _GAME_SETTING_H_
#define _GAME_SETTING_H_

#include "Config.h"

// Include ////////////////////////////////////////////////
#include "ddutil.h"
#include "basicbutton.h"
// class declare     //////////////////////////////////////

class CSurface;
class CCheckGroup;
class CCheckbutton;
class CSliderButton;


class CGamesetting
{

public:
	static	CGamesetting* Create();

	~CGamesetting();

// function declare
public:
	void Init();
	void LoadRes();
	void DeleteResource();
	void ButtonDraw();
	void SetBtnPosition();
	void GameSetLoad();
	void GameSetSave();

	BOOL CreateInstance();
	BOOL IsInstance();

	int  Draw();
	void IsScreenModeChange(unsigned char* mode);

	BOOL IsInside(int x, int y);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	CSurface* GetBackSurface() const
	{
		return m_pBackSurface;
	}

	void move_window(UINT msg, int x, int y);

// variable declare
private:


	CSurface*	m_pBackSurface;

	int			m_ScreenWidth;
	int			m_ScreenHeight;

	int			m_nPosX;
	int			m_nPosY;

	CCheckGroup* m_GShadow ; // �׸��� ǥ�� ---> Checkbutton Count : 2(4)
	CCheckGroup* m_GWeather; // ���� ȿ��   ---> Checkbutton Count : 2(4)
	CCheckGroup* m_GSMode  ; // ȭ�� ���   ---> Checkbutton Count : 2(4)
	CCheckGroup* m_GCBit   ; // �÷� ��Ʈ   ---> Checkbutton Count : 2(4)
	CCheckGroup* m_GResol  ; // �ػ�	    ---> Checkbutton Count : 3(6)

	CCheckbutton* m_pEListen;	// Effect Sound Listen		---> Checkbutton Count : 1(2)
	CCheckbutton* m_pBListen;	// Background sound Listen	---> Checkbutton Count : 1(2)

	CCheckbutton* m_p3DSound;	// Use 3D Sound				---> Checkbutton Count : 1(2)
	CCheckbutton* m_pZListen;	// Zone sound Listen		---> Checkbutton Count : 1(2)

	CBasicButton  m_btRefresh;	// ���� ��ư
	CBasicButton  m_btCancel ;  // ��� ��ư
	CBasicButton  m_btClose  ;  // x â �ݱ� ��ư

	CSliderButton* m_pESlider;	// ����Ʈ �����̵�
	CSliderButton* m_pBSlider;	// ���   �����̵�

	CGamesetting();

};
#endif // _GAME_SETTING_H_