#ifndef _FX_COMMON_LOLO_H_
#define _FX_COMMON_LOLO_H_

class CLolos;
class CFxCommonLolo
{
public:
	CFxCommonLolo();
	~CFxCommonLolo();

public:
	void SetFilename(const std::string& filename);
	void LoadRes();
	BOOL Render(EffectSort& effect_sort);

private:
	CLolos*		m_pLolo;
	std::string m_filename;
};

#endif // _FX_COMMON_LOLO_H_
