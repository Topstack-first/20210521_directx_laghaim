#include "stdafx.h"

//#include "stdafx.h"

#include <tchar.h>
#include <io.h>

#include <time.h>
#include <Def.h>

#include "HelpScript.h"

BOOL CHelpDataManager::LoadDta( const char* FileName )
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
	FileSize -= HELPSCRIPT_FRONT_DUMMY_SIZE + HELPSCRIPT_LAST_DUMMY_SIZE; // 더미를 뺀 사이즈가 원본의 데이터 사이즈.

	FileData = new BYTE[FileSize];
	FrontDummyData = new BYTE[HELPSCRIPT_FRONT_DUMMY_SIZE]; // 더미도 읽어야하니 메모리를 일단 잡는다.
	LastDummyData = new BYTE[HELPSCRIPT_LAST_DUMMY_SIZE];

	pfread( FrontDummyData, sizeof(BYTE), HELPSCRIPT_FRONT_DUMMY_SIZE, fp ); // 더미부 리드.
	pfread( FileData, sizeof(BYTE), FileSize, fp ); // 통째로 읽어온다.
	pfread( LastDummyData, sizeof(BYTE), HELPSCRIPT_LAST_DUMMY_SIZE, fp ); // 더미부 리드.

	pfclose( fp );

	// 저장
	FILE* Tempfp = NULL;
	Tempfp = fopen( "temp.txt", _T("wb") ); // 텍스트라도 그냥 바이너리로 쓴다.

	if( Tempfp )
	{
		for( i = 0 ; i < FileSize ; ++i ) // 암호화를 푼다.
		{
			FileData[i] ^= HELPSCRIPT_ENCODE_BYTE;
		}

		fwrite( FileData, sizeof(BYTE), FileSize, Tempfp ); // 암호화 해제된 데이터 부를 통째로 넣는다.

		fclose( Tempfp );

	}

	delete [] FileData;
	delete [] FrontDummyData;
	delete [] LastDummyData;


	ImportScriptFromText( "temp.txt" );

	remove("temp.txt"); // 기존의 라그하임 런쳐도 지운다.

	return TRUE;
}

void CHelpDataManager::SaveDta( const char* FileName ) // .scr 로 저장
{
	ExportScriptToText( "temp.txt" );

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
	FrontDummyData = new BYTE[HELPSCRIPT_FRONT_DUMMY_SIZE];
	LastDummyData = new BYTE[HELPSCRIPT_LAST_DUMMY_SIZE];

	for( i = 0 ; i < HELPSCRIPT_FRONT_DUMMY_SIZE ; ++i ) // 앞쪽 더미의 랜덤값 생성.
	{
		FrontDummyData[i] = rand()%255; // 각 바이트별로 랜덤하게 더미값을 셋팅.(그냥 초기화 안한 쓰레기값 사용시 0만 잔뜩 있을 수도 있으니 랜덤하게 뽑는다.)
	}
	for( i = 0 ; i < HELPSCRIPT_LAST_DUMMY_SIZE ; ++i ) // 뒤쪽 더미의 랜덤값 생성.
	{
		LastDummyData[i] = rand()%255; // 각 바이트별로 랜덤하게 더미값을 셋팅.(그냥 초기화 안한 쓰레기값 사용시 0만 잔뜩 있을 수도 있으니 랜덤하게 뽑는다.)
	}
	for( i = 0 ; i < FileSize ; ++i ) // 암호화
	{
		FileData[i] ^= HELPSCRIPT_ENCODE_BYTE;
	}

	fp = fopen( FileName, _T("wb") );

	if(!fp)
	{
		delete [] FrontDummyData;
		delete [] LastDummyData;
		delete [] FileData;

		return;
	}

	fwrite( FrontDummyData, sizeof(BYTE), HELPSCRIPT_FRONT_DUMMY_SIZE, fp );

	fwrite( FileData, sizeof(BYTE), FileSize, fp ); // 암호화된 데이터 부를 통째로 넣는다.

	fwrite( LastDummyData, sizeof(BYTE), HELPSCRIPT_LAST_DUMMY_SIZE, fp );

	fclose( fp );

	delete [] FrontDummyData;
	delete [] LastDummyData;
	delete [] FileData;

	remove("temp.txt"); // 기존의 라그하임 런쳐도 지운다.
}

void CHelpDataManager::ExportScriptToText( const char* FileName ) // 텍스트로 저장
{
	FILE *fp;
	fp = fopen(FileName, "wt");
	if( fp ) // 파일 존재 체크.
	{
		// 헤더부.
		fprintf( fp, "#HEADER START\n" ); // 헤더부의 시작입을 표시
		fprintf( fp, "#TOTAL_HELP_NUM %d\n", m_listHelp.size() ); // 헬프의 갯수.
		fprintf( fp, "#HEADER END\n" ); // 헤더부의 끝입을 표시

		fprintf( fp, "\n" );

		// 씬 리스트
		sHelpData *TempHelp;
		list<sHelpData*>::iterator prHelp;

		for( prHelp = m_listHelp.begin(); prHelp != m_listHelp.end(); ++prHelp )
		{
			TempHelp = *prHelp;
			fprintf( fp, "\n" ); // 한줄 띄워주고 시작.
			fprintf( fp, "#HELP_UNIT START %d\n", TempHelp->m_Index ); // 한 헬프의 시작

			if( TempHelp->m_strConArg && strlen(TempHelp->m_strConArg) )
				fprintf( fp, "#CONDITION %d %d \t%s\n", TempHelp->m_ConditionType, strlen(TempHelp->m_strConArg), TempHelp->m_strConArg ); // 조건, 조건 인자.
			else
				fprintf( fp, "#CONDITION %d 0 \t<NONE>\n", TempHelp->m_ConditionType ); // 조건 인자가 없는 경우.

			fprintf( fp, "#COOL_TIME %d\n", TempHelp->m_CoolTime ); // 쿨 타임.

			if( TempHelp->m_ExceptionCode > 0 ) // 예외가 있다면...
				fprintf( fp, "#EXCEPTION %d\n", TempHelp->m_ExceptionCode );


			if( TempHelp->m_strSearchTitle && strlen(TempHelp->m_strSearchTitle) )
				fprintf( fp, "#SEARCH_TITLE %d \t%s\n", strlen(TempHelp->m_strSearchTitle), TempHelp->m_strSearchTitle ); // 검색 결과 표시용 제목.
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#SEARCH_TITLE 0 \t<NONE>\n" ); // 텍스트가 없을경우 <NONE> 을 출력해준다.

			if( TempHelp->m_strSearchWord1 && strlen(TempHelp->m_strSearchWord1) )
				fprintf( fp, "#SEARCH_WORD1 %d \t%s\n", strlen(TempHelp->m_strSearchWord1), TempHelp->m_strSearchWord1 ); // 검색용 키워드
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#SEARCH_WORD1 0 \t<NONE>\n" ); // 텍스트가 없을경우 <NONE> 을 출력해준다.

			if( TempHelp->m_strSearchWord2 && strlen(TempHelp->m_strSearchWord2) )
				fprintf( fp, "#SEARCH_WORD2 %d \t%s\n", strlen(TempHelp->m_strSearchWord2), TempHelp->m_strSearchWord2 ); // 검색용 키워드
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#SEARCH_WORD2 0 \t<NONE>\n" ); // 텍스트가 없을경우 <NONE> 을 출력해준다.

			if( TempHelp->m_strSearchWord3 && strlen(TempHelp->m_strSearchWord3) )
				fprintf( fp, "#SEARCH_WORD3 %d \t%s\n", strlen(TempHelp->m_strSearchWord3), TempHelp->m_strSearchWord3 ); // 검색용 키워드
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#SEARCH_WORD3 0 \t<NONE>\n" ); // 텍스트가 없을경우 <NONE> 을 출력해준다.


			if( TempHelp->m_strNormalTalk && strlen(TempHelp->m_strNormalTalk) )
				fprintf( fp, "#NORMAL_TALK %d \t%s\n", strlen(TempHelp->m_strNormalTalk), TempHelp->m_strNormalTalk ); // 이 씬의 대사. (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#NORMAL_TALK 0 \t<NONE>\n" ); // 텍스트가 없을경우 <NONE> 을 출력해준다.

			if( TempHelp->m_strNormalTalk && strlen(TempHelp->m_strNormalTalk) )
				fprintf( fp, "#NORMAL_TALK %d \t%s\n", strlen(TempHelp->m_strNormalTalk), TempHelp->m_strNormalTalk ); // 이 씬의 대사. (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#NORMAL_TALK 0 \t<NONE>\n" ); // 텍스트가 없을경우 <NONE> 을 출력해준다.

			if( TempHelp->m_strChaoticTalk && strlen(TempHelp->m_strChaoticTalk) )
				fprintf( fp, "#CHAOTIC_TALK %d \t%s\n", strlen(TempHelp->m_strChaoticTalk), TempHelp->m_strChaoticTalk ); // 이 씬의 대사. (텍스트 익스포트를 통한 엑셀 저장 시 번역이 용이하도록 탭으로 문장을 띄워준다.)
			else // 만약 텍스트가 없으면...
				fprintf( fp, "#CHAOTIC_TALK 0 \t<NONE>\n" ); // 텍스트가 없을경우 <NONE> 을 출력해준다.


			fprintf( fp, "#IMAGE TOTAL_NUM %d\n", TempHelp->m_listImage.size() ); // 이미지 출력 갯수.
			int k = 0;
			char *TempImageName;
			vector<char*>::iterator prImageName;
			for( prImageName = TempHelp->m_listImage.begin(); prImageName != TempHelp->m_listImage.end(); ++prImageName )
			{
				TempImageName = *prImageName;

				if( TempImageName && strlen(TempImageName) ) // 파일명이 제대로 없으면..
					fprintf( fp, "#IMAGE FILE %d %s\n", k, TempImageName ); // 이미지 파일명
				else
					fprintf( fp, "#IMAGE FILE %d <NONE>\n", k ); // 이미지 출력 갯수.

				k++;
			}

			fprintf( fp, "#HELP_UNIT END\n" ); // 한 씬의 끝
		}

		fclose(fp);
	}
}

void CHelpDataManager::ImportScriptFromText( const char* FileName ) // 텍스트로부터 읽어와서 구성
{
	Init(); // 초기화 한번 해준다.

	FILE *fp;

	fp = fopen(FileName, "rt");

	if( !fp ) // 파일 존재 체크.
		return;

	int len = 0;
	char strTemp[100]; // 헤더읽기용 버퍼.
	int TempInt = 0;
	sHelpData *TempHelp = NULL; // 헬프 입력시에 사용할 임시용 포인터.
	char *TempString = NULL;

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
		else if( strcmp( strTemp,"#TOTAL_HELP_NUM" ) == 0 ) // 헬프의 총 갯수.
		{
			fscanf( fp, "%d\n", &m_TotalHelpNum );
		}
		else if( strcmp( strTemp,"#HELP_UNIT" ) == 0 ) // 헬프의 시작과 끝.
		{
			fscanf( fp, "%s", strTemp ); // 구분 인자를 읽는다.
			if( strcmp( strTemp,"START" ) == 0 ) // 헤더의 시작.
			{
				// 새로운 씬을 입력할 준비를 한다.
				fscanf( fp, "%d\n", &TempInt ); // 씬 인덱스

				TempHelp = new sHelpData(); // 헤제가 안되어있어도 상관없다. 이녀석의 실질적 해제는 리스트 파괴 시에 일어나야한다.
				if( !TempHelp )
				{
					MessageBox( NULL, "New Help Create Failed! Memory Low", "Error", MB_OK );
					fclose( fp );
					return;
				}

				TempHelp->m_Index = TempInt; // 인덱스 셋팅.
			}
			else if( strcmp( strTemp,"END" ) == 0 ) // 헤더의 끝.
			{
				// 씬을 리스트에 추가한다.
				m_listHelp.push_back( TempHelp );
				TempHelp = NULL; // 메모리 해제 하지 말고 초기화만 해준다.
			}
		}
		else if( strcmp( strTemp,"#CONDITION" ) == 0 ) // 발동 조건.
		{
			if( TempHelp ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d %d ", &TempHelp->m_ConditionType, &TempInt );

				if( TempInt > 0 )
				{
					TempHelp->m_strConArg = new char[TempInt+1];

					if( TempHelp->m_strConArg )
					{
						fgets( TempHelp->m_strConArg, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempHelp->m_strConArg[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#COOL_TIME" ) == 0 ) // 쿨 타임.
		{
			if( TempHelp ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d ", &TempHelp->m_CoolTime );
			}
		}
		else if( strcmp( strTemp,"#EXCEPTION" ) == 0 ) // 예외코드.
		{
			if( TempHelp ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d ", &TempHelp->m_ExceptionCode );
			}
		}
		else if( strcmp( strTemp,"#SEARCH_TITLE" ) == 0 ) // 검색 결과 표시용 제목.
		{
			if( TempHelp ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempHelp->m_strSearchTitle = new char[TempInt+1];

					if( TempHelp->m_strSearchTitle )
					{
						fgets( TempHelp->m_strSearchTitle, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempHelp->m_strSearchTitle[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SEARCH_WORD1" ) == 0 ) // 검색 결과 표시용 제목.
		{
			if( TempHelp ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempHelp->m_strSearchWord1 = new char[TempInt+1];

					if( TempHelp->m_strSearchWord1 )
					{
						fgets( TempHelp->m_strSearchWord1, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempHelp->m_strSearchWord1[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SEARCH_WORD2" ) == 0 ) // 검색 결과 표시용 제목.
		{
			if( TempHelp ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempHelp->m_strSearchWord2 = new char[TempInt+1];

					if( TempHelp->m_strSearchWord2 )
					{
						fgets( TempHelp->m_strSearchWord2, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempHelp->m_strSearchWord2[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SEARCH_WORD3" ) == 0 ) // 검색 결과 표시용 제목.
		{
			if( TempHelp ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempHelp->m_strSearchWord3 = new char[TempInt+1];

					if( TempHelp->m_strSearchWord3 )
					{
						fgets( TempHelp->m_strSearchWord3, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempHelp->m_strSearchWord3[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#NORMAL_TALK" ) == 0 ) // 대사 내용 지정.
		{
			if( TempHelp ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempHelp->m_strNormalTalk = new char[TempInt+1];

					if( TempHelp->m_strNormalTalk )
					{
						fgets( TempHelp->m_strNormalTalk, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempHelp->m_strNormalTalk[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#CHAOTIC_TALK" ) == 0 ) // 대사 내용 지정.
		{
			if( TempHelp ) // 있는지 없는지 확인
			{
				fscanf( fp, "%d ", &TempInt ); // 스트링 길이

				if( TempInt > 0 )
				{
					TempHelp->m_strChaoticTalk = new char[TempInt+1];

					if( TempHelp->m_strChaoticTalk )
					{
						fgets( TempHelp->m_strChaoticTalk, TempInt+1, fp ); // 스트링 (+1해줘야 제대로 받더라.)
						TempHelp->m_strChaoticTalk[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#IMAGE" ) == 0 ) // 이미지 출력 리스트
		{
			fscanf( fp, "%s", strTemp ); // 구분 인자를 읽는다.
			if( strcmp( strTemp,"TOTAL_NUM" ) == 0 ) // 이미지 리스트의 시작.
			{
				fscanf( fp, "%d\n", &TempInt ); // 따로 쓰이진 않는다.
			}
			else if( strcmp( strTemp,"FILE" ) == 0 ) // 이미지 파일명
			{
				fscanf( fp, "%d %s\n", &TempInt, strTemp ); // 파일명. 파일명은 길이가 커봤자니까 이 버퍼에 읽어서 옮긴다.

				len = strlen(strTemp);
				if( len > 98 ) // 파일명이 너무 길면...
				{
					MessageBox( NULL, "IMAGE FILE Name is long", "Error", MB_OK );
					fclose( fp );
					return;
				}

				TempString = new char[len+1];

				if( TempString )
				{
					strcpy( TempString, strTemp );
					TempString[len] = '\0';
				}

				if( TempHelp )
				{
					// 이미지를 씬에 추가한다.
					TempHelp->m_listImage.push_back( TempString );
				}
				TempString = NULL; // 메모리 해제 하지 말고 초기화만 해준다.
			}
		}
	}

	fclose( fp );
}

