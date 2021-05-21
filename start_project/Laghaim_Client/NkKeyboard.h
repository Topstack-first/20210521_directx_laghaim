#ifndef _NK_KEYBOARD_H_
#define _NK_KEYBOARD_H_

#include "Config.h"

// ���ĺ� Ű����.
#define NK_A 0x41
#define NK_B 0x42
#define NK_C 0x43
#define NK_D 0x44
#define NK_E 0x45
#define NK_F 0x46
#define NK_G 0x47
#define NK_H 0x48
#define NK_I 0x49
#define NK_J 0x4A
#define NK_K 0x4B
#define NK_L 0x4C
#define NK_M 0x4D
#define NK_N 0x4E
#define NK_O 0x4F
#define NK_P 0x50
#define NK_Q 0x51
#define NK_R 0x52
#define NK_S 0x53
#define NK_T 0x54
#define NK_U 0x55
#define NK_V 0x56
#define NK_W 0x57
#define NK_X 0x58
#define NK_Y 0x59
#define NK_Z 0x5A

#define NK_AKEY_OFFSET		0x41
#define NK_DEFINED_KEYNUM	26

// ����Ű..
#define NK_LEFT		0x25
#define NK_UP		0x26
#define NK_RIGHT	0x27
#define NK_DOWN		0x28

#define NK_ARROW_KEY_OFFSET	0x25
#define NK_DEFINED_ARROWNUM	4

#define NK_KEY_UP			0
#define NK_KEY_PRESSED		1
#define NK_KEY_UNCHANGED	2

#define NKKEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define NKKEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// ��� �ϰ� ������ ����� ���� �ʴ´�..
// ��? �밡�� ��� ������ �ϰ� �ֱ� ������. �׸��� ���̷�Ʈ ��ǲ�� �ֱ� ������..
// ��,, �� ���� ������..
// �̺�Ʈ �ֵ����� �ƴϱ� ������,,
// �밡�ٰ� �Ǵ±���..
class CNkKeyboard
{
protected:
	// �Ϲ� ���ĺ�.
	int		m_PrevKeyState[NK_DEFINED_KEYNUM];
	int		m_CurKeyState[NK_DEFINED_KEYNUM];
	BOOL	m_bStateChanged[NK_DEFINED_KEYNUM];

	// ȭ��ǥ Ű.
	int		m_PrevArrowState[NK_DEFINED_ARROWNUM];
	int		m_CurArrowState[NK_DEFINED_ARROWNUM];
	BOOL	m_bArrowStateChanged[NK_DEFINED_ARROWNUM];

public:
	CNkKeyboard();
	~CNkKeyboard();

	// ���ĺ�..
	BOOL	IsPressed(int key_code);
	BOOL	IsStateChanged(int key_code);

	// �ַο�.
	BOOL	IsArrowPressed(int key_code);
	BOOL	IsArrowStateChanged(int key_code);

	void	CheckUIMgrKey();
};

#endif // _NK_KEYBOARD_H_