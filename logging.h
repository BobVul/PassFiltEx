#ifndef LOGGING_H
#define LOGGING_H
#pragma once

#pragma warning(push, 0)
#include <Windows.h>
#include <evntprov.h>
#pragma warning(pop)

#define ETW_MAX_STRING_SIZE 2048
#define __FILENAMEW__ (wcsrchr(__FILEW__, L'\\') ? wcsrchr(__FILEW__, L'\\') + 1 : __FILEW__)

extern REGHANDLE gEtwRegHandle;

ULONG EventWriteStringW2(_In_ PCWSTR String, _In_ ...);

#endif