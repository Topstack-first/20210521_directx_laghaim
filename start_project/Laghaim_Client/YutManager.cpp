#include "stdafx.h"
#include "YutManager.h"
#include "LHExcelReader.h"

const int YUT_ROUTE_MAX = 42;
const int YUT_MOVE_MAX = 6;
const int SUCCESS_POSTION = -1;

const int YUT_ROUTE[YUT_ROUTE_MAX][YUT_MOVE_MAX] =
{
	{1, 2, 3, 4, 5, 1}, // 0
	{2, 3, 4, 5, 6, 30}, // 1
	{3, 4, 5, 6, 7, 1}, // 2
	{4, 5, 6, 7, 8, 2}, // 3
	{5, 6, 7, 8, 9, 3}, // 4
	{21, 22, 23, 26, 27, 4}, // 5
	{7, 8, 9, 10, 11, 5}, // 6
	{8, 9, 10, 11, 12, 6}, // 7
	{9, 10, 11, 12, 13, 7}, // 8
	{10, 11, 12, 13, 14, 8}, // 9
	{24, 25, 31, 32, 33, 9}, // 10
	{12, 13, 14, 15, 16, 10}, // 11
	{13, 14, 15, 16, 17, 11}, // 12
	{14, 15, 16, 17, 18, 12}, // 13
	{15, 16, 17, 18, 19, 13}, // 14
	{16, 17, 18, 19, 20, 14}, // 15
	{17, 18, 19, 20, SUCCESS_POSTION, 15}, // 16
	{18, 19, 20, SUCCESS_POSTION, SUCCESS_POSTION, 16}, // 17
	{19, 20, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 17}, // 18
	{20, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 18}, // 19
	{SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 19}, // 20
	{22, 23, 26, 27, 35, 5}, // 21
	{23, 26, 27, 35, 36, 21}, // 22
	{28, 29, 41, SUCCESS_POSTION, SUCCESS_POSTION, 22}, // 23
	{25, 31, 32, 33, 34, 10}, // 24
	{31, 32, 33, 34, SUCCESS_POSTION, 24}, // 25
	{27, 35, 36, 37, 38, 23}, // 26
	{35, 36, 37, 38, 39, 26}, // 27
	{29, 41, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 23}, // 28
	{41, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 28}, // 29
	{SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 1}, // 30
	{32, 33, 34, SUCCESS_POSTION, SUCCESS_POSTION, 25}, // 31
	{33, 34, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 31}, // 32
	{33, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 32}, // 33
	{SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 33}, // 34
	{36, 37, 38, 39, 40, 27}, // 35
	{37, 38, 39, 40, SUCCESS_POSTION, 35}, // 36
	{38, 39, 40, SUCCESS_POSTION, SUCCESS_POSTION, 36}, // 37
	{39, 40, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 37}, // 38
	{40, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 38}, // 39
	{SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 39}, // 40
	{SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, SUCCESS_POSTION, 29} // 41
};

YutManager::YutManager()
	: m_isYutnoriOn(false)
	, m_completeCount(0)
	, m_unitPosIndex(-1)
{
	load();
}

YutManager::~YutManager()
{
}

void YutManager::Clear()
{
	m_completeCount = 0;
	m_unitPosIndex = -1;
}

void YutManager::load()
{
	LHExcelReader er;
	if( er.Open("data/YUT_POS.TXT") == false )
		return;

	POINT pos;
	int col = 0;
	int rowCount = er.GetRowSize();
	for(int i = 0; i < rowCount; i++)
	{
		col = 0;
		pos.x = er.GetInt(i, col++);
		pos.y = er.GetInt(i, col++);

		m_mapPos.insert(std::make_pair(i, pos));
	}
}

POINT YutManager::GetUIPos(int index)
{
	POINT pt;
	pt.x = -1;
	pt.y = -1;

	if( index == -1 )
		index = m_unitPosIndex;

	MAP_POSITION::iterator itFind = m_mapPos.find(index);
	if( itFind == m_mapPos.end() )
		return pt;

	pt = itFind->second;

	return pt;
}

int YutManager::GetRoute(int curIndex, int dice)
{
	int result = -1;

	if( curIndex < 0 || curIndex >= YUT_ROUTE_MAX )
		return result;

	if( dice < 0 || dice >= 6 )
		return result;

	return YUT_ROUTE[curIndex][dice];
}
