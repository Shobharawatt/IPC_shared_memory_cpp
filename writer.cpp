#include <iostream>
#include <string>
#include <cstring>      // For std::memcpy
#include <fcntl.h>      // For O_* constants (O_CREAT, O_RDWR)
#include <sys/stat.h>   // For mode constants (S_IRUSR, S_IWUSR)
#include <sys/mman.h>   // For shm_open, shm_unlink, mmap, munmap, PROT_READ, PROT_WRITE
#include <unistd.h>     // For ftruncate, close

int main() {
    // Shared memory identifiers must start with a single forward slash '/'
    const char* sharedMemName = "/MySharedMemory";
    const int bufferSize = 256;

    // 1. Create and open the shared memory object (Equivalent to CreateFileMappingA)
    // O_CREAT creates it if missing; O_RDWR grants read/write access.
    // 0666 sets read/write permissions for user, group, and others.
    int shm_fd = shm_open(sharedMemName, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        std::cerr << "Could not create shared memory object." << std::endl;
        return 1;
    }

    // 2. Configure the size of the shared memory segment
    // Win32 handles sizing inside CreateFileMapping, but POSIX requires ftruncate.
    if (ftruncate(shm_fd, bufferSize) == -1) {
        std::cerr << "Could not set shared memory buffer size." << std::endl;
        close(shm_fd);
        return 1;
    }

    // 3. Map the shared memory into the process address space (Equivalent to MapViewOfFile)
    char* buffer = (char*) mmap(
        nullptr,
        bufferSize,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        shm_fd,
        0
    );

    if (buffer == MAP_FAILED) {
        std::cerr << "Could not map view of shared memory." << std::endl;
        close(shm_fd);
        return 1;
    }

    // 4. Prompt user and write memory
    std::cout << "Enter a message to write to shared memory: ";
    std::string message;
    std::getline(std::cin, message);

    // Enforce safety bounds to avoid overloading your 256-byte buffer
    size_t writeSize = std::min(message.size() + 1, static_cast<size_t>(bufferSize));
    
    // CopyMemory translates straight to standard C++ std::memcpy
    std::memcpy(buffer, message.c_str(), writeSize);
    buffer[bufferSize - 1] = '\0'; // Guarantee safe null-termination

    std::cout << "Data written: " << buffer << std::endl;
    std::cout << "Press Enter to exit writer (keep this window open until reader has run)...";
    std::cin.get();

    // 5. Clean up local memory handles (Equivalent to UnmapViewOfFile and CloseHandle)
    munmap(buffer, bufferSize);
    close(shm_fd);

    // 6. Destroy the shared memory segment from the OS kernel
    // Unlike Windows, Linux shared memory stays alive in the kernel after processes close.
    // shm_unlink safely marks it for deletion once all handles detach.
    shm_unlink(sharedMemName);

    return 0;
}
