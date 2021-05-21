#ifndef _AUTO_MOUSE_H_
#define _AUTO_MOUSE_H_

#include "Config.h"


#include "ddutil.h"

class CNk2DFrame;
class CSurface;
class CBasicButton;
class CTextBox;
class CTextOutBox;
class CTcpIpConnection;
class CAlphaPopUp;
class CMultiText;

#define NUM_OF_CHECKNUMBER 10 // Ŭ���̾�Ʈ üũ �ѹ��� ����. ///�����Է½ý��ۺ���
#define NUM_OF_CONFIRMNUMBER 10 // Ŭ���̾�Ʈ���� ���丶�콺 üũ ������ ������ ������ �����ѹ��� ����. ///�����Է½ý��ۺ���

#define AUTOMOUSE_TYPE_NUMBER 0 // ���� �Է� �ý���
#define AUTOMOUSE_TYPE_QnA 1    // ���� Ȯ�� �ý���
#define AUTOMOUSE_TYPE_QUIZ 2    // ���� ���� �̺�Ʈ �ý���

// nate 2004 - 3
// ���� Ȯ�� �ý���
class C3d_PosPoint;

class CAutoMouse
{
public:
	void SetTextInFocus();
	void AddWord(char *w1,char *w2,int flag, BOOL bAct);
	CAutoMouse();
	virtual ~CAutoMouse();

	//==================================================================
	// nate 2004 - 3
	// ���� Ȯ�� �ý���
	C3d_PosPoint		*m_pBoard;
	CSurface	 		*m_pSurfaceNum;
	CSurface	 		*m_pSurfaceBack;
	CSurface	 		*m_pQuestionmark;
	CSurface	 		*m_pQuestionmark2;
	CTextBox	 		*m_pTextIn; // ���� Ȯ�� �ڽ�
	CTextOutBox  		*m_pMunjaText; // �������� �� ���� ��� �ڽ�
	CBasicButton 		*m_pBtn1;//Ȯ�ι�ư
	CAlphaPopUp	 		*m_pHelpInfo; // ����
	CBasicButton 		*m_pCloseBtn; // ����� �ݱ� ��ư �߰�.


	BYTE				m_byCheckCount;

	int					m_nNum[ 4 ];
	float				m_fRandom[ 4 ];
	void SetRandomNum();
	//==================================================================

	////////////////////////////////////////////////////////////////////////////////////////
	// ������ : �̿���
	// ������ : 05-04-07
	// �������� : �����Է½ý��� ���� ó��. ///�����Է½ý��ۺ��� // UpdateLog 171�� ����.
	int m_nDecodeBit; // üũ�ѹ��� ��ȣȭ ��Ʈ ���� ����� �����ϰ� ��������.
	int m_nRealCheckNumber[NUM_OF_CHECKNUMBER]; // ���� ����� �����ϰ� �������� 4�ڸ� üũ �ѹ�.( �� m_nDecodeBit������ ���� ��ȣȭ �Ǿ� ����ȴ�. )
	void MakeCheckNumber(); // ���� ���۽� ����. üũ�ѹ��� ��ȣȭ ��Ʈ�� ���������� �����ش�.


	BOOL m_bOneFailed;
	BYTE m_nConfirmNumber[NUM_OF_CONFIRMNUMBER]; // ������ȣ. 5���� ������ ����...5���� �� 5�� �ѹ��� �ð������κ��� Ŭ���̾�Ʈ���� ���Ѵ�.(���ϴ� Ÿ�̹��� �����ڵ带 �����ϴ� Ÿ�ֿ̹�...)
	void SetConfirmNumber( int Arg1, int Arg2 ); // �����κ��� ���� ��ȣ�κ��� ������ȣ 5���� �����Ѵ�.
	void SendConfirm( char *InputStr ); // ������ȣ�� ����ؼ� ������.
	////////////////////////////////////////////////////////////////////////////////////////



	char m_dic_word[10];
	char m_strhelpinfo[100];

	bool m_bOnHelp;
	BOOL m_bCurAct;
	bool m_bActive;

	float m_ftime;
	float m_felapsedtime;


	int m_CurTextView;// ê��â ��ġ,ũ�� flag

	int m_Type; // �����Է����� QnA ����.

	void Init();
	BOOL IsInside(int x, int y);
	void LoadRes(int Type);

	void DeleteRes();
	void Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT QnAMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};


#endif // _AUTO_MOUSE_H_
