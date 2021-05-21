#ifndef _BINGOMANAGER_H_
#define _BINGOMANAGER_H_

#include <vector>
#include "singleton.h"
#include "info_define.h"

#define BINGO_NUMS_MAX	25

class BingoManager : public CSingleton<BingoManager>
{
public:
	BingoManager();
	~BingoManager();

	void Clear();

public:
	void SetBingoOn(const bool bOn)
	{
		m_isBingoOn = bOn;
	}
	const bool GetBingoOn()
	{
		return m_isBingoOn;
	}

	void SetSuccessCount(const int successCount);
	const int GetSuccessCount();

	void SetPreview(int* previews);
	const int* GetPreview();

	void SetPresentState(int* presentStates);
	const int* GetPresentState();

	void SetBingoNums(const int count, int* bingoNums);
	const bool* GetBingoNums();

private:
	bool m_isBingoOn;
	int m_SuccessCount;
	int m_Previews[BINGO_READY_MAX];
	int m_PresentState[BINGO_PRESENT_MAX];
	bool m_isChecked[BINGO_NUMS_MAX];
};

#endif // _BINGOMANAGER_H_
