#ifndef __SKILL_EXTEND_MANAGER_H__
#define __SKILL_EXTEND_MANAGER_H__

struct SkillData
{
	int race;
	int char_min_level;
	int laim;
	int item_index;
	int point;
	
	int updam_per;
	int attr_rate;
};

class SkillExtendManager
{
public:
	typedef std::pair<int,int> key_t;
	typedef std::map<key_t,SkillData> map_t;
	typedef std::map<int,int> map_include_t;

public:
	SkillExtendManager(void) {}
	~SkillExtendManager(void) {}

	static SkillExtendManager* instnace();

	bool Init(std::string filename = "./data/skillextend_config.json");

public:
	int getData(int skill_idx, int skill_level);
	int	getMoney(int skill_idx, int skill_level);
	bool isExtend(int skill_idx);

private:
	bool loadConfigFromFile();

private:
	std::string	filename_;
	std::string fileBuffer_;

	map_t			skill_extend;
	map_include_t	skill_include;
};
#endif
