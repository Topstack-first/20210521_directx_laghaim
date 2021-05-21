#ifndef _GUILD_MARK_H_
#define _GUILD_MARK_H_

#include "Config.h"

#define GUILD_MARK_URL "http://mark.laghaim.com/lh_guild_mark/%d/%d_%d.bmp"
#define GUILD_MARK_FILE "data/guildmark/%d_%d.wxy"


#include "ddutil.h"

class CSurface;
class IndexedTexture;
class CGuildMark
{
public:
	static CGuildMark* Create(int index, char* ImgName);
	static CGuildMark* Create();
	~CGuildMark();

public:
	void Set_GuildIndex(CSurface* surface);
	void Set_GuildMark(int index);

	IndexedTexture* Get_GuildMark()
	{
		return m_marksurface;
	}
	int				Get_GuildIndex()
	{
		return m_guildindex;
	}

	void Render(HWND hWnd, float nPosx, float nPosy);

private:
	//CSurface*	m_marksurface;
	IndexedTexture* m_marksurface;
	int				m_guildindex;
	int				m_texturenum;

	D3DTLVERTEX		TLVec[4];

	CGuildMark(int index, char* ImgName);
	//CGuildMark();
};
#endif // _GUILD_MARK_H_