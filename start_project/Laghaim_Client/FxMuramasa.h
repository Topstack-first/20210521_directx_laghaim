#ifndef __FX_MURAMASA_GHOST__
#define __FX_MURAMASA_GHOST__

class CFxMuramasaGhost ///�Ϻ����������׽�Ʈ ������ ���󸶻� �� �ͽ� ����Ʈ.
{
public:
	CFxMuramasaGhost();
	~CFxMuramasaGhost();

	void DeleteRes();
	void LoadRes();

	void FrameMove( EffectSort &effect_sort, int nAddFrame ); // effect_sort�� �� �� ��Ʈ���� �����ȴ�. �� effect_sort�� �����ذ��鼭 ���������.
	BOOL Render( EffectSort &effect_sort );

	CRectPlane* m_pMainObject;    // ������� �� ���� �ִ� Ŭ����.
	CRectPlane* m_pEffect1;
	CLolos    * m_pLolo1;
};


#endif