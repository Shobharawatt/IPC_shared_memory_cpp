# IPC via Shared Memory (C++ / POSIX API)

Two separate processes — a writer and a reader — communicate 
through a shared memory segment using the POSIX shared memory API 
(shm_open, ftruncate, mmap).

## What it does
`writer.cpp` creates and provisions a named shared memory segment in the Linux kernel and writes a user-provided message into it. `reader.cpp` opens the same shared memory segment (by name) and reads the message back out. The two programs run as separate processes but share data without using sockets, pipes, or physical files on disk.

## Concepts demonstrated
- Inter-process communication (IPC)
- POSIX Shared memory mapping (`shm_open`, `ftruncate`, `mmap`)
- Cross-process data sharing on Linux systems
- Low-level UNIX system calls in C++

## How to build and run
POSIX shared memory requires linking against the real-time system library (`-lrt`) on Linux:

```bash
g++ writer.cpp -o writer -lrt
g++ reader.cpp -o reader -lrt
```

Run in two separate terminal windows (both processes need to be 
running to share memory):

1. In terminal 1: `./writer` — enter a message when prompted, 
   then keep the window open (press Enter to exit once done testing)
2. In terminal 2: `./reader` — reads back the message written 
   by the writer

## Why I built this
Reinforcing IPC concepts used in prior embedded systems work (device-scanning modules with IPC-based communication), using standard cross-platform POSIX methodologies native to modern UNIX and embedded Linux deployments.

## Notes
- This uses the POSIX shared memory API (`shm_open`) rather than the older System V IPC calls (`shmget`/`shmat`) or Windows-specific abstractions (`CreateFileMapping`).
- On Linux, shared memory segments are virtual files backed by the kernel, located under the `/dev/shm/` directory. 
- Names of POSIX shared memory blocks must strictly begin with a leading forward slash (e.g., `/MySharedMemory`).
- Unlike Windows, which reclaims shared allocations automatically when processes exit, Linux memory blocks persist in the kernel until explicitly destroyed via `shm_unlink`.
