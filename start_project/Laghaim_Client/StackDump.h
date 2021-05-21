#ifndef _STACK_DUMP_H_
#define _STACK_DUMP_H_

#include "Config.h"


#include <assert.h>

// ������ �������� ������ �����ϱ� ���ؼ� �ʿ��� �κ�.

// Initialize exception handler
extern void InitExceptionHandler();
// Close exception handler
extern void CloseExceptionHandler(void);
// Convert absolute address from logical address
extern bool GetLogicalAddress(void *pAddr, char *szModule, ULONG ulLen, ULONG &ulSection, ULONG &ulOffset);

#endif // _STACK_DUMP_H_
