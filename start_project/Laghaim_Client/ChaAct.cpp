#include "stdafx.h"


#include <d3d.h>
#include <assert.h>
#include "headers.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ChaAct.h"
#include <VirtualFile.h>
#include "CapSyong.h"
#include "DropStar.h"
#include "TcpUtil.h"
#include "main.h"

#include "LHPath_Define.h"

#include "LHExcelReader.h"

#define CHA_BPD_DIR "data/Character/bpd/"

CChaAct g_ChaBpdList[MAX_CHA_ACT_NUM];
int g_ChaBpdIndex[NUM_RACE * NUM_SEX][ CHA_ACT_INDEX_NUM ];
unsigned short g_DropStarIndex[DROPSTAR_COUNT*6];

extern void Decode( char* strTemp, int nCode, bool bTap );
extern void Dummy( int nCode, FILE *fp );
extern void PDummy( int nCode, PFILE *fp );

extern enum {SKILL_DTA = 0x70, RACE_SKILL = 0x57, BPDINDEX = 0x86, BPDLIST = 0x23, RACECSY = 0x94};

void LoadChaActList()
{

	int i = 0, count = 0, bpd_dir_len = 0, nRaceCount = 0;
	char fileName[256] = "";
	char bpd_buf[64] = "";
	char snd_buf[64] = "";
	PFILE *fp_info = NULL, *fp_bpd = NULL;
	int nAniCount = 0;

	strcpy(fileName, CHA_BPD_DIR);
	bpd_dir_len = strlen(CHA_BPD_DIR);

	if (g_pDSound)
		g_pDSound->SetWaveFilePath("data/character/wav/");

	{
		LHExcelReader er;

		if( er.Open(LHPATH_BPDLIST_TXT) )
		{
			int row_count = er.GetRowSize();
			for(int row=0 ; row < er.GetRowSize() ; ++row)
			{
				int col=0;
				int index = er.GetInt(row,col++);
				const char * bpdname = er.GetData(row,col++);
				const char * sndname = er.GetData(row,col++);
				int snd_frame = er.GetInt(row,col++);
				int att_frame = er.GetInt(row,col++);
				int ani_count = er.GetInt(row,col++);

				while(1)
				{
					if( ani_count > 200 )
						ani_count = ani_count / 10;
					else
						break;
				}

				if( strcmp(bpdname , "-") )
				{
					CBiped *pBiped = new CBiped;
					char temp_name[256] = {0,};
					sprintf(temp_name , "%s%s" , CHA_BPD_DIR, bpdname);
					if( VF_ERR_NONE > pBiped->Read(temp_name) )
					{
						SAFE_DELETE(pBiped);
						char err[MAX_PATH] = { 0, };
						sprintf( err , "File Not Found : %s\n" , bpdname );
						OutputDebugString( err );
					}
					else
					{
						if( pBiped->m_AnimCount != ani_count )
						{
							char err[MAX_PATH] = { 0, };
							sprintf( err , "[%d] Not Match : %s(bpd:%d,list:%d)\n" , row , bpdname ,pBiped->m_AnimCount,ani_count);
							OutputDebugString( err );
						}

						g_ChaBpdList[row].m_AttackFrame = att_frame;
						g_ChaBpdList[row].m_pBiped = pBiped;
					}
				}
				if (strcmp(sndname, "-"))
				{
					if (g_pDSound)
					{
						g_ChaBpdList[row].m_SoundFrame = snd_frame;
						g_ChaBpdList[row].m_SoundNum = g_pDSound->LoadWAV(sndname);
						_asm nop;
					}
				}
				else
				{

				}
			}
		}

		er.Close();
	}

	{
		LHExcelReader er;
		if( er.Open(LHPATH_BPDINDEX_TXT) )
		{
			for(int row=0 ; row<er.GetRowSize() ; ++row)
			{
				int col = 0;
				col++;
				for( int i=0 ; i<NUM_RACE*NUM_SEX ; ++i )
					g_ChaBpdIndex[i][row] = er.GetInt(row, col++);
			}
		}
	}
	_asm nop;


	for (i = 0; i < DROPSTAR_COUNT; i++)
	{
		g_DropStarIndex[i*6+0] = i*4+0;
		g_DropStarIndex[i*6+1] = i*4+1;
		g_DropStarIndex[i*6+2] = i*4+2;
		g_DropStarIndex[i*6+3] = i*4+1;
		g_DropStarIndex[i*6+4] = i*4+3;
		g_DropStarIndex[i*6+5] = i*4+2;
	}
}

void DeleteChaActList()
{
	for (int i = 0; i < MAX_CHA_ACT_NUM; i++)
		SAFE_DELETE(g_ChaBpdList[i].m_pBiped);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChaAct::CChaAct()
{
	m_pBiped = NULL;
	m_SoundNum = -1;
	m_SoundFrame = -1;
	m_AttackFrame = -1;
}

CChaAct::~CChaAct()
{
	// ???
	SAFE_DELETE(m_pBiped);
}
