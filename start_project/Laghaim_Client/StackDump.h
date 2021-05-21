#ifndef _STACK_DUMP_H_
#define _STACK_DUMP_H_

#include "Config.h"


#include <assert.h>

// 게임이 뻑났을때 정보를 저장하기 위해서 필요한 부분.

// Initialize exception handler
extern void InitExceptionHandler();
// Close exception handler
extern void CloseExceptionHandler(void);
// Convert absolute address from logical address
extern bool GetLogicalAddress(void *pAddr, char *szModule, ULONG ulLen, ULONG &ulSection, ULONG &ulOffset);

#endif // _STACK_DUMP_H_
