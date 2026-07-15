#include <windows.h>
#include <iostream>

int main() {
    const char* sharedMemName = "Local\\MySharedMemory";
    const int bufferSize = 256;

    HANDLE hMapFile = OpenFileMappingA(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        sharedMemName
    );

    if (hMapFile == NULL) {
        std::cerr << "Could not open file mapping object. Error: " << GetLastError() << std::endl;
        std::cerr << "Make sure writer.exe is still running in another window." << std::endl;
        return 1;
    }

    char* buffer = (char*) MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        bufferSize
    );

    if (buffer == NULL) {
        std::cerr << "Could not map view of file. Error: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    std::cout << "Data read from shared memory: " << buffer << std::endl;

    UnmapViewOfFile(buffer);
    CloseHandle(hMapFile);
    return 0;
}