#define __MSVCTEST

#if defined(_WIN32) && defined(__MSVCTEST)
#define _CRT_SECURE_NO_WARNINGS
#endif

#if (defined(_WIN32) && !defined(__MSVCTEST))
#include <windows.h>
#include <shlwapi.h>
#include <strsafe.h>
#endif
#if (!defined(_WIN32) || defined(__MSVCTEST))
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#ifndef _WIN32
#include <unistd.h>
#endif
// Win32 Compatibility

#ifdef _WIN32
void __stdcall Sleep(unsigned long ms);
#else
void Sleep(unsigned long ms) {
    sleep(ms / 1000);
}
#endif

#define VOID void
#define WINAPI
#define APIENTRY
#define TRUE 1
#define FALSE 0
#define FILE_ATTRIBUTE_NORMAL 0
#define OPEN_ALWAYS 0

typedef unsigned long long ULONG64;
typedef unsigned long ULONG32, DWORD, * PDWORD;
typedef void* HANDLE, ** PHANDLE, * LPVOID;
typedef void* HINSTANCE;
typedef char* PSTR, * PBYTE, * LPSTR;
typedef const char* PCBYTE, * PCSTR, * LPCSTR;
typedef const void* PCVOID, * LPCVOID;
typedef unsigned char BOOL;

HANDLE GetStdHandle(DWORD Value) {
    static FILE* _stdin = NULL; 
    if (!_stdin)
        _stdin = stdin;
    static FILE* _stdout = NULL; 
    if (!_stdout)
        _stdout = stdout;
    static FILE* _stderr = NULL; 
    if (!stderr)
        _stderr = stderr;

    switch (Value) {
    case 0xFFFFFFF4:
        return &_stdin;
    case 0xFFFFFFF5:
        return &_stdout;
    case 0xFFFFFFF6:
        return &_stderr;
    };

    return NULL;
}

VOID AllocConsole(VOID) {
    return;
}
VOID WriteConsoleA(HANDLE Location, PCVOID Buffer, DWORD Size, PDWORD Outsize, LPVOID Reserved) {
    char* buffer = (char*)malloc(Size + 1);
    memcpy(buffer, Buffer, Size);
    buffer[Size] = '\0';
    if (Outsize)
        *Outsize = strlen(buffer);
    fputs(buffer, *((FILE**)Location));
    free(buffer);
}
HANDLE GetProcessHeap(VOID) {
    return (HANDLE)0x6900;
}
LPVOID HeapAlloc(HANDLE Heap, DWORD Flags, ULONG32 Size) {
    if ((ULONG64)Heap != 0x6900)
        return NULL;
    return malloc(Size);
}
VOID HeapFree(HANDLE Heap, DWORD Flags, LPVOID Pointer) {
    if ((ULONG64)Heap != 0x6900)
        return;
    free(Pointer);
    return;
}
void RtlSecureZeroMemory(void* Buffer, unsigned long Length) {
    for (int i = 0; i < Length; i++)
        ((PBYTE)Buffer)[i] = 0;
}
#define GENERIC_READ  0x80000000
#define GENERIC_WRITE 0x40000000
HANDLE CreateFileA(LPCSTR Name, DWORD Security, DWORD Sharemode, DWORD SecurityAttribute, DWORD Disposition, DWORD Attribute, LPVOID Reserved) {
    char* _Security = (char*)malloc(4);
    if (!_Security)
        return NULL;
    if (Security & GENERIC_READ)
        strcpy(_Security, "r");
    if (Security & GENERIC_WRITE)
        strcpy(_Security, "w");
    if ((Security & (GENERIC_READ | GENERIC_WRITE)) == (GENERIC_READ | GENERIC_WRITE))
        strcpy(_Security, "r+");
    FILE* t = fopen(Name, _Security);
    free(_Security);
    return &t;
}
VOID WriteFile(HANDLE File, LPVOID Buffer, ULONG32 Size, PDWORD Outsize, LPVOID Reserved) {
    fwrite(Buffer, Size, 1, *((FILE**)File));
    if (Outsize)
        *Outsize = Size;
    return;
}
ULONG32 ReadFile(HANDLE File, LPVOID Buffer, ULONG32 Size, PDWORD Outsize, LPVOID Reserved) {
    fread(Buffer, Size, 1, *((FILE**)File));
    if (Outsize)
        *Outsize = Size;
    return 1;
}
VOID StringCbCopyA(PSTR Dest, ULONG32 Size, LPCSTR Src) {
    strncpy(Dest, Src, Size);
    return;
}
VOID StringCbPrintfA(PSTR Dest, ULONG32 Size, LPCSTR Format, ...) {
    va_list args;
    va_start(args, Format);
    vsprintf(Dest, Format, args);
    va_end(args);
    return;
}
VOID StringCbCatA(PSTR Dest, ULONG32 Size, LPCSTR Src) {
    strncat(Dest, Src, Size);
    return;
}
#define FAILED(x) (x > 1)
ULONG32 StringCchLengthA(PSTR String, ULONG32 MaxSize, PDWORD Length) {
    *Length = strnlen(String, MaxSize);
    return 0;
}
VOID CloseHandle(HANDLE Thing) {
    fclose(*((FILE**)Thing));
}
VOID FreeConsole(VOID) {
    return;
}
#endif
typedef struct tagTEST_STRUCTp {
    ULONG32 Test0;
    ULONG32 Test1;
}TEST_STRUCT, * PTEST_STRUCT;

BOOL APIENTRY WinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    PSTR CommandLine,
    ULONG32 ShowMode
) {
    HANDLE StdOut;
    PTEST_STRUCT Buffer;
    HANDLE File;
    PSTR StringBuffer0;
    PSTR StringBuffer1;
    ULONG32 Length;

    StdOut = GetStdHandle(0xFFFFFFF5);
    AllocConsole();
    WriteConsoleA(StdOut, (LPCVOID)"Hello\n\n", 0x7, NULL, NULL);

    Buffer = (PTEST_STRUCT)HeapAlloc(GetProcessHeap(), 0, 0x8);
    StringBuffer0 = (LPSTR)HeapAlloc(GetProcessHeap(), 0, 0x122);
    StringBuffer1 = (LPSTR)HeapAlloc(GetProcessHeap(), 0, 0x122);

    if (!Buffer)
        return 0;

    RtlSecureZeroMemory(Buffer, 0x8);

    Buffer->Test0 = 0xA42069B;
    Buffer->Test1 = 0xC69420D;

    File = CreateFileA("Test.dat", GENERIC_WRITE, 0x0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    WriteFile(File, Buffer, 0x8, &Length, NULL);

    RtlSecureZeroMemory(Buffer, 0x8);


    CloseHandle(File);
    File = CreateFileA("Test.dat", GENERIC_READ, 0x0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (!ReadFile(File, Buffer, 0x8, NULL, NULL))
        return FALSE;

    StringCbCopyA(StringBuffer0, 0x122, "Test.dat:Test0 = ");
    StringCbPrintfA(StringBuffer1, 0x122, "0x%x", Buffer->Test0);
    StringCbCatA(StringBuffer0, 0x122, StringBuffer1);

    StringCbCatA(StringBuffer0, 0x122, "\nTest.dat:Test1 = ");
    StringCbPrintfA(StringBuffer1, 0x122, "0x%x\n", Buffer->Test1);
    StringCbCatA(StringBuffer0, 0x122, StringBuffer1);

    if (FAILED(StringCchLengthA(StringBuffer0, 0x122, &Length)))
        return FALSE;

    WriteConsoleA(StdOut, StringBuffer0, Length, NULL, NULL);
    Sleep(1000);

    FreeConsole();

    HeapFree(GetProcessHeap(), NULL, Buffer);
    HeapFree(GetProcessHeap(), NULL, StringBuffer0);
    HeapFree(GetProcessHeap(), NULL, StringBuffer1);

    return TRUE;
}

int main(int argc, char** argv) {
#if (!defined(_WIN32) || defined(__MSVCTEST))
    char* __crt_args = malloc(4096);
    if (!__crt_args)
        return 0;
    for (int i = 0; i < argc; i++) {
        if (i != 0)
            strcat(__crt_args, " ");
        strcat(__crt_args, argv[i]);
    }
    long ret = WinMain(&main, NULL, __crt_args, 10);
    free(__crt_args);
    return ret;
#else
    return WinMain(GetModuleHandleA(NULL), NULL, GetCommandLineA(), 10);
#endif
}
