#ifndef _EVENT_SCRIPT_
#define _EVENT_SCRIPT_

#include "config.h"

#include <list>
#include <iterator>
using namespace std;

#define EVT_SCR_IMG_STATE_BEFORE 0 // 이미지 출력 전.
#define EVT_SCR_IMG_STATE_FADEIN 1 // 이미지 출력 전. 페이드 인
#define EVT_SCR_IMG_STATE_OUTING 2 // 이미지 출력 중
#define EVT_SCR_IMG_STATE_FADEOUT 3 // 이미지 출력 중. 페이드 아웃
#define EVT_SCR_IMG_STATE_END 4 // 이미지 출력후

#define SCRIPT_FRONT_DUMMY_SIZE 115 // 파일 암호화시 앞쪽에 넣는 더미 싸이즈
#define SCRIPT_LAST_DUMMY_SIZE 47 // 파일 암호화시 뒤쪽에 넣는 더미 싸이즈
#define SCRIPT_ENCODE_BYTE 0x72     // 암호화 비트. 실제 데이터부에 ^= 연산해서 사용한다. 숫자는 막 정한거지만 데이터를 읽어오는 쪽 소스에서도 같게 맞춰줘야한다.

struct sImageOutInfo
{
	char *m_strFileName; // 이미지 파일명
	DWORD m_CreateTime; // 생성 틱. 씬이 시작된 후의 틱 단위.
	int m_FadeInSpeed; // 생성시 페이드인 여부 및 스피드(ms) 이게 0이면 페이드인 없이 바로 출력
	DWORD m_DelTime; // 제거 틱.
	int m_FadeOutSpeed; // 제거시 페이드아웃 여부 및 스피드(ms) 이게 0이면 페이드아웃 없이 바로 제거
	int m_PosX; // 출력 X 위치
	int m_PosY; // 출력 Y 위치
	int m_Width; // 가로 크기
	int m_Height; // 세로 크기

	int m_State; // 이 이미지 출력의 상태.
	int m_ContainerIndex; // 이미지 컨테이너 클래스 상에서의 인덱스.

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

		m_CreateTime = 0; // 생성 틱. 씬이 시작된 후의 틱 단위.
		m_FadeInSpeed = 0; // 생성시 페이드인 여부 및 스피드(ms) 이게 0이면 페이드인 없이 바로 출력
		m_DelTime = 0; // 제거 틱.
		m_FadeOutSpeed = 0; // 제거시 페이드아웃 여부 및 스피드(ms) 이게 0이면 페이드아웃 없이 바로 제거

		m_PosX = 0; // 출력 X 위치
		m_PosY = 0; // 출력 Y 위치
		m_Width = 0; // 가로 크기
		m_Height = 0; // 세로 크기

		m_State = 0; // 이 이미지 출력의 상태.
		m_ContainerIndex = -1;
	}
};

struct sSceneData
{
	int m_Index; // 씬의 인덱스
	RECT m_BoxArea; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
	RECT m_TextArea; // 대화창에서 텍스트가 보여지는 영역. (x,y와 w,h임에 유의)
	int m_TextSpeed; // 텍스트 진행 속도. ms 단위.
	int m_WaitTime; // 이 씬이 시작하기 전에 기달려주는 시간. 초 단위.
	int m_WaitAction; // 이 수치가 0 보다 크다면 해당 액션(이벤트 연출이나 사용자 입력을 기다려야 하는 종류의 액션)이 끝난후 대화가 진행된다.
	int m_FadeInSpeed; // 이 수치가 0 보다 크다면 씬이 시작되고 끝날때 페이드 효과를 넣어준다. 페이드 효과의 속도.
	int m_FadeOutSpeed; // 이 수치가 0 보다 크다면 씬이 시작되고 끝날때 페이드 효과를 넣어준다. 페이드 효과의 속도.

	char *m_strContents; // 대화 내용.

	char *m_strSelection1; // 선택지 1. (만약 선택지 1의 내용이 <ENTER> 라면 엔터로 넘기기...<NEXT> 라면 무 선택으로 넘기기다. <END> 면 스크립트 종료. )
	int m_LinkSel1; // 선택지 1의 링크 인덱스.(이 선택지를 선택했을 경우. 이동하는 씬의 번호)
	char *m_strSelection2; // 선택지 2.
	int m_LinkSel2; // 선택지 2의 링크 인덱스.(이 선택지를 선택했을 경우. 이동하는 씬의 번호)
	char *m_strSelection3; // 선택지 3.
	int m_LinkSel3; // 선택지 3의 링크 인덱스.(이 선택지를 선택했을 경우. 이동하는 씬의 번호)

	list<sImageOutInfo*>		m_listImageOut; // 이미지 출력 정보.

	DWORD m_SceneStartTick; // 씬이 시작된 틱을 기억. 이 틱에 맞춰서 이미지등을 출력

	sSceneData()
	{
		m_strContents = NULL; // 대화 내용.

		m_strSelection1 = NULL; // 선택지 1. (만약 선택지 1의 내용이 <ENTER> 라면 엔터로 넘기기...<NEXT> 라면 무 선택으로 넘기기다. <END> 면 스크립트 종료. )
		m_strSelection2 = NULL; // 선택지 2.
		m_strSelection3 = NULL; // 선택지 3.

		Init();
	}

	~sSceneData()
	{
		Init();
	}

	void Init()
	{
		m_Index = -1; // 씬의 인덱스

		m_BoxArea.left = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_BoxArea.top = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_BoxArea.right = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_BoxArea.bottom = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)

		m_TextArea.left = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_TextArea.top = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_TextArea.right = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_TextArea.bottom = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)

		m_TextSpeed = 0; // 텍스트 진행 속도. ms 단위.
		m_WaitTime = 0; // 이 씬이 시작하기 전에 기달려주는 시간. 초 단위.
		m_WaitAction = -1; // 이 수치가 0 보다 크다면 해당 액션(이벤트 연출이나 사용자 입력을 기다려야 하는 종류의 액션)이 끝난후 대화가 진행된다.
		m_FadeInSpeed = 0; // 이 수치가 0 보다 크다면 씬이 시작되고 끝날때 페이드 효과를 넣어준다. 페이드 효과의 속도.
		m_FadeOutSpeed = 0; // 이 수치가 0 보다 크다면 씬이 시작되고 끝날때 페이드 효과를 넣어준다. 페이드 효과의 속도.

		if( m_strContents )
			SAFE_DELETE_ARRAY( m_strContents );

		if( m_strSelection1 )
			SAFE_DELETE_ARRAY( m_strSelection1 );
		m_LinkSel1 = -1; // 선택지 1의 링크 인덱스.(이 선택지를 선택했을 경우. 이동하는 씬의 번호)
		if( m_strSelection2 )
			SAFE_DELETE_ARRAY( m_strSelection2 );
		m_LinkSel2 = -1; // 선택지 1의 링크 인덱스.(이 선택지를 선택했을 경우. 이동하는 씬의 번호)
		if( m_strSelection3 )
			SAFE_DELETE_ARRAY( m_strSelection3 );
		m_LinkSel3 = -1; // 선택지 1의 링크 인덱스.(이 선택지를 선택했을 경우. 이동하는 씬의 번호)


		sImageOutInfo *pImageOut;
		list<sImageOutInfo*>::iterator prImageOut;
		for( prImageOut = m_listImageOut.begin(); prImageOut != m_listImageOut.end(); ++prImageOut )
		{
			pImageOut = *prImageOut;
			pImageOut->Init(); // 이걸 일일이 해줘서 메모리를 해제해준다.
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

		m_BoxArea.left = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_BoxArea.top = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_BoxArea.right = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_BoxArea.bottom = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)

		m_TextArea.left = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_TextArea.top = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_TextArea.right = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
		m_TextArea.bottom = -1; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)

		sSceneData *pScene;
		list<sSceneData*>::iterator prScene;
		for( prScene = m_listScene.begin(); prScene != m_listScene.end(); ++prScene )
		{
			pScene = *prScene;

			pScene->Init(); // 이걸 일일이 해줘서 메모리를 해제해준다.
			SAFE_DELETE( pScene );
		}
		m_listScene.clear();
	}

	BOOL LoadScript( const char* FileName );
	void SaveScript( const char* FileName ); // .scr 로 저장

	void SaveScriptToText( const char* FileName ); // 텍스트로 저장
	void ImportScriptFromText( const char* FileName ); // 텍스트로부터 읽어와서 구성

	void RunScript();

	char *m_strBoxFileName; // 대화창 이미지 파일명
	RECT m_BoxArea; // 대화창 그래픽 영역. 창의 영역임에 주의 (x,y와 w,h임에 유의)
	RECT m_TextArea; // 대화창에서 텍스트가 보여지는 영역. (x,y와 w,h임에 유의)
	list<sSceneData*>		m_listScene; // 씬들 정보.
};


#endif