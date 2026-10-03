#include <iostream>
#include <string>
#include <fcntl.h>      // For O_* constants
#include <sys/stat.h>   // For mode constants
#include <sys/mman.h>   // For shm_open, mmap, munmap
#include <unistd.h>     // For close

int main() {
    // 1. Define the unique shared memory object name and size
    // Note: Linux shm names must start with a forward slash "/"
    const char* shm_name = "/MySharedMemory";
    const size_t SHM_SIZE = 4096; // Adjust this to match your writer's size

    std::cout << "Opening shared memory segment: " << shm_name << std::endl;

    // 2. Open the existing shared memory segment (Equivalent to OpenFileMapping)
    int shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        std::cerr << "Failed to open shared memory segment. Ensure the writer is running." << std::endl;
        return 1;
    }

    // 3. Map the shared memory into the process address space (Equivalent to MapViewOfFile)
    void* ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Mapping shared memory failed." << std::endl;
        close(shm_fd);
        return 1;
    }

    // 4. Read data from the memory buffer
    // Casting the pointer to a char array to read text, or cast to a custom struct if needed
    std::cout << "Data read from memory: " << static_cast<char*>(ptr) << std::endl;

    // 5. Clean up resources (Equivalent to UnmapViewOfFile and CloseHandle)
    if (munmap(ptr, SHM_SIZE) == -1) {
        std::cerr << "Unmapping memory failed." << std::endl;
    }

    close(shm_fd);

    // Note: The object is unlinked (deleted) by the WRITER using shm_unlink(shm_name);
    return 0;
}
