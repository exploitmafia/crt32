# crt32
win32 joke implementation

## Purpose
Was bored at school today, so made this. Implements equvilents, not exactly the same functions (i/o in linux sucks)

## Functions

These functions do not have the same function as the real function, and this was made around the code snippet provided.
|  Win32 Function      | Standard Function |
|---------------------|-------------------|
| Sleep               | sleep             |
| GetStdHandle        | Custom            |
| AllocConsole        | None              |
| WriteConsoleA       | fputs             |
| GetProcessHeap      | None              |
| HeapAlloc           | malloc            |
| HeapFree            | free              |
| RtlSecureZeroMemory | Custom            |
| CreateFileA         | fopen             |
| WriteFile           | fwrite            |
| ReadFile            | fread             |
| StringCbCopyA       | strncpy           |
| StringCbPrintfA     | vsprintf          |
| StringCbCatA        | strncat           |
| StringCchLengthA    | strnlen           |
| CloseHandle         | fclose            |
| FreeConsole         | None              |

## Example

The following code example functions exactly the same on linux and windows:
```c
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
}
