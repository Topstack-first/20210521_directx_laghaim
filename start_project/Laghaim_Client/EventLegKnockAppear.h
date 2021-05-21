#ifndef _EVENT_LEGKNOCK_APPEAR_H_
#define _EVENT_LEGKNOCK_APPEAR_H_

#include "Config.h"

class CCapsyong;
class CBiped;

class CEventLegKnockAppear // 데카렌 보스 레그녹의 등장씬 이벤트.
{
public:
	CEventLegKnockAppear();
	~CEventLegKnockAppear();

	void Init( D3DVECTOR vOldEye, D3DVECTOR vOldAt ); // 호출될때 마다 초기화해야하는 종류의 것들.
	void LoadRes();
	void DeleteRes();

	BOOL Render( DWORD dwEventStartTime );

	CCapsyong *m_pCapSyong;
	CBiped *m_pBiped;

	CRectPlane* m_pFadeEffect; // 페이드 인 페이드 아웃용.
	CRectPlane* m_pNameOutput; // 이름 출력용.

	D3DVECTOR		m_vEye, m_vAt;
	float           m_PosX, m_PosY, m_PosZ, m_dir; // 레그녹의 위치와 바라볼 방향.
	D3DVECTOR		m_vOldEye, m_vOldAt;

	// 카메라 워크를 위한 변수들.
	float m_fCamDist; // 거리 조절용.
	float m_fCamEyeHeight; // 높이 조절용.
	float m_fCamAtHeight; // 높이 조절용.
};

#endif // _EVENT_LEGKNOCK_APPEAR_H_