# IPC via Shared Memory (C++ / Windows API)

Two separate processes — a writer and a reader — communicate 
through a shared memory segment using the Windows API 
(CreateFileMapping, MapViewOfFile).

## What it does
`writer.cpp` creates a named shared memory mapping and writes a 
user-provided message into it. `reader.cpp` opens the same shared 
memory mapping (by name) and reads the message back out. The two 
programs run as separate processes but share data without using 
sockets, pipes, or files on disk.

## Concepts demonstrated
- Inter-process communication (IPC)
- Shared memory mapping (CreateFileMapping, MapViewOfFile)
- Cross-process data sharing on Windows
- Low-level Win32 API usage in C++

## How to build and run
\`\`\`
g++ writer.cpp -o writer.exe
g++ reader.cpp -o reader.exe
\`\`\`

Run in two separate terminal windows (both processes need to be 
running to share memory):

1. In terminal 1: \`.\writer.exe\` — enter a message when prompted, 
   then keep the window open (press Enter to exit once done testing)
2. In terminal 2: \`.\reader.exe\` — reads back the message written 
   by the writer

## Why I built this
Reinforcing IPC concepts used in prior embedded systems work 
(device-scanning modules with IPC-based communication), adapted 
here to the Windows API since this environment doesn't support 
POSIX shared memory (sys/shm.h).

## Notes
This uses the Windows-specific shared memory API rather than the 
POSIX System V IPC calls (shmget/shmat) used on Linux/Mac, since 
the underlying concept — processes sharing a memory segment — is 
the same regardless of OS-specific API.