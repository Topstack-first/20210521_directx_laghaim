#ifndef _EVENT_SCRIPT_
#define _EVENT_SCRIPT_

#include "config.h"

#include <list>
#include <iterator>
using namespace std;

#define EVT_SCR_IMG_STATE_BEFORE 0 // �̹��� ��� ��.
#define EVT_SCR_IMG_STATE_FADEIN 1 // �̹��� ��� ��. ���̵� ��
#define EVT_SCR_IMG_STATE_OUTING 2 // �̹��� ��� ��
#define EVT_SCR_IMG_STATE_FADEOUT 3 // �̹��� ��� ��. ���̵� �ƿ�
#define EVT_SCR_IMG_STATE_END 4 // �̹��� �����

#define SCRIPT_FRONT_DUMMY_SIZE 115 // ���� ��ȣȭ�� ���ʿ� �ִ� ���� ������
#define SCRIPT_LAST_DUMMY_SIZE 47 // ���� ��ȣȭ�� ���ʿ� �ִ� ���� ������
#define SCRIPT_ENCODE_BYTE 0x72     // ��ȣȭ ��Ʈ. ���� �����ͺο� ^= �����ؼ� ����Ѵ�. ���ڴ� �� ���Ѱ����� �����͸� �о���� �� �ҽ������� ���� ��������Ѵ�.

struct sImageOutInfo
{
	char *m_strFileName; // �̹��� ���ϸ�
	DWORD m_CreateTime; // ���� ƽ. ���� ���۵� ���� ƽ ����.
	int m_FadeInSpeed; // ������ ���̵��� ���� �� ���ǵ�(ms) �̰� 0�̸� ���̵��� ���� �ٷ� ���
	DWORD m_DelTime; // ���� ƽ.
	int m_FadeOutSpeed; // ���Ž� ���̵�ƿ� ���� �� ���ǵ�(ms) �̰� 0�̸� ���̵�ƿ� ���� �ٷ� ����
	int m_PosX; // ��� X ��ġ
	int m_PosY; // ��� Y ��ġ
	int m_Width; // ���� ũ��
	int m_Height; // ���� ũ��

	int m_State; // �� �̹��� ����� ����.
	int m_ContainerIndex; // �̹��� �����̳� Ŭ���� �󿡼��� �ε���.

	sImageOutInfo()
	{
		m_strFileName = NULL;

		Init();
	}

	~sImageOutInfo()
	{
		Init();
	}

	void Init()
	{
		if( m_strFileName )
			SAFE_DELETE_ARRAY( m_strFileName );

		m_CreateTime = 0; // ���� ƽ. ���� ���۵� ���� ƽ ����.
		m_FadeInSpeed = 0; // ������ ���̵��� ���� �� ���ǵ�(ms) �̰� 0�̸� ���̵��� ���� �ٷ� ���
		m_DelTime = 0; // ���� ƽ.
		m_FadeOutSpeed = 0; // ���Ž� ���̵�ƿ� ���� �� ���ǵ�(ms) �̰� 0�̸� ���̵�ƿ� ���� �ٷ� ����

		m_PosX = 0; // ��� X ��ġ
		m_PosY = 0; // ��� Y ��ġ
		m_Width = 0; // ���� ũ��
		m_Height = 0; // ���� ũ��

		m_State = 0; // �� �̹��� ����� ����.
		m_ContainerIndex = -1;
	}
};

struct sSceneData
{
	int m_Index; // ���� �ε���
	RECT m_BoxArea; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
	RECT m_TextArea; // ��ȭâ���� �ؽ�Ʈ�� �������� ����. (x,y�� w,h�ӿ� ����)
	int m_TextSpeed; // �ؽ�Ʈ ���� �ӵ�. ms ����.
	int m_WaitTime; // �� ���� �����ϱ� ���� ��޷��ִ� �ð�. �� ����.
	int m_WaitAction; // �� ��ġ�� 0 ���� ũ�ٸ� �ش� �׼�(�̺�Ʈ �����̳� ����� �Է��� ��ٷ��� �ϴ� ������ �׼�)�� ������ ��ȭ�� ����ȴ�.
	int m_FadeInSpeed; // �� ��ġ�� 0 ���� ũ�ٸ� ���� ���۵ǰ� ������ ���̵� ȿ���� �־��ش�. ���̵� ȿ���� �ӵ�.
	int m_FadeOutSpeed; // �� ��ġ�� 0 ���� ũ�ٸ� ���� ���۵ǰ� ������ ���̵� ȿ���� �־��ش�. ���̵� ȿ���� �ӵ�.

	char *m_strContents; // ��ȭ ����.

	char *m_strSelection1; // ������ 1. (���� ������ 1�� ������ <ENTER> ��� ���ͷ� �ѱ��...<NEXT> ��� �� �������� �ѱ���. <END> �� ��ũ��Ʈ ����. )
	int m_LinkSel1; // ������ 1�� ��ũ �ε���.(�� �������� �������� ���. �̵��ϴ� ���� ��ȣ)
	char *m_strSelection2; // ������ 2.
	int m_LinkSel2; // ������ 2�� ��ũ �ε���.(�� �������� �������� ���. �̵��ϴ� ���� ��ȣ)
	char *m_strSelection3; // ������ 3.
	int m_LinkSel3; // ������ 3�� ��ũ �ε���.(�� �������� �������� ���. �̵��ϴ� ���� ��ȣ)

	list<sImageOutInfo*>		m_listImageOut; // �̹��� ��� ����.

	DWORD m_SceneStartTick; // ���� ���۵� ƽ�� ���. �� ƽ�� ���缭 �̹������� ���

	sSceneData()
	{
		m_strContents = NULL; // ��ȭ ����.

		m_strSelection1 = NULL; // ������ 1. (���� ������ 1�� ������ <ENTER> ��� ���ͷ� �ѱ��...<NEXT> ��� �� �������� �ѱ���. <END> �� ��ũ��Ʈ ����. )
		m_strSelection2 = NULL; // ������ 2.
		m_strSelection3 = NULL; // ������ 3.

		Init();
	}

	~sSceneData()
	{
		Init();
	}

	void Init()
	{
		m_Index = -1; // ���� �ε���

		m_BoxArea.left = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_BoxArea.top = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_BoxArea.right = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_BoxArea.bottom = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)

		m_TextArea.left = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_TextArea.top = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_TextArea.right = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_TextArea.bottom = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)

		m_TextSpeed = 0; // �ؽ�Ʈ ���� �ӵ�. ms ����.
		m_WaitTime = 0; // �� ���� �����ϱ� ���� ��޷��ִ� �ð�. �� ����.
		m_WaitAction = -1; // �� ��ġ�� 0 ���� ũ�ٸ� �ش� �׼�(�̺�Ʈ �����̳� ����� �Է��� ��ٷ��� �ϴ� ������ �׼�)�� ������ ��ȭ�� ����ȴ�.
		m_FadeInSpeed = 0; // �� ��ġ�� 0 ���� ũ�ٸ� ���� ���۵ǰ� ������ ���̵� ȿ���� �־��ش�. ���̵� ȿ���� �ӵ�.
		m_FadeOutSpeed = 0; // �� ��ġ�� 0 ���� ũ�ٸ� ���� ���۵ǰ� ������ ���̵� ȿ���� �־��ش�. ���̵� ȿ���� �ӵ�.

		if( m_strContents )
			SAFE_DELETE_ARRAY( m_strContents );

		if( m_strSelection1 )
			SAFE_DELETE_ARRAY( m_strSelection1 );
		m_LinkSel1 = -1; // ������ 1�� ��ũ �ε���.(�� �������� �������� ���. �̵��ϴ� ���� ��ȣ)
		if( m_strSelection2 )
			SAFE_DELETE_ARRAY( m_strSelection2 );
		m_LinkSel2 = -1; // ������ 1�� ��ũ �ε���.(�� �������� �������� ���. �̵��ϴ� ���� ��ȣ)
		if( m_strSelection3 )
			SAFE_DELETE_ARRAY( m_strSelection3 );
		m_LinkSel3 = -1; // ������ 1�� ��ũ �ε���.(�� �������� �������� ���. �̵��ϴ� ���� ��ȣ)


		sImageOutInfo *pImageOut;
		list<sImageOutInfo*>::iterator prImageOut;
		for( prImageOut = m_listImageOut.begin(); prImageOut != m_listImageOut.end(); ++prImageOut )
		{
			pImageOut = *prImageOut;
			pImageOut->Init(); // �̰� ������ ���༭ �޸𸮸� �������ش�.
			SAFE_DELETE( pImageOut );
		}

		m_listImageOut.clear();

		m_SceneStartTick = 0;
	}
};

class CScriptManager
{
public:
	CScriptManager()
	{
		m_strBoxFileName = NULL;

		Init();
	}

	~CScriptManager()
	{
		Init();
	}

	void Init()
	{
		if( m_strBoxFileName )
			SAFE_DELETE_ARRAY( m_strBoxFileName );

		m_BoxArea.left = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_BoxArea.top = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_BoxArea.right = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_BoxArea.bottom = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)

		m_TextArea.left = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_TextArea.top = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_TextArea.right = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
		m_TextArea.bottom = -1; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)

		sSceneData *pScene;
		list<sSceneData*>::iterator prScene;
		for( prScene = m_listScene.begin(); prScene != m_listScene.end(); ++prScene )
		{
			pScene = *prScene;

			pScene->Init(); // �̰� ������ ���༭ �޸𸮸� �������ش�.
			SAFE_DELETE( pScene );
		}
		m_listScene.clear();
	}

	BOOL LoadScript( const char* FileName );
	void SaveScript( const char* FileName ); // .scr �� ����

	void SaveScriptToText( const char* FileName ); // �ؽ�Ʈ�� ����
	void ImportScriptFromText( const char* FileName ); // �ؽ�Ʈ�κ��� �о�ͼ� ����

	void RunScript();

	char *m_strBoxFileName; // ��ȭâ �̹��� ���ϸ�
	RECT m_BoxArea; // ��ȭâ �׷��� ����. â�� �����ӿ� ���� (x,y�� w,h�ӿ� ����)
	RECT m_TextArea; // ��ȭâ���� �ؽ�Ʈ�� �������� ����. (x,y�� w,h�ӿ� ����)
	list<sSceneData*>		m_listScene; // ���� ����.
};


#endif