#include "stdafx.h"
//-- ȸ������ �ּ�ó��.
/*
 ���� ���� �� �𸣴�
 ������ �� ��

  Ȥ�� �ٽ� ����� �ȴٸ� "ȸ������ �ּ�ó��" ���� �˻�
*/

/*
#include "headers.h"
#include "makeaccpage.h"
#include "sheet.h"
#include "Tcpipcon.h"


extern HINSTANCE g_hDllInst;
extern DWORD	g_dwLangType;
extern DWORD	g_dwClientCountry;
extern char g_strPartner[20];


char *_spChar = "���������������������ޣߣ����������������������������������������������������" // ��
			"�������ۣݣ�����������������������" // ��
			"�������������Сҡӡա�������������" // ��
			"�����ܣơǡȡʢ��������������������������������������������������������������������§çħŧƧǧ�" // ��
			"�ɧʧ˧̧ͧΧϧЧѧҧӧԧէ֧קاڧۧܧݧާߧ��������������"
			"����������" // ��
			"�����������������������������������������������������������¦ŦƦɦʦ˦̦ͦΦϦЦѦҦӦԦզ�"	// ��
			"�צئ٦ڦۦܦݦަߦ�����"
			"������������������������������������������������������������" // ��
			"���£ãģţƣǣȣɣʣˣ̣ͣΣϣУѣңӣԣգ֣ףأ٣ڣ��������������������������������"	// ��
			"���¥ťƥǥȥɥʥ˥̥ͥϥХѥӥԥ֥���������������"	// ��
			"����������������"	// ��
			"�������������������������������������������������������������������ªêĪŪƪǪȪɪʪ˪̪ͪΪϪЪѪҪӪԪժ�" // ��
			"�תت٪ڪ۪ܪݪުߪ��������������������"
			"�������������������������������������������������������������������«ëīūƫǫȫɫʫ˫̫ͫΫϫЫѫҫӫԫի�"	// ��
			"�׫ث٫ګ۫ܫݫޫ߫�������������������������";


char *ExamSpChar2(char *str)
{
	static char found_sp[3];

	int len = strlen(str);
	int splen = strlen(_spChar);

	for (int pos = 0; pos < len; pos++) {
		if ((unsigned char) str[pos] > 127) {
			for (int sp_pos = 0; sp_pos < splen; sp_pos += 2) {
				if (!strncmp(str+pos, _spChar+sp_pos, 2)) {
					found_sp[0] = _spChar[sp_pos];
					found_sp[1] = _spChar[sp_pos+1];
					found_sp[2] = '\0';

					return found_sp;
				}
			}
			pos++;
		}
	}

	return NULL;
}



CMakeAccPage::CMakeAccPage()
{
	Init();
	m_bPartner = FALSE;
}


CMakeAccPage::CMakeAccPage(CSheet *pSheet)
{

	Init();
	SetSheetPtr( pSheet );
}


void CMakeAccPage::SetSheetPtr( CSheet* pSheet )
{
	m_pSheet = pSheet;

}

CMakeAccPage::~CMakeAccPage()
{
	DeleteRes();
	DeleteDLLStringTable();
}


void CMakeAccPage::Init()
{

	ids_cannot_connect = NULL;
	ids_create_acc = NULL;
	ids_sever_test = NULL;
	ids_exist_id = NULL;
	ids_confirm_item = NULL;
	ids_input_name = NULL;
	ids_narrow_name = NULL;
	ids_narrow_id = NULL;
	ids_id_limit = NULL;
	ids_unable_string_id = NULL;
	ids_unable_string_name = NULL;
	ids_input_pass = NULL;
	ids_pass_limit = NULL;
	ids_unable_hangeul = NULL;
	ids_narrow_pass = NULL;
	ids_input_email = NULL;
	ids_narrow_email = NULL;
	ids_input_question = NULL;
	ids_input_answer = NULL;
	ids_input_jumin = NULL;
	ids_narrow_jumin = NULL;
	ids_fault_jumin = NULL;
	ids_enable_number = NULL;


	LoadDLLStringTable();

	int i;
	static int text_size = 12;
	static DWORD back_color = 0x000000;
	static DWORD fore_color = RGB(205, 205, 205);
	m_pDis  = NULL;
	m_pBack = NULL;
	m_pSheet = NULL;

	for(i=0; i<INPUTBOX_CNT; i++)
	{
		m_InputBox[i].Init(0, 0, text_size, back_color, fore_color);

	}
	for(i=0; i<HELP_TEXT_CNT; i++)
	{
		m_pHelpText[i] = NULL;
	}
	m_InputBox[0].SetPos(154, 42); // ����
	//m_InputBox[1].SetPos(154, 70); // ����
	m_InputBox[1].SetPos(154, 98); // ���̵�
	m_InputBox[2].SetPos(154, 126); // ��й�ȣ
	m_InputBox[3].SetPos(154, 154); // ��й�ȣ Ȯ��
	m_InputBox[4].SetPos(47, 209);  // e-mail �ּ�
	m_InputBox[5].SetPos(47, 264);  // ��й�ȣ �нǽ� ����
	m_InputBox[6].SetPos(47, 318);  // ��й�ȣ �нǽ� �亯
	m_InputBox[7].SetPos(74, 372);  // �ֹε�Ϲ�ȣ  ���ڸ�
	m_InputBox[8].SetPos(175, 371);  // �ֹε�Ϲ�ȣ  ���ڸ�

	// for jpn..
	m_InputBox[9].SetPos(68, 369);    // year
	m_InputBox[10].SetPos(160, 369);  // menth
	m_InputBox[11].SetPos(227, 369);  // day


	m_InputBox[0].SetEnable(true); // ó�� Ŀ���� ��ġ
//	m_InputBox[0].SetStringLength(16); // "�̸�" �Է� ����
//	m_InputBox[1].SetStringLength(20); // "���̵�"
//	m_InputBox[2].SetStringLength(20); // "��й�ȣ"
//	m_InputBox[3].SetStringLength(20); // "��й�ȣ Ȯ��"
// ä�� ����Ʈ ����
	m_InputBox[0].SetStringLength(12); // "�̸�" �Է� ����
	m_InputBox[1].SetStringLength(12); // "���̵�"
	m_InputBox[2].SetStringLength(12); // "��й�ȣ"
	m_InputBox[3].SetStringLength(12); // "��й�ȣ Ȯ��"

	m_InputBox[4].SetStringLength(30); // "e-mail"
	m_InputBox[5].SetStringLength(30); // "����"
	m_InputBox[6].SetStringLength(30); // "�亯"
	m_InputBox[7].SetStringLength(6); // "�ֹε�Ϲ�ȣ" ���ڸ�
	m_InputBox[8].SetStringLength(7); // "�ֹε�Ϲ�ȣ" ���ڸ�

	// for jpn..
	m_InputBox[9].SetStringLength(4); // year
	m_InputBox[10].SetStringLength(2); // menth
	m_InputBox[11].SetStringLength(2); // day

	m_InputBox[2].SetPassword_Flg(true);  // ��ȣ�κ��� "*"ǥ�� ǥ���Ѵ�
	m_InputBox[3].SetPassword_Flg(true);

	m_Btn_Ok.SetFileName("btn_confirm");
	m_Btn_Cancle.SetFileName("btn_cancle");
	m_Btn_HomePage.SetFileName("homepage");
	m_Btn_Male.SetFileName("btn_male");
	m_Btn_FeMale.SetFileName("btn_female");
	m_Btn_Male.m_iBtnFlg = BTN_DOWN;



	strcpy(m_SaveData.Sex, "M");	// sex�� ����Ʈ ���� " ����" dlek

	m_Msg.Init(TYPE_NOR_OK);

	//g_dwLangType = 1;

}

void CMakeAccPage::FindPartners()
{
	FILE *fp=NULL;

	if ( (fp = fopen("interface/mp.bmp", "r")) != NULL) {
		strcpy(g_strPartner, "MP");
		m_bPartner = TRUE;
		fclose(fp);
	} else {
		g_strPartner[0] = '\0';
		m_bPartner = FALSE;
	}
}


void CMakeAccPage::LoadRes(CDisplay* pDis, CTcpIpConnection *pTcpIp)
{
	int i;
	char name[256];
	if(!pDis)
		return;

	m_pDis = pDis;
	m_pDis->CreateSurfaceFromBitmap(&m_pBack, "interface/back_mackaccpage.bmp");
	m_pTcpIp = pTcpIp;



	for(i=0; i<INPUTBOX_CNT; i++)
	{
		m_InputBox[i].SetDisp(m_pDis);
	}
	for(i=0; i<HELP_TEXT_CNT; i++)
	{
		sprintf(name, "interface/helptext%d.bmp", i);
		m_pDis->CreateSurfaceFromBitmap(&m_pHelpText[i], name);
		m_pHelpText[i]->SetColorKey(RGB(0, 0, 0));
	}


	m_Btn_Ok.LoadRes(m_pDis);
	m_Btn_Cancle.LoadRes(m_pDis);
	m_Btn_HomePage.LoadRes(m_pDis);
	m_Btn_Male.LoadRes(pDis);
	m_Btn_FeMale.LoadRes(pDis);

	m_Btn_Ok.SetPosition(67, 400);
	m_Btn_Cancle.SetPosition(248, 400);
	m_Btn_HomePage.SetPosition(437, 400);
	m_Btn_Male.SetPosition(149, 70);
	m_Btn_FeMale.SetPosition(220, 70);


	m_Msg.LoadRes(m_pDis);

	Read_URL_Add("explorer.txt");
}

void CMakeAccPage::DeleteRes()
{
	int i;
	SAFE_DELETE(m_pBack);


	for(i=0; i<INPUTBOX_CNT; i++)
	{
		m_InputBox[i].DeleteRes();
	}
	for(i=0; i<HELP_TEXT_CNT; i++)
	{
		SAFE_DELETE(m_pHelpText[i]);
	}



	m_Btn_Ok.DeleteRes();
	m_Btn_Cancle.DeleteRes();
	m_Btn_HomePage.DeleteRes();
	m_Btn_Male.DeleteRes();
	m_Btn_FeMale.DeleteRes();

	m_Msg.DeleteRes();


}

void CMakeAccPage::Draw()
{
	int i;
	m_pDis->Blt(0, 0, m_pBack);




	if(g_dwClientCountry == 1){// for jpn mode.
		m_InputBox[0].Draw();
		m_InputBox[1].Draw();
		m_InputBox[2].Draw();
		m_InputBox[3].Draw();
		m_InputBox[4].Draw();
		m_InputBox[5].Draw();
		m_InputBox[6].Draw();
		m_InputBox[9].Draw();
		m_InputBox[10].Draw();
		m_InputBox[11].Draw();
		if(m_InputBox[0].GetEnable())
				m_pDis->Blt(346, 300, m_pHelpText[0]);
		if(m_InputBox[1].GetEnable())
				m_pDis->Blt(346, 300, m_pHelpText[1]);
		if(m_InputBox[2].GetEnable())
				m_pDis->Blt(346, 300, m_pHelpText[2]);
		if(m_InputBox[3].GetEnable())
				m_pDis->Blt(346, 300, m_pHelpText[3]);
		if(m_InputBox[4].GetEnable())
				m_pDis->Blt(346, 300, m_pHelpText[4]);
		if(m_InputBox[5].GetEnable())
				m_pDis->Blt(346, 300, m_pHelpText[5]);
		if(m_InputBox[6].GetEnable())
				m_pDis->Blt(346, 300, m_pHelpText[6]);
		if(m_InputBox[9].GetEnable() || m_InputBox[10].GetEnable() || m_InputBox[11].GetEnable())
				m_pDis->Blt(346, 300, m_pHelpText[9]);

	}
	else { // for korea mode.

		//m_InputBox[0].SetPos(154, 42); // ����
		//m_InputBox[1].SetPos(154, 70); // ����
		//m_InputBox[1].SetPos(154, 98); // ���̵�
		//m_InputBox[2].SetPos(154, 126); // ��й�ȣ
		//m_InputBox[3].SetPos(154, 154); // ��й�ȣ Ȯ��
		//m_InputBox[4].SetPos(47, 209);  // e-mail �ּ�
		//m_InputBox[5].SetPos(47, 264);  // ��й�ȣ �нǽ� ����
		//m_InputBox[6].SetPos(47, 318);  // ��й�ȣ �нǽ� �亯
		//m_InputBox[7].SetPos(74, 372);  // �ֹε�Ϲ�ȣ  ���ڸ�
		//m_InputBox[8].SetPos(175, 371);  // �ֹε�Ϲ�ȣ  ���ڸ�


		m_InputBox[0].Draw(231, 172, 141);
		m_InputBox[1].Draw(256, 240, 120);
		m_InputBox[2].Draw(256, 275, 120);
		m_InputBox[3].Draw(256, 308, 120);
		m_InputBox[4].Draw(428, 204, 218);
		m_InputBox[5].Draw(428, 274, 218);
		m_InputBox[6].Draw(428, 342, 218);
		m_InputBox[7].Draw(428, 411, 105);
		m_InputBox[8].Draw(540, 411, 105);

		for(i=0; i<INPUTBOX_CNT-3; i++)
		{

			if(m_InputBox[i].GetEnable())
				m_pDis->Blt(158, 352, m_pHelpText[i]);
		}
	}


	m_Btn_Ok.Draw(187, 505);
	m_Btn_Cancle.Draw(323,505);
	m_Btn_HomePage.Draw(479,505);
	m_Btn_Male.Draw(220,199);
	m_Btn_FeMale.Draw(300,199);

	m_Msg.Draw();
}


void CMakeAccPage::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int i;
	int x = LOWORD (lParam);
	int y = HIWORD (lParam);


	// ���� �޼����� ���Դ°�??
	if(m_Msg.CheckMsgExist() == TRUE)
	{

		//if(msg == WM_MOVE)
		//	 return 0;

		int res = m_Msg.MsgProc(hWnd, msg, wParam, lParam);
		if(Receive_Res==1  && res ==1){
			m_Btn_Male.m_iBtnFlg = BTN_DOWN;
			m_Btn_FeMale.m_iBtnFlg = BTN_NORMAL;

			m_pSheet->GoPage(LOGINPAGE);
		}
		return;
	}


	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		for(i=0; i<INPUTBOX_CNT; i++)
		{
			m_InputBox[i].MsgProc( hWnd, msg, wParam, lParam );

		}
		m_Btn_Ok.MsgProc( hWnd, msg, wParam, lParam );
		m_Btn_Cancle.MsgProc( hWnd, msg, wParam, lParam );
		m_Btn_HomePage.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn_Male.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn_FeMale.MsgProc(hWnd, msg, wParam, lParam);


		if(m_InputBox[2].GetEnable())  // �н����� �Է½ÿ��� �ڵ����� �������� ��ȯ�Ѵ�
				SetEngMode(hWnd);
		if(m_InputBox[3].GetEnable())  // �н����� �Է½ÿ��� �ڵ����� �������� ��ȯ�Ѵ�
				SetEngMode(hWnd);

		break;

	case WM_LBUTTONUP:
		m_Btn_Ok.MsgProc( hWnd, msg, wParam, lParam );
		m_Btn_Cancle.MsgProc( hWnd, msg, wParam, lParam );
		m_Btn_HomePage.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn_Male.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn_FeMale.MsgProc(hWnd, msg, wParam, lParam);


		if(m_Btn_Ok.GetState() == BTN_ACTION){
			Action(0);
			m_Btn_Ok.m_iBtnFlg = BTN_NORMAL;
		}
		if(m_Btn_Cancle.GetState() == BTN_ACTION){
			Action(1);
			m_Btn_Cancle.m_iBtnFlg = BTN_NORMAL;
		}
		if(m_Btn_HomePage.GetState() == BTN_ACTION){
			m_Btn_HomePage.m_iBtnFlg = BTN_NORMAL;
			ShellExecute(hWnd, "open" , m_AddRes, NULL, NULL, SW_SHOWNORMAL);
			exit(1);
		}
		if(m_Btn_Male.GetState() == BTN_ACTION)
		{
			m_Btn_Male.m_iBtnFlg = BTN_DOWN;
			m_Btn_FeMale.m_iBtnFlg = BTN_NORMAL;
			strcpy(m_SaveData.Sex, "M");
		}
		if(m_Btn_FeMale.GetState() == BTN_ACTION)
		{
			m_Btn_FeMale.m_iBtnFlg = BTN_DOWN;
			m_Btn_Male.m_iBtnFlg = BTN_NORMAL;
			strcpy(m_SaveData.Sex, "F");
		}



		break;

	case WM_MOUSEMOVE:
		m_Btn_Ok.MsgProc( hWnd, msg, wParam, lParam );
		m_Btn_Cancle.MsgProc( hWnd, msg, wParam, lParam );
		m_Btn_HomePage.MsgProc(hWnd, msg, wParam, lParam);
		break;

	case WM_KEYDOWN:
		if(wParam ==VK_TAB || wParam ==VK_RETURN )
		{
			if(g_dwClientCountry == 1){// for jpn mode.
				for(i=0; i<INPUTBOX_CNT; i++)
				{
					if(m_InputBox[i].GetEnable())
					{
						if(i == INPUTBOX_CNT-1) {
							if(wParam == VK_RETURN)		Action(0);
							else
							{
								m_InputBox[i].SetEnable(false);
								m_InputBox[0].SetEnable(true);
							}
						}
						m_InputBox[i].SetEnable(false);

						if(i == 6) i=i+2;
						m_InputBox[++i].SetEnable(true);
					}
				}

			}
			else{
				for(i=0; i<INPUTBOX_CNT-3; i++)
				{
					if(m_InputBox[i].GetEnable()){
						if(i == INPUTBOX_CNT-4) {
							if(wParam == VK_RETURN){
								Action(0);
							}
							else{
								m_InputBox[i].SetEnable(false);
								m_InputBox[0].SetEnable(true);
							}
							return;
						}
						m_InputBox[i].SetEnable(false);
						m_InputBox[++i].SetEnable(true);
					}
				}
			}
			if(m_InputBox[2].GetEnable())  // �н����� �Է½ÿ��� �ڵ����� �������� ��ȯ�Ѵ�
				SetEngMode(hWnd);
			if(m_InputBox[3].GetEnable())  // �н����� �Է½ÿ��� �ڵ����� �������� ��ȯ�Ѵ�
				SetEngMode(hWnd);
		}

	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		for(i=0; i<INPUTBOX_CNT; i++)
		{
			m_InputBox[i].MsgProc(hWnd, msg, wParam, lParam);
		}
		if(m_InputBox[2].GetEnable())  // �н����� �Է½ÿ��� �ڵ����� �������� ��ȯ�Ѵ�
				SetEngMode(hWnd);
		if(m_InputBox[3].GetEnable())  // �н����� �Է½ÿ��� �ڵ����� �������� ��ȯ�Ѵ�
				SetEngMode(hWnd);
		break;
	}
}


void CMakeAccPage::Action(int act)
{
	int res = 0;

	switch(act)
	{
	case 0:
		if(!Check_All())
			return;

		if(SendMessage())
			Receive_Res = res = ReceiveMessage();
		else {
			m_Msg.PutString(ids_cannot_connect);
			m_InputBox[0].SetEnable(true); // ó�� Ŀ���� ��ġ
			m_Btn_Male.m_iBtnFlg = BTN_DOWN;
			m_Btn_FeMale.m_iBtnFlg = BTN_NORMAL;
			return;
		}

		if (res == 1) {
			m_Msg.PutString(ids_create_acc);
			return;
		} else if (res == -1) {
			m_Msg.PutString(ids_sever_test);
			return;
		} else if (res == -2) {
			m_Msg.PutString(ids_exist_id);
			Clear_OneLine(1);
			m_InputBox[1].SetEnable(true);
			return;
		} else if (res == -3) {
			m_Msg.PutString(ids_confirm_item);			// �̷� ������ ������ �ȵ�.
			m_Btn_Male.m_iBtnFlg = BTN_DOWN;
			m_Btn_FeMale.m_iBtnFlg = BTN_NORMAL;

			m_InputBox[0].SetEnable(true);
			return;
		}
		break;

	case 1:
		m_pSheet->GoPage(LOGINPAGE);
		m_InputBox[0].SetEnable(true); // ó�� Ŀ���� ��ġ
		m_Btn_Male.m_iBtnFlg = BTN_DOWN;
		m_Btn_FeMale.m_iBtnFlg = BTN_NORMAL;

		Clear_All();
		break;
	}
}




void CMakeAccPage::Clear_All()
{
	for(int i=0; i<INPUTBOX_CNT; i++)
		m_InputBox[i].ClearText();
}

void CMakeAccPage::Clear_OneLine(int index)
{
	m_InputBox[index].ClearText();
}

BOOL CMakeAccPage::Check_All()
{
	if(!Check_Name(0)){
		Clear_OneLine(0);
		SetEnableAll(false);
		m_InputBox[0].SetEnable(true); // ó�� Ŀ���� ��ġ
		return false;
	}
	if(!Check_Id(1)){
		Clear_OneLine(1);
		SetEnableAll(false);
		m_InputBox[1].SetEnable(true); // ó�� Ŀ���� ��ġ
		return false;
	}
	if(!Check_Pw(2)){
		Clear_OneLine(2);
		Clear_OneLine(3);
		SetEnableAll(false);
		m_InputBox[2].SetEnable(true);
		return false;
	}
	if(!Check_Pw_Confirm(3)){
		Clear_OneLine(2);
		Clear_OneLine(3);
		SetEnableAll(false);
		m_InputBox[2].SetEnable(true);
		return false;
	}
	if(!Check_Email(4))	{
		Clear_OneLine(4);
		SetEnableAll(false);
		m_InputBox[4].SetEnable(true);
		return false;
	}
	if(!Check_Q(5))	{
		Clear_OneLine(5);
		SetEnableAll(false);
		m_InputBox[5].SetEnable(true); // ó�� Ŀ���� ��ġ
		return false;
	}
	if(!Check_A(6))	{
		Clear_OneLine(6);
		SetEnableAll(false);
		m_InputBox[6].SetEnable(true); // ó�� Ŀ���� ��ġ
		return false;
	}
	if(g_dwClientCountry == 0){
		if(!Check_Iden_Num1(7))	{
			Clear_OneLine(7);
			Clear_OneLine(8);
			SetEnableAll(false);
			m_InputBox[7].SetEnable(true); // ó�� Ŀ���� ��ġ
			return false;
		}
		if(!Check_Iden_Num2(8))	{
			Clear_OneLine(7);
			Clear_OneLine(8);
			SetEnableAll(false);
			m_InputBox[7].SetEnable(true); // ó�� Ŀ���� ��ġ
			return false;
		}
	} else if (g_dwClientCountry == 1) {
		if(!Check_birthday1(9))	{
			Clear_OneLine(9);
			SetEnableAll(false);
			m_InputBox[9].SetEnable(true); // ó�� Ŀ���� ��ġ
			return false;
		}
		if(!Check_birthday2(10))	{
			Clear_OneLine(10);
			SetEnableAll(false);
			m_InputBox[10].SetEnable(true); // ó�� Ŀ���� ��ġ
			return false;
		}
		if(!Check_birthday3(11))	{
			Clear_OneLine(11);
			SetEnableAll(false);
			m_InputBox[11].SetEnable(true); // ó�� Ŀ���� ��ġ
			return false;
		}
	} else {
		if(!Check_Iden_Num1(7))	{
			Clear_OneLine(7);
			Clear_OneLine(8);
			SetEnableAll(false);
			m_InputBox[7].SetEnable(true); // ó�� Ŀ���� ��ġ
			return false;
		}
		if(!Check_Iden_Num2(8))	{
			Clear_OneLine(7);
			Clear_OneLine(8);
			SetEnableAll(false);
			m_InputBox[7].SetEnable(true); // ó�� Ŀ���� ��ġ
			return false;
		}
	}

	return true;

}

void CMakeAccPage::SetEnableAll(BOOL flg)
{
	for(int i=0; i<INPUTBOX_CNT; i++)
		m_InputBox[i].SetEnable(flg); // ó�� Ŀ���� ��ġ
}

BOOL CMakeAccPage::Check_Name(int index)
{
	if(index < 0 || index >= INPUTBOX_CNT)
		return false;

	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		m_Msg.PutString(ids_input_name);
		return false;
	}
	if(CheckSpace(m_InputBox[index].GetStrBuf())){
		m_Msg.PutString(ids_narrow_name);
		return false;
	}

	int i,idx = 0;
	if (strstr(m_InputBox[index].GetStrBuf(), "&")) {
		char *str = m_InputBox[index].GetStrBuf();
		for (i=0; i < strlen(str); i++) {
			if ( str[i] == '&') {
				m_SaveData.name[idx] = '%';
				idx++;
				m_SaveData.name[idx] = '2';
				idx++;
				m_SaveData.name[idx] = '6';
				idx++;
			} else {
				m_SaveData.name[idx] = str[i];
				idx++;
			}
		}
		m_SaveData.name[idx] = NULL;
	} else {
		strcpy(m_SaveData.name, m_InputBox[index].GetStrBuf());
	}
	return true;
}


BOOL CMakeAccPage::Check_Sex(int index)
{
	if(index < 0 || index >= INPUTBOX_CNT)
		return false;

	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		//m_Msg.PutString("������ üũ���ּ���");
		return false;
	}
	return true;
}

BOOL CMakeAccPage::Check_Id(int index)
{
	char* found_sp = NULL;
	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		m_Msg.PutString(ids_id_limit);
		return false;
	}

	if(CheckSpace(m_InputBox[index].GetStrBuf())){ // ���鹮�ڰ� �ִ��� �˻��Ѵ�
		m_Msg.PutString(ids_narrow_id);
		return false;
	}

	int len = strlen(m_InputBox[index].GetStrBuf());
	if(len <4 || len>20){
		m_Msg.PutString(ids_id_limit);
		return false;
	}

	char tmp[100];
	strcpy(tmp, m_InputBox[index].GetStrBuf());
	for(int i=0; i<len; i++){  //  id���� "%"���ڿ��� �Է¹��� �� ����
		if(tmp[i] == '%'){
			m_Msg.PutString(ids_unable_string_id);
			return false;
		}
	}

	if((found_sp = ExamSpChar2(m_InputBox[index].GetStrBuf())) != NULL)
	{
		char msgStr[100];
		sprintf(msgStr, ids_unable_string_name, found_sp);
		m_Msg.PutString(msgStr);
		return false;
	}

	int idx = 0;
	if (strstr(m_InputBox[index].GetStrBuf(), "&")) {
		char *str = m_InputBox[index].GetStrBuf();
		for (i=0; i < strlen(str); i++) {
			if ( str[i] == '&') {
				m_SaveData.id[idx] = '%';
				idx++;
				m_SaveData.id[idx] = '2';
				idx++;
				m_SaveData.id[idx] = '6';
				idx++;
			} else {
				m_SaveData.id[idx] = str[i];
				idx++;
			}
		}
		m_SaveData.id[idx] = NULL;
	} else {
		strcpy(m_SaveData.id, m_InputBox[index].GetStrBuf());
	}

	return true;
}



BOOL CMakeAccPage::Check_Pw(int index)
{
	int len;

	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		m_Msg.PutString(ids_input_pass);
		return false;
	}

	len = strlen(m_InputBox[index].GetStrBuf());
	if(len <4 || len>20){
		m_Msg.PutString(ids_pass_limit);
		return false;
	}

	if(Check_HanEng(m_InputBox[index].GetStrBuf())==2) // �ѱ��� �ԷµȰ��
	{
		m_Msg.PutString(ids_unable_hangeul);
		return false;
	}


	if(CheckSpace(m_InputBox[index].GetStrBuf())){
		m_Msg.PutString(ids_narrow_pass);
		return false;
	}

	int i,idx = 0;
	if (strstr(m_InputBox[index].GetStrBuf(), "&")) {
		char *str = m_InputBox[index].GetStrBuf();
		for (i=0; i < strlen(str); i++) {
			if ( str[i] == '&') {
				m_SaveData.pw[idx] = '%';
				idx++;
				m_SaveData.pw[idx] = '2';
				idx++;
				m_SaveData.pw[idx] = '6';
				idx++;
			} else {
				m_SaveData.pw[idx] = str[i];
				idx++;
			}
		}
		m_SaveData.pw[idx] = NULL;
	} else {
		strcpy(m_SaveData.pw, m_InputBox[index].GetStrBuf());
	}

	return true;
}

BOOL CMakeAccPage::Check_Pw_Confirm(int index)
{
	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		//m_Msg.PutString("PASSWORD Ȯ���� �Է��ϼ���");
		return false;
	}
	if(CheckSpace(m_InputBox[index].GetStrBuf())){
		m_Msg.PutString(ids_narrow_pass);
		return false;
	}

	if(strcmp(m_InputBox[index-1].GetStrBuf(), m_InputBox[index].GetStrBuf()) !=0){
		m_Msg.PutString("Not Identity Pass");
		return false;
	}

	strcpy(m_SaveData.pw_confirm, m_InputBox[index].GetStrBuf());
	return true;
}

BOOL CMakeAccPage::Check_Email(int index)
{
	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		m_Msg.PutString(ids_input_email);
		return false;
	}
	if(CheckSpace(m_InputBox[index].GetStrBuf())){
		m_Msg.PutString(ids_narrow_email);
		return false;
	}

	int i,idx = 0;
	if (strstr(m_InputBox[index].GetStrBuf(), "&")) {
		char *str = m_InputBox[index].GetStrBuf();
		for (i=0; i < strlen(str); i++) {
			if ( str[i] == '&') {
				m_SaveData.email[idx] = '%';
				idx++;
				m_SaveData.email[idx] = '2';
				idx++;
				m_SaveData.email[idx] = '6';
				idx++;
			} else {
				m_SaveData.email[idx] = str[i];
				idx++;
			}
		}
		m_SaveData.email[idx] = NULL;
	} else {
		strcpy(m_SaveData.email, m_InputBox[index].GetStrBuf());
	}

	return true;
}

BOOL CMakeAccPage::Check_Q(int index)
{
	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		m_Msg.PutString(ids_input_question);
		return false;
	}

	int i,idx = 0;
	if (strstr(m_InputBox[index].GetStrBuf(), "&")) {
		char *str = m_InputBox[index].GetStrBuf();
		for (i=0; i < strlen(str); i++) {
			if ( str[i] == '&') {
				m_SaveData.question[idx] = '%';
				idx++;
				m_SaveData.question[idx] = '2';
				idx++;
				m_SaveData.question[idx] = '6';
				idx++;
			} else {
				m_SaveData.question[idx] = str[i];
				idx++;
			}
		}
		m_SaveData.question[idx] = NULL;
	} else {
		strcpy(m_SaveData.question, m_InputBox[index].GetStrBuf());
	}

	// check space
	char tmp[256];

	idx = 0;
	strcpy(tmp, m_SaveData.question);
	for(i=0; i < strlen(tmp); i++) {
		if (tmp[i] == ' ') {
			m_SaveData.question[idx] = '%';
			idx++;
			m_SaveData.question[idx] = '2';
			idx++;
			m_SaveData.question[idx] = '0';
			idx++;
		} else {
			m_SaveData.question[idx] = tmp[i];
			idx++;
		}
	}

	return true;
}

BOOL CMakeAccPage::Check_A(int index)
{
	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		m_Msg.PutString(ids_input_answer);
		return false;
	}
	int i,idx = 0;
	if (strstr(m_InputBox[index].GetStrBuf(), "&")) {
		char *str = m_InputBox[index].GetStrBuf();
		for (i=0; i < strlen(str); i++) {
			if ( str[i] == '&') {
				m_SaveData.answer[idx] = '%';
				idx++;
				m_SaveData.answer[idx] = '2';
				idx++;
				m_SaveData.answer[idx] = '6';
				idx++;
			} else {
				m_SaveData.answer[idx] = str[i];
				idx++;
			}
		}
		m_SaveData.answer[idx] = NULL;
	} else
		strcpy(m_SaveData.answer, m_InputBox[index].GetStrBuf());

	// check space
	char tmp[256];

	idx = 0;
	strcpy(tmp, m_SaveData.answer);
	for(i=0; i < strlen(tmp); i++) {
		if (tmp[i] == ' ') {
			m_SaveData.answer[idx] = '%';
			idx++;
			m_SaveData.answer[idx] = '2';
			idx++;
			m_SaveData.answer[idx] = '0';
			idx++;
		} else {
			m_SaveData.answer[idx] = tmp[i];
			idx++;
		}
	}

	return true;
}


BOOL CMakeAccPage::Check_Iden_Num1(int index)
{
	int hr;
	int len;
	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		m_Msg.PutString(ids_input_jumin);
		return false;
	}

	if(CheckSpace(m_InputBox[index].GetStrBuf())){
		m_Msg.PutString(ids_narrow_jumin);
		return false;
	}

	len = strlen(m_InputBox[index].GetStrBuf());
	if(len<6){
		m_Msg.PutString(ids_fault_jumin);
		return false;
	}

	if(hr = Check_Digit(m_InputBox[index].GetStrBuf())==false)
	{
		m_Msg.PutString(ids_enable_number);
		return false;
	}


	strcpy(m_SaveData.iden_num1, m_InputBox[index].GetStrBuf());
	return true;
}


BOOL CMakeAccPage::Check_Iden_Num2(int index)
{
	int hr;
	int len;
	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0){
		m_Msg.PutString(ids_input_jumin);
		return false;
	}

	if(CheckSpace(m_InputBox[index].GetStrBuf())){
		m_Msg.PutString(ids_narrow_jumin);
		return false;
	}

	len = strlen(m_InputBox[index].GetStrBuf());
	if(len<7){
		m_Msg.PutString(ids_fault_jumin);
		return false;
	}

	if(hr = Check_Digit(m_InputBox[index].GetStrBuf())==false)
	{
		m_Msg.PutString(ids_enable_number);
		return false;
	}


	strcpy(m_SaveData.iden_num2, m_InputBox[index].GetStrBuf());
	return true;
}

BOOL CMakeAccPage::Check_birthday1(int index)
{
	int hr;

	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0)	return false;

	if(CheckSpace(m_InputBox[index].GetStrBuf())) 		return false;

	if(hr = Check_Digit(m_InputBox[index].GetStrBuf())==false)		return false;

	strcpy(m_SaveData.birthday1, m_InputBox[index].GetStrBuf());
	return true;
}

BOOL CMakeAccPage::Check_birthday2(int index)
{
	int hr;

	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0)	return false;

	if(CheckSpace(m_InputBox[index].GetStrBuf())) 		return false;

	if(hr = Check_Digit(m_InputBox[index].GetStrBuf())==false)		return false;

	strcpy(m_SaveData.birthday2, m_InputBox[index].GetStrBuf());
	return true;
}

BOOL CMakeAccPage::Check_birthday3(int index)
{
	int hr;

	if(strcmp(m_InputBox[index].GetStrBuf(), "") == 0)	return false;

	if(CheckSpace(m_InputBox[index].GetStrBuf())) 		return false;

	if(hr = Check_Digit(m_InputBox[index].GetStrBuf())==false)		return false;

	strcpy(m_SaveData.birthday3, m_InputBox[index].GetStrBuf());
	return true;
}





// �޼��� ������...................

BOOL CMakeAccPage::SendMessage()
{
	// ��������
	if (g_dwClientCountry == 0)
		GetTotalContent(m_SaveData.name, m_SaveData.id, m_SaveData.pw,
			m_SaveData.Sex,  m_SaveData.email, m_SaveData.question,
			m_SaveData.answer, m_SaveData.iden_num1, m_SaveData.iden_num2);
	else if (g_dwClientCountry == 1)
		jGetTotalContent(m_SaveData.name, m_SaveData.id, m_SaveData.pw,
			m_SaveData.Sex,  m_SaveData.email, m_SaveData.question,
			m_SaveData.answer, m_SaveData.birthday1, m_SaveData.birthday2, m_SaveData.birthday3);
	else
		GetTotalContent(m_SaveData.name, m_SaveData.id, m_SaveData.pw,
			m_SaveData.Sex,  m_SaveData.email, m_SaveData.question,
			m_SaveData.answer, m_SaveData.iden_num1, m_SaveData.iden_num2);


	// ������ �ּ� / ��Ʈ ����
	m_pTcpIp->ResetConnection();
	//m_pTcpIp->ConnectToServer("www.laghaim.com", 80);
	if (g_dwClientCountry == 0) {
		m_pTcpIp->ConnectToServer("61.78.55.11", 80);
	} else if (g_dwClientCountry == 1) {
		m_pTcpIp->ConnectToServer("61.78.55.126", 80);
	} else {
		m_pTcpIp->ConnectToServer("61.78.55.11", 80);
	}

	DWORD lastTime = timeGetTime();
	DWORD curTime = lastTime;
	BOOL  connected = FALSE;
	char tmp[100];
	do {
		if ( m_pTcpIp->ReadMessage(tmp, sizeof(tmp)-1) >= 0) {
			connected = TRUE;
			break;
		}
		curTime = timeGetTime();
	} while (connected == FALSE && curTime < lastTime + 6000); // 5�� ���� ��ٸ���...

	if (connected)
	{
		m_pTcpIp->SendNetMessage(m_Buf_Total, TRUE, FALSE);
	}
	else{
		return false;
	}
	m_pTcpIp->SendNetMessage("\r\n\r\n", TRUE, FALSE);
	return true;
}


// �޼��� �ޱ�............
int CMakeAccPage::ReceiveMessage()
{
	char msg[1024];
	BOOL bRes;

	bRes = GetOneSentence(msg);  // HTTP/1.1 200 0K
	if (bRes == TRUE) {
		if (strstr(msg, "OK") == NULL) {
			return -1;			// page not found or unknown
		}
	}
	//MessageBox(NULL, msg, "info", MB_OK);

	bRes = GetOneSentence(msg);					// Server: Microsoft-IIS/5.0
	if (bRes == FALSE) return -1;				// error
	//MessageBox(NULL, msg, "info", MB_OK);

	bRes = GetOneSentence(msg);					// Date: Wed, 26 Sep 2001 12:34:06 GMT
	if (bRes == FALSE) return -1;				// error
	//MessageBox(NULL, msg, "info", MB_OK);

	bRes = GetOneSentence(msg);					// Connection: Keep-Alive
	if (bRes == FALSE) return -1;				// error
	//MessageBox(NULL, msg, "info", MB_OK);

	bRes = GetOneSentence(msg);					// Content-Length: 31
	if (bRes == FALSE) return -1;				// error
	//MessageBox(NULL, msg, "info", MB_OK);

	bRes = GetOneSentence(msg);					// Content-Type: text/html
	if (bRes == FALSE) return -1;				// error
	//MessageBox(NULL, msg, "info", MB_OK);

	bRes = GetOneSentence(msg);					// Set-Cookie: ASPSESSIONIDQQQQGXEG=CAJBGEKAHMABMCCGBFLNPEJE; path=/
	if (bRes == FALSE) return -1;				// error
	//MessageBox(NULL, msg, "info", MB_OK);

	bRes = GetOneSentence(msg);					// Cache-control: private
	if (bRes == FALSE) return -1;				// error
	//MessageBox(NULL, msg, "info", MB_OK);

	bRes = GetOneSentence(msg);
	//MessageBox(NULL, msg, "info", MB_OK);
  //-- Lyul �����Ϸ�
  //-- Ok : ���� �Ϸ� Exist : ���̵� ���� lack : ���̵� ����(lm_ Ȥ�� mn_)
  //-- �̿��� ���� �� ��� return 1 �̵ȴ�. ������ �ȴٴ� �̾߱��. ����.
	if ( bRes == TRUE)
  {

		if (strstr(msg, "ok"))
    {
			return 1;
		}
    else if (strstr(msg, "exist"))
    {
			return -2;
		}
    else if (strstr(msg, "lack"))
    {
			return -3;
		}
    else return -3;
	}

	return 1;
}

BOOL CMakeAccPage::GetOneSentence(char *msg)
{
	char buf[30];
	int nCount = 0;

	// ������ ó���� ������ �������� �����Ѵ�.
	while (1)
	{
		int read_len;
		read_len = m_pTcpIp->ReadMessage(buf, 1, FALSE);

		if (read_len > 0)
		{
			if (IS_NEWLINE(buf[0])) {
				if (nCount == 0)
					continue;
				else
					break;
			}
			*(msg++) = buf[0];
			nCount++;
		} else if (read_len < 0) {
			return FALSE;
		} else {
			Sleep(100);
		}

	}
	*msg = '\0';

	return TRUE;
}


void CMakeAccPage::GetTotalContent(char*name, char*id, char* pw,
								   char* sex, char* email, char* pw_hint_text,
								   char* pw_hint_answer, char* jumin1, char* jumin2)
{
	m_Buf_A[0] = NULL;
	m_Buf_B[0] = NULL;
	m_Buf_C[0] = NULL;
	m_Buf_Total[0] = NULL;

	// �⺻���� ���
  */
//strcpy(m_Buf_A, "POST /members/memb_from_client.asp HTTP/1.0\n"
//		"User-Agent: Mozilla/4.7(WinNT: MSIE)\n"
//	"Accept: image/gif, image/x-bitmap, image/jpeg, image/pjpeg, */*\n"
//"Host: www.laghaim.com\n"
//"Content-type: application/x-www-form-urlencoded\n");
/*

	// ���� �Է»��� ���
	int cnt = 10;
	char tmp[11][256];				// include mall id

	strcpy(tmp[0], "name=");
	strcat(tmp[0],  name);

	strcpy(tmp[1], "&id=");
	strcat(tmp[1],  id);

	strcpy(tmp[2], "&pw=");
	strcat(tmp[2],  pw);

	strcpy(tmp[3], "&sex=");
	strcat(tmp[3],  sex);

	strcpy(tmp[4], "&email=");
	strcat(tmp[4],  email);

	strcpy(tmp[5], "&pw_hint=");
	strcat(tmp[5],  "09");

	strcpy(tmp[6], "&pw_hint_text=");
	strcat(tmp[6],  pw_hint_text);

	strcpy(tmp[7], "&pw_answer=");
	strcat(tmp[7],  pw_hint_answer);

	strcpy(tmp[8], "&jumin1=");
	strcat(tmp[8],  jumin1);

	strcpy(tmp[9], "&jumin2=");
	strcat(tmp[9],  jumin2);


	// �߿�.. checkup
	if (m_bPartner) {
		cnt = 11;
		strcpy(tmp[10], "&mall_id=");
		strcat(tmp[10], g_strPartner);
		strcat(tmp[10], "\n");
	} else {
		cnt = 10;
		strcat(tmp[9], "\n");
	}

	for(int i=0; i<cnt; i++)
	{
		strcat(m_Buf_C, tmp[i]);
	}
	int Len = strlen(m_Buf_C);


	// ��Ʈ�� ���� ���������� �׸� ���
	char tmp2[10];
	strcpy(m_Buf_B, "Content-length:");
	itoa(Len, tmp2, 10);
	strcat(m_Buf_B, tmp2 );
	strcat(m_Buf_B, "\n");


	strcpy(m_Buf_Total, m_Buf_A);
	strcat(m_Buf_Total, m_Buf_B);
	strcat(m_Buf_Total, m_Buf_C);
//	strcat(m_Buf_Total, "\r\n\r\n");
}

void CMakeAccPage::jGetTotalContent(char*name, char*id, char* pw,
								   char* sex, char* email, char* pw_hint_text,
								   char* pw_hint_answer, char* year, char *month, char *day)
{
	m_Buf_A[0] = NULL;
	m_Buf_B[0] = NULL;
	m_Buf_C[0] = NULL;
	m_Buf_Total[0] = NULL;

	// �⺻���� ���
  */
//strcpy(m_Buf_A, "POST /members/memb_from_client.asp HTTP/1.0\n"
//	"User-Agent: Mozilla/4.7(WinNT: MSIE)\n"
//	"Accept: image/gif, image/x-bitmap, image/jpeg, image/pjpeg, */*\n"
//	"Host: www.laghaim.com\n"
//	"Content-type: application/x-www-form-urlencoded\n");
/*

	// ���� �Է»��� ���
	static int cnt = 11;
	char tmp[11][256];

	strcpy(tmp[0], "name=");
	strcat(tmp[0],  name);

	strcpy(tmp[1], "&id=");
	strcat(tmp[1],  id);

	strcpy(tmp[2], "&pw=");
	strcat(tmp[2],  pw);

	strcpy(tmp[3], "&sex=");
	strcat(tmp[3],  sex);

	strcpy(tmp[4], "&email=");
	strcat(tmp[4],  email);

	strcpy(tmp[5], "&pw_hint=");
	strcat(tmp[5],  "09");

	strcpy(tmp[6], "&pw_hint_text=");
	strcat(tmp[6],  pw_hint_text);

	strcpy(tmp[7], "&pw_answer=");
	strcat(tmp[7],  pw_hint_answer);

	strcpy(tmp[8], "&birth_yy=");
	strcat(tmp[8],  year);

	strcpy(tmp[9], "&birth_mm=");
	strcat(tmp[9],  month);

	strcpy(tmp[10], "&birth_dd=");
	strcat(tmp[10], day);
	strcat(tmp[9], "\n");

	for(int i=0; i<cnt; i++)
	{
		strcat(m_Buf_C, tmp[i]);
	}
	int Len = strlen(m_Buf_C);


	// ��Ʈ�� ���� ���������� �׸� ���
	char tmp2[10];
	strcpy(m_Buf_B, "Content-length:");
	itoa(Len, tmp2, 10);
	strcat(m_Buf_B, tmp2 );
	strcat(m_Buf_B, "\n");


	strcpy(m_Buf_Total, m_Buf_A);
	strcat(m_Buf_Total, m_Buf_B);
	strcat(m_Buf_Total, m_Buf_C);
//	strcat(m_Buf_Total, "\r\n\r\n");
}


BOOL CMakeAccPage::CheckSpace(char* String)
{
	int len = strlen(String);
	for(int i=0; i<len; i++)
	{
		if(is_space(String[i]) == TRUE)
			return TRUE;
	}
	return FALSE;
}


BOOL CMakeAccPage::Check_Digit(char* buf)
{
	int k = strlen(buf);

	// �ֻ��� ��Ʈ������ ���ؼ�..�ѱ� ������ �����Ѵ�
	for(int i=0; i<k; i++){
		if( ! isdigit(buf[i]))
			return false;
	}
	return true;
}


void CMakeAccPage::Read_URL_Add(char *filename)
{
   FILE *stream;
   char line[100];

   if( (stream = fopen( filename, "r" )) != NULL )
   {
      if( fgets( line, 100, stream ) == NULL)
         return;
      else
         printf( "%s", line);
      fclose( stream );
   }
   strcpy(m_AddRes, line);
}


int CMakeAccPage::Check_HanEng(char* buf)
{
	int hr;
	int k = strlen(buf);
	// �ֻ��� ��Ʈ������ ���ؼ�..�ѱ� ������ �����Ѵ�
	for(int i=0; i<k; i++){
		if(((unsigned char)buf[k]) > 0x70)
			hr = 2;      // �ѱ��� ���
		else
			hr = 1;      // ������ ���
	}
	return hr;
}



void CMakeAccPage::SetHanMode(HWND hWnd)
{
    HIMC hIMC = ImmGetContext(hWnd);
    DWORD dwConv, dwSent;
    DWORD dwTemp;

    ImmGetConversionStatus(hIMC,&dwConv,&dwSent);

    dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
    dwTemp |= IME_CMODE_NATIVE;
    dwConv = dwTemp;
         ImmSetConversionStatus(hIMC,dwConv,dwSent);
    ImmReleaseContext(hWnd,hIMC);
}



void CMakeAccPage::SetEngMode(HWND hWnd)
{
    HIMC hIMC = ImmGetContext(hWnd);
    DWORD dwConv, dwSent;
    DWORD dwTemp;

    ImmGetConversionStatus(hIMC,&dwConv,&dwSent);

    dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
    dwTemp |= IME_CMODE_ALPHANUMERIC;
    dwConv = dwTemp;
         ImmSetConversionStatus(hIMC,dwConv,dwSent);

         ImmReleaseContext(hWnd,hIMC);
}




#include "resource.h"
void CMakeAccPage::LoadDLLStringTable()
{
	HINSTANCE hInst = g_hDllInst;
	char temp[1024];

	char error[100];
	strcpy(error, "Fail!!! Load Dll String");

	if(LoadString(hInst, IDS_MAKEACC_CANNOT_CONNECT, temp, 1024) != NULL){
		ids_cannot_connect = new char[strlen(temp)+2]; // ������ ������ �� �����ϴ�
		strcpy(ids_cannot_connect, temp);
	}else{
		ids_cannot_connect = new char[strlen(error)+2]; // ������ ������ �� �����ϴ�
		strcpy(ids_cannot_connect, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_CREATED_ACC, temp, 1024) != NULL){
		ids_create_acc = new char[strlen(temp)+2];     // ������ ����������ϴ�.
		strcpy(ids_create_acc, temp);
	}else{
		ids_create_acc = new char[strlen(error)+2];     // ������ ����������ϴ�.
		strcpy(ids_create_acc, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_SEVER_TEST, temp, 1024) != NULL){
		ids_sever_test = new char[strlen(temp)+2];     // ���� �������Դϴ�. ���������� Ȩ����������
		strcpy(ids_sever_test, temp);
	}else{
		ids_sever_test = new char[strlen(error)+2];     // ���� �������Դϴ�. ���������� Ȩ����������
		strcpy(ids_sever_test, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_EXIST_ID, temp, 1024) != NULL){
		ids_exist_id = new char[strlen(temp)+2];     // ���̵� �����մϴ�
		strcpy(ids_exist_id, temp);
	}else{
		ids_exist_id = new char[strlen(error)+2];     // ���̵� �����մϴ�
		strcpy(ids_exist_id, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_CONFIRM_ITEM, temp, 1024) != NULL){
		ids_confirm_item = new char[strlen(temp)+2];     // ���Ի����� Ȯ���ϼ���
		strcpy(ids_confirm_item, temp);
	}else{
		ids_confirm_item = new char[strlen(error)+2];     // ���Ի����� Ȯ���ϼ���
		strcpy(ids_confirm_item, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_INPUT_NAME, temp, 1024) != NULL){
		ids_input_name = new char[strlen(temp)+2];     // �̸��� �Է��ϼ���
		strcpy(ids_input_name, temp);
	}else{
		ids_input_name = new char[strlen(error)+2];     // �̸��� �Է��ϼ���
		strcpy(ids_input_name, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_NARROW_NAME, temp, 1024) != NULL){
		ids_narrow_name = new char[strlen(temp)+2];     // �̸��� �ٿ�������
		strcpy(ids_narrow_name, temp);
	}else{
		ids_narrow_name = new char[strlen(error)+2];     // �̸��� �ٿ�������
		strcpy(ids_narrow_name, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_NARROW_ID, temp, 1024) != NULL){
		ids_narrow_id = new char[strlen(temp)+2];     // ���̵� �ٿ�������
		strcpy(ids_narrow_id, temp);
	}else{
		ids_narrow_id = new char[strlen(error)+2];     // ���̵� �ٿ�������
		strcpy(ids_narrow_id, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_ID_LIMIT, temp, 1024) != NULL){
		ids_id_limit = new char[strlen(temp)+2];     // ���̵�� 20�� �̳� �Դϴ�
		strcpy(ids_id_limit, temp);
	}else{
		ids_id_limit = new char[strlen(error)+2];     // ���̵�� 20�� �̳� �Դϴ�
		strcpy(ids_id_limit, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_UNABLE_STRING_ID, temp, 1024) != NULL){
		ids_unable_string_id = new char[strlen(temp)+2];     // ���̵� "%s"���ڴ� ������� �ʽ��ϴ�.
		strcpy(ids_unable_string_id, temp);
	}else{
		ids_unable_string_id = new char[strlen(error)+2];     // ���̵� "%s"���ڴ� ������� �ʽ��ϴ�.
		strcpy(ids_unable_string_id, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_UNABLE_STRING_NAME, temp, 1024) != NULL){
		ids_unable_string_name = new char[strlen(temp)+2];     // �ɸ����̸��� "%s"���ڴ� ������� �ʽ��ϴ�.
		strcpy(ids_unable_string_name, temp);
	}else{
		ids_unable_string_name = new char[strlen(error)+2];     // �ɸ����̸��� "%s"���ڴ� ������� �ʽ��ϴ�.
		strcpy(ids_unable_string_name, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_INPUT_PASS, temp, 1024) != NULL){
		ids_input_pass = new char[strlen(temp)+2];     // password�� �Է��ϼ���
		strcpy(ids_input_pass, temp);
	}else{
		ids_input_pass = new char[strlen(error)+2];     // password�� �Է��ϼ���
		strcpy(ids_input_pass, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_PASS_LIMIT, temp, 1024) != NULL){
		ids_pass_limit = new char[strlen(temp)+2];     // password�� 20�� �̳� �Դϴ�
		strcpy(ids_pass_limit, temp);
	}else{
		ids_pass_limit = new char[strlen(error)+2];     // password�� 20�� �̳� �Դϴ�
		strcpy(ids_pass_limit, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_UNABLE_HANGEUL, temp, 1024) != NULL){
		ids_unable_hangeul = new char[strlen(temp)+2];     // password�� �ѱ��� ������� �ʽ��ϴ�
		strcpy(ids_unable_hangeul, temp);
	}else{
		ids_unable_hangeul = new char[strlen(error)+2];     // password�� �ѱ��� ������� �ʽ��ϴ�
		strcpy(ids_unable_hangeul, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_NARROW_PASS, temp, 1024) != NULL){
		ids_narrow_pass = new char[strlen(temp)+2];     // password�� �ٿ� ������
		strcpy(ids_narrow_pass, temp);
	}else{
		ids_narrow_pass = new char[strlen(error)+2];     // password�� �ٿ� ������
		strcpy(ids_narrow_pass, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_INPUT_EMAIL, temp, 1024) != NULL){
		ids_input_email = new char[strlen(temp)+2];     // e_mail�ּҸ� �Է��ϼ���
		strcpy(ids_input_email, temp);
	}else{
		ids_input_email = new char[strlen(error)+2];     // e_mail�ּҸ� �Է��ϼ���
		strcpy(ids_input_email, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_NARROW_EMAIL, temp, 1024) != NULL){
		ids_narrow_email = new char[strlen(temp)+2];     // e_mail�ּҸ� �ٿ�������
		strcpy(ids_narrow_email, temp);
	}else{
		ids_narrow_email = new char[strlen(error)+2];     // e_mail�ּҸ� �ٿ�������
		strcpy(ids_narrow_email, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_INPUT_QUESTION, temp, 1024) != NULL){
		ids_input_question = new char[strlen(temp)+2];     // ��й�ȣ �нǽ� ������ �Է��ϼ���
		strcpy(ids_input_question, temp);
	}else{
		ids_input_question = new char[strlen(error)+2];     // ��й�ȣ �нǽ� ������ �Է��ϼ���
		strcpy(ids_input_question, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_INPUT_ANSWER, temp, 1024) != NULL){
		ids_input_answer = new char[strlen(temp)+2];     // ��й�ȣ �нǽ� �亯�� �Է��ϼ���
		strcpy(ids_input_answer, temp);
	}else{
		ids_input_answer = new char[strlen(error)+2];     // ��й�ȣ �нǽ� �亯�� �Է��ϼ���
		strcpy(ids_input_answer, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_INPUT_JUMIN, temp, 1024) != NULL){
		ids_input_jumin = new char[strlen(temp)+2];     // �ֹε�Ϲ�ȣ�� �Է��ϼ���
		strcpy(ids_input_jumin, temp);
	}else{
		ids_input_jumin = new char[strlen(error)+2];     // �ֹε�Ϲ�ȣ�� �Է��ϼ���
		strcpy(ids_input_jumin, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_NARROW_JUMIN, temp, 1024) != NULL){
		ids_narrow_jumin = new char[strlen(temp)+2];     // �ֹε�Ϲ�ȣ�� �ٿ�������
		strcpy(ids_narrow_jumin, temp);
	}else{
		ids_narrow_jumin = new char[strlen(error)+2];     // �ֹε�Ϲ�ȣ�� �ٿ�������
		strcpy(ids_narrow_jumin, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_FAULT_JUMIN, temp, 1024) != NULL){
		ids_fault_jumin = new char[strlen(temp)+2];     // �ֹε�Ϲ�ȣ �Է��� �߸� �Ǿ����ϴ�.
		strcpy(ids_fault_jumin, temp);
	}else{
		ids_fault_jumin = new char[strlen(error)+2];     // �ֹε�Ϲ�ȣ �Է��� �߸� �Ǿ����ϴ�.
		strcpy(ids_fault_jumin, error);
	}
	if(LoadString(hInst, IDS_MAKEACC_ENABLE_NUMBER, temp, 1024) != NULL){
		ids_enable_number = new char[strlen(temp)+2];     // �ֹε�Ϲ�ȣ �Է��� ���ڸ� �����մϴ�
		strcpy(ids_enable_number, temp);
	}else{
		ids_enable_number = new char[strlen(error)+2];     // �ֹε�Ϲ�ȣ �Է��� ���ڸ� �����մϴ�
		strcpy(ids_enable_number, error);
	}
}

void CMakeAccPage::DeleteDLLStringTable()
{
	SAFE_DELETE_ARRAY(ids_cannot_connect);
	SAFE_DELETE_ARRAY(ids_create_acc);
	SAFE_DELETE_ARRAY(ids_sever_test);
	SAFE_DELETE_ARRAY(ids_exist_id);
	SAFE_DELETE_ARRAY(ids_confirm_item);
	SAFE_DELETE_ARRAY(ids_input_name);
	SAFE_DELETE_ARRAY(ids_narrow_name);
	SAFE_DELETE_ARRAY(ids_narrow_id);
	SAFE_DELETE_ARRAY(ids_id_limit);
	SAFE_DELETE_ARRAY(ids_unable_string_id);
	SAFE_DELETE_ARRAY(ids_unable_string_name);
	SAFE_DELETE_ARRAY(ids_input_pass);
	SAFE_DELETE_ARRAY(ids_pass_limit);
	SAFE_DELETE_ARRAY(ids_unable_hangeul);
	SAFE_DELETE_ARRAY(ids_narrow_pass);
	SAFE_DELETE_ARRAY(ids_input_email);
	SAFE_DELETE_ARRAY(ids_narrow_email);
	SAFE_DELETE_ARRAY(ids_input_question);
	SAFE_DELETE_ARRAY(ids_input_answer);
	SAFE_DELETE_ARRAY(ids_input_jumin);
	SAFE_DELETE_ARRAY(ids_narrow_jumin);
	SAFE_DELETE_ARRAY(ids_fault_jumin);
	SAFE_DELETE_ARRAY(ids_enable_number);
}


*/