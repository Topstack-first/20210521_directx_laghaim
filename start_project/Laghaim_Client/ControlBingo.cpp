#include "stdafx.h"
#include "ControlBingo.h"
#include <boost/format.hpp>
#include "Nk2DFrame.h"
#include "BingoManager.h"
#include "PacketSend.h"

ControlBingo::ControlBingo()
	: m_curState(eBingoState_Idle)
	, m_timeAccum(0)
	, m_numLast(0)
	, m_isReqRefresh(false)
	, m_surBigNum(NULL)
	, m_surSmallNum(NULL)
	, m_surSeq(NULL)
	, m_surLine(NULL)
	, m_regType(0)
{
	m_time = GetTickCount();
}

ControlBingo::~ControlBingo()
{
	DeleteRes();
}

HRESULT ControlBingo::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlBingo::LoadRes()
{
	if( BingoManager::GetInstance()->GetBingoOn() == false )
	{
		return;
	}

	m_background.Init("BINGO/bingo");
	m_btnStart.Init("BINGO/start_button_01");
	m_btnStartTen.Init("BINGO/start_button_02");
	m_btnClose.Init("common/btn_close_01");
	m_imgBingo.Init("BINGO/bingo_text_01");
	for(int i = 0; i < BINGO_PRESENT_MAX; i++)
	{
		m_btnReward[i].Init("BINGO/present_button_01");
	}

	m_background.Align( 6 , g_pNk2DFrame->GetClientWidth() , g_pNk2DFrame->GetClientHeight() );
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btnClose.SetPosition(bgx + 738 + 15, bgy + 22 - 17);
	m_btnStart.SetPosition(bgx + 564, bgy + 243);
	m_btnStartTen.SetPosition(bgx + 659, bgy + 243);
	m_imgBingo.SetPos(bgx + 175, bgy + 197);
	int posX_BtnPresent = 404;
	int posY_BtnPresent = 335;
	DWORD textColor = RGB(255, 255, 255);
	int present[BINGO_PRESENT_MAX] = {1, 3, 5, 7, 10, 15, 20, 25};
	for(int i = 0; i < BINGO_PRESENT_MAX; i++)
	{
		if( i == 4 )
		{			
			posY_BtnPresent = 385;
		}

		posX_BtnPresent = 404 + ((i % 4) * 77);

		m_btnReward[i].SetPosition(bgx + posX_BtnPresent, bgy + posY_BtnPresent);
		
		m_textReward[i].Init(13, textColor, bgx + posX_BtnPresent + 42, bgy + posY_BtnPresent + 9, 72, 21, TRUE);
		m_textReward2[i].Init(13, textColor, bgx + posX_BtnPresent + 42, bgy + posY_BtnPresent + 9 + 18, 72, 21, TRUE);
		
		std::string s = boost::str(boost::format("%d회") % present[i]);
		m_textReward[i].SetString_withOutLine((char*)s.c_str(), RGB(255, 255, 255), RGB(0, 0, 0));
		m_textReward2[i].SetString_withOutLine("상품받기", RGB(255, 255, 255), RGB(0, 0, 0));
	}

	m_textCompleteCount.Init(12, textColor, bgx + 258, bgy + 402, 110, 21, TRUE);
	m_textCompleteCount.SetString("빙고 완료 0회");
	m_textItemCount.Init(12, textColor, bgx + 140, bgy + 402, 50, 21, TRUE);
	m_textItemCount.SetString("0장");
	
	int index = 0;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			index = (i * 5) + j;
			
			{ // big nums
				m_rcBigNum[index].left = j * 143 + j;
				m_rcBigNum[index].top = i * 143 + i;
				m_rcBigNum[index].right = j * 143 + j + 143;
				m_rcBigNum[index].bottom = i * 143 + i + 143;
			}
			{ // small nums
				m_rcSmallNum[index].left = j * 54 + j;
				m_rcSmallNum[index].top = i * 54 + i;
				m_rcSmallNum[index].right = j * 54 + j + 54;
				m_rcSmallNum[index].bottom = i * 54 + i + 54;
			}
		}
	}

	// seq
	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			index = (i * 6) + j;

			if( index < 0 || index >= BINGO_SEQ_MAX )
				break;

			m_rcSeq[index].left = j * 160 + j + 1;
			m_rcSeq[index].top = i * 160 + i + 1;
			m_rcSeq[index].right = j * 160 + j + 1 + 160;
			m_rcSeq[index].bottom = i * 160 + i + 1 + 160;
		}
	}

	m_rcLineSlash.left = 0;
	m_rcLineSlash.top = 0;
	m_rcLineSlash.right = 270;
	m_rcLineSlash.bottom = 270;

	m_rcLineBackSlash.left = 270;
	m_rcLineBackSlash.top = 0;
	m_rcLineBackSlash.right = 540;
	m_rcLineBackSlash.bottom = 270;

	m_rcLineWidth.left = 0;
	m_rcLineWidth.top = 270;
	m_rcLineWidth.right = 280;
	m_rcLineWidth.bottom = 292;

	m_rcLineHeight.left = 0;
	m_rcLineHeight.top = 292;
	m_rcLineHeight.right = 22;
	m_rcLineHeight.bottom = 562; // 282 + 280;

	m_rcCircle.left = 22;
	m_rcCircle.top = 292;	
	m_rcCircle.right = 76; // 22 + 54;
	m_rcCircle.bottom = 338; // 282 + 56;

	m_rcEraser.left = 76;
	m_rcEraser.top = 292;
	m_rcEraser.right = 281; // 76 + 205;
	m_rcEraser.bottom = 448; // 292 + 156;

	DWORD clrKey = RGB(0, 0, 0);	
	// big num
	g_pDisplay->CreateSurfaceFromBitmap(&m_surBigNum, "INTERFACE/BINGO/number_bic_01.bmp");
	if( m_surBigNum )
	{
		m_surBigNum->SetColorKey(clrKey);
	}	

	// small num
	g_pDisplay->CreateSurfaceFromBitmap(&m_surSmallNum, "INTERFACE/BINGO/number_mini_01.bmp");
	if( m_surSmallNum )
	{
		m_surSmallNum->SetColorKey(clrKey);
	}

	// seq
	g_pDisplay->CreateSurfaceFromBitmap(&m_surSeq, "INTERFACE/BINGO/bingo_seq.bmp");
	if( m_surSeq )
	{
		m_surSeq->SetColorKey(clrKey);
	}

	// line
	g_pDisplay->CreateSurfaceFromBitmap(&m_surLine, "INTERFACE/BINGO/bingo_ac_01.bmp");
	if( m_surLine )
	{
		m_surLine->SetColorKey(clrKey);
	}

	RefreshRewardText();
}

void ControlBingo::DeleteRes()
{
	if( BingoManager::GetInstance()->GetBingoOn() == false )
	{
		return;
	}

	m_background.DeleteRes();
	m_btnStart.DeleteRes();
	m_btnStartTen.DeleteRes();
	m_btnClose.DeleteRes();

	m_imgBingo.DeleteRes();
	m_imgIcon.DeleteRes();

	for(int i = 0; i < BINGO_PRESENT_MAX; i++)
	{
		m_btnReward[i].DeleteRes();	
		m_textReward[i].DeleteRes();
		m_textReward2[i].DeleteRes();
	}

	m_textCompleteCount.DeleteRes();
	m_textItemCount.DeleteRes();
		
	SAFE_DELETE(m_surBigNum);
	SAFE_DELETE(m_surSmallNum);
	SAFE_DELETE(m_surSeq);
	SAFE_DELETE(m_surLine);
}

float ControlBingo::Update()
{
	DWORD timeCur = GetTickCount();
	DWORD timeElapsed = timeCur - m_time;
	m_time = timeCur;

	float fElapsed = (float)(timeElapsed) / 1000.0f;
	m_timeAccum += fElapsed;

	UpdateText();

	switch(m_curState)
	{
	case eBingoState_Idle:		UpdateIdle(fElapsed);	break;
	case eBingoState_Regist:	UpdateRegist(fElapsed);	break;
	case eBingoState_Result:	UpdateResult(fElapsed);	break;
	case eBingoState_Bingo:		UpdateBingo(fElapsed);	break;
	case eBingoState_Erase:		UpdateErase(fElapsed);	break;
	default:	break;
	}

	return 0;
}

void ControlBingo::UpdateText()
{
	BingoManager* bingoMgr = BingoManager::GetInstance();
	std::string strCompleteCount = boost::str(boost::format("빙고 완료 %d회") % bingoMgr->GetSuccessCount());
	m_textCompleteCount.SetString((char*)strCompleteCount.c_str());
	
	const int itemNum = 3801;
	int count = g_pRoh->GetCountInPack(itemNum);
	std::string strItemCount = boost::str(boost::format("%d장") % count);
	m_textItemCount.SetString((char*)strItemCount.c_str());
}

void ControlBingo::UpdateIdle(float elapsedTime)
{
}

void ControlBingo::UpdateRegist(float elapsedTime)
{
	if( m_timeAccum >= 2.0f && m_isReqRefresh == false )
	{
		m_isReqRefresh = true;
		SendBingoReg(m_regType);
	}
}

void ControlBingo::UpdateResult(float elapsedTime)
{
	if( m_timeAccum >= 1.0f )
	{
		SetState(eBingoState_Idle);
	}
}

void ControlBingo::UpdateBingo(float elapsedTime)
{
	if( m_timeAccum >= 1.0f )
	{
		SetState(eBingoState_Erase);
	}
}

void ControlBingo::UpdateErase(float elapsedTime)
{
	if( m_timeAccum >= 3.0f && m_isReqRefresh == false )
	{
		m_isReqRefresh = true;
		SetState(eBingoState_ReqInfo);		
	}
}

void ControlBingo::Draw()
{
	DrawCommon();

	switch(m_curState)
	{
	case eBingoState_Idle:		DrawIdle();		break;
	case eBingoState_Regist:	DrawRegist();	break;
	case eBingoState_Result:	DrawResult();	break;
	case eBingoState_Bingo:		DrawBingo();	break;
	case eBingoState_Erase:		DrawErase();	break;
	default:	break;
	}
}

void ControlBingo::DrawCommon()
{
	if( g_pDisplay == NULL )
		return;

	m_background.Draw();
	m_btnStart.Draw();
	m_btnStartTen.Draw();
	m_btnClose.Draw();
	for(int i = 0; i < BINGO_PRESENT_MAX; i++)
	{
		m_btnReward[i].Draw();
		m_textReward[i].Draw();
		m_textReward2[i].Draw();
	}

	m_textCompleteCount.Draw();
	m_textItemCount.Draw();

	DrawLastNum();
	DrawPreview();
	DrawCircle();
	DrawLine();
	DrawText();
}

void ControlBingo::DrawLastNum()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	int lastNum = m_numLast;
	if( lastNum < 1 || lastNum > BINGO_NUMS_MAX )
	{
		lastNum = 1;
	}

	g_pDisplay->Blt(bgx + 568, bgy + 64, m_surBigNum, &m_rcBigNum[lastNum - 1]);	
}

void ControlBingo::DrawPreview()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	BingoManager* bingoMgr = BingoManager::GetInstance();
	
	int posX[BINGO_READY_MAX] = {395, 456, 395, 456, 395};
	int posY[BINGO_READY_MAX] = {67,  103, 140, 186, 213};

	const int* previews = bingoMgr->GetPreview();
	for(int i = 0; i < BINGO_READY_MAX; i++)
	{
		if( previews[i] < 1 || previews[i] > 25 )
			continue;

		int seqNum = previews[i] - 1;
		if( seqNum < 0 || seqNum >= BINGO_NUMS_MAX )
			continue;

		g_pDisplay->Blt(bgx + posX[i], bgy + posY[i], m_surSmallNum, &m_rcSmallNum[seqNum]);
	}
}

void ControlBingo::DrawCircle()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	BingoManager* bingoMgr = BingoManager::GetInstance();
	const bool* isChecked = bingoMgr->GetBingoNums();

	int index = 0;
	int posX = 0;
	int posY = 0;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			index = (i * 5) + j;

			if( isChecked[index] == false )
				continue;

			posX = 61 + (j * 56);
			posY = 103 + (i * 56);

			g_pDisplay->Blt(bgx + posX, bgy + posY, m_surLine, &m_rcCircle);
		}
	}
}

void ControlBingo::DrawLine()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	BingoManager* bingoMgr = BingoManager::GetInstance();
	const bool* isChecked = bingoMgr->GetBingoNums();
		
	// 가로 줄
	if( isChecked[0] && isChecked[1] && isChecked[2] && isChecked[3] && isChecked[4] )
	{
		g_pDisplay->Blt(bgx + 61, bgy + 120 + (0 * 56), m_surLine, &m_rcLineWidth);
	}

	if( isChecked[5] && isChecked[6] && isChecked[7] && isChecked[8] && isChecked[9] )
	{
		g_pDisplay->Blt(bgx + 61, bgy + 120 + (1 * 56), m_surLine, &m_rcLineWidth);
	}

	if( isChecked[10] && isChecked[11] && isChecked[12] && isChecked[13] && isChecked[14] )
	{
		g_pDisplay->Blt(bgx + 61, bgy + 120 + (2 * 56), m_surLine, &m_rcLineWidth);
	}

	if( isChecked[15] && isChecked[16] && isChecked[17] && isChecked[18] && isChecked[19] )
	{
		g_pDisplay->Blt(bgx + 61, bgy + 120 + (3 * 56), m_surLine, &m_rcLineWidth);
	}

	if( isChecked[20] && isChecked[21] && isChecked[22] && isChecked[23] && isChecked[24] )
	{
		g_pDisplay->Blt(bgx + 61, bgy + 120 + (4 * 56), m_surLine, &m_rcLineWidth);
	}

	// 세로 줄
	if( isChecked[0] && isChecked[5] && isChecked[10] && isChecked[15] && isChecked[20] )
	{
		g_pDisplay->Blt(bgx + 77 + (0 * 56), bgy + 103, m_surLine, &m_rcLineHeight);
	}

	if( isChecked[1] && isChecked[6] && isChecked[11] && isChecked[16] && isChecked[21] )
	{
		g_pDisplay->Blt(bgx + 77 + (1 * 56), bgy + 103, m_surLine, &m_rcLineHeight);
	}

	if( isChecked[2] && isChecked[7] && isChecked[12] && isChecked[17] && isChecked[22] )
	{
		g_pDisplay->Blt(bgx + 77 + (2 * 56), bgy + 103, m_surLine, &m_rcLineHeight);
	}

	if( isChecked[3] && isChecked[8] && isChecked[13] && isChecked[18] && isChecked[23] )
	{
		g_pDisplay->Blt(bgx + 77 + (3 * 56), bgy + 103, m_surLine, &m_rcLineHeight);
	}

	if( isChecked[4] && isChecked[9] && isChecked[14] && isChecked[19] && isChecked[24] )
	{
		g_pDisplay->Blt(bgx + 77 + (4 * 56), bgy + 103, m_surLine, &m_rcLineHeight);
	}

	// 사선
	if( isChecked[0] && isChecked[6] && isChecked[12] && isChecked[18] && isChecked[24] )
	{
		g_pDisplay->Blt(bgx + 67, bgy + 107, m_surLine, &m_rcLineBackSlash);
	}

	if( isChecked[4] && isChecked[8] && isChecked[12] && isChecked[16] && isChecked[20] )
	{
		g_pDisplay->Blt(bgx + 67, bgy + 107, m_surLine, &m_rcLineSlash);
	}
}

void ControlBingo::DrawText()
{
}

void ControlBingo::DrawIdle()
{
}

void ControlBingo::DrawRegist()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;	
	
	float perSec = 0.1f;
	int curFrame = (int)(m_timeAccum / perSec);
	curFrame %= 40;

	g_pDisplay->Blt(bgx + 558, bgy + 53, m_surSeq, &m_rcSeq[curFrame]);	
}

void ControlBingo::DrawResult()
{
}

void ControlBingo::DrawBingo()
{
	m_imgBingo.Draw();
}

void ControlBingo::DrawErase()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	int pointX[4] = {342, 59,  342, 59};
	int pointY[4] = {101, 242, 217, 384};

	float posX = 0.0f;
	float posY = 0.0f;
	
	int gapX = 0;
	int gapY = 0;

	float elapX = 0;
	float elapY = 0;

	float weight = 0;

	if( m_timeAccum < 1.0f )
	{
		weight = m_timeAccum;

		gapX = pointX[0] - pointX[1];
		gapY = pointY[1] - pointY[0];

		elapX = (float)gapX * weight;
		elapY = (float)gapY * weight;

		posX = (float)pointX[0] - elapX;
		posY = (float)pointY[0] + elapY;
	}
	else if( m_timeAccum < 2.0f )
	{
		weight = m_timeAccum - 1.0f;

		gapX = pointX[2] - pointX[1];
		gapY = pointY[2] - pointY[1];

		elapX = (float)gapX * weight;
		elapY = (float)gapY * weight;

		posX = (float)pointX[1] + elapX;
		posY = (float)pointY[1] + elapY;
	}
	else if( m_timeAccum < 3.0f )
	{
		weight = m_timeAccum - 2.0f;

		gapX = pointX[2] - pointX[3];
		gapY = pointY[3] - pointY[2];

		elapX = (float)gapX * weight;
		elapY = (float)gapY * weight;

		posX = (float)pointX[2] - elapX;
		posY = (float)pointY[2] + elapY;
	}
	else
	{
		posX = 59;
		posY = 384;
	}

	g_pDisplay->Blt(bgx + (int)posX, bgy + (int)posY, m_surLine, &m_rcEraser);
}

void ControlBingo::ActionRegist(int type)
{
	const int itemNum = 3801;
	int count = g_pRoh->GetCountInPack(itemNum);
	int needCount = 1;
	
	m_regType = type;

	std::string errMsg = "빙고 이용권이 부족합니다.";

	if( type == eBingoRegType_Many )
	{
		needCount = 10;
		errMsg = "빙고 이용권 10개가 없어 10회 돌리기를 하실 수 없습니다.";
	}
	
	if( count < needCount )
	{
		g_pNk2DFrame->InsertPopup((char*)errMsg.c_str(), TYPE_NOR_OK, 1);
		return;
	}

	SetState(eBingoState_Regist);
	m_btnStart.SetDisable(true);
	m_btnStartTen.SetDisable(true);
}

void ControlBingo::ActionReward(int index)
{
	SetState(eBingoState_ReqReward);
	SendBingoPresent(index);
}

LRESULT ControlBingo::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x, y;

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

			m_btnClose.MsgProc(hWnd, msg, wParam, lParam);
			if( m_btnClose.GetState() == BTN_ACTION )
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(FALSE, BINGO);
				m_btnClose.SetState(BTN_NORMAL);
				return 1;
			}

			if( m_curState != eBingoState_Idle )
			{
				return 1;
			}
			
			m_btnStart.MsgProc(hWnd, msg, wParam, lParam);
			m_btnStartTen.MsgProc(hWnd, msg, wParam, lParam);
			
			for(int i = 0; i < BINGO_PRESENT_MAX; i++)
			{
				m_btnReward[i].MsgProc(hWnd, msg, wParam, lParam);
				if( m_btnReward[i].GetState() == BTN_ACTION )
				{
					m_btnReward[i].SetState(BTN_NORMAL);
					ActionReward(i);
					break;
				}
			}

			if( m_btnStart.GetState() == BTN_ACTION )
			{
				m_btnStart.SetState(BTN_NORMAL);
				ActionRegist(eBingoRegType_One);
			}

			if( m_btnStartTen.GetState() == BTN_ACTION )
			{
				m_btnStartTen.SetState(BTN_NORMAL);
				g_pNk2DFrame->InsertPopup("When turning 10 times, 10LP is consumed. Would you like to use it?", TYPE_NOR_OKCANCLE, POPUP_BINGO_TEN_TIME);				
			}

			return 1;
		}
	}

	return 0;
}

void ControlBingo::SetState(int state)
{
	m_curState = (eBingoState)state;	
	m_timeAccum = 0;
	m_isReqRefresh = false;

	if( m_curState == eBingoState_Idle )
	{
		m_btnStart.SetDisable(false);
		m_btnStartTen.SetDisable(false);
	}
	if( m_curState == eBingoState_ReqInfo )
	{
		SendBingoInfo();
	}
}

eBingoState ControlBingo::GetState()
{
	return m_curState;
}

void ControlBingo::SetLastNum(int num)
{
	m_numLast = num;
}

void ControlBingo::RefreshRewardText()
{
	BingoManager* bingoMgr = BingoManager::GetInstance();

	int present[BINGO_PRESENT_MAX] = {1, 3, 5, 7, 9, 12, 15, 20};
	const int* state = bingoMgr->GetPresentState();
	COLORREF col = RGB(255, 255, 255);
	COLORREF colOut = RGB(0, 0, 0);
	std::string strCount;
	std::string strPresent;
	for(int i = 0; i < BINGO_PRESENT_MAX; i++)
	{
		if( state[i] == eBingoPresentStateType_Not )
		{
			m_btnReward[i].SetDisable(true);			
			strPresent = "상품받기";
			col = RGB(128, 128, 128);
		}
		else if( state[i] == eBingoPresentStateType_Can )
		{
			m_btnReward[i].SetDisable(false);
			strPresent = "상품받기";
			col = RGB(255, 255, 255);
		}
		else if( state[i] == eBingoPresentStateType_Already )
		{
			m_btnReward[i].SetDisable(true);
			strPresent = "지급완료";
			col = RGB(128, 128, 128);
		}

		std::string strCount = boost::str(boost::format("%d회") % present[i]);
		m_textReward[i].SetString_withOutLine((char*)strCount.c_str(), col, colOut);
		m_textReward2[i].SetString_withOutLine((char*)strPresent.c_str(), col, colOut);
	}
}
