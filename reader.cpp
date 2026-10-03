#include <iostream>
#include <fcntl.h>      // For O_* constants (O_RDWR)
#include <sys/mman.h>   // For shm_open, mmap, munmap, PROT_READ, PROT_WRITE
#include <unistd.h>     // For close()

int main() {
    // Linux shared memory identifiers must start with a leading forward slash '/'
    // and cannot contain backslashes like 'Local\'
    const char* sharedMemName = "/MySharedMemory";
    const int bufferSize = 256;

    // 1. Open the existing shared memory object (Equivalent to OpenFileMappingA)
    // We use O_RDWR to match your original FILE_MAP_ALL_ACCESS permissions
    int shm_fd = shm_open(sharedMemName, O_RDWR, 0666);

    if (shm_fd == -1) {
        std::cerr << "Could not open shared memory object. Make sure the writer is running." << std::endl;
        return 1;
    }

    // 2. Map the shared memory into the process address space (Equivalent to MapViewOfFile)
    char* buffer = (char*) mmap(
        nullptr,
        bufferSize,
        PROT_READ | PROT_WRITE, // Equivalent to FILE_MAP_ALL_ACCESS
        MAP_SHARED,
        shm_fd,
        0
    );

    if (buffer == MAP_FAILED) {
        std::cerr << "Could not map view of shared memory." << std::endl;
        close(shm_fd);
        return 1;
    }

    // 3. Output the contents of the buffer
    std::cout << "Data read from shared memory: " << buffer << std::endl;

    // 4. Clean up resources (Equivalent to UnmapViewOfFile and CloseHandle)
    munmap(buffer, bufferSize);
    close(shm_fd);

    return 0;
}
