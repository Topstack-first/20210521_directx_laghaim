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
	DWORD FileSize = 0; // ���� �������� ũ��.
	BYTE *FileData = NULL; // ���� ������.

	BYTE *FrontDummyData = NULL; // ���ʿ� �� ���� ������.
	BYTE *LastDummyData = NULL; // ���ʿ� �� ���� ������.

	DWORD i = 0;

	fp = pfopen( FileName, _T("rb"), true ); // �ؽ�Ʈ�� �׳� ���̳ʸ��� �д´�.

	if (!fp)
		return FALSE;

	pfseek( fp, 0, SEEK_END );
	FileSize = pftell( fp );
	pfseek( fp, 0, SEEK_SET );
	//FileSize = filelength( fileno(fp) ); // ���� ũ�⸦ ���Ѵ�.
	FileSize -= HELPSCRIPT_FRONT_DUMMY_SIZE + HELPSCRIPT_LAST_DUMMY_SIZE; // ���̸� �� ����� ������ ������ ������.

	FileData = new BYTE[FileSize];
	FrontDummyData = new BYTE[HELPSCRIPT_FRONT_DUMMY_SIZE]; // ���̵� �о���ϴ� �޸𸮸� �ϴ� ��´�.
	LastDummyData = new BYTE[HELPSCRIPT_LAST_DUMMY_SIZE];

	pfread( FrontDummyData, sizeof(BYTE), HELPSCRIPT_FRONT_DUMMY_SIZE, fp ); // ���̺� ����.
	pfread( FileData, sizeof(BYTE), FileSize, fp ); // ��°�� �о�´�.
	pfread( LastDummyData, sizeof(BYTE), HELPSCRIPT_LAST_DUMMY_SIZE, fp ); // ���̺� ����.

	pfclose( fp );

	// ����
	FILE* Tempfp = NULL;
	Tempfp = fopen( "temp.txt", _T("wb") ); // �ؽ�Ʈ�� �׳� ���̳ʸ��� ����.

	if( Tempfp )
	{
		for( i = 0 ; i < FileSize ; ++i ) // ��ȣȭ�� Ǭ��.
		{
			FileData[i] ^= HELPSCRIPT_ENCODE_BYTE;
		}

		fwrite( FileData, sizeof(BYTE), FileSize, Tempfp ); // ��ȣȭ ������ ������ �θ� ��°�� �ִ´�.

		fclose( Tempfp );

	}

	delete [] FileData;
	delete [] FrontDummyData;
	delete [] LastDummyData;


	ImportScriptFromText( "temp.txt" );

	remove("temp.txt"); // ������ ������� ���ĵ� �����.

	return TRUE;
}

void CHelpDataManager::SaveDta( const char* FileName ) // .scr �� ����
{
	ExportScriptToText( "temp.txt" );

	int i;
	DWORD FileSize = 0; // ���� �������� ũ��.
	BYTE *FileData = NULL; // ���� ������.

	FILE *fp;
	fp = fopen( "Temp.txt", _T("rb")); // �ؽ�Ʈ�� �׳� ���̳ʸ��� �д´�.

	if (!fp)
		return;

	FileSize = filelength( fileno(fp) ); // ���� ũ�⸦ ���Ѵ�.

	FileData = new BYTE[FileSize];

	fread( FileData, sizeof(BYTE), FileSize, fp ); // ��°�� �о�´�.

	fclose( fp );

	srand( (unsigned)time( NULL ) );

	BYTE *FrontDummyData = NULL; // ���ʿ� �� ���� ������.
	BYTE *LastDummyData = NULL; // ���ʿ� �� ���� ������.

	// ���� �������� ������ ���� �������� ��ȣȭ.
	FrontDummyData = new BYTE[HELPSCRIPT_FRONT_DUMMY_SIZE];
	LastDummyData = new BYTE[HELPSCRIPT_LAST_DUMMY_SIZE];

	for( i = 0 ; i < HELPSCRIPT_FRONT_DUMMY_SIZE ; ++i ) // ���� ������ ������ ����.
	{
		FrontDummyData[i] = rand()%255; // �� ����Ʈ���� �����ϰ� ���̰��� ����.(�׳� �ʱ�ȭ ���� �����Ⱚ ���� 0�� �ܶ� ���� ���� ������ �����ϰ� �̴´�.)
	}
	for( i = 0 ; i < HELPSCRIPT_LAST_DUMMY_SIZE ; ++i ) // ���� ������ ������ ����.
	{
		LastDummyData[i] = rand()%255; // �� ����Ʈ���� �����ϰ� ���̰��� ����.(�׳� �ʱ�ȭ ���� �����Ⱚ ���� 0�� �ܶ� ���� ���� ������ �����ϰ� �̴´�.)
	}
	for( i = 0 ; i < FileSize ; ++i ) // ��ȣȭ
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

	fwrite( FileData, sizeof(BYTE), FileSize, fp ); // ��ȣȭ�� ������ �θ� ��°�� �ִ´�.

	fwrite( LastDummyData, sizeof(BYTE), HELPSCRIPT_LAST_DUMMY_SIZE, fp );

	fclose( fp );

	delete [] FrontDummyData;
	delete [] LastDummyData;
	delete [] FileData;

	remove("temp.txt"); // ������ ������� ���ĵ� �����.
}

void CHelpDataManager::ExportScriptToText( const char* FileName ) // �ؽ�Ʈ�� ����
{
	FILE *fp;
	fp = fopen(FileName, "wt");
	if( fp ) // ���� ���� üũ.
	{
		// �����.
		fprintf( fp, "#HEADER START\n" ); // ������� �������� ǥ��
		fprintf( fp, "#TOTAL_HELP_NUM %d\n", m_listHelp.size() ); // ������ ����.
		fprintf( fp, "#HEADER END\n" ); // ������� ������ ǥ��

		fprintf( fp, "\n" );

		// �� ����Ʈ
		sHelpData *TempHelp;
		list<sHelpData*>::iterator prHelp;

		for( prHelp = m_listHelp.begin(); prHelp != m_listHelp.end(); ++prHelp )
		{
			TempHelp = *prHelp;
			fprintf( fp, "\n" ); // ���� ����ְ� ����.
			fprintf( fp, "#HELP_UNIT START %d\n", TempHelp->m_Index ); // �� ������ ����

			if( TempHelp->m_strConArg && strlen(TempHelp->m_strConArg) )
				fprintf( fp, "#CONDITION %d %d \t%s\n", TempHelp->m_ConditionType, strlen(TempHelp->m_strConArg), TempHelp->m_strConArg ); // ����, ���� ����.
			else
				fprintf( fp, "#CONDITION %d 0 \t<NONE>\n", TempHelp->m_ConditionType ); // ���� ���ڰ� ���� ���.

			fprintf( fp, "#COOL_TIME %d\n", TempHelp->m_CoolTime ); // �� Ÿ��.

			if( TempHelp->m_ExceptionCode > 0 ) // ���ܰ� �ִٸ�...
				fprintf( fp, "#EXCEPTION %d\n", TempHelp->m_ExceptionCode );


			if( TempHelp->m_strSearchTitle && strlen(TempHelp->m_strSearchTitle) )
				fprintf( fp, "#SEARCH_TITLE %d \t%s\n", strlen(TempHelp->m_strSearchTitle), TempHelp->m_strSearchTitle ); // �˻� ��� ǥ�ÿ� ����.
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#SEARCH_TITLE 0 \t<NONE>\n" ); // �ؽ�Ʈ�� ������� <NONE> �� ������ش�.

			if( TempHelp->m_strSearchWord1 && strlen(TempHelp->m_strSearchWord1) )
				fprintf( fp, "#SEARCH_WORD1 %d \t%s\n", strlen(TempHelp->m_strSearchWord1), TempHelp->m_strSearchWord1 ); // �˻��� Ű����
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#SEARCH_WORD1 0 \t<NONE>\n" ); // �ؽ�Ʈ�� ������� <NONE> �� ������ش�.

			if( TempHelp->m_strSearchWord2 && strlen(TempHelp->m_strSearchWord2) )
				fprintf( fp, "#SEARCH_WORD2 %d \t%s\n", strlen(TempHelp->m_strSearchWord2), TempHelp->m_strSearchWord2 ); // �˻��� Ű����
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#SEARCH_WORD2 0 \t<NONE>\n" ); // �ؽ�Ʈ�� ������� <NONE> �� ������ش�.

			if( TempHelp->m_strSearchWord3 && strlen(TempHelp->m_strSearchWord3) )
				fprintf( fp, "#SEARCH_WORD3 %d \t%s\n", strlen(TempHelp->m_strSearchWord3), TempHelp->m_strSearchWord3 ); // �˻��� Ű����
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#SEARCH_WORD3 0 \t<NONE>\n" ); // �ؽ�Ʈ�� ������� <NONE> �� ������ش�.


			if( TempHelp->m_strNormalTalk && strlen(TempHelp->m_strNormalTalk) )
				fprintf( fp, "#NORMAL_TALK %d \t%s\n", strlen(TempHelp->m_strNormalTalk), TempHelp->m_strNormalTalk ); // �� ���� ���. (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#NORMAL_TALK 0 \t<NONE>\n" ); // �ؽ�Ʈ�� ������� <NONE> �� ������ش�.

			if( TempHelp->m_strNormalTalk && strlen(TempHelp->m_strNormalTalk) )
				fprintf( fp, "#NORMAL_TALK %d \t%s\n", strlen(TempHelp->m_strNormalTalk), TempHelp->m_strNormalTalk ); // �� ���� ���. (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#NORMAL_TALK 0 \t<NONE>\n" ); // �ؽ�Ʈ�� ������� <NONE> �� ������ش�.

			if( TempHelp->m_strChaoticTalk && strlen(TempHelp->m_strChaoticTalk) )
				fprintf( fp, "#CHAOTIC_TALK %d \t%s\n", strlen(TempHelp->m_strChaoticTalk), TempHelp->m_strChaoticTalk ); // �� ���� ���. (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#CHAOTIC_TALK 0 \t<NONE>\n" ); // �ؽ�Ʈ�� ������� <NONE> �� ������ش�.


			fprintf( fp, "#IMAGE TOTAL_NUM %d\n", TempHelp->m_listImage.size() ); // �̹��� ��� ����.
			int k = 0;
			char *TempImageName;
			vector<char*>::iterator prImageName;
			for( prImageName = TempHelp->m_listImage.begin(); prImageName != TempHelp->m_listImage.end(); ++prImageName )
			{
				TempImageName = *prImageName;

				if( TempImageName && strlen(TempImageName) ) // ���ϸ��� ����� ������..
					fprintf( fp, "#IMAGE FILE %d %s\n", k, TempImageName ); // �̹��� ���ϸ�
				else
					fprintf( fp, "#IMAGE FILE %d <NONE>\n", k ); // �̹��� ��� ����.

				k++;
			}

			fprintf( fp, "#HELP_UNIT END\n" ); // �� ���� ��
		}

		fclose(fp);
	}
}

void CHelpDataManager::ImportScriptFromText( const char* FileName ) // �ؽ�Ʈ�κ��� �о�ͼ� ����
{
	Init(); // �ʱ�ȭ �ѹ� ���ش�.

	FILE *fp;

	fp = fopen(FileName, "rt");

	if( !fp ) // ���� ���� üũ.
		return;

	int len = 0;
	char strTemp[100]; // ����б�� ����.
	int TempInt = 0;
	sHelpData *TempHelp = NULL; // ���� �Է½ÿ� ����� �ӽÿ� ������.
	char *TempString = NULL;

	while( !feof(fp) )
	{
		fscanf( fp, "%s", strTemp ); // ����� �д´�.

		if( strcmp( strTemp,"#HEADER" ) == 0 ) // ��ũ��Ʈ ���.
		{
			fscanf( fp, "%s", strTemp ); // ���� ���ڸ� �д´�.
			if( strcmp( strTemp,"START" ) == 0 ) // ����� ����.
			{
				// ��� ���� ���� ���� ������ �� ���� �ʿ䰡 ����.
			}
			else if( strcmp( strTemp,"END" ) == 0 ) // ����� ��.
			{
				// ��� ���� ���� ���� ������ �� ���� �ʿ䰡 ����.
			}
		}
		else if( strcmp( strTemp,"#TOTAL_HELP_NUM" ) == 0 ) // ������ �� ����.
		{
			fscanf( fp, "%d\n", &m_TotalHelpNum );
		}
		else if( strcmp( strTemp,"#HELP_UNIT" ) == 0 ) // ������ ���۰� ��.
		{
			fscanf( fp, "%s", strTemp ); // ���� ���ڸ� �д´�.
			if( strcmp( strTemp,"START" ) == 0 ) // ����� ����.
			{
				// ���ο� ���� �Է��� �غ� �Ѵ�.
				fscanf( fp, "%d\n", &TempInt ); // �� �ε���

				TempHelp = new sHelpData(); // ������ �ȵǾ��־ �������. �̳༮�� ������ ������ ����Ʈ �ı� �ÿ� �Ͼ���Ѵ�.
				if( !TempHelp )
				{
					MessageBox( NULL, "New Help Create Failed! Memory Low", "Error", MB_OK );
					fclose( fp );
					return;
				}

				TempHelp->m_Index = TempInt; // �ε��� ����.
			}
			else if( strcmp( strTemp,"END" ) == 0 ) // ����� ��.
			{
				// ���� ����Ʈ�� �߰��Ѵ�.
				m_listHelp.push_back( TempHelp );
				TempHelp = NULL; // �޸� ���� ���� ���� �ʱ�ȭ�� ���ش�.
			}
		}
		else if( strcmp( strTemp,"#CONDITION" ) == 0 ) // �ߵ� ����.
		{
			if( TempHelp ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d %d ", &TempHelp->m_ConditionType, &TempInt );

				if( TempInt > 0 )
				{
					TempHelp->m_strConArg = new char[TempInt+1];

					if( TempHelp->m_strConArg )
					{
						fgets( TempHelp->m_strConArg, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempHelp->m_strConArg[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#COOL_TIME" ) == 0 ) // �� Ÿ��.
		{
			if( TempHelp ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d ", &TempHelp->m_CoolTime );
			}
		}
		else if( strcmp( strTemp,"#EXCEPTION" ) == 0 ) // �����ڵ�.
		{
			if( TempHelp ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d ", &TempHelp->m_ExceptionCode );
			}
		}
		else if( strcmp( strTemp,"#SEARCH_TITLE" ) == 0 ) // �˻� ��� ǥ�ÿ� ����.
		{
			if( TempHelp ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempHelp->m_strSearchTitle = new char[TempInt+1];

					if( TempHelp->m_strSearchTitle )
					{
						fgets( TempHelp->m_strSearchTitle, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempHelp->m_strSearchTitle[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SEARCH_WORD1" ) == 0 ) // �˻� ��� ǥ�ÿ� ����.
		{
			if( TempHelp ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempHelp->m_strSearchWord1 = new char[TempInt+1];

					if( TempHelp->m_strSearchWord1 )
					{
						fgets( TempHelp->m_strSearchWord1, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempHelp->m_strSearchWord1[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SEARCH_WORD2" ) == 0 ) // �˻� ��� ǥ�ÿ� ����.
		{
			if( TempHelp ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempHelp->m_strSearchWord2 = new char[TempInt+1];

					if( TempHelp->m_strSearchWord2 )
					{
						fgets( TempHelp->m_strSearchWord2, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempHelp->m_strSearchWord2[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SEARCH_WORD3" ) == 0 ) // �˻� ��� ǥ�ÿ� ����.
		{
			if( TempHelp ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempHelp->m_strSearchWord3 = new char[TempInt+1];

					if( TempHelp->m_strSearchWord3 )
					{
						fgets( TempHelp->m_strSearchWord3, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempHelp->m_strSearchWord3[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#NORMAL_TALK" ) == 0 ) // ��� ���� ����.
		{
			if( TempHelp ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempHelp->m_strNormalTalk = new char[TempInt+1];

					if( TempHelp->m_strNormalTalk )
					{
						fgets( TempHelp->m_strNormalTalk, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempHelp->m_strNormalTalk[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#CHAOTIC_TALK" ) == 0 ) // ��� ���� ����.
		{
			if( TempHelp ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempHelp->m_strChaoticTalk = new char[TempInt+1];

					if( TempHelp->m_strChaoticTalk )
					{
						fgets( TempHelp->m_strChaoticTalk, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempHelp->m_strChaoticTalk[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#IMAGE" ) == 0 ) // �̹��� ��� ����Ʈ
		{
			fscanf( fp, "%s", strTemp ); // ���� ���ڸ� �д´�.
			if( strcmp( strTemp,"TOTAL_NUM" ) == 0 ) // �̹��� ����Ʈ�� ����.
			{
				fscanf( fp, "%d\n", &TempInt ); // ���� ������ �ʴ´�.
			}
			else if( strcmp( strTemp,"FILE" ) == 0 ) // �̹��� ���ϸ�
			{
				fscanf( fp, "%d %s\n", &TempInt, strTemp ); // ���ϸ�. ���ϸ��� ���̰� Ŀ���ڴϱ� �� ���ۿ� �о �ű��.

				len = strlen(strTemp);
				if( len > 98 ) // ���ϸ��� �ʹ� ���...
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
					// �̹����� ���� �߰��Ѵ�.
					TempHelp->m_listImage.push_back( TempString );
				}
				TempString = NULL; // �޸� ���� ���� ���� �ʱ�ȭ�� ���ش�.
			}
		}
	}

	fclose( fp );
}

