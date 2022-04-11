#ifndef IPCSHMLIB_H
#define IPCSHMLIB_H

#include "filehandlerlib.h"
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <vector>
#include <getopt.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <algorithm>

// Buffer data structures
#define MAX_BUFFERS 10

#define LOGFILE "/tmp/example.log"

#define SEM_MUTEX_NAME "/sem-mutex"
#define SEM_BUFFER_COUNT_NAME "/sem-buffer-count"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal"
#define SHARED_MEM_NAME "/posix-shared-mem-example"

struct shared_memory {
    char buf [MAX_BUFFERS] [256];
    int buffer_index;
    int buffer_print_index;
};



/**
 * SharedMemorySender
 * 
 */


class SharedMemorySender {

    private:
    const std::string _file_name;
    sem_t * _semaphore_p[2];
    int shmid;
    const int _shm_size = 4096;
    const std::string _shm_name = "/shm"; 
    const std::string _semaphoresender_name = "/semsender"; 
    const std::string _semaphorereceiver_name  = "/semreceiver"; 
    int _shm_fd;
    void* _ptr;
    SharedMemorySender();

    public:
    SharedMemorySender(const std::string filename);
    virtual ~SharedMemorySender(){};

};

class SharedMemoryReceiver {

    private:
    const std::string _file_name;
    sem_t * _semaphore_p[2];
    int shmid;
    const int _shm_size = 4096;
    const std::string _shm_name = "/shm"; 
    const std::string _semaphoresender_name = "/semsender"; 
    const std::string _semaphorereceiver_name  = "/semreceiver"; 
    int _shm_fd;
    void* _ptr;
    SharedMemoryReceiver();

    public:
    SharedMemoryReceiver(const std::string filename);
    virtual ~SharedMemoryReceiver(){};

};
#endif