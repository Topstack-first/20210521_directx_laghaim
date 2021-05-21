//-----------------------------------------------------------------------------
// File: D3DApp.h
//
// Desc: Application class for the Direct3D samples framework library.
//
// Copyright (c) NAKO Interative. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef  D3DAPP_H
#define  D3DAPP_H
#define  D3D_OVERLOADS
#include <d3d.h>
#include "D3DFrame.h"
#include "D3DEnum.h"
#include "D3DUtil.h"
//#include "resource.h"

//-----------------------------------------------------------------------------
// Name: class CD3DApplication
// Desc:
//-----------------------------------------------------------------------------

class CD3DApplication
{
protected:
	CD3DFramework7* m_pFramework;
	BOOL            m_bActive;
	BOOL            m_bReady;

	BOOL            m_bFrameMoving;
	BOOL            m_bSingleStep;
	DWORD           m_dwBaseTime;
	DWORD           m_dwStopTime;
	RECT			m_rcClient;					// 클라이언트 창의 크기.

	virtual HRESULT ProcessNextFrame(HWND hwnd);							// 2D, 3D Rendering 호출 루틴.
	virtual HRESULT Render2DEnvironment(HWND hwnd);
	virtual HRESULT Render3DEnvironment();
	virtual VOID    Cleanup3DEnvironment();
	VOID    DisplayFrameworkError( HRESULT, DWORD );

public:
	HRESULT Change3DEnvironment();

protected:
	HRESULT EnumerateAndSelect();
	HRESULT Initialize3DEnvironment();

	HWND            m_hWnd;						// 메인 윈도우 핸들
	//DWORD			m_dwWsStyle;
	D3DEnum_DeviceInfo*  m_pDeviceInfo;
	LPDIRECTDRAW7        m_pDD;
	LPDIRECT3D7          m_pD3D;
	LPDIRECT3DDEVICE7    m_pd3dDevice;
	LPDIRECTDRAWSURFACE7 m_pddsRenderTarget;
	LPDIRECTDRAWSURFACE7 m_pddsRenderTargetLeft; // For stereo modes
	DDSURFACEDESC2       m_ddsdRenderTarget;

	// pick buffer 용
	LPDIRECT3DDEVICE7    m_pd3dDevicePick;
	BOOL				 m_bRGB565;
	BOOL				 m_bGameMode;

	// Overridable variables for the app
	TCHAR*				 m_strWindowTitle;
	BOOL                 m_bAppUseZBuffer;
	BOOL                 m_bAppUseStereo;
	BOOL                 m_bShowStats;
	BOOL				 m_bShowStats_Nalrim;
	HRESULT              (*m_fnConfirmDevice)(DDCAPS*, D3DDEVICEDESC7*);
	BOOL				 m_bWindowed;

	// Overridable functions for the 3D scene created by the app
	virtual HRESULT OneTimeSceneInit()
	{
		return S_OK;
	}
	virtual HRESULT InitDeviceObjects()
	{
		return S_OK;
	}
	virtual HRESULT Init2DFrameObjects()
	{
		return S_OK;
	}
	virtual HRESULT DeleteDeviceObjects()
	{
		return S_OK;
	}
	virtual HRESULT Render()
	{
		return S_OK;
	}
	virtual HRESULT DisplayFrame()
	{
		return S_OK;
	}
	virtual HRESULT FrameMove( FLOAT )
	{
		return S_OK;
	}
	virtual HRESULT RestoreSurfaces()
	{
		return S_OK;
	}
	virtual HRESULT FinalCleanup()
	{
		return S_OK;
	}

	BOOL	m_bInitLoadingScr;
	virtual void	InitLoadingScr()
	{
		return ;
	}

	// Overridable power management (APM) functions
	virtual LRESULT OnQuerySuspend( DWORD dwFlags );
	virtual LRESULT OnResumeSuspend( DWORD dwData );

public:

	D3DMATRIX m_matWorld;
	D3DMATRIX m_matProj;
	// View control functions (for stereo-enabled applications)
	D3DMATRIX m_matLeftView;
	D3DMATRIX m_matRightView;
	D3DMATRIX m_matView;
	D3DMATRIX m_matCamera;

protected:
	VOID    SetAppLeftViewMatrix( D3DMATRIX mat )
	{
		m_matLeftView  = mat;
	}
	VOID    SetAppRightViewMatrix( D3DMATRIX mat )
	{
		m_matRightView = mat;
	}
	VOID    SetAppViewMatrix( D3DMATRIX mat )
	{
		m_matView      = mat;
	}
	VOID    SetViewParams( D3DVECTOR* vEyePt, D3DVECTOR* vLookatPt,
						   D3DVECTOR* vUpVec, FLOAT fEyeDistance );

	// Miscellaneous functions
	VOID    ShowStats();
	VOID    OutputText( DWORD x, DWORD y, TCHAR* str );
public:
	// Functions to create, run, pause, and clean up the application
	virtual HRESULT Create( HINSTANCE, TCHAR* );
	virtual INT     Run();
	virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual VOID    Pause( BOOL bPause );

	// Accessor functions
	CD3DFramework7 *     GetFramework()
	{
		return m_pFramework;
	};
	LPDIRECTDRAWSURFACE7 GetRenderTarget()
	{
		return m_pddsRenderTarget;
	};
	HWND                 Get_hWnd()
	{
		return m_hWnd;
	};
	BOOL				 GetbActive()
	{
		return m_bActive;
	};
	BOOL				 GetbReady()
	{
		return m_bReady;
	};
	VOID				 SetbReady(BOOL val)
	{
		m_bReady = val;
	};
	VOID				 SetbActive(BOOL val)
	{
		m_bActive = val;
	};
	VOID				 SetbFrameMoving(BOOL val)
	{
		m_bFrameMoving = val;
	};
	LPDIRECT3DDEVICE7    GetDevice()
	{
		return m_pd3dDevice;
	};
	LPDIRECT3DDEVICE7    GetDevicePick()
	{
		return m_pd3dDevicePick;
	};
	BOOL				 GetbFrameMoving()
	{
		return m_bFrameMoving;
	};
	DWORD				 GetBaseTime()
	{
		return m_dwBaseTime;
	};
	DWORD				 GetStopTime()
	{
		return m_dwStopTime;
	};
	VOID				 SetBaseTime(DWORD val)
	{
		m_dwBaseTime = val;
	};
	VOID				 SetStopTime(DWORD val)
	{
		m_dwStopTime = val;
	};
	VOID				 SetFramework(CD3DFramework7* pFramework)
	{
		m_pFramework = pFramework;
	};
	VOID				 DeleteFramework()
	{
		SAFE_DELETE( m_pFramework );
	};
	BOOL				 GetSingleStep()
	{
		return m_bSingleStep;
	};
	VOID				 SetSingleStep(BOOL val)
	{
		m_bSingleStep = val;
	};
	BOOL				 IsWinMode()
	{
		return m_bWindowed;
	};
	// 내가 넣은 것
	D3DEnum_DeviceInfo*  GetDeviceInfo()
	{
		return m_pDeviceInfo;
	};
	DWORD				m_nResolution;
	void				SetResolution (DWORD nBit)
	{
		m_nResolution = nBit;
	};
	DWORD				GetCurResolution ()
	{
		return m_nResolution;
	};

	// Class constructor
	CD3DApplication();
};
#endif // D3DAPP_H
