#include "stdafx.h"
#include "headers.h"
#include "s_grade_control.h"
#include "Country.h"
#include "2DResMgr.h"
#include "TextBoard.h"
#include "s_guild_manager.h"
#include "multilistbox.h"
#include "Tcpipcon.h"

#include "guild_data.h"
#include "g_stringmanager.h"


#include "createcharacterpage.h"

extern DWORD		g_dwClientCountry;




extern DWORD				g_dwLangType;


#define		SAFE_DELETE(p)	{ if(p) { delete (p); (p)=NULL; } }

CGradeChangeControl* CGradeChangeControl::Create(int nBaseX, int nBaseY)
{
	return new CGradeChangeControl(nBaseX, nBaseY);
}

CGradeChangeControl::CGradeChangeControl(int nBaseX, int nBaseY)
{
	m_hWnd = NULL;

	m_BasePosX = nBaseX;
	m_BasePosY = nBaseY;

	for(int i = 0; i < GRADE_TOTAL; i++)
		m_pPiece[i] = NULL;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	Init();
}

CGradeChangeControl::~CGradeChangeControl()
{
	DeleteRes();

	for(int i = 0; i < GRADE_TOTAL; i++)
		SAFE_DELETE(m_pPiece[i]);
}

void CGradeChangeControl::Init()
{
	for(int i = 0; i < CHANGE_TOTAL; i++)
		m_BtChange[i].SetFileName("guild/modify_s");

}

void CGradeChangeControl::LoadRes()
{
	if(!g_pDisplay)
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	for(int i = 0; i < GRADE_TOTAL; i++)
		SAFE_DELETE(m_pPiece[i]);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pPiece[GRADE_TITLE], "interface/guild/guild_grade/admin_grade.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pPiece[GRADE_MASTER], "interface/guild/guild_grade/gra_master_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pPiece[GRADE_ASSISTMASTER], "interface/guild/guild_grade/gra_submaster_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pPiece[GRADE_KNIGHT], "interface/guild/guild_grade/gra_manager_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pPiece[GRADE_MEMBER], "interface/guild/guild_grade/gra_member_bk.bmp");

	for(int i = 0; i < CHANGE_TOTAL; i++)
	{
		m_BtChange[i].LoadRes();

		m_Textbox[i].Init(m_BasePosX, m_BasePosY, 18, RGB(255,255,255), RGB(255, 179, 0), FALSE, MSG_POPUP);
		m_Textbox[i].SetfontSize();
		m_Textbox[i].SetEnable( FALSE);
	}

	SetPosition();
}

void CGradeChangeControl::Restore()
{

}

void CGradeChangeControl::SetPosition()
{
	m_pPiece[GRADE_TITLE]->Xpos = m_BasePosX + (279 - m_pPiece[GRADE_TITLE]->GetWidth())*0.5;
	m_pPiece[GRADE_TITLE]->Ypos = m_BasePosY + 142;

	m_pPiece[GRADE_MASTER]->Xpos = m_BasePosX + 17;
	m_pPiece[GRADE_MASTER]->Ypos = m_BasePosY + 183;

	m_pPiece[GRADE_ASSISTMASTER]->Xpos = m_BasePosX + 17;
	m_pPiece[GRADE_ASSISTMASTER]->Ypos = m_BasePosY + 237;

	m_pPiece[GRADE_KNIGHT]->Xpos = m_BasePosX + 17;
	m_pPiece[GRADE_KNIGHT]->Ypos = m_BasePosY + 291;

	m_pPiece[GRADE_MEMBER]->Xpos = m_BasePosX + 17;
	m_pPiece[GRADE_MEMBER]->Ypos = m_BasePosY + 345;

	//m_BtSave.SetPosition(m_BasePosX + 66, m_BasePosY + 455);
	//m_BtCancel.SetPosition(m_BasePosX + 152, m_BasePosY + 455);

	m_BtChange[CHANGE_MASTER].SetPosition(m_BasePosX + 220, m_BasePosY + 205);
	m_BtChange[CHANGE_ASSISTMASTER].SetPosition(m_BasePosX + 220, m_BasePosY + 259);
	m_BtChange[CHANGE_KNIGHT].SetPosition(m_BasePosX + 220, m_BasePosY + 313);
	m_BtChange[CHANGE_MEMBER].SetPosition(m_BasePosX + 220, m_BasePosY + 367);

	m_Textbox[CHANGE_MASTER].SetPos(m_BasePosX + 66, m_BasePosY + 211);
	m_Textbox[CHANGE_ASSISTMASTER].SetPos(m_BasePosX + 66, m_BasePosY + 266);
	m_Textbox[CHANGE_KNIGHT].SetPos(m_BasePosX + 66, m_BasePosY + 320);
	m_Textbox[CHANGE_MEMBER].SetPos(m_BasePosX + 66, m_BasePosY + 372);
}

void CGradeChangeControl::Draw()
{
	if( !g_pDisplay || (m_BasePosX == 0 && m_BasePosY == 0))
		return;

	for(int i = 0; i < GRADE_TOTAL; i++)
	{
		g_pDisplay->Blt(m_pPiece[i]->Xpos, m_pPiece[i]->Ypos, m_pPiece[i]);
	}

	//m_BtSave.Draw();
	//m_BtCancel.Draw();

	for(int i = 0; i < CHANGE_TOTAL; i++)
	{
		m_BtChange[i].Draw();
		m_Textbox[i].Draw();
	}


}

void CGradeChangeControl::DeleteRes()
{
//	m_BtSave.DeleteRes();
//	m_BtCancel.DeleteRes();

	for(int i = 0; i < CHANGE_TOTAL; i++)
	{
		m_BtChange[i].DeleteRes();
		m_Textbox[i].DeleteRes();
	}
}

void CGradeChangeControl::SetGradeName(char* pszname, int nLevel)
{
	switch(nLevel)
	{
	case 10 :
		m_Textbox[CHANGE_MASTER].ClearText();
		m_Textbox[CHANGE_MASTER].SetText(pszname);
		break;
	case 9  :
		m_Textbox[CHANGE_ASSISTMASTER].ClearText();
		m_Textbox[CHANGE_ASSISTMASTER].SetText(pszname);
		break;
	case 7  :
	case 6  :
	case 5  :
	case 4  :
	case 3  :
		m_Textbox[CHANGE_KNIGHT].ClearText();
		m_Textbox[CHANGE_KNIGHT].SetText(pszname);
		break;
	case 1  :
		m_Textbox[CHANGE_MEMBER].ClearText();
		m_Textbox[CHANGE_MEMBER].SetText(pszname);
		break;
	}
}

void CGradeChangeControl::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}


BOOL CGradeChangeControl::CheckSpace(char* String)
{
	int len = strlen(String);
	for(int i=0; i<len; i++)
	{
		if(is_space(String[i]) == TRUE)
			return TRUE;
	}
	return FALSE;
}


bool CGradeChangeControl::CheckGuldGradeName(int grade)
{

	/*	if (strlen(m_Text[job].GetStrBuf()) == 0)
	{	// IDS_GUILD_PLZ_NAME_INPUT "길드명을 입력해주세요."
	g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_PLZ_NAME_INPUT), TYPE_GUILD_CONFIRM );
	return false;
	}
	*/
	if( CheckSpace(m_Textbox[grade].GetStrBuf()) == TRUE )
	{
		// IDS_CANNOT_SPACE "공백문자는 허용하지 않습니다."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CANNOT_SPACE), TYPE_GUILD_CONFIRM );
		return false;
	}


	char *found_sp = NULL;


	switch(g_dwClientCountry)
	{
	case CTRY_KOR://LANG_KOR:
		{
			char str1[]= {"~!@#$%^&*()_-+=|\\<>,./?;:'[]{}"};
			char str2[]=
			{
				"　！＇ㄱ，．／：；？＾＿｀｜￣、。·‥…¨〃­―∥＼∼´～ˇ˘˝˚˙¸˛¡¿ː" // ㄱ
				"＂（）［］｛｝‘’“”〔〕〈〉《》" // ㄴ
				"＋－＜＝＞×∠⌒∂≡≪≫∈⊂∪∧∨￢" // ㄷ
				"＄％￦Ｆ′″Å℉㎕㎖㎗ℓ㎘㏄㎣㎤㎥㎦㎙㎚㎛㎜㎝㎞㎟㎠㎡㎢㏊㎍㎎㎏㏏㎈㎉㏈㎧㎨㎰㎱㎲㎳㎴㎵㎶㎷㎸㎹" // ㄹ
				"㎀㎁㎂㎃㎄㎺㎻㎼㎽㎾㎿㎐㎑㎒㎓㎔㏀㏁㎊㎋㎌㏖㏅㎭㏛㎩㎪㎫㎬㏝㏐㏓㏃㏉㏜㏆"
				"＃＆＊＠§※☆★○●◎◇◆□■△▲▽▼→←↑↓↔〓◁◀▷▶♤♠♡♥♧♣⊙◈▣◐◑▒▤▥▨▧▦▩♨☏☎☜☞¶†‡↕↗↙↖↘♭♩♪♬㉿↗№㏇™㏂㏘℡®ªª"
				"─│┐└├┬┤┴┼━┃┓┗┣┳┫┻╋┠┯┨┷┿┝┰┥┸╂┒┑┖┕┞┟┡┢┦┧┩┪┭┮┱┲┵┶"	// ㄹ
				"┹┺┽┾╀╁╃╄╅╆╇╈╉╊"
				"０１２３４５６７８９ⅰⅱⅲⅳⅴⅵⅶⅷⅸⅹⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩ" // ㅈ
				"ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ"	// ㅍ
				"ΑΒΕΖΗΘΙΚΛΜΝΟΠΡΤΥΧαβεκμνορστυχω"	// ㅎ
				"ĿŊıĳĸŀßŉ"	// ㄲ
				"ぁあぃいいぅうぇえぉおかがきぎくぐけげこごさざしじすせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶ" // ㄸ
				"ぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん"
				"ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブ"	// ㅃ
				"プヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ"
			}; //ㅇ

			char msgStr[100] = "";

			bool b2BbyteWord = false;
			char *gName = m_Textbox[grade].GetStrBuf() ;

			int len1 = strlen(str1);
			int len2 = strlen(str2);
			int glen = strlen(gName);

			int nRlt = 1;
			for( int j=0; j<glen; j++)
			{

				if( gName[j] > 0 && gName[j] < 127 )
				{
					b2BbyteWord = false;
					for( int i=0; i<len1; i++)
					{
						nRlt = strncmp(&gName[j], &str1[i], 1) ;
						if( nRlt == 0)
						{
							char temp[2]= {0,};
							memcpy(&temp, &str1[i], 1 );
							// IDS_GUILD_CREATE_DONT_WORD "길드명에 %'%s%'문자를 허용하지 않습니다."
							sprintf(msgStr, (char*)G_STRING(IDS_GUILD_JOBNAME_FAIL), &temp);
							g_pNk2DFrame->InsertPopup(msgStr, TYPE_GUILD_CONFIRM );
							return false;
						}
					}
				}
				else
				{
					b2BbyteWord = true;
					for( int i=0; i<len2; i+=2)
					{
						nRlt = strncmp(&gName[j], &str2[i], 2) ;
						if( nRlt == 0)
						{
							char temp[3]= {0,};
							memcpy(&temp, &str2[i], 2 );
							// IDS_GUILD_CREATE_DONT_WORD "길드명에 %'%s%'문자를 허용하지 않습니다."
							sprintf(msgStr, (char*)G_STRING(IDS_GUILD_JOBNAME_FAIL), &temp);
							g_pNk2DFrame->InsertPopup(msgStr, TYPE_GUILD_CONFIRM );
							return false;
						}
					}
				}

				if(b2BbyteWord)
					j++;

				b2BbyteWord = false;
			}
		}
		break;

	case CTRY_JPN:
		char str1[]= {"~!@#$%^&*()_+=|\\<>,./?;:'[]{}"};  //  -
		char str2[]=
		{
			"　！＇ㄱ，．／：；？＾＿｀｜￣、。·‥…¨〃∥＼∼´～ˇ˘˝˚˙¸˛¡¿ː" //  ­―
			"＂（）［］｛｝‘’“”〔〕〈〉《》" // ㄴ
			"＋＜＝＞×∠⌒∂≡≪≫∈⊂∪∧∨" // ㄷ   －￢
			"＄％￦Ｆ′″Å℉㎕㎖㎗ℓ㎘㏄㎣㎤㎥㎦㎙㎚㎛㎜㎝㎞㎟㎠㎡㎢㏊㎍㎎㎏㏏㎈㎉㏈㎧㎨㎰㎱㎲㎳㎴㎵㎶㎷㎸㎹" // ㄹ
			"㎀㎁㎂㎃㎄㎺㎻㎼㎽㎾㎿㎐㎑㎒㎓㎔㏀㏁㎊㎋㎌㏖㏅㎭㏛㎩㎪㎫㎬㏝㏐㏓㏃㏉㏜㏆"
			"＃＆＊＠§※☆★○●◎◇◆□■△▲▽▼→←↑↓↔〓◁◀▷▶♤♠♡♥♧♣⊙◈▣◐◑▒▤▥▨▧▦▩♨☏☎☜☞¶†‡↕↗↙↖↘♭♩♪♬㉿↗№㏇™㏂㏘℡®ªª"
			"│┐└├┬┤┴┼━┃┓┗┣┳┫┻╋┠┯┨┷┿┝┰┥┸╂┒┑┖┕┞┟┡┢┦┧┩┪┭┮┱┲┵┶"	// ㄹ  ─
			"┹┺┽┾╀╁╃╄╅╆╇╈╉╊"
			"０１２３４５６７８９ⅰⅱⅲⅳⅴⅵⅶⅷⅸⅹⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩ" // ㅈ
			"ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ"	// ㅍ
			"ΑΒΕΖΗΘΙΚΛΜΝΟΠΡΤΥΧαβεκμνορστυχω"	// ㅎ
			"ĿŊıĳĸŀßŉ"	// ㄲ
			//"ぁあぃいいぅうぇえぉおかがきぎくぐけげこごさざしじすせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶ" // ㄸ
			//"ぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん"
			//"ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブ"	// ㅃ
			//"プヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ"
		}; //ㅇ

		char msgStr[100] = "";

		bool b2BbyteWord = false;
		char *gName = m_Textbox[grade].GetStrBuf() ;

		int len1 = strlen(str1);
		int len2 = strlen(str2);
		int glen = strlen(gName);

		int nRlt = 1;
		for( int j=0; j<glen; j++)
		{

			if( gName[j] > 0 && gName[j] < 127 )
			{
				b2BbyteWord = false;
				for( int i=0; i<len1; i++)
				{
					nRlt = strncmp(&gName[j], &str1[i], 1) ;
					if( nRlt == 0)
					{
						char temp[2]= {0,};
						memcpy(&temp, &str1[i], 1 );
						// IDS_GUILD_CREATE_DONT_WORD "길드명에 %'%s%'문자를 허용하지 않습니다."
						sprintf(msgStr, (char*)G_STRING(IDS_GUILD_JOBNAME_FAIL), &temp);
						g_pNk2DFrame->InsertPopup(msgStr, TYPE_GUILD_CONFIRM );
						return false;
					}
				}
			}
			else
			{
				b2BbyteWord = true;
				for( int i=0; i<len2; i+=2)
				{
					nRlt = strncmp(&gName[j], &str2[i], 2) ;
					if( nRlt == 0)
					{
						char temp[3]= {0,};
						memcpy(&temp, &str2[i], 2 );
						// IDS_GUILD_CREATE_DONT_WORD "길드명에 %'%s%'문자를 허용하지 않습니다."
						sprintf(msgStr, (char*)G_STRING(IDS_GUILD_JOBNAME_FAIL), &temp);
						g_pNk2DFrame->InsertPopup(msgStr, TYPE_GUILD_CONFIRM );
						return false;
					}
				}
			}

			if(b2BbyteWord)
				j++;

			b2BbyteWord = false;
		}

	}


	return true;
}


// 1 일반길원
// 3 길드 부장[하이브리더]
// 4 길드 부장[휴먼]
// 5 길드 부장[에이디아]
// 6 길드 부장[카이립톤]
// 7 길드 부장[불칸]
// 9 길드 부마스터
// 10 길드 마스터

LRESULT CGradeChangeControl::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int res = 0;
	for(int i = 0; i < CHANGE_TOTAL; i++)
	{
		m_BtChange[i].MsgProc(hWnd, msg, wParam, lParam);
		if( m_Textbox[i].MsgProc(hWnd, msg, wParam, lParam) != E_FAIL)
		{
			res = 3;
		}
	}

	if( res == 3 )
		return res;

	switch(msg)
	{
	case WM_LBUTTONUP :
		{
			if(m_BtChange[CHANGE_MASTER].GetState() == BTN_ACTION)
			{
				if( !CheckGuldGradeName(CHANGE_MASTER) ) // 길드 직책 네임 필터링...
				{
					// g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LHSTRING1885), TYPE_GUILD_CONFIRM, 42);
				}
				else
				{
					sprintf(m_MsgBuf, "mg nick 10 %s\n", m_Textbox[CHANGE_MASTER].GetStrBuf());
					SendMsg();
				}
			}
			else if(m_BtChange[CHANGE_ASSISTMASTER].GetState() == BTN_ACTION)
			{
				if( !CheckGuldGradeName(CHANGE_ASSISTMASTER) ) // 길드 직책 네임 필터링...
				{
					// g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LHSTRING1885), TYPE_GUILD_CONFIRM, 42);
				}
				else
				{

					sprintf(m_MsgBuf, "mg nick 9 %s\n", m_Textbox[CHANGE_ASSISTMASTER].GetStrBuf());
					SendMsg();
				}
			}
			else if(m_BtChange[CHANGE_KNIGHT].GetState() == BTN_ACTION)
			{
				if( !CheckGuldGradeName(CHANGE_KNIGHT) ) // 길드 직책 네임 필터링...
				{
					// g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LHSTRING1885), TYPE_GUILD_CONFIRM, 42);
				}
				else
				{

					sprintf(m_MsgBuf, "mg nick 3 %s\n", m_Textbox[CHANGE_KNIGHT].GetStrBuf());
					SendMsg();
				}
			}
			else if(m_BtChange[CHANGE_MEMBER].GetState() == BTN_ACTION)
			{
				if( !CheckGuldGradeName(CHANGE_MEMBER) ) // 길드 직책 네임 필터링...
				{
					// g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LHSTRING1885), TYPE_GUILD_CONFIRM, 42);
				}
				else
				{

					sprintf(m_MsgBuf, "mg nick 1 %s\n", m_Textbox[CHANGE_MEMBER].GetStrBuf());
					SendMsg();
				}
			}
		}
		break;
	}

	return 1;
}


BOOL CGradeChangeControl::IsInside(int x, int y)
{

	return FALSE;
}

BOOL CGradeChangeControl::IsFocus()
{
	for(int i=0; i<CHANGE_TOTAL; i++)
	{
		if( m_Textbox[i].GetEnable() )
			return true;
	}

	return false;
}
