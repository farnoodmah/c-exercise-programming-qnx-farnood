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

const size_t file_data_chunk = 4096;
const std::string shm_name = "shm"; 
const std::string semaphoresender_name = "/semsender"; 
const std::string semaphorereceiver_name  = "/semreceiver"; 
class SharedMemorySender {

    private:
    
      struct _shm_data_struct{
       size_t datasize;
       unsigned char data[file_data_chunk];
   };
    sem_t *_sem_receiver;
    sem_t *_sem_sender;
    const std::string _file_name;
    const int _shm_size = sizeof(_shm_data_struct);    
    int _shm_fd = -1;
    struct _shm_data_struct *_ptr;
    int _err;
    SharedMemorySender();

    public:
    SharedMemorySender(const std::string filename);
    ~SharedMemorySender();

};

class SharedMemoryReceiver {

    private:
     struct _shm_data_struct{
       size_t datasize;
       unsigned char data[file_data_chunk];
    };

    const std::string _file_name;;
    const int _shm_size = sizeof(_shm_data_struct);
    int _shm_fd = -1; 
    struct _shm_data_struct *_ptr;
    sem_t *_sem_receiver;
    sem_t *_sem_sender;
    struct timespec _ts;
    const size_t _buffer_size = file_data_chunk;
   
    SharedMemoryReceiver();

    public:
    SharedMemoryReceiver(const std::string filename);
    ~SharedMemoryReceiver();

};
#endif