#ifndef __DEF_H__
#define __DEF_H__

#include "VirtualFile.h"
#include "FileManager.h"

#define pfscanf				FileManager::GetInstance()->Scanf
#define pfgets				FileManager::GetInstance()->Gets
#define pfgetc				FileManager::GetInstance()->Getc
#define pfseek				FileManager::GetInstance()->Seek
#define pfopen				FileManager::GetInstance()->Open
#define pfclose				FileManager::GetInstance()->Close
#define pfread				FileManager::GetInstance()->Read
#define pftell				FileManager::GetInstance()->Tell
#define pPackInitialize		FileManager::GetInstance()->PackInit
#define pPackUnInitialize	FileManager::GetInstance()->PackUninit
#define pDecode				FileManager::GetInstance()->Decode

#endif //__DEF_H__