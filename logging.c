#define WIN32_LEAN_AND_MEAN

#ifndef  UNICODE

#define UNICODE

#endif

#include "logging.h"

#pragma warning(push, 0) 
#include <Windows.h>
#include <stdio.h>
#pragma warning(pop)

ULONG EventWriteStringW2(_In_ PCWSTR String, _In_ ...)
{
	wchar_t FormattedString[ETW_MAX_STRING_SIZE] = { 0 };

	va_list ArgPointer = NULL;

	va_start(ArgPointer, String);

	_vsnwprintf_s(FormattedString, sizeof(FormattedString) / sizeof(wchar_t), _TRUNCATE, String, ArgPointer);

	va_end(ArgPointer);

#if DEBUG
	wprintf(L"%ls\r\n", FormattedString);
#endif

	return(EventWriteString(gEtwRegHandle, 0, 0, FormattedString));
}

