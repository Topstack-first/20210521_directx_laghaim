#ifndef _LHRenderTarget_Character_H_
#define _LHRenderTarget_Character_H_

#pragma once
// #include "ddutil.h"
// #include "dxutil.h"
// #include "D3DEnum.h"

#include "ChaAct.h"

class CNkCharacter;

enum EquipPos
{
	EquipPos__None = -1,
	EquipPos__Helmet,
	EquipPos__ARMOR,
	EquipPos__Pants,
	EquipPos__Cloak,
	EquipPos__SHOES,
	EquipPos__Shield,
	EquipPos__Necklace,
	EquipPos__Ring,
	EquipPos__Gloves,
	EquipPos__Weapon,

	EquipPos__EndOfEnum
};


class LHRenderTarget_Character
{
public:
	bool m_initialized;

	int m_w;
	int m_h;


	LPDIRECTDRAWSURFACE7	m_pddsRT;
	LPDIRECTDRAWSURFACE7	m_pddsZBuffer;

	CNkCharacter * m_character;
	int m_race;
	int m_sex;


	EquipPos m_wearing[EquipPos__EndOfEnum];



public:
	void Init(int w, int h, int race, int sex);
	void Reset();
	void Release();


	void InitCharacter(int race, int sex);

public:
	void Equip(int wear, int vnum);
	void UnEquip(int wear, int vnum);


public:
	HRESULT Create(int w=800, int h=600);

	void Invalid();
	void Render(int x, int y);
	void FrameMove();
	void Restore();

//	void RenderCha(int x, int y, int width, int height, CNkCharacter *pNkCha, int page_num, BOOL bSelected = 0);

public:
	LHRenderTarget_Character(void);
	virtual ~LHRenderTarget_Character(void);
};
#endif // _LHRenderTarget_Character_H_