#ifndef _EVENT_LEGKNOCK_APPEAR_H_
#define _EVENT_LEGKNOCK_APPEAR_H_

#include "Config.h"

class CCapsyong;
class CBiped;

class CEventLegKnockAppear // ��ī�� ���� ���׳��� ����� �̺�Ʈ.
{
public:
	CEventLegKnockAppear();
	~CEventLegKnockAppear();

	void Init( D3DVECTOR vOldEye, D3DVECTOR vOldAt ); // ȣ��ɶ� ���� �ʱ�ȭ�ؾ��ϴ� ������ �͵�.
	void LoadRes();
	void DeleteRes();

	BOOL Render( DWORD dwEventStartTime );

	CCapsyong *m_pCapSyong;
	CBiped *m_pBiped;

	CRectPlane* m_pFadeEffect; // ���̵� �� ���̵� �ƿ���.
	CRectPlane* m_pNameOutput; // �̸� ��¿�.

	D3DVECTOR		m_vEye, m_vAt;
	float           m_PosX, m_PosY, m_PosZ, m_dir; // ���׳��� ��ġ�� �ٶ� ����.
	D3DVECTOR		m_vOldEye, m_vOldAt;

	// ī�޶� ��ũ�� ���� ������.
	float m_fCamDist; // �Ÿ� ������.
	float m_fCamEyeHeight; // ���� ������.
	float m_fCamAtHeight; // ���� ������.
};

#endif // _EVENT_LEGKNOCK_APPEAR_H_