#ifndef _LETTER_MANAGER_H_
#define _LETTER_MANAGER_H_

#include "singleton.h"
#include "info_define.h"

#define LETTER_TYPE_MAX	3

struct LetterListData
{
	int index;
	int state;
	std::string name;
	std::string title;
	int send_time;
	int keep_end_time;
};

struct LetterContent
{
	int index;
	bool hasContent;
	std::string name;
	std::string title;
	std::string content;
};

class LetterManager : public CSingleton<LetterManager>
{
public:
	typedef std::vector<LetterListData> VEC_LETTER_LIST;
	typedef std::map<int, LetterContent> MAP_LETTER_CONTENT;
public:
	LetterManager();
	~LetterManager();

public:
	void Clear();

	void SetCurPage(const int nCurPage);
	const int GetCurPage();
	void SetMaxPage(const int nMaxPage);
	const int GetMaxPage();

	void ClearList();
	void AddListData(int type, const NoteMessage& data);
	const VEC_LETTER_LIST& GetList();

	const int GetLetterIndex(int arrIdx);

	void UpdateContent(int type, int index, const std::string& content);
	LetterContent GetContent(int type, int index);

	LetterContent GetContentByArrayIndex(int type, int arrIdx);

private:
	int					m_nCurPage;
	int					m_nMaxPage;

	VEC_LETTER_LIST		m_vecList;
	MAP_LETTER_CONTENT	m_mapContent[LETTER_TYPE_MAX];
};

#endif // _LETTER_MANAGER_H_
