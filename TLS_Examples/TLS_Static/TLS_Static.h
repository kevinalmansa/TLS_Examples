#pragma once
#include <Windows.h>

/*****************************************************************************
Set Windows Linking Options for use of TLS and TLS_CALLBACK.

TLS is a part of the PE Header Format, these options notify the Linker
to correctly set the TLS Header, which contains the TLS Callback Address,
in the PE Header.
******************************************************************************/
#ifdef _WIN64
#pragma comment (linker, "/INCLUDE:_tls_used")
#pragma comment (linker, "/INCLUDE:tls_callback_func")
#else
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:_tls_callback_func")
#endif

/*****************************************************************************
Definition of the TLS Callback functions to execute.
/*****************************************************************************/
void	tls_callback1(PVOID hModule, DWORD dwReason, PVOID pContext);
void	tls_callback2(PVOID hModule, DWORD dwReason, PVOID pContext);
