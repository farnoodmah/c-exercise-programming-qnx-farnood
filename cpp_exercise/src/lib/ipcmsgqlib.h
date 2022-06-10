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
#include <time.h>



const std::string msg_queue_name = "/mymsgqueue";

class MsgQueueSender{

    private:
    const std::string _file_name;
    int ret;
    std::vector<unsigned char> _read_file;
    mqd_t _msg_queue;
    struct mq_attr _attrs;
    size_t _msg_queue_msgsize = file_data_read_size;
    int _msg_queue_maxnummsg = 10;
    unsigned int _priority = 1;
    struct   timespec _ts;
    int counter = 0;
    MsgQueueSender();
    void  checkingMsgQueueEmpty();

    public:
    MsgQueueSender(const std::string & filename);
    void msgqTransfer();
    ~MsgQueueSender();
}; 

class MsgQueueReceiver{
    
    private:
        const std::string _file_name;
        int ret;
        std::vector<unsigned char> _read_file;
        int _receive_size;
        mqd_t _msg_queue;
        struct mq_attr _attrs;
        unsigned int _priority = 1;
        int _msg_queue_msgsize = file_data_read_size;
        int _msg_queue_maxnummsg = 10;
        int _check_empty = 0;
        struct  timespec _ts;
        int counter = 0;
        MsgQueueReceiver();
    
    public:
        MsgQueueReceiver(const std::string & filename);
        void msgqTransfer();
        ~MsgQueueReceiver();
}; 


#endif