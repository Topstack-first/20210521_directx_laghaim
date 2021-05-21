#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include "ControlLotto.h"
#include "Nk2DFrame.h"
#include "ControlBottom.h"
#include "g_stringmanager.h"
#include "CommonConfig.h"
#include "PacketSend.h"

int posX_input[MAX_LOTTO_INPUT] = {76, 157, 237, 76, 157, 237};
int posY_input[MAX_LOTTO_INPUT] = {90, 90, 90, 188, 188, 188};

ControlLotto::ControlLotto()
	: m_curState(eLottoState_Idle)
	, m_timeAccum(0)
	, m_curPage(0)
	, m_maxPage(1)
	, m_stepRandom(0)
{
	m_time = GetTickCount();
}

ControlLotto::~ControlLotto()
{
	DeleteRes();
}

HRESULT ControlLotto::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlLotto::LoadRes()
{
	if( LottoManager::GetInstance()->GetLottoOn() == false )
	{
		return;
	}

	m_background.Init("lotto/LOTTO_BOARD");
	m_btnRegist.Init("lotto/BTN_SIGN");
	m_btnAuto.Init("lotto/BTN_AUTOSIGN");
	m_btnPageLeft.Init("lotto/BTN_PAGE_L");
	m_btnPageRight.Init("lotto/BTN_PAGE_R");
	
	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btnRegist.SetPosition(bgx + 54, bgy + 276);
	m_btnAuto.SetPosition(bgx + 173, bgy + 276);
	m_btnPageLeft.SetPosition(bgx + 336, bgy + 183);
	m_btnPageRight.SetPosition(bgx + 408, bgy + 183);

	DWORD textColor = RGB(255, 255, 255);
	m_textRegistPage.Init(12, textColor, bgx + 382, bgy + 189, 20, 21, TRUE);
	m_textRegistPage.SetString("1");
	m_textItemNeedCount.Init(12, textColor, bgx + 382, bgy + 276, 20, 21, TRUE);
	m_textItemNeedCount.SetString("5");
	m_textItemHaveCount.Init(12, textColor, bgx + 382, bgy + 321, 20, 21, TRUE);
	m_textItemHaveCount.SetString("0");

	for(int i = 0; i < MAX_LOTTO_REGIST_LIST; i++)
	{
		m_textRegistList[i].Init(12, textColor, bgx + 382, bgy + 80 + (i * 19), 20, 21, TRUE);		
	}
	
	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		m_tbInput[i].Init(bgx + posX_input[i], bgy + posY_input[i], 3, RGB(0,0,0), RGB(255, 255, 255), 0, NORMAL);
		m_tbInput[i].SetfontSize();
	}
}

void ControlLotto::DeleteRes()
{
	if( LottoManager::GetInstance()->GetLottoOn() == false )
	{
		return;
	}

	m_background.DeleteRes();
	m_btnRegist.DeleteRes();
	m_btnAuto.DeleteRes();
	m_btnPageLeft.DeleteRes();
	m_btnPageRight.DeleteRes();	
	m_textRegistPage.DeleteRes();
	m_textItemNeedCount.DeleteRes();
	m_textItemHaveCount.DeleteRes();

	for(int i = 0; i < MAX_LOTTO_REGIST_LIST; i++)
	{
		m_textRegistList[i].DeleteRes();
	}

	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		m_tbInput[i].DeleteRes();
	}
}

float ControlLotto::Update()
{
	DWORD timeCur = GetTickCount();
	DWORD timeElapsed = timeCur - m_time;
	m_time = timeCur;

	float fElapsed = (float)(timeElapsed) / 1000.0f;
	m_timeAccum += fElapsed;

	LottoManager* lottoMgr = LottoManager::GetInstance();
	int cntTicket = lottoMgr->GetTicketCount();
	m_textItemHaveCount.SetString(cntTicket);

	switch(m_curState)
	{
	case eLottoState_Idle:		UpdateIdle(fElapsed);	break;
	case eLottoState_Auto:		UpdateAuto(fElapsed);	break;
	case eLottoState_Regist:	UpdateRegist(fElapsed);	break;
	default:	break;
	}

	return 0;
}

void ControlLotto::UpdateIdle(float elapsedTime)
{
	AlignInputs();
}

void ControlLotto::UpdateAuto(float elapsedTime)
{
	AlignInputs();

	float time = CommonConfig::Instance()->GetLottoAutoTime();	

	if( m_timeAccum > time )
	{
		if( m_stepRandom >= MAX_LOTTO_INPUT )
		{			
			if( CheckAndSend() == false )
			{
				ButtonEnable(true);
				SetState((int)eLottoState_Idle);
				return;
			}

			SetState((int)eLottoState_Regist);
			return;
		}

		m_timeAccum = 0;
		
		int num = MakeRandom(m_stepRandom);
		char bufNum[MAX_PATH] = {0};
		itoa(num, bufNum, 10);
		m_tbInput[m_stepRandom].SetText(bufNum);
		
		++m_stepRandom;
	}
}

void ControlLotto::UpdateRegist(float elapsedTime)
{
}

void ControlLotto::Draw()
{
	if( g_pDisplay == NULL )
		return;

	m_background.Draw();
	m_btnRegist.Draw();
	m_btnAuto.Draw();
	m_btnPageLeft.Draw();
	m_btnPageRight.Draw();
	m_textRegistPage.Draw();
	m_textItemNeedCount.Draw();
	m_textItemHaveCount.Draw();

	for(int i = 0; i < MAX_LOTTO_REGIST_LIST; i++)
	{
		m_textRegistList[i].Draw();
	}

	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		m_tbInput[i].Draw();
	}

	switch(m_curState)
	{
	case eLottoState_Idle:		DrawIdle();		break;
	case eLottoState_Auto:		DrawAuto();		break;
	case eLottoState_Regist:	DrawRegist();	break;
	default:	break;
	}
}

void ControlLotto::DrawIdle()
{
}

void ControlLotto::DrawAuto()
{
}

void ControlLotto::DrawRegist()
{
}

LRESULT ControlLotto::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( m_curState != eLottoState_Idle )
	{
		return 0;
	}

	int x, y;	

	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		if( m_tbInput[i].GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() )
		{
			m_tbInput[i].SetEnable(FALSE);
			pCMyApp->ConvertImmStatus();
		}
	}	

	switch( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_background.IsIn(x, y) == false )
				return 0;

			if( msg == WM_LBUTTONDOWN )
			{
				bool notFail = false;
				for(int i = 0; i < MAX_LOTTO_INPUT; i++)
				{
					int bInputEnable = m_tbInput[i].GetEnable();

					if( E_FAIL != m_tbInput[i].MsgProc(hWnd, msg, wParam, lParam) )
					{
						notFail = true;						
					}

					if( bInputEnable == FALSE && m_tbInput[i].GetEnable() )
					{
						pCMyApp->RestoreImmStatus();						
					}
				}

				if( notFail == true )
				{
					return 1;
				}
			}
			else if( msg == WM_LBUTTONUP )
			{
				for(int i = 0; i < MAX_LOTTO_INPUT; i++)
				{
					m_tbInput[i].MsgProc(hWnd, msg, wParam, lParam);
				}
			}

			m_btnRegist.MsgProc(hWnd, msg, wParam, lParam);
			m_btnAuto.MsgProc(hWnd, msg, wParam, lParam);
			m_btnPageLeft.MsgProc(hWnd, msg, wParam, lParam);
			m_btnPageRight.MsgProc(hWnd, msg, wParam, lParam);

			if( m_btnRegist.GetState() == BTN_ACTION )
			{
				m_btnRegist.SetState(BTN_NORMAL);
				ActionRegist();
			}

			if( m_btnAuto.GetState() == BTN_ACTION )
			{
				m_btnAuto.SetState(BTN_NORMAL);
				ActionAuto();
			}

			if( m_btnPageLeft.GetState() == BTN_ACTION )
			{
				m_btnPageLeft.SetState(BTN_NORMAL);
				ActionListLeft();
			}

			if( m_btnPageRight.GetState() == BTN_ACTION )
			{
				m_btnPageRight.SetState(BTN_NORMAL);
				ActionListRight();
			}

			return 1;
		}
		break;

	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		{
			bool bEnable = false;
			for(int i = 0; i < MAX_LOTTO_INPUT; i++)
			{
				if( m_tbInput[i].GetEnable() )
				{
					m_tbInput[i].MsgProc(hWnd, msg, wParam, lParam);					
				}

				if( m_tbInput[i].GetEnable() )
				{
					bEnable = true;
				}
			}

			if( bEnable == true )
			{
				return 1;
			}
		}
		break;
	}
	return 0;
}

void ControlLotto::ActionRegist()
{	
	// tbInput 확인하여 서버에 전달
	ButtonEnable(false);
	SetState((int)eLottoState_Regist);
	if( CheckAndSend() == false )
	{
		ButtonEnable(true);
		SetState((int)eLottoState_Idle);
		return;
	}
}

void ControlLotto::ActionAuto()
{
	ButtonEnable(false);
	SetState((int)eLottoState_Auto);

	ClearNums();
	m_stepRandom = 0;
}

void ControlLotto::ActionListLeft()
{	
	if( m_curPage <= 0 )
		return;

	int page = m_curPage;
	--page;

	ButtonEnable(false);

	SendLottoList(page);
}

void ControlLotto::ActionListRight()
{
	if( m_maxPage <= (m_curPage + 1) )
		return;

	int page = m_curPage;
	++page;

	ButtonEnable(false);

	SendLottoList(page);
}

void ControlLotto::SetState(int state)
{
	m_curState = (eLottoState)state;
	m_timeAccum = 0;
}

void ControlLotto::ButtonEnable(bool enable)
{
	bool dis = ! enable;

	m_btnRegist.SetDisable(dis);
	m_btnAuto.SetDisable(dis);
	m_btnPageLeft.SetDisable(dis);
	m_btnPageRight.SetDisable(dis);
}

bool ControlLotto::IsFocus()
{
	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		if( m_tbInput[i].GetEnable() != 0 )
		{
			return true;
		}
	}

	return false;
}

void ControlLotto::ClearNums()
{
	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		m_tbInput[i].ClearText();
		m_tbInput[i].SetText("\0");		
	}
}

int ControlLotto::MakeRandom(const int step)
{
	boost::mt19937 rand_engine_int(time(0));
	boost::uniform_int<> rand_int(1, 45);
	boost::variate_generator<boost::mt19937, boost::uniform_int<>> rand_gen_int(rand_engine_int, rand_int);

	std::string strNum;
	int oldnum = 0;
	int num = 0;
	bool isDuplicate = false;
	do 
	{
		isDuplicate = false;		
		num = rand_gen_int();

		for(int i = 0; i < step; i++)
		{
			strNum = m_tbInput[i].GetString();			

			// 숫자가 아닌 문자인 경우
			try
			{
				oldnum = boost::lexical_cast<int>(strNum);
			}
			catch(boost::bad_lexical_cast)
			{
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LOTTO_ERR_WRONGNUM), TYPE_NOR_OK, 1);
				return num;
			}

			if( num == oldnum )
			{
				isDuplicate = true;				
			}
		}

	} while (isDuplicate == true);
	

	return num;
}

bool ControlLotto::CheckAndSend()
{
	std::string strNum[MAX_LOTTO_INPUT];
	int num[MAX_LOTTO_INPUT] = {0};

	// 아이템이 없는 경우
	LottoManager* lottoMgr = LottoManager::GetInstance();
	int cntTicket = lottoMgr->GetTicketCount();
	if( cntTicket < 5 )
	{
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LOTTO_ERR_NOITEM), TYPE_NOR_OK, 1);
		return false;
	}
	
	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		// 숫자가 비어있는 경우
		strNum[i] = m_tbInput[i].GetString();
		if( strNum[i].length() < 1 )
		{
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LOTTO_ERR_WRONGNUM), TYPE_NOR_OK, 1);
			return false;
		}

		// 숫자가 아닌 문자인 경우
		try
		{
			num[i] = boost::lexical_cast<int>(strNum[i]);
		}
		catch(boost::bad_lexical_cast)
		{
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LOTTO_ERR_WRONGNUM), TYPE_NOR_OK, 1);
			return false;
		}

		// 1~45 가 아닌 경우
		if( num[i] < 1 || num[i] > 45 )
		{
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LOTTO_ERR_RANGE), TYPE_NOR_OK, 1);
			return false;
		}
	}	

	// 중복된 숫자가 있는 경우
	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		for(int j = i + 1; j < MAX_LOTTO_INPUT; j++)
		{
			if( num[i] == num[j] )
			{
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LOTTO_ERR_DUPLICATE), TYPE_NOR_OK, 1);
				return false;
			}
		}
	}

	std::vector<int> numbers(num, num + (sizeof(num) / sizeof(num[0])));

	std::sort(numbers.begin(), numbers.end());

	SendLotto(numbers);

	return true;
}

void ControlLotto::AlignInputs()
{
	int bgx = m_background.m_x;
	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		std::string str = m_tbInput[i].GetString();
		if( str.length() > 1 )
		{
			m_tbInput[i].m_X = bgx + posX_input[i] - 4;
		}
		else
		{
			m_tbInput[i].m_X = bgx + posX_input[i];
		}
	}
}

void ControlLotto::RefreshListReq(const int page)
{
	int sendPage = page == -1 ? m_curPage : page;
	ButtonEnable(false);
	SetState((int)eLottoState_Refresh);
	SendLottoList(sendPage);
}

void ControlLotto::RefreshList()
{
	for(int i = 0; i < MAX_LOTTO_REGIST_LIST; i++)
	{
		m_textRegistList[i].SetString("");
	}

	LottoManager* lottoMgr = LottoManager::GetInstance();
	int count = lottoMgr->GetListCount();
	bool bFail = false;
	LottoPack pack;
	for(int i = 0; i < count; i++)
	{
		pack = lottoMgr->GetPack(i, bFail);

		if( bFail == true )
		{
			return;
		}

		char buf[MAX_PATH] = {0};
		sprintf(buf, "%d,%d,%d,%d,%d,%d", pack.num[0], pack.num[1], pack.num[2], pack.num[3], pack.num[4], pack.num[5]);
		m_textRegistList[i].SetString(buf);
	}

	ButtonEnable(true);
	SetState((int)eLottoState_Idle);
	ClearNums();
}

void ControlLotto::SetCurPage(const int curPage)
{
	m_curPage = curPage;
	m_textRegistPage.SetString(curPage + 1);
}

void ControlLotto::SetMaxPage(const int maxPage)
{
	m_maxPage = maxPage;
}
