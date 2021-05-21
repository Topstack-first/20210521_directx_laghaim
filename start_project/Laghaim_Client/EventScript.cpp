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
	FileSize -= SCRIPT_FRONT_DUMMY_SIZE + SCRIPT_LAST_DUMMY_SIZE; // ���̸� �� ����� ������ ������ ������.

	FileData = new BYTE[FileSize];
	FrontDummyData = new BYTE[SCRIPT_FRONT_DUMMY_SIZE]; // ���̵� �о���ϴ� �޸𸮸� �ϴ� ��´�.
	LastDummyData = new BYTE[SCRIPT_LAST_DUMMY_SIZE];

	pfread( FrontDummyData, sizeof(BYTE), SCRIPT_FRONT_DUMMY_SIZE, fp ); // ���̺� ����.
	pfread( FileData, sizeof(BYTE), FileSize, fp ); // ��°�� �о�´�.
	pfread( LastDummyData, sizeof(BYTE), SCRIPT_LAST_DUMMY_SIZE, fp ); // ���̺� ����.

	pfclose( fp );

	// ����
	FILE* tempfp = NULL;
	tempfp = fopen( "temp.txt", _T("wb") ); // �ؽ�Ʈ�� �׳� ���̳ʸ��� ����.

	if( tempfp )
	{
		for( i = 0 ; i < FileSize ; ++i ) // ��ȣȭ�� Ǭ��.
		{
			FileData[i] ^= SCRIPT_ENCODE_BYTE;
		}

		fwrite( FileData, sizeof(BYTE), FileSize, tempfp ); // ��ȣȭ ������ ������ �θ� ��°�� �ִ´�.

		fclose( tempfp );

	}

	delete [] FileData;
	delete [] FrontDummyData;
	delete [] LastDummyData;


	ImportScriptFromText( "temp.txt" );

	remove("temp.txt"); // ������ ������� ���ĵ� �����.


	return TRUE;
}

void CScriptManager::SaveScript( const char* FileName ) // .scr �� ����
{
	SaveScriptToText( "temp.txt" );

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
	FrontDummyData = new BYTE[SCRIPT_FRONT_DUMMY_SIZE];
	LastDummyData = new BYTE[SCRIPT_LAST_DUMMY_SIZE];

	for( i = 0 ; i < SCRIPT_FRONT_DUMMY_SIZE ; ++i ) // ���� ������ ������ ����.
	{
		FrontDummyData[i] = rand()%255; // �� ����Ʈ���� �����ϰ� ���̰��� ����.(�׳� �ʱ�ȭ ���� �����Ⱚ ���� 0�� �ܶ� ���� ���� ������ �����ϰ� �̴´�.)
	}
	for( i = 0 ; i < SCRIPT_LAST_DUMMY_SIZE ; ++i ) // ���� ������ ������ ����.
	{
		LastDummyData[i] = rand()%255; // �� ����Ʈ���� �����ϰ� ���̰��� ����.(�׳� �ʱ�ȭ ���� �����Ⱚ ���� 0�� �ܶ� ���� ���� ������ �����ϰ� �̴´�.)
	}
	for( i = 0 ; i < FileSize ; ++i ) // ��ȣȭ
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

	fwrite( FileData, sizeof(BYTE), FileSize, fp ); // ��ȣȭ�� ������ �θ� ��°�� �ִ´�.

	fwrite( LastDummyData, sizeof(BYTE), SCRIPT_LAST_DUMMY_SIZE, fp );

	fclose( fp );

	delete [] FrontDummyData;
	delete [] LastDummyData;
	delete [] FileData;

	remove("temp.txt"); // ������ ������� ���ĵ� �����.
}

void CScriptManager::SaveScriptToText( const char* FileName ) // �ؽ�Ʈ�� ����
{
	FILE *fp;
	fp = fopen(FileName, "wt");
	if( fp ) // ���� ���� üũ.
	{
		// �����.
		fprintf( fp, "#HEADER START\n" ); // ������� �������� ǥ��

		if( m_strBoxFileName && strlen(m_strBoxFileName) )
			fprintf( fp, "#TALKBOX_FILE %s\n", m_strBoxFileName ); // �⺻ ��ȭâ ���ϸ�
		else
			fprintf( fp, "#TALKBOX_FILE <NONE>\n" ); // �⺻ ��ȭâ ���ϸ�
		fprintf( fp, "#DEFAULT_BOX_AREA %d %d %d %d\n", m_BoxArea.left, m_BoxArea.top, m_BoxArea.right, m_BoxArea.bottom ); // ��ȭâ ����.
		fprintf( fp, "#DEFAULT_TALK_AREA %d %d %d %d\n", m_TextArea.left, m_TextArea.top, m_TextArea.right, m_TextArea.bottom ); // ��ȭâ ��� ����.
		fprintf( fp, "#SCENE_NUM %d\n", m_listScene.size() ); // ���� ����.

		fprintf( fp, "#HEADER END\n" ); // ������� ������ ǥ��

		fprintf( fp, "\n" );

		// �� ����Ʈ
		sSceneData *TempScene;
		list<sSceneData*>::iterator prScene;

		for( prScene = m_listScene.begin(); prScene != m_listScene.end(); ++prScene )
		{
			TempScene = *prScene;
			fprintf( fp, "\n" ); // ���� ����ְ� ����.
			fprintf( fp, "#SCENE START %d\n", TempScene->m_Index ); // �� ���� ����
			fprintf( fp, "#BOX_AREA %d %d %d %d\n", TempScene->m_BoxArea.left, TempScene->m_BoxArea.top, TempScene->m_BoxArea.right, TempScene->m_BoxArea.bottom ); // ��ȭâ ����.
			fprintf( fp, "#TALK_AREA %d %d %d %d\n", TempScene->m_TextArea.left, TempScene->m_TextArea.top, TempScene->m_TextArea.right, TempScene->m_TextArea.bottom ); // ��ȭâ ��� ����.
			fprintf( fp, "#TALK_SPEED %d\n", TempScene->m_TextSpeed ); // ��ȭ ���� �ӵ�.
			fprintf( fp, "#WAIT_TIME %d\n", TempScene->m_WaitTime ); // �� ���� ���۵Ǳ� ���� ��ٷ��ֱ� ���� �ð�
			fprintf( fp, "#WAIT_ACTION %d\n", TempScene->m_WaitAction ); // �� ���� ���۵Ǳ� ���� ��ٷ�����ϴ� �̺�Ʈ.(���ο��� ��������� ����Ǵ� �̺�Ʈ)
			fprintf( fp, "#FADEIN_SPEED %d\n", TempScene->m_FadeInSpeed ); // �� ���� ���۵ǰ� ������ ���̵��� ���̵� �ƿ��� �ӵ�.
			fprintf( fp, "#FADEOUT_SPEED %d\n", TempScene->m_FadeOutSpeed ); // �� ���� ���۵ǰ� ������ ���̵��� ���̵� �ƿ��� �ӵ�.
			if( TempScene->m_strContents && strlen(TempScene->m_strContents) )
				fprintf( fp, "#TALK %d \t%s\n", strlen(TempScene->m_strContents), TempScene->m_strContents ); // �� ���� ���. (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#TALK 0 \t<NONE>\n" ); // �ؽ�Ʈ�� ������� <NONE> �� ������ش�.

			if( TempScene->m_strSelection1 && strlen(TempScene->m_strSelection1) )
				fprintf( fp, "#SELECTION1 %d %d \t%s\n", TempScene->m_LinkSel1, strlen(TempScene->m_strSelection1), TempScene->m_strSelection1 ); // �� ������1 (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#SELECTION1 -1 0 \t<NONE>\n" ); // �� ������1 (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)

			if( TempScene->m_strSelection2 && strlen(TempScene->m_strSelection2) )
				fprintf( fp, "#SELECTION2 %d %d \t%s\n", TempScene->m_LinkSel2, strlen(TempScene->m_strSelection2), TempScene->m_strSelection2 ); // �� ������1 (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#SELECTION2 -1 0 \t<NONE>\n" ); // �� ������2 (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)

			if( TempScene->m_strSelection3 && strlen(TempScene->m_strSelection3) )
				fprintf( fp, "#SELECTION3 %d %d \t%s\n", TempScene->m_LinkSel3, strlen(TempScene->m_strSelection3), TempScene->m_strSelection3 ); // �� ������1 (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)
			else // ���� �ؽ�Ʈ�� ������...
				fprintf( fp, "#SELECTION3 -1 0 \t<NONE>\n" ); // �� ������3 (�ؽ�Ʈ �ͽ���Ʈ�� ���� ���� ���� �� ������ �����ϵ��� ������ ������ ����ش�.)

			int k = 0;
			sImageOutInfo *TempImage;
			list<sImageOutInfo*>::iterator prImage;
			for( prImage = TempScene->m_listImageOut.begin(); prImage != TempScene->m_listImageOut.end(); ++prImage )
			{
				TempImage = *prImage;

				fprintf( fp, "#IMAGE START\n" ); // �̹��� ��� ����.
				if( TempImage->m_strFileName && strlen(TempImage->m_strFileName) ) // ���ϸ��� ����� ������..
					fprintf( fp, "#IMAGE FILE %s\n", TempImage->m_strFileName ); // �̹��� ���ϸ�
				else
					fprintf( fp, "#IMAGE FILE <NONE>\n" ); // �̹��� ��� ����.

				fprintf( fp, "#IMAGE CREATE %d\n", TempImage->m_CreateTime ); // ���� ƽ
				fprintf( fp, "#IMAGE FADE_IN %d\n", TempImage->m_FadeInSpeed ); // ���̵��� ���ǵ�
				fprintf( fp, "#IMAGE DELETE %d\n", TempImage->m_DelTime ); // ���� ƽ
				fprintf( fp, "#IMAGE FADE_OUT %d\n", TempImage->m_FadeOutSpeed ); // ���̵�ƿ� ���ǵ�
				fprintf( fp, "#IMAGE AREA %d %d %d %d\n", TempImage->m_PosX, TempImage->m_PosY, TempImage->m_Width, TempImage->m_Height ); // �̹��� ��� ����.(640 ����)
				fprintf( fp, "#IMAGE END\n" ); // �̹��� �� ����.

				k++;
			}

			fprintf( fp, "#SCENE END\n" ); // �� ���� ��
		}

		fclose(fp);
	}
}

void CScriptManager::ImportScriptFromText( const char* FileName ) // �ؽ�Ʈ�κ��� �о�ͼ� ����
{
	Init(); // �ʱ�ȭ �ѹ� ���ش�.

	FILE *fp;
	fp = fopen(FileName, "rt");
	if( !fp ) // ���� ���� üũ.
		return;

	int len = 0;
	char strTemp[100]; // ����б�� ����.
	int TempInt = 0;
	sSceneData *TempScene = NULL; // �� �Է½ÿ� ����� �ӽÿ� ������.
	sImageOutInfo *TempImage = NULL; // �� �Է½ÿ� ����� �ӽÿ� ������.

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
		else if( strcmp( strTemp,"#TALKBOX_FILE" ) == 0 ) // ��ȭâ �׷��� ���� ����.
		{
			fscanf( fp, "%s\n", strTemp ); // ���ϸ�. ���ϸ��� ���̰� Ŀ���ڴϱ� �� ���ۿ� �о �ű��.

			len = strlen(strTemp);
			if( len > 98 ) // ���ϸ��� �ʹ� ���...
			{
				MessageBox( NULL, "TALKBOX_FILE is long", "Error", MB_OK );
				fclose(fp);
				return;
			}

			if( strcmp( strTemp,"<NONE>" ) == 0 ) // �׷��� ������ �����ȵ� ���.
			{
				// �ϴ� ���� ���� ���� ó�� ����.
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
		else if( strcmp( strTemp,"#DEFAULT_BOX_AREA" ) == 0 ) // ��ȭâ �׷��� ���� ����.
		{
			fscanf( fp, "%d %d %d %d\n", &m_BoxArea.left, &m_BoxArea.top, &m_BoxArea.right, &m_BoxArea.bottom );
		}
		else if( strcmp( strTemp,"#DEFAULT_TALK_AREA" ) == 0 ) // ��ȭâ ��� ���� ����.
		{
			fscanf( fp, "%d %d %d %d\n", &m_TextArea.left, &m_TextArea.top, &m_TextArea.right, &m_TextArea.bottom );
		}
		else if( strcmp( strTemp,"#SCENE_NUM" ) == 0 ) // ���� ����.
		{
			fscanf( fp, "%d\n", &TempInt ); // ���� Ư���� �ʿ����.
		}
		else if( strcmp( strTemp,"#SCENE" ) == 0 ) // ���� ���۰� ��.
		{
			fscanf( fp, "%s", strTemp ); // ���� ���ڸ� �д´�.
			if( strcmp( strTemp,"START" ) == 0 ) // ����� ����.
			{
				// ���ο� ���� �Է��� �غ� �Ѵ�.
				fscanf( fp, "%d\n", &TempInt ); // �� �ε���

				TempScene = new sSceneData(); // ������ �ȵǾ��־ �������. �̳༮�� ������ ������ ����Ʈ �ı� �ÿ� �Ͼ���Ѵ�.
				if( !TempScene )
				{
					MessageBox( NULL, "New Scene Create Failed! Memory Low", "Error", MB_OK );
					fclose(fp);
					return;
				}

				TempScene->m_Index = TempInt; // �ε��� ����.
			}
			else if( strcmp( strTemp,"END" ) == 0 ) // ����� ��.
			{
				// ���� ����Ʈ�� �߰��Ѵ�.
				m_listScene.push_back( TempScene );
				TempScene = NULL; // �޸� ���� ���� ���� �ʱ�ȭ�� ���ش�.
			}
		}
		else if( strcmp( strTemp,"#BOX_AREA" ) == 0 ) // ��ȭâ �׷��� ���� ����.
		{
			if( TempScene ) // �ִ� ������ Ȯ��
			{
				fscanf( fp, "%d %d %d %d\n", &TempScene->m_BoxArea.left, &TempScene->m_BoxArea.top, &TempScene->m_BoxArea.right, &TempScene->m_BoxArea.bottom );
			}
		}
		else if( strcmp( strTemp,"#TALK_AREA" ) == 0 ) // ��ȭâ ��� ���� ����.
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d %d %d %d\n", &TempScene->m_TextArea.left, &TempScene->m_TextArea.top, &TempScene->m_TextArea.right, &TempScene->m_TextArea.bottom );
			}
		}
		else if( strcmp( strTemp,"#TALK_SPEED" ) == 0 ) // ��� ���� �ӵ� ����.
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d\n", &TempScene->m_TextSpeed );
			}
		}
		else if( strcmp( strTemp,"#WAIT_TIME" ) == 0 ) // ��ȭ ���� �� ���ð� ����.
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d\n", &TempScene->m_WaitTime );
			}
		}
		else if( strcmp( strTemp,"#WAIT_ACTION" ) == 0 ) // ��ȭ ���� �� ��� �̺�Ʈ ����.
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d\n", &TempScene->m_WaitAction );
			}
		}
		else if( strcmp( strTemp,"#FADEIN_SPEED" ) == 0 ) // ��ȭ ������ ���̵��� ���� �� �ӵ�
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d\n", &TempScene->m_FadeInSpeed );
			}
		}
		else if( strcmp( strTemp,"#FADEOUT_SPEED" ) == 0 ) // ��ȭ ����� ���̵�ƿ� ���� �� �ӵ�
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d\n", &TempScene->m_FadeOutSpeed );
			}
		}
		else if( strcmp( strTemp,"#TALK" ) == 0 ) // ��� ���� ����.
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempScene->m_strContents = new char[TempInt+1];

					if( TempScene->m_strContents )
					{
						fgets( TempScene->m_strContents, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempScene->m_strContents[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SELECTION1" ) == 0 ) // ������1 ����.
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d", &TempScene->m_LinkSel1 );
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempScene->m_strSelection1 = new char[TempInt+1];

					if( TempScene->m_strSelection1 )
					{
						fgets( TempScene->m_strSelection1, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempScene->m_strSelection1[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SELECTION2" ) == 0 ) // ������2 ����.
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d", &TempScene->m_LinkSel2 );
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempScene->m_strSelection2 = new char[TempInt+1];

					if( TempScene->m_strSelection2 )
					{
						fgets( TempScene->m_strSelection2, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempScene->m_strSelection2[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#SELECTION3" ) == 0 ) // ������3 ����.
		{
			if( TempScene ) // �ִ��� ������ Ȯ��
			{
				fscanf( fp, "%d", &TempScene->m_LinkSel3 );
				fscanf( fp, "%d ", &TempInt ); // ��Ʈ�� ����

				if( TempInt > 0 )
				{
					TempScene->m_strSelection3 = new char[TempInt+1];

					if( TempScene->m_strSelection3 )
					{
						fgets( TempScene->m_strSelection3, TempInt+1, fp ); // ��Ʈ�� (+1����� ����� �޴���.)
						TempScene->m_strSelection3[TempInt] = '\0';
					}
				}
			}
		}
		else if( strcmp( strTemp,"#IMAGE" ) == 0 ) // �̹��� ��� ����Ʈ
		{
			fscanf( fp, "%s", strTemp ); // ���� ���ڸ� �д´�.
			if( strcmp( strTemp,"START" ) == 0 ) // �̹��� ����Ʈ�� ����.
			{
//				fscanf( fp, "%d\n", &TempInt ); // �̹����� ����. ����μ� ���� �Ⱦ���.

				TempImage = new sImageOutInfo(); // ������ �ȵǾ��־ �������. �̳༮�� ������ ������ ����Ʈ �ı� �ÿ� �Ͼ���Ѵ�.
				if( !TempImage )
				{
					MessageBox( NULL, "New ImageInfo Create Failed! Memory Low", "Error", MB_OK );
					fclose(fp);
					return;
				}
			}
			else if( strcmp( strTemp,"END" ) == 0 ) // �̹��� ����Ʈ �� ��.
			{
				if( TempScene )
				{
					// �̹����� ���� �߰��Ѵ�.
					TempScene->m_listImageOut.push_back( TempImage );
				}
				TempImage = NULL; // �޸� ���� ���� ���� �ʱ�ȭ�� ���ش�.
			}
			else if( strcmp( strTemp,"FILE" ) == 0 ) // �̹��� ���ϸ�
			{
				if( TempImage ) // �ִ��� ������ Ȯ��
				{
					fscanf( fp, "%s\n", strTemp ); // ���ϸ�. ���ϸ��� ���̰� Ŀ���ڴϱ� �� ���ۿ� �о �ű��.

					len = strlen(strTemp);
					if( len > 98 ) // ���ϸ��� �ʹ� ���...
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
			else if( strcmp( strTemp,"CREATE" ) == 0 ) // �̹��� ���� �ð�
			{
				if( TempImage ) // �ִ��� ������ Ȯ��
				{
					fscanf( fp, "%d\n", &TempImage->m_CreateTime );
				}
			}
			else if( strcmp( strTemp,"FADE_IN" ) == 0 ) // ������ ���̵��� �ӵ�.
			{
				if( TempImage ) // �ִ��� ������ Ȯ��
				{
					fscanf( fp, "%d\n", &TempImage->m_FadeInSpeed );
				}
			}
			else if( strcmp( strTemp,"DELETE" ) == 0 ) // �̹��� ���� �ð�
			{
				if( TempImage ) // �ִ��� ������ Ȯ��
				{
					fscanf( fp, "%d\n", &TempImage->m_DelTime );
				}
			}
			else if( strcmp( strTemp,"FADE_OUT" ) == 0 ) // ������ ���̵� �ƿ� �ӵ�.
			{
				if( TempImage ) // �ִ��� ������ Ȯ��
				{
					fscanf( fp, "%d\n", &TempImage->m_FadeOutSpeed );
				}
			}
			else if( strcmp( strTemp,"AREA" ) == 0 ) // �̹��� ��ġ �� ũ�� ����.
			{
				if( TempImage ) // �ִ��� ������ Ȯ��
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