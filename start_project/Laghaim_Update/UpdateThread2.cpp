#include "Stdafx.h"
//#include "resource.h"
//#include "LaghaimDlg.h"
//#include "StipulationDlg.h"
#include "AutoUpgrade.h"
//#include "CapExtract.h"
#include "unzip.h"
#include <time.h>
//#include "Language.h"
#include "UrlFile.h"

extern int do_extract(unzFile uf, const char *target_dir, int opt_extract_without_path,
					  int opt_overwrite, int ver_idx, int ver_diff/*, CProgressCtrl *pUpdating*/);

void UpdateThread2(LPVOID lParam)
{
}
