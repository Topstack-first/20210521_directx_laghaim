#ifndef _FX_POISON_FOG_H_
#define _FX_POISON_FOG_H_

class CFxPoisonFog // ��ī�� ������ ��� ���̴� ���Ȱ� ����Ʈ. ȭ���� ��ü ���� ������ �� ����Ʈ�̴�. ///�δ�1
{
public:
	CFxPoisonFog();
	~CFxPoisonFog();

	BOOL Render( EffectSort &effect_sort ); // �ְ�� ����� ��ο�.(����� 2D�� ����ӿ� ����. �����ú��� 3D�� �ƴ϶� 2D��ǥ��.)
};

#endif // _FX_POISON_FOG_H_