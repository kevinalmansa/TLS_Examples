// TLS_Static.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "TLS_Static.h"

/*****************************************************************************
CRT allows the program to register TLS Callbacks.
The Callbacks to execute are found in a NULL terminated Array.

A cariable of type PIMAGE_TLS_CALLBACK pointing to the callback must be
declared in the CRT to register it in this array.

The compiler can concatenate into one section using the $ symbol.

The CRT section makes use of a specific naming convention; .CRT$XLx where x
can be anything between A-Z. A is the beinning, Z is the null terminator.
All XLx are concatenated into the .CRT section.

Concatenation is done alphabetically, so the callback in .CRT$XLB will be
called before .CRT$XLC.
******************************************************************************/


/*****************************************************************************
Decalre first CRT, containing only one callback pointer.

Note: under x64 it must be a const_seg, for x86 it must be a data_seg.
******************************************************************************/
#ifdef _WIN64
#pragma const_seg(".CRT$XLB")
EXTERN_C const
#else
#pragma data_seg(".CRT$XLB")
EXTERN_C
#endif

// The name of the variable & function bellow can be whatever you want. It's 
// the section name it belongs to (above) and the variable type that is
// important.
// Note: it is declared with EXTERN_C to avoid name wrangling of CPP.
PIMAGE_TLS_CALLBACK tls_callback_func = (PIMAGE_TLS_CALLBACK)tls_callback1;

#ifdef _WIN64
#pragma const_seg()
#else
#pragma data_seg()
#endif //_WIN64 End section decleration


/*****************************************************************************
Declare second CRT, containing, again, only one callback pointer.
******************************************************************************/
#ifdef _WIN64
#pragma const_seg(".CRT$XLC")
EXTERN_C const
#else
#pragma data_seg(".CRT$XLC")
EXTERN_C
#endif

PIMAGE_TLS_CALLBACK tls_callback_func2 = (PIMAGE_TLS_CALLBACK)tls_callback2;

#ifdef _WIN64
#pragma const_seg()
#else
#pragma code_seg()
#endif //_WIN64 End section decleration


/*****************************************************************************
First TLS Callback
Set above to segment CRT$XLB
******************************************************************************/
void	tls_callback1(PVOID hModule, DWORD dwReason, PVOID pContext)
{
	if (dwReason == DLL_THREAD_ATTACH) {
		// This will be loaded in each DLL thread attach
		MessageBox(0, _T("TLS Callback 1: Thread Attach Triggered"), _T("TLS"), 0);
	}

	if (dwReason == DLL_PROCESS_ATTACH) {
		MessageBox(0, _T("TLS Callback: Process Attach Triggered"), _T("TLS"), 0);
		// DEBUG - Help understand how this is being stored in memory.
		printf("TLS Callback Addresses:\n    Function Address: %p\n    CRT Callback Address: %p\n",
			tls_callback1, &tls_callback_func);
	}
}


/*****************************************************************************
Second TLS Callback
Set above to segment CRT$XLC
******************************************************************************/
void	tls_callback2(PVOID hModule, DWORD dwReason, PVOID pContext)
{
	if (dwReason == DLL_THREAD_ATTACH) {
		// This will be loaded in each DLL thread attach
		MessageBox(0, _T("TLS Callback 2: Thread Attach Triggered"), _T("TLS_Thread"), 0);
	}

	if (dwReason == DLL_PROCESS_ATTACH) {
		MessageBox(0, _T("TLS Callback 1: Process Attach Triggered"), _T("TLS_Process"), 0);
		// DEBUG - Help understand how this is being stored in memory.
		printf("TLS Callback Addresses:\n    Function Address: %p\n    CRT Callback Address: %p\n",
			tls_callback2, &tls_callback_func2);
	}
}


/*****************************************************************************
The actual main()

Code in the TLS Callbacks set above will execute BEFORE the main.
Setting a breakpoint here will not stop the callbacks from executing first.
******************************************************************************/
int main()
{
	printf("Main():\n    Hello World\n");
	system("pause");
	return 0;
}
