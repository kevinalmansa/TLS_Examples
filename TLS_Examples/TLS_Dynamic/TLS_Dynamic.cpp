// TLS_Dynamic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TLS_Dynamic.h"

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

// End section decleration
#ifdef _WIN64
#pragma const_seg()
#else
#pragma data_seg()
#endif //_WIN64 End section decleration


/*****************************************************************************
First TLS Callback
Set above to segment CRT$XLB
******************************************************************************/
void	tls_callback1(PVOID hModule, DWORD dwReason, PVOID pContext)
{
	if (dwReason == DLL_THREAD_ATTACH)
	{
		// This will be loaded in each DLL thread attach
		MessageBox(0, _T("TLS Callback: Thread Attach Triggered"),
			_T("TLS"), 0);
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		MessageBox(0, _T("TLS Callback: Process Attach Triggered"),
			_T("TLS"), 0);
		// DEBUG - Help understand how this is being stored in memory.
		printf("TLS Callback Addresses:\n\tFunction Address: %p\n\tCRT Callback Address: %p\n",
			tls_callback1, &tls_callback_func);

		// The location of the next element in the array of TLS callbacks in memory
		PIMAGE_TLS_CALLBACK *dynamic_callback = (PIMAGE_TLS_CALLBACK*)&tls_callback_func + 1;

		// The default Page Permissions do not necessairly allow us to write to this
		// part of (our) memory. We need to set Write Permissions to the memory range
		// we'll be writing to (here only one callback, thus sizeof(dynamic_callback).
		//
		// Tip: This can be done slightly more stealthy by using the PEB to access
		// kernel32.dll and call this manually.
		DWORD	old;
		VirtualProtect(dynamic_callback, sizeof(dynamic_callback), PAGE_EXECUTE_READWRITE, &old);

		// Finally, set the callback in memory, which is next in line to be
		// executed (in our case).
		*dynamic_callback = (PIMAGE_TLS_CALLBACK)tls_callback_secret;
	}
}


/*****************************************************************************
This TLS will be set dynamically at run-time.
This means is it will not appear in the PE Headers on disk.
******************************************************************************/
void	tls_callback_secret(PVOID hModule, DWORD dwReason, PVOID pContext)
{
	if (dwReason == DLL_THREAD_ATTACH)
	{
		// This will be loaded in each DLL thread attach
		MessageBox(0, _T("Dynamic TLS Callback: Thread Attach Triggered"),
			_T("Dynamic_TLS_Thread"), 0);
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		MessageBox(0, _T("Dynamic TLS Callback: Process Attach Triggered"),
			_T("TLS_Process"), 0);
		// DEBUG - Help understand how this is being stored in memory.
		printf("TLS Callback Addresses:\n\tFunction Address: %p\n\tCRT Callback Address: %p\n",
			tls_callback_secret, &tls_callback_func + 1);
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
