#include "stdafx.h"


#include "headers.h"
#include "main.h"
#include "TextOutBox.h"
#include "ControlRoyale.h"
#include "ddutil.h"
#include "NkCharacter.h"
#include "Nk2DFrame.h"
#include "3d_PosPoint.h"
#include "mouse.h"
#include "uimgr.h"
#include "Land.h"
#include "IndexedTexture.h"
#include "LoadEffectSound.h"
#include "controlparty.h"
#include "tcpipcon.h"
#include "g_stringmanager.h"
// nate 2004 - 7 : Image Manager
#include "ImageDefine.h"

#include "controlbottom.h" ///นฬดฯธสฑๆตๅฟ๘รฃฑE

#include "DmitronBattle.h"




extern BOOL			g_bDungeon;
extern POINT		g_CursorPoint;
extern DWORD		g_dwLangType;
extern DWORD		g_dwClientCountry;
extern BOOL			g_bGhost;
// nate 2005-03-02 : Matrix
extern int			g_MatrixSvr;



extern BOOL g_bUseMatrixCountry; // ธลฦฎธฏฝบ(ธบ๑ฟE?พฦทนณช)ฐก ตตภิตศ ณชถ๓ภฮฐกภว วรทกฑื.(ตตภิ พศตศณชถ๓ดย ธบ๑ฟE?ตฅภฬลอธฆ ฦะฤก พศวุมึฑEถงนฎฟก ภฬฐิ FALSEฟฉพ฿วัดู.) ///วุฟธบ๑ฟE?
extern BOOL		g_bAdmin2;


CControlRoyale::CControlRoyale()
	: m_hWnd(NULL)
{
	// nate 2004 - 5 : ลฉธฎฝบลป ผาภฏภฺ ภงฤก วฅฝร
	m_hWnd = NULL;
	m_pBack			= NULL;	// น้ฑืถ๓ฟ๛?EภฬนฬมE
	curReward = 0;
	m_dwAniTime = 0L;
	m_nAniframe = 0;

//========================================================================

	Init();
}

CControlRoyale::~CControlRoyale()
{
	DeleteRes();
	
}

void CControlRoyale::Init()
{
	

}

void CControlRoyale::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);



	SAFE_DELETE(m_pBack);
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/royale/royale.BMP") ) )
		return;

	m_pBack->SetColorKey( TRANS_COLOR ); 

	
	m_pBack->Xpos = m_ClientRc.right - m_pBack->GetWidth();
	m_pBack->Ypos = 0;


	m_hp.Init( "royale/RY_EXP" );

	int bgx = m_pBack->Xpos;
	int bgy = m_pBack->Ypos;

	//static int a = 30;
	//static int b = 30;

	m_hp.SetPos( bgx + 160 , bgy + 390 );


	m_textout[0].Init( 14 , RGB(255, 0, 0) , bgx + 650 , bgy + 388 );

	m_textout[0].SetString("0%");

	for(int i=0;i<100;i++)
	{
		char filename[200];
		sprintf(filename,"interface/royale/patent/lv_%d.BMP",i);
		g_pDisplay->CreateSurfaceFromBitmap( &m_Icon[i] , filename );
	}

	m_faceICon.SetPos( bgx + 50 , bgy + 95 );
	m_faceICon.Init( "interface/royale/patent/lv_0.BMP" );

	char fnt[30];
	//sprintf(fnt,"Level : %d",g_pRoh->m_bBrLv);
	m_textout[1].Init( 30 , RGB(255, 190, 0) , bgx + 35 , bgy + 510 );
	m_textout[1].SetString("");	

	//sprintf(fnt,"%I64d / %I64d",g_pRoh->m_bpexp,g_pRoh->m_bpmax);
	m_textout[2].Init( 16 , RGB(255, 0, 0) , bgx + 397 , bgy + 370,-1,-1,1 );
	m_textout[2].SetString("");	

	//------Reward

	m_textout[3].Init( 14 , RGB(255, 0, 0) , bgx + 250 , bgy + 420,-1,-1,1 );
	m_textout[3].SetString("Lv.1");	

	m_textout[4].Init( 14 , RGB(255, 0, 0) , bgx + 400 , bgy + 420,-1,-1,1 );
	m_textout[4].SetString("Lv.2");	

	m_textout[5].Init( 14 , RGB(255, 0, 0) , bgx + 550 , bgy + 420,-1,-1,1 );
	m_textout[5].SetString("Lv.3");	

	/*m_textout[6].Init( 14 , RGB(255, 0, 0) , bgx + 229 , bgy + 258,-1,-1,1 );
	m_textout[6].SetString("Lv.4");	*/
	//------------


	m_btnLeft.Init("royale/BTN_L");
	m_btnRight.Init("royale/BTN_R");

	m_btnLeft.SetPosition(bgx + 308, bgy + 530);
	m_btnRight.SetPosition(bgx + 447, bgy + 530);

	m_bodytext[0].Init( 12 , RGB(255, 0, 0) , bgx + 250 , bgy + 450,-1,-1,1 );
	m_bodytext[1].Init( 12 , RGB(255, 0, 0) , bgx + 250 , bgy + 470,-1,-1,1 );
	m_bodytext[2].Init( 12 , RGB(255, 0, 0) , bgx + 250 , bgy + 490,-1,-1,1 );
	m_bodytext[3].Init( 12 , RGB(255, 0, 0) , bgx + 250 , bgy + 510,-1,-1,1 );
	m_bodytext[4].Init( 12 , RGB(255, 0, 0) , bgx + 250 , bgy + 530,-1,-1,1 );

	m_bodytext[5].Init( 12 , RGB(255, 0, 0) , bgx + 400 , bgy + 450,-1,-1,1 );
	m_bodytext[6].Init( 12 , RGB(255, 0, 0) , bgx + 400 , bgy + 470,-1,-1,1 );
	m_bodytext[7].Init( 12 , RGB(255, 0, 0) , bgx + 400 , bgy + 490,-1,-1,1 );
	m_bodytext[8].Init( 12 , RGB(255, 0, 0) , bgx + 400 , bgy + 510,-1,-1,1 );
	m_bodytext[9].Init( 12 , RGB(255, 0, 0) , bgx + 400 , bgy + 530,-1,-1,1 );

	m_bodytext[10].Init( 12 , RGB(255, 0, 0) , bgx + 550 , bgy + 450,-1,-1,1 );
	m_bodytext[11].Init( 12 , RGB(255, 0, 0) , bgx + 550 , bgy + 470,-1,-1,1 );
	m_bodytext[12].Init( 12 , RGB(255, 0, 0) , bgx + 550 , bgy + 490,-1,-1,1 );
	m_bodytext[13].Init( 12 , RGB(255, 0, 0) , bgx + 550 , bgy + 510,-1,-1,1 );
	m_bodytext[14].Init( 12 , RGB(255, 0, 0) , bgx + 550 , bgy + 530,-1,-1,1 );

	for(int ii=0;ii<100;ii++)
	{
		strcpy(rewardbody[ii],G_STRING(4559+ii));
	}
}
void CControlRoyale::Draw_Bar(CBasicStatic* ui , int base_value, int max_value)
{
	double ratio = (double)base_value / max_value;
	if( ratio > 1.0f )
	{
		ratio = 1.0f;
	}
	else if( ratio < 0.0f )
	{
		ratio = 0.0f;
	}

	ui->Draw( (float)ratio , 3 );
}
void CControlRoyale::Draw_Percent( CTextOutBox* textout , __int64 base_value, __int64 max_value)
{
	long double ratio = (long double)base_value / max_value;
	if( ratio > 1.0f )
	{
		ratio = 1.0f;
	}
	else if( ratio < 0.0f )
	{
		ratio = 0.0f;
	}

	char buf[256] = {0};
	sprintf( buf , "%I64d%%" , (__int64)(ratio * 100) );
	textout->SetString( buf );
}


void CControlRoyale::DeleteRes()
{
	//int i;
	SAFE_DELETE(m_pBack);
	m_hp.DeleteRes();
	m_faceICon.DeleteRes();

	m_textout[1].DeleteRes();
	m_textout[2].DeleteRes();
	m_textout[3].DeleteRes();
	m_textout[4].DeleteRes();
	m_textout[5].DeleteRes();
	//m_textout[6].DeleteRes();

	m_bodytext[0].DeleteRes();
	m_bodytext[1].DeleteRes();
	m_bodytext[2].DeleteRes();
	m_bodytext[3].DeleteRes();
	m_bodytext[4].DeleteRes();

	m_bodytext[5].DeleteRes();
	m_bodytext[6].DeleteRes();
	m_bodytext[7].DeleteRes();
	m_bodytext[8].DeleteRes();
	m_bodytext[9].DeleteRes();

	m_bodytext[10].DeleteRes();
	m_bodytext[11].DeleteRes();
	m_bodytext[12].DeleteRes();
	m_bodytext[13].DeleteRes();
	m_bodytext[14].DeleteRes();

	m_bodytext[15].DeleteRes();
	m_bodytext[16].DeleteRes();
	m_bodytext[17].DeleteRes();
	m_bodytext[18].DeleteRes();
	m_bodytext[19].DeleteRes();

	m_btnLeft.DeleteRes();
	m_btnRight.DeleteRes();
}

void CControlRoyale::Draw()
{
	if( !g_pDisplay || !m_pBack )
		return;
	// น่ฐๆภฬนฬมE
	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	Draw_Bar( &m_hp , g_pRoh->m_bpexp, g_pRoh->m_bpmax );
	Draw_Percent( &m_textout[0] , (__int64)g_pRoh->m_bpexp, (__int64)g_pRoh->m_bpmax );
	m_textout[0].Draw();

	//g_pRoh->m_bBrLv
	int classlevel = g_pRoh->m_bBrLv / 2; //2level usando msm patente
	m_faceICon.m_surface = m_Icon[classlevel];
	m_faceICon.m_surface->SetColorKey( TRANS_COLOR );
	m_faceICon.Draw();



	char fnt[30];
	sprintf(fnt,"Lv : %d",g_pRoh->m_bBrLv);
	m_textout[1].SetString(fnt);
	m_textout[1].Draw();


	sprintf(fnt,"%I64d / %I64d",g_pRoh->m_bpexp,g_pRoh->m_bpmax);
	m_textout[2].SetString(fnt);
	m_textout[2].Draw();


	//curReward

	if(curReward <= g_pRoh->m_bBrLv)
	{
		m_textout[3].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_textout[3].m_FontColor = RGB(255,0,0);
	}
	sprintf(fnt,"Lv.%d",curReward);
	m_textout[3].SetString(fnt);
	m_textout[3].Draw();

	if(curReward+1 <= g_pRoh->m_bBrLv)
	{
		m_textout[3].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_textout[3].m_FontColor = RGB(255,0,0);
	}
	sprintf(fnt,"Lv.%d",curReward+1);
	m_textout[4].SetString(fnt);
	m_textout[4].Draw();

	if(curReward+2 <= g_pRoh->m_bBrLv)
	{
		m_textout[3].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_textout[3].m_FontColor = RGB(255,0,0);
	}
	sprintf(fnt,"Lv.%d",curReward+2);
	m_textout[5].SetString(fnt);
	m_textout[5].Draw();

	//sprintf(fnt,"Lv.%d",curReward+3);
	//m_textout[6].SetString(fnt);
	//m_textout[6].Draw();


	m_btnLeft.Draw();
	m_btnRight.Draw();


	//----reward body--

	DrawRewardBodyMessage();

	//-----------------

}

void CControlRoyale::DrawRewardBodyMessage()
{
	char msg[100];
	//curReward
	char msg2[10];
	int lenstr=0;

	if(curReward<100)
	{
	strcpy(msg,rewardbody[curReward]);
	lenstr = strlen(rewardbody[curReward]);
	if(curReward <= g_pRoh->m_bBrLv)
	{
		m_bodytext[0].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[0].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>0 )
	{
		strncpy(msg2,msg,10);
		if(lenstr>=10)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr] = '\0';
		}

		m_bodytext[0].SetString(msg2);
		m_bodytext[0].Draw();
	}
	else
	{
		m_bodytext[0].SetString("");
		m_bodytext[0].Draw();
	}
	if(curReward <= g_pRoh->m_bBrLv)
	{
		m_bodytext[1].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[1].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>10)
	{
		strncpy(msg2,msg+10,10);
		if(lenstr>=20)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[1].SetString(msg2);
		m_bodytext[1].Draw();
	}
	else
	{
		m_bodytext[1].SetString("");
		m_bodytext[1].Draw();
	}
	if(curReward <= g_pRoh->m_bBrLv)
	{
		m_bodytext[2].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[2].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>20)
	{
		strncpy(msg2,msg+20,10);
		if(lenstr>=30)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[2].SetString(msg2);
		m_bodytext[2].Draw();
	}
	else
	{
		m_bodytext[2].SetString("");
		m_bodytext[2].Draw();
	}
	if(curReward <= g_pRoh->m_bBrLv)
	{
		m_bodytext[3].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[3].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>30)
	{
		strncpy(msg2,msg+30,10);
		if(lenstr>=32)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[3].SetString(msg2);
		m_bodytext[3].Draw();
	}
	else
	{
		m_bodytext[3].SetString("");
		m_bodytext[3].Draw();
	}
	if(curReward <= g_pRoh->m_bBrLv)
	{
		m_bodytext[4].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[4].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>40)
	{
		strncpy(msg2,msg+40,10);
		if(lenstr>=40)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[4].SetString(msg2);
		m_bodytext[4].Draw();
	}
	else
	{
		m_bodytext[4].SetString("");
		m_bodytext[4].Draw();
	}
	}
	//m_bodytext[1].Draw();m_bodytext[2].Draw();m_bodytext[3].Draw();m_bodytext[4].Draw();

	/*m_bodytext[5].SetString(msg);
	m_bodytext[6].SetString(msg);
	m_bodytext[7].SetString(msg);
	m_bodytext[8].SetString(msg);
	m_bodytext[9].SetString(msg);
	m_bodytext[5].Draw();m_bodytext[6].Draw();m_bodytext[7].Draw();m_bodytext[8].Draw();m_bodytext[9].Draw();*/

	if(curReward+1 < 100)
	{
	strcpy(msg,rewardbody[curReward+1]);
	lenstr = strlen(rewardbody[curReward+1]);

	if(curReward+1 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[5].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[5].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>0 )
	{
		strncpy(msg2,msg,10);
		if(lenstr>=10)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr] = '\0';
		}

		m_bodytext[5].SetString(msg2);
		m_bodytext[5].Draw();
	}
	else
	{
		m_bodytext[5].SetString("");
		m_bodytext[5].Draw();
	}
	if(curReward+1 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[6].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[6].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>10)
	{
		strncpy(msg2,msg+10,10);
		if(lenstr>=20)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[6].SetString(msg2);
		m_bodytext[6].Draw();
	}
	else
	{
		m_bodytext[6].SetString("");
		m_bodytext[6].Draw();
	}
	if(curReward+1 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[7].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[7].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>20)
	{
		strncpy(msg2,msg+20,10);
		if(lenstr>=30)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[7].SetString(msg2);
		m_bodytext[7].Draw();
	}
	else
	{
		m_bodytext[7].SetString("");
		m_bodytext[7].Draw();
	}
	if(curReward+1 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[8].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[8].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>30)
	{
		strncpy(msg2,msg+30,10);
		if(lenstr>=32)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[8].SetString(msg2);
		m_bodytext[8].Draw();
	}
	else
	{
		m_bodytext[8].SetString("");
		m_bodytext[8].Draw();
	}
	if(curReward+1 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[9].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[9].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>40)
	{
		strncpy(msg2,msg+40,10);
		if(lenstr>=40)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[9].SetString(msg2);
		m_bodytext[9].Draw();
	}
	else
	{
		m_bodytext[9].SetString("");
		m_bodytext[9].Draw();
	}
	}
	/*m_bodytext[10].SetString(msg);
	m_bodytext[11].SetString(msg);
	m_bodytext[12].SetString(msg);
	m_bodytext[13].SetString(msg);
	m_bodytext[14].SetString(msg);
	m_bodytext[10].Draw();m_bodytext[11].Draw();m_bodytext[12].Draw();m_bodytext[13].Draw();m_bodytext[14].Draw();
	*/
	if(curReward+2 <100)
	{
	strcpy(msg,rewardbody[curReward+2]);
	lenstr = strlen(rewardbody[curReward+2]);

	if(curReward+2 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[10].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[10].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>0 )
	{
		strncpy(msg2,msg,10);
		if(lenstr>=10)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr] = '\0';
		}

		m_bodytext[10].SetString(msg2);
		m_bodytext[10].Draw();
	}
	else
	{
		m_bodytext[10].SetString("");
		m_bodytext[10].Draw();
	}
	if(curReward+2 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[11].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[11].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>10)
	{
		strncpy(msg2,msg+10,10);
		if(lenstr>=20)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[11].SetString(msg2);
		m_bodytext[11].Draw();
	}
	else
	{
		m_bodytext[11].SetString("");
		m_bodytext[11].Draw();
	}
	if(curReward+2 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[12].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[12].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>20)
	{
		strncpy(msg2,msg+20,10);
		if(lenstr>=30)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[12].SetString(msg2);
		m_bodytext[12].Draw();
	}
	else
	{
		m_bodytext[12].SetString("");
		m_bodytext[12].Draw();
	}
	if(curReward+2 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[13].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[13].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>30)
	{
		strncpy(msg2,msg+30,10);
		if(lenstr>=32)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[13].SetString(msg2);
		m_bodytext[13].Draw();
	}
	else
	{
		m_bodytext[13].SetString("");
		m_bodytext[13].Draw();
	}
	if(curReward+2 <= g_pRoh->m_bBrLv)
	{
		m_bodytext[14].m_FontColor = RGB(128,128,128);
	}
	else
	{
		m_bodytext[14].m_FontColor = RGB(255,0,0);
	}
	if(lenstr>40)
	{
		strncpy(msg2,msg+40,10);
		if(lenstr>=40)
		{
			msg2[10] = '\0';
		}
		else
		{
			msg2[lenstr % 10] = '\0';
		}
		m_bodytext[14].SetString(msg2);
		m_bodytext[14].Draw();
	}
	else
	{
		m_bodytext[14].SetString("");
		m_bodytext[14].Draw();
	}
	}
	/*m_bodytext[15].SetString(msg);
	m_bodytext[16].SetString(msg);
	m_bodytext[17].SetString(msg);
	m_bodytext[18].SetString(msg);
	m_bodytext[19].SetString(msg);
	m_bodytext[15].Draw();m_bodytext[16].Draw();m_bodytext[17].Draw();m_bodytext[18].Draw();m_bodytext[19].Draw();*/
}

//m_bpexp = 0;
	//m_bpmax = 0;

LRESULT CControlRoyale::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !g_pRoh )
		return 0;

	int x, y;
	int ret;
	int i = 0;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	m_btnLeft.MsgProc(hWnd, msg, wParam, lParam);
	m_btnRight.MsgProc(hWnd, msg, wParam, lParam);


	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		m_eState = MOUSE_CLICK;
		if(IsInside(x, y))
		{
			return 1;
		}
		else
			return 0;

		break;

	case WM_LBUTTONUP:
		if(IsInside(x, y))
		{
		

		
		m_eState		= MOUSE_NONE; // ภฬฐษวุมเพ฿ ผผบฮธสต้พ๛ฬกดยฐิ ฟภภตฟ พศวัดู. ///นฬดฯธสฑๆตๅฟ๘รฃฑEฝรฟก น๖ฑืธฆ น฿ฐ฿วฯฟฉ ผ๖มค

			m_btnLeft.MsgProc( hWnd, msg, wParam, lParam );
			if( m_btnLeft.GetState() == BTN_ACTION )
			{
				if(curReward>0)
				{
					curReward = curReward - 3;
					if(curReward<0)curReward = 0;
				}
				return 1;
			}

			m_btnRight.MsgProc(hWnd, msg, wParam, lParam);
			if( m_btnRight.GetState() == BTN_ACTION )
			{
				if(curReward<100)
				{
					curReward = curReward + 3;
					if(curReward>100)curReward = 100;
				}
				return 1;

			}
			// ฑๆตๅฟEฦ๗ภฮฦฎ ลฌธฏฝร ฑำธปทฮ ฐกฐิ รณธฎ... ///นฬดฯธสฑๆตๅฟ๘รฃฑE
		}
		


		break;

	case WM_RBUTTONDOWN:
		if(IsInside(x, y))
		{
			return 1;
		}
		else
			return 0;

		break;

	case WM_MOUSEMOVE:
		m_eState = MOUSE_MOVE;
		if(IsInside(x, y))
		{
			return 1;
		}
		else
			return 0;


	}
	return 0;
}
BOOL CControlRoyale::IsInside(int x, int y)
{
	if(m_pBack
			&& x >= m_pBack->Xpos
			&& y >= m_pBack->Ypos
			&& y < m_pBack->Ypos + m_pBack->GetHeight() )
		return TRUE;

	return FALSE;
}
