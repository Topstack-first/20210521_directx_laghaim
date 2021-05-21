#include "stdafx.h"

#include <assert.h>
#include "NkKeyboard.h"

CNkKeyboard::CNkKeyboard()
{
	int i;

	for (i=0; i < NK_DEFINED_KEYNUM; i++)
	{
		m_PrevKeyState[i] = NK_KEY_UP;
		m_CurKeyState[i] = NK_KEY_UP;
		m_bStateChanged[i] = FALSE;
	}

	for (i=0; i < NK_DEFINED_ARROWNUM; i++)
	{
		m_PrevArrowState[i] = NK_KEY_UP;
		m_CurArrowState[i] = NK_KEY_UP;
		m_bArrowStateChanged[i] = FALSE;
	}
}

CNkKeyboard::~CNkKeyboard()
{}

BOOL CNkKeyboard::IsPressed(int key_code)
{
	return m_CurKeyState[key_code-NK_AKEY_OFFSET];
}

BOOL CNkKeyboard::IsStateChanged(int key_code)
{
	return m_bStateChanged[key_code-NK_AKEY_OFFSET];
}

BOOL CNkKeyboard::IsArrowPressed(int key_code)
{
	return m_CurArrowState[key_code-NK_ARROW_KEY_OFFSET];
}

BOOL CNkKeyboard::IsArrowStateChanged(int key_code)
{
	return m_bArrowStateChanged[key_code-NK_ARROW_KEY_OFFSET];
}

void CNkKeyboard::CheckUIMgrKey()
{
	// [2008/7/14 Theodoric] 몇개만 했더걸 다해버리자... for문 둘려도 얼마 않되는걸... ㅡㅡ;;
	for(int i=NK_A; i<NK_Z+0X01; i++)
	{
		if(NKKEY_DOWN(i))
		{
			if (m_PrevKeyState[i - NK_AKEY_OFFSET] == NK_KEY_PRESSED)
			{
				m_CurKeyState[i - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[i - NK_AKEY_OFFSET] = FALSE;
			}
			else
			{
				m_PrevKeyState[i - NK_AKEY_OFFSET] = m_CurKeyState[i - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[i - NK_AKEY_OFFSET] = TRUE;
			}
		}
		else
		{
			if (m_PrevKeyState[i - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[i - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[i - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[i - NK_AKEY_OFFSET] = m_CurKeyState[i - NK_AKEY_OFFSET] = NK_KEY_UP;
		}
	}


	for( int i=NK_LEFT; i<NK_DOWN+1; i++)
	{
		if (NKKEY_DOWN(i))
		{
			if (m_PrevArrowState[i - NK_ARROW_KEY_OFFSET] == NK_KEY_PRESSED)
			{
				m_CurArrowState[i - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[i - NK_ARROW_KEY_OFFSET] = FALSE;
			}
			else
			{
				m_PrevArrowState[i - NK_ARROW_KEY_OFFSET] = m_CurArrowState[i - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[i - NK_ARROW_KEY_OFFSET] = TRUE;
			}
		}
		else
		{
			if (m_PrevArrowState[i - NK_ARROW_KEY_OFFSET] == NK_KEY_UP)
				m_bArrowStateChanged[i - NK_ARROW_KEY_OFFSET] = FALSE;
			else
				m_bArrowStateChanged[i - NK_ARROW_KEY_OFFSET] = TRUE;

			m_PrevArrowState[i - NK_ARROW_KEY_OFFSET] = m_CurArrowState[i - NK_ARROW_KEY_OFFSET] = NK_KEY_UP;
		}
	}
	/*
		// C 키
		if (NKKEY_DOWN(NK_C)) {
			if (m_PrevKeyState[NK_C - NK_AKEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurKeyState[NK_C - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_C - NK_AKEY_OFFSET] = FALSE;
			} else {
				m_PrevKeyState[NK_C - NK_AKEY_OFFSET] = m_CurKeyState[NK_C - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_C - NK_AKEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevKeyState[NK_C - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[NK_C - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[NK_C - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[NK_C - NK_AKEY_OFFSET] = m_CurKeyState[NK_C - NK_AKEY_OFFSET] = NK_KEY_UP;
		}

		// I 키
		if (NKKEY_DOWN(NK_I)) {
			if (m_PrevKeyState[NK_I - NK_AKEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurKeyState[NK_I - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_I - NK_AKEY_OFFSET] = FALSE;
			} else {
				m_PrevKeyState[NK_I - NK_AKEY_OFFSET] = m_CurKeyState[NK_I - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_I - NK_AKEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevKeyState[NK_I - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[NK_I - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[NK_I - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[NK_I - NK_AKEY_OFFSET] = m_CurKeyState[NK_I - NK_AKEY_OFFSET] = NK_KEY_UP;
		}

		// M키.
		if (NKKEY_DOWN(NK_M)) {
			if (m_PrevKeyState[NK_M - NK_AKEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurKeyState[NK_M - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_M - NK_AKEY_OFFSET] = FALSE;
			} else {
				m_PrevKeyState[NK_M - NK_AKEY_OFFSET] = m_CurKeyState[NK_M - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_M - NK_AKEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevKeyState[NK_M - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[NK_M - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[NK_M - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[NK_M - NK_AKEY_OFFSET] = m_CurKeyState[NK_M - NK_AKEY_OFFSET] = NK_KEY_UP;
		}

		// R키
		if (NKKEY_DOWN(NK_R)) {
			if (m_PrevKeyState[NK_R - NK_AKEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurKeyState[NK_R - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_R - NK_AKEY_OFFSET] = FALSE;
			} else {
				m_PrevKeyState[NK_R - NK_AKEY_OFFSET] = m_CurKeyState[NK_R - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_R - NK_AKEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevKeyState[NK_R - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[NK_R - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[NK_R - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[NK_R - NK_AKEY_OFFSET] = m_CurKeyState[NK_R - NK_AKEY_OFFSET] = NK_KEY_UP;
		}

		// T
		if (NKKEY_DOWN(NK_T)) {
			if (m_PrevKeyState[NK_T - NK_AKEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurKeyState[NK_T - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_T - NK_AKEY_OFFSET] = FALSE;
			} else {
				m_PrevKeyState[NK_T - NK_AKEY_OFFSET] = m_CurKeyState[NK_T - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_T - NK_AKEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevKeyState[NK_T - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[NK_T - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[NK_T - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[NK_T - NK_AKEY_OFFSET] = m_CurKeyState[NK_T - NK_AKEY_OFFSET] = NK_KEY_UP;
		}

		// W
		if (NKKEY_DOWN(NK_W)) {
			if (m_PrevKeyState[NK_W - NK_AKEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurKeyState[NK_W - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_W - NK_AKEY_OFFSET] = FALSE;
			} else {
				m_PrevKeyState[NK_W - NK_AKEY_OFFSET] = m_CurKeyState[NK_W - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_W - NK_AKEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevKeyState[NK_W - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[NK_W - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[NK_W - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[NK_W - NK_AKEY_OFFSET] = m_CurKeyState[NK_W - NK_AKEY_OFFSET] = NK_KEY_UP;
		}

		// S
		if (NKKEY_DOWN(NK_S)) {
			if (m_PrevKeyState[NK_S - NK_AKEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurKeyState[NK_S - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_S - NK_AKEY_OFFSET] = FALSE;
			} else {
				m_PrevKeyState[NK_S - NK_AKEY_OFFSET] = m_CurKeyState[NK_S - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_S - NK_AKEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevKeyState[NK_S - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[NK_S - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[NK_S - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[NK_S - NK_AKEY_OFFSET] = m_CurKeyState[NK_S - NK_AKEY_OFFSET] = NK_KEY_UP;
		}

		// A
		if (NKKEY_DOWN(NK_A)) {
			if (m_PrevKeyState[NK_A - NK_AKEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurKeyState[NK_A - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_A - NK_AKEY_OFFSET] = FALSE;
			} else {
				m_PrevKeyState[NK_A - NK_AKEY_OFFSET] = m_CurKeyState[NK_A - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_A - NK_AKEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevKeyState[NK_A - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[NK_A - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[NK_A - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[NK_A - NK_AKEY_OFFSET] = m_CurKeyState[NK_A - NK_AKEY_OFFSET] = NK_KEY_UP;
		}

		// D
		if (NKKEY_DOWN(NK_D)) {
			if (m_PrevKeyState[NK_D - NK_AKEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurKeyState[NK_D - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_D - NK_AKEY_OFFSET] = FALSE;
			} else {
				m_PrevKeyState[NK_D - NK_AKEY_OFFSET] = m_CurKeyState[NK_D - NK_AKEY_OFFSET] = NK_KEY_PRESSED;
				m_bStateChanged[NK_D - NK_AKEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevKeyState[NK_D - NK_AKEY_OFFSET] == NK_KEY_UP)
				m_bStateChanged[NK_D - NK_AKEY_OFFSET] = FALSE;
			else
				m_bStateChanged[NK_D - NK_AKEY_OFFSET] = TRUE;

			m_PrevKeyState[NK_D - NK_AKEY_OFFSET] = m_CurKeyState[NK_D - NK_AKEY_OFFSET] = NK_KEY_UP;
		}

		// left
		if (NKKEY_DOWN(NK_LEFT)) {
			if (m_PrevArrowState[NK_LEFT - NK_ARROW_KEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurArrowState[NK_LEFT - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[NK_LEFT - NK_ARROW_KEY_OFFSET] = FALSE;
			} else {
				m_PrevArrowState[NK_LEFT - NK_ARROW_KEY_OFFSET] = m_CurArrowState[NK_LEFT - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[NK_LEFT - NK_ARROW_KEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevArrowState[NK_LEFT - NK_ARROW_KEY_OFFSET] == NK_KEY_UP)
				m_bArrowStateChanged[NK_LEFT - NK_ARROW_KEY_OFFSET] = FALSE;
			else
				m_bArrowStateChanged[NK_LEFT - NK_ARROW_KEY_OFFSET] = TRUE;

			m_PrevArrowState[NK_LEFT - NK_ARROW_KEY_OFFSET] = m_CurArrowState[NK_LEFT - NK_ARROW_KEY_OFFSET] = NK_KEY_UP;
		}


		// up
		if (NKKEY_DOWN(NK_UP)) {
			if (m_PrevArrowState[NK_UP - NK_ARROW_KEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurArrowState[NK_UP - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[NK_UP - NK_ARROW_KEY_OFFSET] = FALSE;
			} else {
				m_PrevArrowState[NK_UP - NK_ARROW_KEY_OFFSET] = m_CurArrowState[NK_UP - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[NK_UP - NK_ARROW_KEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevArrowState[NK_UP - NK_ARROW_KEY_OFFSET] == NK_KEY_UP)
				m_bArrowStateChanged[NK_UP - NK_ARROW_KEY_OFFSET] = FALSE;
			else
				m_bArrowStateChanged[NK_UP - NK_ARROW_KEY_OFFSET] = TRUE;

			m_PrevArrowState[NK_UP - NK_ARROW_KEY_OFFSET] = m_CurArrowState[NK_UP - NK_ARROW_KEY_OFFSET] = NK_KEY_UP;
		}

		// right
		if (NKKEY_DOWN(NK_RIGHT)) {
			if (m_PrevArrowState[NK_RIGHT - NK_ARROW_KEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurArrowState[NK_RIGHT - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[NK_RIGHT - NK_ARROW_KEY_OFFSET] = FALSE;
			} else {
				m_PrevArrowState[NK_RIGHT - NK_ARROW_KEY_OFFSET] = m_CurArrowState[NK_RIGHT - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[NK_RIGHT - NK_ARROW_KEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevArrowState[NK_RIGHT - NK_ARROW_KEY_OFFSET] == NK_KEY_UP)
				m_bArrowStateChanged[NK_RIGHT - NK_ARROW_KEY_OFFSET] = FALSE;
			else
				m_bArrowStateChanged[NK_RIGHT - NK_ARROW_KEY_OFFSET] = TRUE;

			m_PrevArrowState[NK_RIGHT - NK_ARROW_KEY_OFFSET] = m_CurArrowState[NK_RIGHT - NK_ARROW_KEY_OFFSET] = NK_KEY_UP;
		}

		// down
		if (NKKEY_DOWN(NK_DOWN)) {
			if (m_PrevArrowState[NK_DOWN - NK_ARROW_KEY_OFFSET] == NK_KEY_PRESSED) {
				m_CurArrowState[NK_DOWN - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[NK_DOWN - NK_ARROW_KEY_OFFSET] = FALSE;
			} else {
				m_PrevArrowState[NK_DOWN - NK_ARROW_KEY_OFFSET] = m_CurArrowState[NK_DOWN - NK_ARROW_KEY_OFFSET] = NK_KEY_PRESSED;
				m_bArrowStateChanged[NK_DOWN - NK_ARROW_KEY_OFFSET] = TRUE;
			}
		} else {
			if (m_PrevArrowState[NK_DOWN - NK_ARROW_KEY_OFFSET] == NK_KEY_UP)
				m_bArrowStateChanged[NK_DOWN - NK_ARROW_KEY_OFFSET] = FALSE;
			else
				m_bArrowStateChanged[NK_DOWN - NK_ARROW_KEY_OFFSET] = TRUE;

			m_PrevArrowState[NK_DOWN - NK_ARROW_KEY_OFFSET] = m_CurArrowState[NK_DOWN - NK_ARROW_KEY_OFFSET] = NK_KEY_UP;
		}
	*/
}