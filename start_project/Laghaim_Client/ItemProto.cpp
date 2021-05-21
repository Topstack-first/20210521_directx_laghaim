#include "stdafx.h"

#include <Def.h>
#include "ItemProto.h"
#include "IndexedTexture.h"
#include "Capsyong.h"
#include "Country.h" // ����� ���� ���� ���� ����ó�� ������ �߰� ///07-01-31
#include "LHExcelReader.h"
#include "ItemOutlineMgr.h"
#include <assert.h>

CItemList g_ItemList;

#define ITEM_DATA_FILE		"data/item/item_new.dta"
#define ITEM_DATA_TEST_FILE	"data/item/item_test.dta"
#define ITEM_STRING_FILE	"data/item/item.str" ///�����ۼ����Է�
#define ITEM_CSY_DIR		"data/item/csy/"
#define ITEM_LOLO_DIR		"data/item/lolo/"
#define ITEM_TEXTR_DIR		"data/item/textr/"

#define MAX_ITEM_LOLO	3000
//#define MAX_ITEM_CSY	3000   // 050526 ���� �ø�.( by ���� ) // ����� �̰� �÷��� �޸� ��°� �þ�� �� �ƴϴ�. // �̰� �����ϸ� �� ����.(������� ��, ����׿��� assert )

//#define MAX_ITEM_CSY	5000
//#define MAX_ITEM_CSY	6000   // 091210 ���� �ø�
#define MAX_ITEM_CSY	7000   // 111123 ���� �ø�


#define ENC_INT_SEED		(2136)
#define ENC_CHAR_SEED		(43)
#define ENC_INT_VAL			(3765)
#define ENC_CHAR_VAL		(139)
#define ENC_DUMMY_COUNT		(17)

///�����ۼ����Է�
#define ENC_ISTRING_INT_SEED		(2136)
#define ENC_ISTRING_CHAR_SEED		(43)
#define ENC_ISTRING_INT_VAL			(3765)
#define ENC_ISTRING_CHAR_VAL		(139)
#define ENC_ISTRING_DUMMY_COUNT		(17)


extern DWORD g_dwClientCountry; // ����� ���� ���� ���� ����ó�� ������ �߰� ///07-01-31
CLolos	*g_FlyStones[FLY_STONE_NUM];
CLolos	*g_FlyItems[FLY_ITEM_NUM];
CLolos	*g_pMattingLolo; // ���ڸ� ������Ʈ �ε�� ///���ڸ�
CLolos	*g_pDisplayStandLolo[3]; // ���λ��� ��ǰ������ ������Ʈ �ε�� ///���λ���
CLolos	*g_pFairyProtect = NULL; // �� ����Ʈ �ѷ� ///��
CLolos	*g_pChouchin = NULL; ///�Ϻ����������׽�Ʈ ��� ������ ����Ʈ��.
int g_AirshipProto[7];
int g_AirshipProto_VNUM[7];
int g_BasicWingsProto;
int g_BasicWingsProto_VNUM;
CVirtualFile *g_pvfTextr = NULL;
// nate 2004 - 8
// �Ź��� ���� - ����Ʈ ����������
int g_nMutantTextureNum[ 9 ] = { -1, };
int g_nEmptyTextureNum = -1;

BOOL LoadItems()
{
	// ������ �ε��Ǿ� �ִ� Items ����Ʈ�� �ִٸ� �� �����.
	g_ItemList.DeleteAllData();

	// ���� �ε�.
	g_pCapsyongTexture->SetTexturePath(ITEM_TEXTR_DIR);
	g_ItemList.SetIndexedTexture(g_pCapsyongTexture);
	g_ItemList.ReadItems();

	// �߹��� �� �ѷ� �б�
	for (int i = 0; i < FLY_STONE_NUM; i++)
	{
		char dirName[256];

		sprintf(dirName, "data/item/lolo/fly_stone%d.lol", i);

		CLolos *pLolo;
		pLolo = new CLolos(g_pCapsyongTexture);
		pLolo->Read(dirName);
		g_FlyStones[i] = pLolo;
	}

	int airshipVnum[7] = {872, 873, 874, 874, 874, 874, 874};
	for (int i = 0; i < 7; i++)
	{
		g_AirshipProto_VNUM[i] = airshipVnum[i];
		g_AirshipProto[i] = g_ItemList.FindItem(airshipVnum[i]);
	}

	g_BasicWingsProto = g_ItemList.FindItem(895);	// �⺻����: 895
	g_BasicWingsProto_VNUM = 895;
	// ���༮ ��ȣ ����.

	// ������ ������ ť�� �б�
	for (int i = 0; i < FLY_ITEM_NUM; i++)
	{
		char dirName[256];
		PFILE *fp;

		sprintf(dirName, "data/item/lolo/sky_item%d.lol", i);

		if (!(fp = pfopen(dirName, "rb")))
			return FALSE;

		CLolos *pLolo;
		pLolo = new CLolos(g_pCapsyongTexture);
		if( pLolo )
			pLolo->Read(fp);

		pfclose(fp);

		g_FlyItems[i] = pLolo;
	}


	///////////////////////////////////////////////////////////////////////
	// ���ڸ�(�Ҿƹ���,�ҸӴ�,���λ��������� ����) ///���ڸ�
	PFILE *fp;

	if (!(fp = pfopen("data/building/start/lolos/grandparentback.LOL", "rb")))
	{
		return FALSE;
	}

	g_pCapsyongTexture->SetTexturePath( "data/building/textr/" );

	CLolos *pLolo;
	pLolo = new CLolos(g_pCapsyongTexture);
	pLolo->Read(fp);

	pfclose(fp);

	g_pMattingLolo = pLolo;

	g_pCapsyongTexture->SetTexturePath(ITEM_TEXTR_DIR); // Ȥ�� �𸣴ϱ� ������� ��������.
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// ���λ��� ��ǰ������ ������Ʈ �ε�� ///���λ���
//	FILE *fp;

	//g_pCapsyongTexture->SetTexturePath( "data/building/textr/" );
	g_pCapsyongTexture->SetTexturePath2( "data/Npc/textr/" );
	char strTemp[256];

	for( int i = 0 ; i < 3 ; ++i )
	{
		sprintf( strTemp, "data/Npc/lolo/uua_displaystand_%02d.LOL", i );
		if( !(fp = pfopen(strTemp, "rb")) )
		{
			return FALSE;
		}

		CLolos *pLolo;
		pLolo = new CLolos(g_pCapsyongTexture);
		pLolo->Read(fp);

		pfclose(fp);

		g_pDisplayStandLolo[i] = pLolo;
	}
	g_pCapsyongTexture->SetTexturePath(ITEM_TEXTR_DIR); // Ȥ�� �𸣴ϱ� ������� ��������.
	///////////////////////////////////////////////////////////////////////

	// �� ����Ʈ �ѷ� ///��
	if (!(fp = pfopen("data/effect/lolos/fairy_lolo_protect.LOL", "rb")))
	{
		return FALSE;
	}

	g_pCapsyongTexture->SetTexturePath( "data/effect/textr/" );

//	CLolos *pLolo;
	pLolo = new CLolos(g_pCapsyongTexture);
	pLolo->Read(fp);

	pfclose(fp);

	g_pFairyProtect = pLolo;



	///�Ϻ����������׽�Ʈ ��տ� ����Ʈ.
	fp = pfopen("data/effect/lolos/chouchin_main.LOL", "rb");
	if( fp ) // �������� �е���...�̰� �������� �� �� ������ �ִ� ����.
	{
		g_pCapsyongTexture->SetTexturePath( "data/effect/textr/" );

		pLolo = new CLolos(g_pCapsyongTexture);
		pLolo->Read(fp);

		pfclose(fp);

		g_pChouchin = pLolo;
	}


	g_pCapsyongTexture->SetTexturePath(ITEM_TEXTR_DIR); // Ȥ�� �𸣴ϱ� ������� ��������.

	return TRUE;
}

void DeleteItems()
{
	delete g_pChouchin;  ///�Ϻ����������׽�Ʈ ��� ������ ����Ʈ��.

	for (int i = 0; i < FLY_STONE_NUM; i++)
		delete g_FlyStones[i];

	for (int i = 0; i < FLY_ITEM_NUM; i++)
		delete g_FlyItems[i];

	delete g_pMattingLolo; ///���ڸ� ///���λ��� �� �߰�
	for(int i = 0 ; i < 3 ; ++i )
		delete g_pDisplayStandLolo[i];  // ���λ��� ��ǰ������ ������Ʈ �ε�� ///���λ���

	delete g_pFairyProtect;  // �� ����Ʈ �ѷ� ///��
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemList::CItemList(IndexedTexture *pIndexedTexture)
{
	m_pIndexedTexture = pIndexedTexture;
	m_ItemArray = NULL;
	m_CsyDir = ITEM_CSY_DIR;
	m_LoloDir = ITEM_LOLO_DIR;
	m_ItemCount = 0;
}

CItemList::~CItemList()
{
	DeleteAllData();
}

void CItemList::DeleteAllData()
{
	SAFE_DELETE_ARRAY(m_ItemArray);
}

void CItemList::SetIndexedTexture(IndexedTexture *pIndexedTexture)
{
	m_pIndexedTexture = pIndexedTexture;
}

BOOL CItemList::ReadItems()
{
	{

	}


	////////////////////////////////////////////////////////////////////////////////////
	///�����ۼ����Է�
	sItemDescription *pItemDescList = NULL;
	int nItemStringMaxIndex = 0; // ���� ū �ε����� ����. ���� ����� �� ũ�⸸ŭ �迭�� �����Ѵ�.

	PFILE *ItemStringFP;
	ItemStringFP = pfopen( ITEM_STRING_FILE, "rb" );

	if( ItemStringFP ) // �������� ó��, ������ �����ϰ� �׳� ����ϸ��.
	{
		int int_string_seed = ENC_ISTRING_INT_SEED;
		char char_string_seed = ENC_ISTRING_CHAR_SEED;

		int nNumOfStringSet = 0; // �Էµ� ��Ʈ������ �������� �� ����.

		// ���̸� �о ���ش�!
		for (int dummy_count = 0; dummy_count < ENC_ISTRING_DUMMY_COUNT; dummy_count++)
		{
			int dummy;
			pfread(&dummy, sizeof(int), 1, ItemStringFP);
		}

		ReadInt( &nItemStringMaxIndex, ItemStringFP, int_string_seed); // ���� ������ �ƴ϶� ���� ���� �ε��� ��ȣ��.
		ReadInt( &nNumOfStringSet, ItemStringFP, int_string_seed); // ���� ����

		pItemDescList = new sItemDescription[nItemStringMaxIndex+1]; // ���� ���� ��ŭ�� �ƴ϶�. ���� ���� �ε��� ��ȣ��ŭ ����.
		if( !pItemDescList )
		{
			pfclose( ItemStringFP );
			return FALSE;
		}
		ZeroMemory( pItemDescList, sizeof(sItemDescription)*(nItemStringMaxIndex+1) ); // �� ��������Ѵ�. �׷��� �����Ͱ� NULL�� ��.

		int nIndex = 0;
		char strTemp[512] = ""; // �̸��� 50��, ������ 511�ڱ����� ���ѵǾ��ִ�.
		int strTempLen = 0;

		for( int i = 0 ; i < nNumOfStringSet ; ++i )
		{
			ReadInt( &nIndex, ItemStringFP, int_string_seed); // �̹� �������� �ε���.

			if( nIndex > nItemStringMaxIndex ) // �ִ�ġ ���� ������ ���� �߲ٴ�.
			{
				MessageBox( NULL, "Item String Loading Error!!", "Error", MB_OK );
				pfclose( ItemStringFP );
				delete [] pItemDescList;
				return FALSE;
			}

			// �̸� �б�.
			ZeroMemory( strTemp, sizeof(strTemp) );
			ReadString( strTemp, ItemStringFP, int_string_seed, char_string_seed );
			strTempLen = strlen( strTemp );
			if( strTempLen > 0 )
			{
				strncpy( pItemDescList[nIndex].m_strLocalName, strTemp, strTempLen );
				pItemDescList[nIndex].m_strLocalName[strTempLen] = '\0';
			}

			// ���� �б�.
			ZeroMemory( strTemp, sizeof(strTemp) );
			ReadString( strTemp, ItemStringFP, int_string_seed, char_string_seed );
			strTempLen = strlen( strTemp );
			if( strTempLen > 0 )
			{
				strncpy( pItemDescList[nIndex].m_strDesc, strTemp, strTempLen );
				pItemDescList[nIndex].m_strDesc[strTempLen] = '\0';
			}
		}
		pfclose( ItemStringFP );
	}

	////////////////////////////////////////////////////////////////////////////////////

	PFILE *fp;

	if (!(fp = pfopen(ITEM_DATA_FILE, "rb")))
	{
		MessageBox(NULL, "Failed to load item data", "Error", MB_OK);
		SAFE_DELETE_ARRAY(pItemDescList);
		return FALSE;
	}

	// ���̸� �о ���ش�!
	for (int dummy_count = 0; dummy_count < ENC_DUMMY_COUNT; dummy_count++)
	{
		int dummy;
		pfread(&dummy, sizeof(int), 1, fp);
	}
	int int_seed = ENC_INT_SEED;
	char char_seed = ENC_CHAR_SEED;

	ReadInt(&m_ItemCount, fp, int_seed);

	SAFE_DELETE_ARRAY(m_ItemArray);
	m_ItemArray = new CItemProto[m_ItemCount];

	char strTemp[256] = "";
	// ���������� �о����.
	for (int i = 0; i < m_ItemCount; i++)
	{
		ReadInt(&m_ItemArray[i].m_Vnum, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Type, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Shape, fp, int_seed);
		ReadString(strTemp, fp, int_seed, char_seed);
		m_ItemArray[i].m_Name = strTemp;

		if( pItemDescList && m_ItemArray[i].m_Vnum <= nItemStringMaxIndex )
		{
			int TempLen = 0;
			TempLen = strlen(pItemDescList[m_ItemArray[i].m_Vnum].m_strLocalName);
			if( TempLen > 0 ) // ����ȭ �̸��� �ִٸ�..
			{
				m_ItemArray[i].m_Name = pItemDescList[m_ItemArray[i].m_Vnum].m_strLocalName;
			}
			TempLen = strlen(pItemDescList[m_ItemArray[i].m_Vnum].m_strDesc);
			if( TempLen > 0 ) // ����ȭ ������ �ִٸ�..
			{
				m_ItemArray[i].m_strDesc = pItemDescList[m_ItemArray[i].m_Vnum].m_strDesc;
			}
		}


		ReadInt(&m_ItemArray[i].m_RaceFlag, fp, int_seed);

		ReadString(strTemp, fp, int_seed, char_seed);
		m_ItemArray[i].m_LoloNum = -1;//LoadLolo(strTemp);
		m_ItemArray[i].m_LoloFile = strTemp;
		ReadString(strTemp, fp, int_seed, char_seed);
		m_ItemArray[i].m_LoloTextrNum = -1;//SetTexture(strTemp);
		m_ItemArray[i].m_LoloTextureFile = strTemp;
		ReadString(strTemp, fp, int_seed, char_seed);
		m_ItemArray[i].m_SubCsyNum = -1;//LoadCsy(strTemp);
		m_ItemArray[i].m_SubCsyFile = strTemp;

		for( int race = 0 ; race < NUM_RACE*NUM_SEX ; race++ )
		{
			ReadString(strTemp, fp, int_seed, char_seed);
			m_ItemArray[i].m_CsyNum[race] = -1;//LoadCsy(strTemp);
			m_ItemArray[i].m_CsyFile[race] = strTemp;
			ReadString(strTemp, fp, int_seed, char_seed);
			m_ItemArray[i].m_TextrNum[race] = -1;//SetTexture(strTemp);
			m_ItemArray[i].m_CsyTextureFile[race] = strTemp;

			if( (m_ItemArray[i].m_CsyFile[race]).length() == 0 )
			{
				m_ItemArray[i].m_CsyFile[race] = m_ItemArray[i].m_CsyFile[race%2];
			}

			if (m_ItemArray[i].m_CsyNum[race] == -1)
			{
				m_ItemArray[i].m_CsyNum[race] = m_ItemArray[i].m_CsyNum[race%2];
			}
		}

		ReadString(strTemp, fp, int_seed, char_seed);
		m_ItemArray[i].m_EffectTextrNum = SetTexture(strTemp);
		ReadInt(&m_ItemArray[i].m_RenderFlag, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_AttFlag, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num0, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num1, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num2, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num3, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num4, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num5, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num6, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num7, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num8, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Num9, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Weight, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Int, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Dex, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_MinLevel, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_MaxUse, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Price, fp, int_seed);

		// ����� ���� ���尡���� �ٲ�޶�� �ؼ� ����ó�� ///07-01-31
		if( g_dwClientCountry == CTRY_BRA )
		{
			if( m_ItemArray[i].m_Vnum == 1356 ) // ���� ����
				m_ItemArray[i].m_Price = 20000;
			else if( m_ItemArray[i].m_Vnum == 1357 ) // ���� �ʱ� ����
				m_ItemArray[i].m_Price = 3000;
		}

		ReadInt(&m_ItemArray[i].m_Width, fp, int_seed);
		ReadInt(&m_ItemArray[i].m_Height, fp, int_seed);
		m_ItemArray[i].m_WidthForScale = m_ItemArray[i].m_Width;
		m_ItemArray[i].m_HeightForScale = m_ItemArray[i].m_Height;
		
		m_ItemArray[i].m_Width = 1;		
		m_ItemArray[i].m_Height = 1;
	}

	g_nMutantTextureNum[ 0 ] = SetTexture( "eff_yellow.tga" );
	g_nMutantTextureNum[ 1 ] = SetTexture( "eff_skyblue.tga" );
	g_nMutantTextureNum[ 2 ] = SetTexture( "eff_red.tga" );
	g_nMutantTextureNum[ 3 ] = SetTexture( "eff_yellow_890.tga" );
	g_nMutantTextureNum[ 4 ] = SetTexture( "eff_skyblue_890.tga" );
	g_nMutantTextureNum[ 5 ] = SetTexture( "eff_red_890.tga" );
	g_nMutantTextureNum[ 6 ] = SetTexture( "eff_yellow_910.tga" );
	g_nMutantTextureNum[ 7 ] = SetTexture( "eff_skyblue_910.tga" );
	g_nMutantTextureNum[ 8 ] = SetTexture( "eff_red_910.tga" );
	g_nEmptyTextureNum		 = SetTexture( "EFF_EMPTY.tga" );

	ItemOutlineMgr::GetInstance()->Reload();

	pfclose(fp);

	if( pItemDescList )
	{
		// �޸𸮸� �����Ѵ�.
		SAFE_DELETE_ARRAY( pItemDescList );
	}

	return TRUE;
}

BOOL CItemList::RefreshItems()
{
	PFILE *fp;

	if (!(fp = pfopen(ITEM_DATA_FILE, "rb")))
	{
		MessageBox(NULL, "Failed to load item data", "Error", MB_OK);
		return FALSE;
	}

	// ���̸� �о ���ش�!
	for (int dummy_count = 0; dummy_count < ENC_DUMMY_COUNT; dummy_count++)
	{
		int dummy;
		pfread(&dummy, sizeof(int), 1, fp);
	}
	int int_seed = ENC_INT_SEED;
	char char_seed = ENC_CHAR_SEED;

	int item_count, vnum, i_temp;
	ReadInt(&item_count, fp, int_seed);

	char strTemp[256] = "";

	for (int i = 0; i < item_count; i++)
	{
		ReadInt(&vnum, fp, int_seed);
		ReadInt(&i_temp, fp, int_seed); // type
		ReadInt(&i_temp, fp, int_seed); // shape
		ReadString(strTemp, fp, int_seed, char_seed); // Name

		ReadString(strTemp, fp, int_seed, char_seed); // Lolo
		ReadString(strTemp, fp, int_seed, char_seed); // Texture
		ReadString(strTemp, fp, int_seed, char_seed); // M_Csy
		ReadString(strTemp, fp, int_seed, char_seed); // Texture
		ReadString(strTemp, fp, int_seed, char_seed); // W_Csy
		ReadString(strTemp, fp, int_seed, char_seed); // Texture

		int proto_num = 0;
		for ( proto_num = 0; proto_num < m_ItemCount; proto_num++)
		{
			if (m_ItemArray[proto_num].m_Vnum == vnum)
				break;
		}
		if (proto_num >= m_ItemCount)
		{
			MessageBox(NULL, "Item refresh information is different.", "Error", MB_OK);
			pfclose(fp);
			return FALSE;
		}
		ReadInt(&m_ItemArray[proto_num].m_RaceFlag, fp, int_seed); // rece flag
		ReadString(strTemp, fp, int_seed, char_seed); // Effect Texture
		ReadInt(&m_ItemArray[proto_num].m_RenderFlag, fp, int_seed); // render flag
		ReadInt(&m_ItemArray[proto_num].m_AttFlag, fp, int_seed); // render flag
		ReadInt(&m_ItemArray[proto_num].m_Num0, fp, int_seed); // num0
		ReadInt(&m_ItemArray[proto_num].m_Num1, fp, int_seed); // num1
		ReadInt(&m_ItemArray[proto_num].m_Num2, fp, int_seed); // num2
		ReadInt(&m_ItemArray[proto_num].m_Num3, fp, int_seed); // num3
		ReadInt(&m_ItemArray[proto_num].m_Num4, fp, int_seed); // num4
		ReadInt(&m_ItemArray[proto_num].m_Num5, fp, int_seed); // num5
		ReadInt(&m_ItemArray[proto_num].m_Num6, fp, int_seed); // num6
		ReadInt(&m_ItemArray[proto_num].m_Num7, fp, int_seed); // num7
		ReadInt(&m_ItemArray[proto_num].m_Num8, fp, int_seed); // num8
		ReadInt(&m_ItemArray[proto_num].m_Num9, fp, int_seed); // num9
		ReadInt(&m_ItemArray[proto_num].m_Weight, fp, int_seed); // weight
		ReadInt(&m_ItemArray[proto_num].m_Int, fp, int_seed); // int
		ReadInt(&m_ItemArray[proto_num].m_Dex, fp, int_seed); // dex
		ReadInt(&m_ItemArray[proto_num].m_MinLevel, fp, int_seed); // min level
		ReadInt(&m_ItemArray[proto_num].m_MaxUse, fp, int_seed); // max use
		ReadInt(&m_ItemArray[proto_num].m_Price, fp, int_seed); // price

		ReadInt(&i_temp, fp, int_seed); // width
		ReadInt(&i_temp, fp, int_seed); // height
	}

	pfclose(fp);
	return TRUE;
}

void CItemList::ReadInt(int *i_data, PFILE *fp, int &int_seed)
{
	pfread(i_data, sizeof(int), 1, fp);
	int seed_temp = *i_data;
	*i_data = *i_data - int_seed - ENC_INT_VAL;//3765
	int_seed = seed_temp;
}

void CItemList::ReadString(char *str, PFILE *fp, int &int_seed, char &char_seed)
{
	int len;

	pfread(&len, sizeof(int), 1, fp);
	// ���� ��ȣ Ǯ��
	int seed_temp = len;
	len = len - int_seed - ENC_INT_VAL;
	int_seed = seed_temp;

	if (len > 511)
	{
		*str = '\0';
		return;
	}

	pfread(str, len, 1, fp);
	str[len] = '\0';

	// ���ڿ� ��ȣ Ǯ��
	for (int i = 0; i < len; i++)
	{
		int char_seed_temp = str[i];
		str[i] = str[i] - char_seed - ENC_CHAR_VAL;
		char_seed = char_seed_temp;
	}
}

void CItemList::ReadStringDup(char* &str, PFILE *fp, int &int_seed, char &char_seed)
{
	int len;
	char *str_temp;

	pfread(&len, sizeof(int), 1, fp);
	// ���� ��ȣ Ǯ��
	int seed_temp = len;
	len = len - int_seed - ENC_INT_VAL;
	int_seed = seed_temp;

	str_temp = new char[len+1];
	pfread(str_temp, len, 1, fp);
	str_temp[len] = '\0';
	str = str_temp;
	// ���ڿ� ��ȣ Ǯ��
	for (int i = 0; i < len; i++)
	{
		int char_seed_temp = str[i];
		str[i] = str[i] - char_seed - ENC_CHAR_VAL;
		char_seed = char_seed_temp;
	}
}

int CItemList::SetTexture(char *fileName)
{
	if (m_pIndexedTexture)
		return m_pIndexedTexture->CreateTextureFromFile(fileName);
	else
		return -1;
}

int CItemList::FindItem(int vnum)
{
	int idx_min = -1;
	int idx_max = m_ItemCount;
	int idx = m_ItemCount / 2;
	BOOL found = FALSE;

	while (idx_min != idx && idx_max != idx)
	{
		if (m_ItemArray[idx].m_Vnum > vnum)
		{
			idx_max = idx;
			idx = (idx_min + idx) / 2;
		}
		else if (m_ItemArray[idx].m_Vnum < vnum)
		{
			idx_min = idx;
			idx = (idx + idx_max) / 2;
		}
		else     // ���ڵ�!!
		{
			found = TRUE;
			break;
		}
	}

	if (found)
		return idx;

	return -1;
}

CItemProto::CItemProto()
{
}

CItemProto::~CItemProto()
{
}

