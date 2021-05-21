#include "stdafx.h"
#include "LHOverInfo.h"
#include "main.h"
#include "Nk2DFrame.h"
#include "ddutil.h"





//#include "AlphaPopUp.h"

LHOverInfo::LHOverInfo(void)
	: m_current_type(Over_Info_Type__Item)
{
}

LHOverInfo::~LHOverInfo(void)
{
}

void LHOverInfo::Init()
{

}

void LHOverInfo::LoadRes()
{

}

void LHOverInfo::DeleteRes()
{

}

void LHOverInfo::SetInfo(Over_Info_Type type, int index)
{

}

void LHOverInfo::Draw(int x, int y)
{
	if( m_current_type == Over_Info_Type__None )
		return;

	switch( m_current_type )
	{
	case Over_Info_Type__Item:
		DrawItemInfo(x,y);
		break;
	case Over_Info_Type__Skill:
		DrawSkillInfo(x,y);
		break;
	case Over_Info_Type__Etc:
		DrawEtcInfo(x,y);
		break;
	}
}

void LHOverInfo::DrawItemInfo(int x, int y)
{

}

void LHOverInfo::DrawSkillInfo(int x, int y)
{
}

void LHOverInfo::DrawEtcInfo(int x, int y)
{
}