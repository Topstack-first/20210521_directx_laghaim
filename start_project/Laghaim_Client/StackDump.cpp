#include "stdafx.h"
#include "StackDump.h"
#include <time.h>

static LPTOP_LEVEL_EXCEPTION_FILTER _efPreviousFilter; // Previous filter
static char _fnReportFileName[MAX_PATH] = "ErrorLog.txt"; // Report file name

// dummy function for converting abs addressess to logical ones
extern volatile ULONG __RefFuncAtAbsAddr(void)
{
	return -1;
}

static void _tprintf(HANDLE hFile, const char *format, ...)
{
	char achBuffer[1024];

	va_list argptr;
	va_start(argptr, format);
	wvsprintf(achBuffer, format, argptr);
	va_end(argptr);

	int iLength = strlen(achBuffer);
	ULONG ulBytesWritten;
	WriteFile(hFile,achBuffer,iLength, &ulBytesWritten, 0);
}

static LPTSTR GetExceptionString(ULONG ulCode)
{
#define EXCEPTION( x ) case EXCEPTION_##x: return #x;

	switch(ulCode)
	{
		EXCEPTION( ACCESS_VIOLATION )
		EXCEPTION( DATATYPE_MISALIGNMENT )
		EXCEPTION( BREAKPOINT )
		EXCEPTION( SINGLE_STEP )
		EXCEPTION( ARRAY_BOUNDS_EXCEEDED )
		EXCEPTION( FLT_DENORMAL_OPERAND )
		EXCEPTION( FLT_DIVIDE_BY_ZERO )
		EXCEPTION( FLT_INEXACT_RESULT )
		EXCEPTION( FLT_INVALID_OPERATION )
		EXCEPTION( FLT_OVERFLOW )
		EXCEPTION( FLT_STACK_CHECK )
		EXCEPTION( FLT_UNDERFLOW )
		EXCEPTION( INT_DIVIDE_BY_ZERO )
		EXCEPTION( INT_OVERFLOW )
		EXCEPTION( PRIV_INSTRUCTION )
		EXCEPTION( IN_PAGE_ERROR )
		EXCEPTION( ILLEGAL_INSTRUCTION )
		EXCEPTION( NONCONTINUABLE_EXCEPTION )
		EXCEPTION( STACK_OVERFLOW )
		EXCEPTION( INVALID_DISPOSITION )
		EXCEPTION( GUARD_PAGE )
		EXCEPTION( INVALID_HANDLE )
	}
	// If not one of the "known" exceptions, try to get the string
	// from NTDLL.DLL's message table.

	static TCHAR szBuffer[512] = { 0 };
	FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE,
				  GetModuleHandle("NTDLL.DLL"),
				  ulCode, 0, szBuffer, sizeof( szBuffer ), 0 );

	return szBuffer;
}

static void IntelStackWalk(HANDLE hFile, PCONTEXT pContext)
{
	_tprintf(hFile, "\nmanual stack frame walk begin:\n");
	_tprintf(hFile, "\n");
	_tprintf(hFile, "Address   Frame     Logical addr  Module\n");

	DWORD pc = pContext->Eip;
	PDWORD pFrame, pPrevFrame;
	pFrame = (PDWORD)pContext->Ebp;

	for(int iDepth=0; iDepth<100; iDepth++)
	{
		char szModule[MAX_PATH] = "";
		DWORD section = 0, offset = 0;
		_tprintf(hFile, "%08X %08X ", pc, pFrame);
		DWORD symDisplacement = 0;  // Displacement of the input address, relative to the start of the symbol

		BOOL bSuccess = GetLogicalAddress((PVOID)pc, szModule,sizeof(szModule),section,offset );
		assert(bSuccess);
		_tprintf(hFile, "$adr: %s %04X:%08X", szModule, section, offset);

		_tprintf(hFile, "\n");
		pc = pFrame[1];
		pPrevFrame = pFrame;
		pFrame = (PDWORD)pFrame[0]; // proceed to next higher frame on stack
		if((DWORD)pFrame&3)   // Frame pointer must be aligned on a
		{
			break;            // DWORD boundary.  Bail if not so.
		}

		if(pFrame<=pPrevFrame)
		{
			break;
		}

		// Can two DWORDs be read from the supposed frame address?
		if(IsBadWritePtr(pFrame, sizeof(PVOID)*2))
		{
			break;
		}
	}
	_tprintf(hFile, "\nmanual stack frame walk end:\n" );
}

static void GenerateExceptionReport(HANDLE hFile, PEXCEPTION_POINTERS pExceptionInfo)
{
	_tprintf(hFile, "--------------------------------------------------------------\n");
	char strTime[80];
	char strDate[80];
	_strtime(strTime);
	_strdate(strDate);
	_tprintf(hFile, "Crashed at: %s %s\n", strDate, strTime);

	PEXCEPTION_RECORD pExceptionRecord = pExceptionInfo->ExceptionRecord;

	// First print information about the type of fault
	_tprintf(hFile, "Exception code: %08X %s\n", pExceptionRecord->ExceptionCode,
			 GetExceptionString(pExceptionRecord->ExceptionCode));

	// Now print information about where the fault occured
	TCHAR szFaultingModule[MAX_PATH];
	DWORD section, offset;
	BOOL bSuccess = GetLogicalAddress(pExceptionRecord->ExceptionAddress, szFaultingModule,
									  sizeof(szFaultingModule), section, offset);
	assert(bSuccess);
	_tprintf(hFile, "Fault address:  %08X %02X:%08X %s\n", pExceptionRecord->ExceptionAddress,
			 section, offset, szFaultingModule);

	PCONTEXT pCtx = pExceptionInfo->ContextRecord;

	// Show the registers
#ifdef _M_IX86  // Intel Only!
	_tprintf(hFile, "\nRegisters:\n");

	_tprintf(hFile, "EAX:%08X\nEBX:%08X\nECX:%08X\nEDX:%08X\nESI:%08X\nEDI:%08X\n",
			 pCtx->Eax, pCtx->Ebx, pCtx->Ecx, pCtx->Edx, pCtx->Esi, pCtx->Edi );

	_tprintf(hFile, "CS:EIP:%04X:%08X\n", pCtx->SegCs, pCtx->Eip );
	_tprintf(hFile, "SS:ESP:%04X:%08X  EBP:%08X\n", pCtx->SegSs, pCtx->Esp, pCtx->Ebp );
	_tprintf(hFile, "DS:%04X  ES:%04X  FS:%04X  GS:%04X\n", pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs );
	_tprintf(hFile, "Flags:%08X\n", pCtx->EFlags );

#endif // #ifdef _M_IX86

#ifdef _M_IX86  // Intel Only!
	// Walk the stack using x86 specific code
	IntelStackWalk(hFile, pCtx);
#endif // #ifdef _M_IX86

	// Done
	_tprintf(hFile, "\n" );
}

static LONG WINAPI _UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	BOOL bDumpReportFile = true;

	if(bDumpReportFile)
	{
		HANDLE hFile = CreateFile(_fnReportFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, 0);
		if(hFile!=INVALID_HANDLE_VALUE)
		{
			// Dump report at eof
			SetFilePointer(hFile, 0, 0, FILE_END);
			// Dump report
			GenerateExceptionReport(hFile, pExceptionInfo);
			// Done
			CloseHandle(hFile);
		}
	}

	if(_efPreviousFilter)
	{
		return _efPreviousFilter(pExceptionInfo);
	}
	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

extern void InitExceptionHandler()
{
	// Set current exception filter function
	_efPreviousFilter = SetUnhandledExceptionFilter(_UnhandledExceptionFilter);
}

extern void CloseExceptionHandler(void)
{
	// Return old exception filter
	SetUnhandledExceptionFilter(_efPreviousFilter);
}

// Convert absolute address from logical address
extern bool GetLogicalAddress(void *pAddr, char *szModule, ULONG ulLen, ULONG &ulSection, ULONG &ulOffset)
{
	MEMORY_BASIC_INFORMATION mbi;

	if(!VirtualQuery(pAddr, &mbi, sizeof(mbi)))
	{
		return false;
	}

	DWORD hMod = (DWORD)mbi.AllocationBase;
	if(!GetModuleFileName((HMODULE)hMod, szModule, ulLen))
	{
		return false;
	}

	// Point to the DOS header in memory
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;

	// From the DOS header, find the NT (PE) header
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );
	DWORD rva = (DWORD)pAddr - hMod; // RVA is offset from module load address

	// Iterate through the section table, looking for the one that encompasses
	// the linear address.
	for ( unsigned int i=0; i<pNtHdr->FileHeader.NumberOfSections; i++,pSection++)
	{
		DWORD sectionStart = pSection->VirtualAddress;
		DWORD sectionEnd = sectionStart + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

		// Is the address in this section???
		if((rva>=sectionStart) && (rva<=sectionEnd))
		{
			// Yes, address is in the section.  Calculate section and offset,
			// and store in the "section" & "offset" params, which were
			// passed by reference.
			ulSection = i+1;
			ulOffset = rva - sectionStart;
			return true;
		}
	}
	return false;   // Should never get here!
}