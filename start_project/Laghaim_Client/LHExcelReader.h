#ifndef _LHExcelReader_H_
#define _LHExcelReader_H_

#include <vector>
#include <string>
#include <sstream>

#include "LHFile.h"

class LHExcelReader
{
	typedef std::vector<std::string>			rowVec;
	typedef rowVec::iterator					rowVec_i;
	typedef rowVec::value_type					rowVec_vt;

	typedef std::vector<rowVec>					sheetVec;
	typedef sheetVec::iterator					sheetVec_i;
	typedef sheetVec::value_type				sheetVec_vt;


private:
	bool		m_opened;

	rowVec		m_title;

	sheetVec	m_sheet;



private:
	void Initial();
	void Release();


	void _ReadTitle(LHFile& f);
	void _SkipLine(LHFile& f);
	void _ReadData(LHFile& f);


public:
	bool Open(const TCHAR * filename, LHFileMode mode=LHFileMode__ReadText);
	void Close();


public:
	int		GetColSize()
	{
		return (int)m_title.size();
	}
	int		GetRowSize()
	{
		return (int)m_sheet.size();
	}

	const char * GetData(int row, int col);

	bool	GetBool(int row, int col);
	int		GetInt(int row, int col);
	float	GetFloat(int row, int col);
	DWORD	GetHex(int row, int col);

public:
	void ReadLine(FILE * fp);



public:
	LHExcelReader(void);
	~LHExcelReader(void);
};
#endif // _LHExcelReader_H_