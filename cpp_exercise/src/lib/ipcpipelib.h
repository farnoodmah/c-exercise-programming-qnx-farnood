#ifndef IPCPIPE_H
#define IPCPIPE_H

#include "filehandlerlib.h"
#include "ipcexceptionlib.h"
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <vector>
#include <getopt.h>



#define PIPE_FIFO_NAME "/tmp/myfifo"

class PipeSender{
    private:
    const std::string _file_name;
    size_t _file_size;
    const char * _myfifo = PIPE_FIFO_NAME;
    int ret;
    std::vector<unsigned char> _read_file;
    int _check_fifo = -1;
    
    uint16_t fd;
    PipeSender();
    

    public:
    PipeSender(const std::string & filename);
    virtual ~PipeSender();

};


class PipeReceiver{
    private:
    const std::string _file_name;
    size_t _file_size;
    const char * _myfifo = PIPE_FIFO_NAME;
    int ret;
    std::string _read_file;
    int _check_fifo = -1;
    std::vector<unsigned char> _readbuffer;
    int _fifo = -1;
    uint16_t fd;
    PipeReceiver();
    

    public:
    PipeReceiver(const std::string & filename);
    virtual ~PipeReceiver();

};


#endif