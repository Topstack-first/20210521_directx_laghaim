#ifndef _BUFFICON_H_
#define _BUFFICON_H_


#include <list>
#include <set>
#include <map>
#include <sstream>
#include "BasicStatic.h"

enum eBuffStates
{
	EBS_UNAFFECTED	= 0,		// 적용안됨
	EBS_AFFECTED	= 1,		// 적용됨
	EBS_AFFECTING	= 2,		// 적용중 (refresh because)
};

enum eBuffTypes
{
	EBT_INCREASE_MAX_HP				= 0, // 최대 생명력 상승
	EBT_INCREASE_MAX_MP				= 1, // 최대 마나 상승
	EBT_INCREASE_MAX_SP				= 2, // 최대 스테미너 상승
	EBT_INCREASE_MAX_EP				= 3, // 최대 전기력 상승

	EBT_INCREASE_ATT				= 4, // 공격력 상승
	EBT_INCREASE_DEF				= 5, // 방어력 상승
	EBT_INCREASE_INCOMMING_EXP		= 6, // 경험치 획득량 상승
	EBT_INCREASE_INCOMMING_LAIM		= 7, // 라임 획득량 상승

	EBT_INCREASE_AE					= 8, // 공격력, 경험치 획득량 상승
	EBT_INCREASE_DE					= 9, // 방어력, 경험치 획득량 상승
	EBT_INCREASE_AD					= 10, // 공격력, 방어력 상승
	EBT_INCREASE_ADE				= 11, // 공격력, 방어력, 경험치 획득량 상승

	EBT_INCREASE_MOMENTS_ATT		= 12, // 순간 공격력 상승
	EBT_INCREASE_CRITICAL_HIT_RATIO	= 13, // 크리티컬 적중률 상승
	EBT_DECREASE_TARGET_MOB_HP		= 14, // 대상 몬스터 체력 하락
	EBT_DECREASE_DAMAGE				= 15, // 피해 데미지 하락

	EBT_INCREASE_SPEED				= 16, // 이동속도 상승
	EBT_INCREASE_QUEST_POINT		= 17, // 파멸의 인형 퀘스트 포인트 획득량 상승
	EBT_INCREASE_STATUS_PER_MEMBER	= 18, // 파티원 수 만큼 능력치 상승 (hp, mp, sp, ep)

	EBT_COUNT,
};

class CAlphaPopUp;

class CBuffIcon
{
private:
	class CBuffInfo
	{
	private:
		eBuffStates affect_state;
		eBuffTypes buff_type;
		int rate_value;
		unsigned int remain_time;			// tooltip으로 표시될 남은 시간

		unsigned int refreshed_time;		// 마지막 리프레쉬된 시간, 동기화 시간
		unsigned int refreshed_remain_time;	// 서버로부터 받은 남은 시간 (값 유지)

	public:
		CBuffInfo()
			: affect_state(EBS_UNAFFECTED), buff_type(EBT_COUNT)
			, rate_value(0), remain_time(0)
			, refreshed_time(timeGetTime()), refreshed_remain_time(0) {}

		CBuffInfo(eBuffStates state, eBuffTypes type, int rate, unsigned int time)
			: affect_state(state), buff_type(type)
			, rate_value(rate), remain_time(time)
			, refreshed_time(timeGetTime()), refreshed_remain_time(time) {}

		unsigned int tick()
		{
			unsigned int curr = timeGetTime();
			unsigned int time = (curr - refreshed_time) / 1000;

			if (refreshed_remain_time < time)
			{
				remain_time = 0;
				return remain_time;
			}

			// 남은 시간(표시될) = refreshed 남은 시간 - 지난시간
			remain_time = refreshed_remain_time - time;
			return remain_time;
		}

		void refresh(unsigned int time)
		{
			refreshed_time = timeGetTime();
			refreshed_remain_time = time;
		}

		std::string getInfo2Text()
		{
			std::stringstream text;
			text << "rate : " << rate_value << " time : " << remain_time;
			return text.str();
		}

		operator const eBuffTypes() const
		{
			return buff_type;
		}

		bool operator==(const CBuffInfo& other) const
		{
			return affect_state == other.affect_state && buff_type == other.buff_type
				   && rate_value == other.rate_value;
		}
	};

	class CBuff
	{
	private:
		CBuffInfo* buffinfo;
		eBuffTypes icontype;

		bool visible;

	public:
		CBuff()
			: buffinfo(NULL), icontype(EBT_COUNT), visible(false) {}

		CBuff(CBuffInfo* buff)
			: buffinfo(buff), icontype(*buff), visible(true) {}

		CBuff(CBuffInfo* buff, eBuffTypes type)
			: buffinfo(buff), icontype(type), visible(true) {}

		void tick() const
		{
			if (buffinfo)
			{
				unsigned int t = buffinfo->tick();

				if ((t <= 10) && (t % 2 == 0))
					((CBuff*)this)->visible = false;
				else
					((CBuff*)this)->visible = true;
			}
		}

		void refresh(unsigned int time)
		{
			if (!buffinfo)
				return;

			buffinfo->refresh(time);
		}

		bool isVisible() const
		{
			return visible;
		}

		std::string getInfo2Text() const
		{
			if (!buffinfo)
				return std::string("");

			return buffinfo->getInfo2Text();
		}

		operator const eBuffTypes() const
		{
			return icontype;
		}

		bool operator<(const CBuff& other) const
		{
			return icontype < other.icontype;
		}

		bool operator==(const CBuff& other) const
		{
			return icontype == other.icontype && buffinfo == other.buffinfo;
		}

		bool operator==(const CBuffInfo& info) const
		{
			return *buffinfo == info;
		}
	};

	class lessType
	{
	public:
		bool operator()(const CBuff& b1, const CBuff& b2) const
		{
			return b1 < b2;
		}
	};

	class eraseBuff : public std::binary_function<CBuff, CBuffInfo, bool>
	{
	public:
		bool operator()(const CBuff& buff, const CBuffInfo& info) const
		{
			return buff == info;
		}
		bool operator()(CBuff& buff, CBuffInfo& info)
		{
			return buff == info;
		}
	};

	typedef std::map<int, CBasicStaticEx> ImageMap;
	typedef std::map<eBuffTypes, CBuffInfo> BuffList;
	typedef std::multiset<CBuff, lessType> IconSet;

	ImageMap m_ImageMap;
	BuffList m_BuffList;
	IconSet m_IconSet;
	CAlphaPopUp* m_pPopup;

public:
	CBuffIcon();
	~CBuffIcon();

	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();

	void addBuff(eBuffTypes type, int rate, unsigned int time);
	void refreshBuff(eBuffTypes type, unsigned int time);
	void removeBuff(eBuffTypes type);
	void removeAll();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void _addBuffIcon(CBuffInfo& buff);
};

#endif // _BUFFICON_H_