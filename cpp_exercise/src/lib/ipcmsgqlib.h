#ifndef IPCMSGQLIB_H
#define IPCMSGQLIB_H

#include "filehandlerlib.h"
#include <mqueue.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <mqueue.h>
#include <errno.h>
#include <iterator>

#define P4IPC_MSGSIZE 100
class MsgQueueSender{


    private:
    const std::string _file_name;
    int ret;
    std::vector<unsigned char> _read_file;
    MsgQueueSender();
    key_t _private_key;
    int _msgid;

    public:
    MsgQueueSender(const std::string & filename);
    virtual ~MsgQueueSender(){};
}; 

class MsgQueueReceiver{

    private:
    const std::string _file_name;
    int ret;
    std::vector<unsigned char> _read_file;
    MsgQueueReceiver();
    key_t _private_key;
    int _msgid;
    
    public:
    MsgQueueReceiver(const std::string & filename);
    virtual ~MsgQueueReceiver(){};
}; 


#endif