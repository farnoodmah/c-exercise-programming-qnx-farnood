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



/**
 * SharedMemorySender
 * 
 */


class SharedMemorySender {

    private:
      struct _shm_data_struct{
       size_t datasize;
       unsigned char data[4096];
   };
    sem_t *_sem_receiver;
    sem_t *_sem_sender;
    const std::string _file_name;
    const int _shm_size = 4104;
    const std::string _shm_name = "shm"; 
    int _shm_fd = -1;
    const std::string _semaphoresender_name = "/semsender"; 
    const std::string _semaphorereceiver_name  = "/semreceiver"; 
    struct _shm_data_struct *_ptr;
    int _err;
    SharedMemorySender();

    public:
    SharedMemorySender(const std::string filename);
    virtual ~SharedMemorySender();

};

class SharedMemoryReceiver {

    private:
     struct _shm_data_struct{
       size_t datasize;
       unsigned char data[4096];
    };

    const std::string _file_name;;
    const int _shm_size = 4104;
    const std::string _shm_name = "shm"; 
    int _shm_fd = -1;
    const std::string _semaphoresender_name = "/semsender"; 
    const std::string _semaphorereceiver_name  = "/semreceiver"; 
    struct _shm_data_struct *_ptr;
    sem_t *_sem_receiver;
    sem_t *_sem_sender;
    struct timespec _ts;
    const size_t _buffer_size = 4096;
   
    SharedMemoryReceiver();

    public:
    SharedMemoryReceiver(const std::string filename);
    virtual ~SharedMemoryReceiver();

};
#endif