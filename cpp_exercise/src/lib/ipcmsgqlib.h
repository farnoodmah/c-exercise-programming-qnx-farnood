#ifndef IPCMSGQLIB_H
#define IPCMSGQLIB_H

#include "filehandlerlib.h"
#include <mqueue.h>
#include <sys/ipc.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <iterator>
#include <time.h>
#include <sys/resource.h>
#include <sys/mman.h>


#define MSG_QUEUE_NAME "/mymsgqueue"
class MsgQueueSender{


    private:
    const std::string _file_name;
    int ret;
    std::vector<unsigned char> _read_file;
    mqd_t _msg_queue;
    struct mq_attr _attrs;
    long unsigned int _msg_queue_msgsize = 4096;
    long unsigned int _msg_queue_maxnummsg = 10;
    std::string _msg_queue_name = MSG_QUEUE_NAME;
    int _check_empty = -1;
    unsigned int _priority;

    MsgQueueSender();
  

    public:
    MsgQueueSender(const std::string & filename);
    virtual ~MsgQueueSender(){};
}; 

class MsgQueueReceiver{

    private:
    const std::string _file_name;
    int ret;
    std::vector<unsigned char> _read_file;
    int _receive_size;
    mqd_t _msg_queue;
    struct mq_attr _attrs;
    unsigned int _priority;
    long unsigned int _msg_queue_msgsize = 4096;
    long unsigned int _msg_queue_maxnummsg = 10;
    int _check_empty = 0;
    std::string _msg_queue_name = MSG_QUEUE_NAME;
    struct   timespec _ts;
    MsgQueueReceiver();
    
    public:
    MsgQueueReceiver(const std::string & filename);
    virtual ~MsgQueueReceiver(){};
}; 


#endif