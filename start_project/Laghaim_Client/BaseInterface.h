#ifndef _BASE_INTERFACE_H_
#define _BASE_INTERFACE_H_

#include "Config.h"

class CBaseInterface
{
public:
	CBaseInterface() {}
	virtual ~CBaseInterface() {}
public:
	virtual void LoadRes() = 0;
	virtual void Draw() = 0;
	virtual void DeleteRes() = 0;
	virtual LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) = 0;
	virtual float Update()
	{
		return 0;
	}
};

#endif // _BASE_INTERFACE_H_
