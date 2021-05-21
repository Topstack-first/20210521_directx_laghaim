#ifndef _LOTTOMANAGER_H_
#define _LOTTOMANAGER_H_

#include "singleton.h"

#define MAX_LOTTO_INPUT			6
#define MAX_LOTTO_REGIST_LIST	5

struct LottoPack
{
	int num[MAX_LOTTO_INPUT];
};

class LottoManager : public CSingleton<LottoManager>
{
public:
	typedef std::vector<LottoPack> VEC_LOTTOPACK;

public:
	LottoManager();
	~LottoManager();

public:
	void Clear();
	void ClearList();

public:
	void SetLottoOn(bool bOn)
	{
		m_isLottoOn = bOn;
	}
	bool GetLottoOn()
	{
		return m_isLottoOn;
	}

	void SetTicketCount(const int count)
	{
		m_cntTicket = count;
	}
	const int GetTicketCount()
	{
		return m_cntTicket;
	}

	void AddList(const std::vector<int>& numbers);
	const int GetListCount();
	LottoPack GetPack(const int index, bool& bFail);

private:
	bool m_isLottoOn;
	int	m_cntTicket;

	VEC_LOTTOPACK	m_vecPack;
};

#endif // _LOTTOMANAGER_H_
