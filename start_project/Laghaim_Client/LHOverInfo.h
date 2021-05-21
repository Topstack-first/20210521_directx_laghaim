#ifndef _LHOverInfo_H_
#define _LHOverInfo_H_
#pragma once

class CAlphaPopUp;

enum Over_Info_Type
{
	Over_Info_Type__None = -1,
	Over_Info_Type__Item,
	Over_Info_Type__Skill,
	Over_Info_Type__Etc,

	Over_Info_Type__EndOfEnum
};

class LHOverInfo
{
public:
//	CAlphaPopUp * m_poup_info;

	Over_Info_Type m_current_type;


public:
	void Init();
	void LoadRes();
	void DeleteRes();
	void SetInfo(Over_Info_Type type, int index);
	void Draw(int x, int y);


public:
	void DrawItemInfo(int x, int y);
	void DrawSkillInfo(int x, int y);
	void DrawEtcInfo(int x, int y);


public:
	LHOverInfo(void);
	virtual ~LHOverInfo(void);
};
#endif // _LHOverInfo_H_