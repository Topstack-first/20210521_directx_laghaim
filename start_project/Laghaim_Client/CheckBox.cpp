#include "stdafx.h"
#include "CheckBox.h"
#include "ddutil.h"
#include "headers.h"

extern char	g_strFontName[];
extern HFONT G_GetFont(int h, int w, const char * const fontname);

CheckBox::CheckBox()
	: posX(0)
	, posY(0)
	, width(0)
	, height(0)
	, isChecked(false)
	, isEnable(false)
	, isVisible(false)
	, pSurChecked(NULL)
	, pSurUnchecked(NULL)
	, pSurText(NULL)
{
}

CheckBox::~CheckBox()
{
	DeleteRes();
}

void CheckBox::Init(const std::string& checked, const std::string& unchecked)
{
	filenameChecked = checked;
	filenameUnchecked = unchecked;
}

void CheckBox::LoadRes()
{
	if( g_pDisplay == NULL )
		return;

	g_pDisplay->CreateSurfaceFromBitmap(&pSurChecked, const_cast<char*>(filenameChecked.c_str()));
	if( pSurChecked )
	{
		width = pSurChecked->GetWidth();
		height = pSurChecked->GetHeight();
		pSurChecked->SetColorKey(TRANS_COLOR);
	}

	g_pDisplay->CreateSurfaceFromBitmap(&pSurUnchecked, const_cast<char*>(filenameUnchecked.c_str()));
	if( pSurUnchecked )
	{
		pSurUnchecked->SetColorKey(TRANS_COLOR);
	}
}

void CheckBox::DeleteRes()
{
	SAFE_DELETE(pSurChecked);
	SAFE_DELETE(pSurUnchecked);
	SAFE_DELETE(pSurText);
}

HRESULT CheckBox::MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if( isEnable == false )
		return E_FAIL;

	if( isVisible == false )
		return E_FAIL;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			if( isInside(LOWORD(lparam), HIWORD(lparam)) == true )
			{
				bool checked = GetChecked();
				SetChecked( ! checked );
				return S_OK;
			}
		}
		break;
	}

	return E_FAIL;
}

void CheckBox::Draw()
{
	if( g_pDisplay == NULL )
		return;

	if( isVisible == false )
		return;

	if( pSurChecked == NULL )
		return;

	if( pSurUnchecked == NULL )
		return;

	CSurface* pSurf = NULL;
	if( isChecked == true )
	{
		pSurf = pSurChecked;
	}
	else
	{
		pSurf = pSurUnchecked;
	}

	if( pSurf )
	{
		g_pDisplay->Blt(posX, posY, pSurf);
	}

	if( pSurText )
	{
		g_pDisplay->Blt(posX + width + 2, posY + 5, pSurText);
	}
}

void CheckBox::SetPosition(const int posX, const int posY)
{
	this->posX = posX;
	this->posY = posY;
}

void CheckBox::SetChecked(const bool checked)
{
	isChecked = checked;
}

bool CheckBox::GetChecked()
{
	return isChecked;
}

void CheckBox::SetEnable(const bool enable)
{
	isEnable = enable;
}

bool CheckBox::GetEnable()
{
	return isEnable;
}

void CheckBox::SetVisible(const bool visible)
{
	isVisible = visible;
}

bool CheckBox::GetVisible()
{
	return isVisible;
}

void CheckBox::SetText(const std::string& text)
{
	this->text = text;
	SAFE_DELETE(pSurText);
	HFONT font = G_GetFont(12, 0, g_strFontName);
	
	g_pDisplay->CreateSurfaceFromText(&pSurText, font, const_cast<char*>(this->text.c_str()), RGB(0, 0, 1), _FCOLOR_GREEN);
	if( pSurText )
	{
		pSurText->SetColorKey(RGB(0, 0, 1));
	}
}

const std::string& CheckBox::GetText()
{
	return text;
}

bool CheckBox::isInside(int x, int y)
{
	if( posX < x && posX + width > x &&
		posY < y && posY + height > y )
		return true;

	return false;
}
