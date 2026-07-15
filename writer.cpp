#include <windows.h>
#include <iostream>
#include <string>

int main() {
    const char* sharedMemName = "Local\\MySharedMemory";
    const int bufferSize = 256;

    HANDLE hMapFile = CreateFileMappingA(
        INVALID_HANDLE_VALUE,    // use system paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // max size (high-order DWORD)
        bufferSize,              // max size (low-order DWORD)
        sharedMemName            // name of mapping object
    );

    if (hMapFile == NULL) {
        std::cerr << "Could not create file mapping object. Error: " << GetLastError() << std::endl;
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

    std::cout << "Enter a message to write to shared memory: ";
    std::string message;
    std::getline(std::cin, message);

    CopyMemory(buffer, message.c_str(), message.size() + 1);
    std::cout << "Data written: " << message << std::endl;
    std::cout << "Press Enter to exit writer (keep this window open until reader has run)...";
    std::cin.get();

    UnmapViewOfFile(buffer);
    CloseHandle(hMapFile);
    return 0;
}