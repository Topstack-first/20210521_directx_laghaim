#include "stdafx.h"

//#include "stdafx.h"

#include <tchar.h>
#include <io.h>

#include <time.h>

#include <Def.h>
#include "EventScript.h"


BOOL CScriptManager::LoadScript( const char* FileName )
{
	PFILE *fp;
	DWORD FileSize = 0; // 원본 데이터의 크기.
	BYTE *FileData = NULL; // 원본 데이터.

	BYTE *FrontDummyData = NULL; // 앞쪽에 들어갈 더미 데이터.
	BYTE *LastDummyData = NULL; // 뒤쪽에 들어갈 더미 데이터.

	DWORD i = 0;

	fp = pfopen( FileName, _T("rb"), true ); // 텍스트라도 그냥 바이너리로 읽는다.

	if (!fp)
		return FALSE;

	pfseek( fp, 0, SEEK_END );
	FileSize = pftell( fp );
	pfseek( fp, 0, SEEK_SET );
	//FileSize = filelength( fileno(fp) ); // 파일 크기를 구한다.
	FileSize -= SCRIPT_FRONT_DUMMY_SIZE + SCRIPT_LAST_DUMMY_SIZE; // 더미를 뺀 사이즈가 원본의 데이터 사이즈.

	FileData = new BYTE[FileSize];
	FrontDummyData = new BYTE[SCRIPT_FRONT_DUMMY_SIZE]; // 더미도 읽어야하니 메모리를 일단 잡는다.
	LastDummyData = new BYTE[SCRIPT_LAST_DUMMY_SIZE];

	pfread( FrontDummyData, sizeof(BYTE), SCRIPT_FRONT_DUMMY_SIZE, fp ); // 더미부 리드.
	pfread( FileData, sizeof(BYTE), FileSize, fp ); // 통째로 읽어온다.
	pfread( LastDummyData, sizeof(BYTE), SCRIPT_LAST_DUMMY_SIZE, fp ); // 더미부 리드.

	pfclose( fp );

	// 저장
	FILE* tempfp = NULL;
	tempfp = fopen( "temp.txt", _T("wb") ); // 텍스트라도 그냥 바이너리로 쓴다.

	if( tempfp )
	{
		for( i = 0 ; i < FileSize ; ++i ) // 암호화를 푼다.
		{
			FileData[i] ^= SCRIPT_ENCODE_BYTE;
		}

		fwrite( FileData, sizeof(BYTE), FileSize, tempfp ); // 암호화 해제된 데이터 부를 통째로 넣는다.

		fclose( tempfp );

	}

	delete [] FileData;
	delete [] FrontDummyData;
	delete [] LastDummyData;


	ImportScriptFromText( "temp.txt" );

	remove("temp.txt"); // 기존의 라그하임 런쳐도 지운다.


	return TRUE;
}

void CScriptManager::SaveScript( const char* FileName ) // .scr 로 저장
{
	SaveScriptToText( "temp.txt" );

	int i;
	DWORD FileSize = 0; // 원본 데이터의 크기.
	BYTE *FileData = NULL; // 원본 데이터.

	FILE *fp;
	fp = fopen( "Temp.txt", _T("rb")); // 텍스트라도 그냥 바이너리로 읽는다.

	if (!fp)
		return;

	FileSize = filelength( fileno(fp) ); // 파일 크기를 구한다.

	FileData = new BYTE[FileSize];

	fread( FileData, sizeof(BYTE), FileSize, fp ); // 통째로 읽어온다.

	fclose( fp );

	srand( (unsigned)time( NULL ) );

	BYTE *FrontDummyData = NULL; // 앞쪽에 들어갈 더미 데이터.
	BYTE *LastDummyData = NULL; // 뒤쪽에 들어갈 더미 데이터.

	// 더미 데이터의 생성과 원본 데이터의 암호화.
	FrontDummyData = new BYTE[SCRIPT_FRONT_DUMMY_SIZE];
	LastDummyData = new BYTE[SCRIPT_LAST_DUMMY_SIZE];

	for( i = 0 ; i < SCRIPT_FRONT_DUMMY_SIZE ; ++i ) // 앞쪽 더미의 랜덤값 생성.
	{
		FrontDummyData[i] = rand()%255; // 각 바이트별로 랜덤하게 더미값을 셋팅.(그냥 초기화 안한 쓰레기값 사용시 0만 잔뜩 있을 수도 있으니 랜덤하게 뽑는다.)
	}
	for( i = 0 ; i < SCRIPT_LAST_DUMMY_SIZE ; ++i ) // 뒤쪽 더미의 랜덤값 생성.
	{
		LastDummyData[i] = rand()%255; // 각 바이트별로 랜덤하게 더미값을 셋팅.(그냥 초기화 안한 쓰레기값 사용시 0만 잔뜩 있을 수도 있으니 랜덤하게 뽑는다.)
	}
	for( i = 0 ; i < FileSize ; ++i ) // 암호화
	{
		FileData[i] ^= SCRIPT_ENCODE_BYTE;
	}

	fp = fopen( FileName, _T("wb") );

	if(!fp)
	{
		delete [] FrontDummyData;
		delete [] LastDummyData;
		delete [] FileData;

		return;
	}

	fwrite( FrontDummyData, sizeof(BYTE), SCRIPT_FRONT_DUMMY_SIZE, fp );

	fwrite( FileData, sizeof(BYTE), FileSize, fp ); // 암호화된 데이터 부를 통째로 넣는다.

	fwrite( LastDummyData, sizeof(BYTE), SCRIPT_LAST_DUMMY_SIZE, fp );

	fclose( fp );

	delete [] FrontDummyData;
	delete [] LastDummyData;
	delete [] FileData;

	remove("temp.txt"); // 기존의 라그하임 런쳐도 지운다.
}

void CScriptManager::SaveScriptToText( const char* FileName ) // 텍스트로 저장
{
	FILE *fp;
	fp = fopen(FileName, "wt");
	if( fp ) // 파일 존재 체크.
	{
		// 헤더부.
		fprintf( fp, "#HEADER START\n" ); // 헤더부의 시작입을 표시

		if( m_strBoxFileName && strlen(m_strBoxFileName) )
			fprintf( fp, "#TALKBOX_FILE %s\n", m_strBoxFileName ); // 기본 대화창 파일명
		else
			fprintf( fp, "#TALKBOX_FILE <NONE>\n" ); // 기본 대화창 파일명
		fprintf( fp, "#DEFAULT_BOX_AREA %d %d %d %d\n", m_BoxArea.left, m_BoxArea.top, m_BoxArea.right, m_BoxArea.bottom ); // 대화창 영역.
		fprintf( fp, "#DEFAULT_TALK_AREA %d %d %d %d\n", m_TextArea.left, m_TextArea.top, m_TextArea.right, m_TextArea.bottom ); // 대화창 대사 영역.
		fprintf( fp, "#SCENE_NUM %d\n", m_listScene.size() ); // 씬의 갯수.

		fprintf( fp, "#HEADER END\n" ); // 헤더부의 끝입을 표시

		fprintf( fp, "\n" );

		// 씬 리스트
		sSceneData *TempScene;
		list<sSceneData*>::iterator prScene;

		for( prScene = m_listScene.begin(); prScene != m_listScene.end(); ++prScene )
		{
			TempScene = *prScene;
			fprintf( fp, "\n" ); // 한줄 띄워주고 시작.
			fprintf( fp, "#SCENE START %d\n", TempScene->m_Index ); // 한 씬의 시작
			fprintf( fp, "#BOX_AREA %d %d %d %d\n", TempScene->m_BoxArea.left, TempScene->m_BoxArea.top, TempScene->m_BoxArea.right, TempScene->m_BoxArea.bottom ); // 대화창 영역.
			fprintf( fp, "#TALK_AREA %d %d %d %d\n", TempScene->m_TextArea.left, TempScene->m_TextArea.top, TempScene->m_TextArea.right, TempScene->m_TextArea.bottom ); // 대화창 대사 영역.
			fprintf( fp, "#TALK_SPEED %d\n", TempScene->m_TextSpeed ); // 대화 진행 속도.
			fprintf( fp, "#WAIT_TIME %d\n", TempScene->m_WaitTime ); // 이 씬이 시작되기 직전 기다려주기 위한 시간
			fprintf( fp, "#WAIT_ACTION %d\n", TempScene->m_WaitAction ); // 이 씬이 시작되기 직전 기다려줘야하는 이벤트.(내부에서 리턴해줘야 진행되는 이벤트)
			fprintf( fp, "#FADEIN_SPEED %d\n", TempScene->m_FadeInSpeed ); // 이 씬이 시작되고 끝날때 페이드인 페이드 아웃의 속도.
			fprintf( fp, "#FADEOUT_SPEED %d\n", TempScene->m_FadeOutSpeed ); // 이 씬이 시작되고 끝날때 페이드인 페이드 아웃의 속도.
			if( TempScene->m_strContents && strlen(TempScene->m_strContents) )
				fprintf( fp, "#TALK %d \t%s\n", strlen(TempScene->m_strContents), TempScene->m_strContents ); // 이 씬의 대사. (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#TALK 0 \t<NONE>\n" ); // 텍스트가 없을경우 <NONE> 을 출력해준다.

			if( TempScene->m_strSelection1 && strlen(TempScene->m_strSelection1) )
				fprintf( fp, "#SELECTION1 %d %d \t%s\n", TempScene->m_LinkSel1, strlen(TempScene->m_strSelection1), TempScene->m_strSelection1 ); // 이 선택지1 (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#SELECTION1 -1 0 \t<NONE>\n" ); // 이 선택지1 (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)

			if( TempScene->m_strSelection2 && strlen(TempScene->m_strSelection2) )
				fprintf( fp, "#SELECTION2 %d %d \t%s\n", TempScene->m_LinkSel2, strlen(TempScene->m_strSelection2), TempScene->m_strSelection2 ); // 이 선택지1 (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#SELECTION2 -1 0 \t<NONE>\n" ); // 이 선택지2 (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)

			if( TempScene->m_strSelection3 && strlen(TempScene->m_strSelection3) )
				fprintf( fp, "#SELECTION3 %d %d \t%s\n", TempScene->m_LinkSel3, strlen(TempScene->m_strSelection3), TempScene->m_strSelection3 ); // 이 선택지1 (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#SELECTION3 -1 0 \t<NONE>\n" ); // 이 선택지3 (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)

			int k = 0;
			sImageOutInfo *TempImage;
			list<sImageOutInfo*>::iterator prImage;
			for( prImage = TempScene->m_listImageOut.begin(); prImage != TempScene->m_listImageOut.end(); ++prImage )
			{
				TempImage = *prImage;

				fprintf( fp, "#IMAGE START\n" ); // 이미지 출력 갯수.
				if( TempImage->m_strFileName && strlen(TempImage->m_strFileName) ) // 파일명이 제대로 없으면..
					fprintf( fp, "#IMAGE FILE %s\n", TempImage->m_strFileName ); // 이미지 파일명
				else
					fprintf( fp, "#IMAGE FILE <NONE>\n" ); // 이미지 출력 갯수.

				fprintf( fp, "#IMAGE CREATE %d\n", TempImage->m_CreateTime ); // 생성 틱
				fprintf( fp, "#IMAGE FADE_IN %d\n", TempImage->m_FadeInSpeed ); // 페이드인 스피드
				fprintf( fp, "#IMAGE DELETE %d\n", TempImage->m_DelTime ); // 저거 틱
				fprintf( fp, "#IMAGE FADE_OUT %d\n", TempImage->m_FadeOutSpeed ); // 페이드아웃 스피드
				fprintf( fp, "#IMAGE AREA %d %d %d %d\n", TempImage->m_PosX, TempImage->m_PosY, TempImage->m_Width, TempImage->m_Height ); // 이미지 출력 영역.(640 기준)
				fprintf( fp, "#IMAGE END\n" ); // 이미지 부 종료.

				k++;
			}

			fprintf( fp, "#SCENE END\n" ); // 한 씬의 끝
		}

		fclose(fp);
	}
}

void CScriptManager::ImportScriptFromText( const char* FileName ) // 텍스트로부터 읽어와서 구성
{
	Init(); // 초기화 한번 해준다.

	FILE *fp;
	fp = fopen(FileName, "rt");
	if( !fp ) // 파일 존재 체크.
		return;

	int len = 0;
	char strTemp[100]; // 헤더읽기용 버퍼.
	int TempInt = 0;
	sSceneData *TempScene = NULL; // 씬 입력시에 사용할 임시용 포인터.
	sImageOutInfo *TempImage = NULL; // 씬 입력시에 사용할 임시용 포인터.

	while( !feof(fp) )
	{
		fscanf( fp, "%s", strTemp ); // 헤더를 읽는다.

		if( strcmp( strTemp,"#HEADER" ) == 0 ) // 스크립트 헤더.
		{
			fscanf( fp, "%s", strTemp ); // 구분 인자를 읽는다.
			if( strcmp( strTemp,"START" ) == 0 ) // 헤더의 시작.
			{
				// 헤더 시작 끝은 아직 별도로 뭘 해줄 필요가 없다.
			}
			else if( strcmp( strTemp,"END" ) == 0 ) // 헤더의 끝.
			{
				// 헤더 시작 끝은 아직 별도로 뭘 해줄 필요가 없다.
			}
		}
		else if( strcmp( strTemp,"#TALKBOX_FILE" ) == 0 ) // 대화창 그래픽 파일 지정.
		{
			fscanf( fp, "%s\n", strTemp ); // 파일명. 파일명은 길이가 커봤자니까 이 버퍼에 읽어서 옮긴다.

			len = strlen(strTemp);
			if( len > 98 ) // 파일명이 너무 길면...
			{
				MessageBox( NULL, "TALKBOX_FILE is long", "Error", MB_OK );
				fclose(fp);
				return;
			}

			if( strcmp( strTemp,"<NONE>" ) == 0 ) // 그래픽 파일이 지정안된 경우.
			{
				// 일단 아직 안한 경우는 처리 없음.
			}
			else
			{
				m_strBoxFileName = new char[len+1];

				if( m_strBoxFileName )
				{
					strcpy( m_strBoxFileName, strTemp );
					m_strBoxFileName[len] = '\0';
				}
			}
		}
		else if( strcmp( strTemp,"#DEFAULT_BOX_AREA" ) == 0 ) // 대화창 그래픽 영역 지정.
		{
			fscanf( fp, "%d %d %d %d\n", &m_BoxArea.left, &m_BoxArea.top, &m_BoxArea.right, &m_BoxArea.bottom );
		}
		else if( strcmp( strTemp,"#DEFAULT_TALK_AREA" ) == 0 ) // 대화창 대사 영역 지정.
		{
			fscanf( fp, "%d %d %d %d\n", &m_TextArea.left, &m_TextArea.top, &m_TextArea.right, &m_TextArea.bottom );
		}
		else if( strcmp( strTemp,"#SCENE_NUM" ) == 0 ) // 씬의 갯수.
		{
			fscanf( fp, "%d\n", &TempInt ); // 아직 특별히 필요없다.
		}
		else if( strcmp( strTemp,"#SCENE" ) == 0 ) // 씬의 시작과 끝.
		{
			fscanf( fp, "%s", strTemp ); // 구분 인자를 읽는다.
			if( strcmp( strTemp,"START" ) == 0 ) // 헤더의 시작.
			{
				// 새로운 씬을 입력할 준비를 한다.
				fscanf( fp, "%d\n", &TempInt ); // 씬 인덱스

				TempScene = new sSceneData(); // 헤제가 안되어있어도 상관없다. 이녀석의 실질적 해제는 리스트 파괴 시에 일어나야한다.
				if( !TempScene )
				{
					MessageBox( NULL, "New Scene Create Failed! Memory Low", "Error", MB_OK );
					fclose(fp);
					return;
				}

				TempScene->m_Index = TempInt; // 인덱스 셋팅.
			}
			else if( strcmp( strTemp,"END" ) == 0 ) // 헤더의 끝.
			{
				// 씬을 리스트에 추가한다.
				m_listScene.push_back( TempScene );
				TempScene = NULL; // 메모리 해제 하지 말고 초기화만 해준다.
			}
		}
		else if( strcmp( strTemp,"#BOX_AREA" ) == 0 ) // 대화창 그래픽 영역 지정.
		{
			if( TempScene ) // 있는 없는지 확인
			{
				fscanf( fp, "%d %d %d %d\n", &TempScene->m_BoxArea.left, &TempScene->m_BoxArea.top, &TempScene->m_BoxArea.right, &TempScene->m_BoxArea.bottom );
			}
		}
		else if( strcmp( strTemp,"#TALK_AREA" ) == 0 ) // 대화창 대사 영역 지정.
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d %d %d %d\n", &TempScene->m_TextArea.left, &TempScene->m_TextArea.top, &TempScene->m_TextArea.right, &TempScene->m_TextArea.bottom );
			}
		}
		else if( strcmp( strTemp,"#TALK_SPEED" ) == 0 ) // 대사 진행 속도 지정.
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d\n", &TempScene->m_TextSpeed );
			}
		}
		else if( strcmp( strTemp,"#WAIT_TIME" ) == 0 ) // 대화 시작 전 대기시간 지정.
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d\n", &TempScene->m_WaitTime );
			}
		}
		else if( strcmp( strTemp,"#WAIT_ACTION" ) == 0 ) // 대화 시작 전 대기 이벤트 지정.
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d\n", &TempScene->m_WaitAction );
			}
		}
		else if( strcmp( strTemp,"#FADEIN_SPEED" ) == 0 ) // 대화 시작전 페이드인 여부 및 속도
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d\n", &TempScene->m_FadeInSpeed );
			}
		}
		else if( strcmp( strTemp,"#FADEOUT_SPEED" ) == 0 ) // 대화 종료시 페이드아웃 여부 및 속도
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d\n", &TempScene->m_FadeOutSpeed );
			}
		}
		else if( strcmp( strTemp,"#TALK" ) == 0 ) // 대사 내용 지정.
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempScene->m_strContents = new char[TempInt+1];

					if( TempScene->m_strContents )
					{
						fgets( TempScene->m_strContents, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempScene->m_strContents[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SELECTION1" ) == 0 ) // 선택지1 설정.
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d", &TempScene->m_LinkSel1 );
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempScene->m_strSelection1 = new char[TempInt+1];

					if( TempScene->m_strSelection1 )
					{
						fgets( TempScene->m_strSelection1, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempScene->m_strSelection1[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SELECTION2" ) == 0 ) // 선택지2 설정.
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d", &TempScene->m_LinkSel2 );
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempScene->m_strSelection2 = new char[TempInt+1];

					if( TempScene->m_strSelection2 )
					{
						fgets( TempScene->m_strSelection2, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempScene->m_strSelection2[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SELECTION3" ) == 0 ) // 선택지3 설정.
		{
			if( TempScene ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d", &TempScene->m_LinkSel3 );
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempScene->m_strSelection3 = new char[TempInt+1];

					if( TempScene->m_strSelection3 )
					{
						fgets( TempScene->m_strSelection3, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempScene->m_strSelection3[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#IMAGE" ) == 0 ) // 이미지 출력 리스트
		{
			fscanf( fp, "%s", strTemp ); // 구분 인자를 읽는다.
			if( strcmp( strTemp,"START" ) == 0 ) // 이미지 리스트의 시작.
			{
//				fscanf( fp, "%d\n", &TempInt ); // 이미지들 갯수. 현재로선 따로 안쓰임.

				TempImage = new sImageOutInfo(); // 헤제가 안되어있어도 상관없다. 이녀석의 실질적 해제는 리스트 파괴 시에 일어나야한다.
				if( !TempImage )
				{
					MessageBox( NULL, "New ImageInfo Create Failed! Memory Low", "Error", MB_OK );
					fclose(fp);
					return;
				}
			}
			else if( strcmp( strTemp,"END" ) == 0 ) // 이미지 리스트 의 끝.
			{
				if( TempScene )
				{
					// 이미지를 씬에 추가한다.
					TempScene->m_listImageOut.push_back( TempImage );
				}
				TempImage = NULL; // 메모리 해제 하지 말고 초기화만 해준다.
			}
			else if( strcmp( strTemp,"FILE" ) == 0 ) // 이미지 파일명
			{
				if( TempImage ) // 있는지 없는지 확인
				{
					fscanf( fp, "%s\n", strTemp ); // 파일명. 파일명은 길이가 커봤자니까 이 버퍼에 읽어서 옮긴다.

					len = strlen(strTemp);
					if( len > 98 ) // 파일명이 너무 길면...
					{
						MessageBox( NULL, "IMAGE FILE Name is long", "Error", MB_OK );
						fclose(fp);
						return;
					}

					TempImage->m_strFileName = new char[len+1];

					if( TempImage->m_strFileName )
					{
						strcpy( TempImage->m_strFileName, strTemp );
						TempImage->m_strFileName[len] = '\0';
					}
				}
			}
			else if( strcmp( strTemp,"CREATE" ) == 0 ) // 이미지 생성 시간
			{
				if( TempImage ) // 있는지 없는지 확인
				{
					fscanf( fp, "%d\n", &TempImage->m_CreateTime );
				}
			}
			else if( strcmp( strTemp,"FADE_IN" ) == 0 ) // 생성중 페이드인 속도.
			{
				if( TempImage ) // 있는지 없는지 확인
				{
					fscanf( fp, "%d\n", &TempImage->m_FadeInSpeed );
				}
			}
			else if( strcmp( strTemp,"DELETE" ) == 0 ) // 이미지 제거 시간
			{
				if( TempImage ) // 있는지 없는지 확인
				{
					fscanf( fp, "%d\n", &TempImage->m_DelTime );
				}
			}
			else if( strcmp( strTemp,"FADE_OUT" ) == 0 ) // 제거중 페이드 아웃 속도.
			{
				if( TempImage ) // 있는지 없는지 확인
				{
					fscanf( fp, "%d\n", &TempImage->m_FadeOutSpeed );
				}
			}
			else if( strcmp( strTemp,"AREA" ) == 0 ) // 이미지 위치 및 크기 정보.
			{
				if( TempImage ) // 있는지 없는지 확인
				{
					fscanf( fp, "%d %d %d %d\n", &TempImage->m_PosX, &TempImage->m_PosY, &TempImage->m_Width, &TempImage->m_Height );
				}
			}
		}
	}

	fclose( fp );
}

void CScriptManager::RunScript()
{
}