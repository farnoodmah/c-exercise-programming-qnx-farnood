#ifndef IPCPIPELIB_H
#define IPCPIPELIB_H

#include "filehandlerlib.h"
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <signal.h>
#include <sys/types.h>

const std::string pipe_fifo_name = "/tmp/myfifo";


class PipeSender{
    private:
        const std::string _file_name; 
        size_t _file_size;
        int ret;
        std::vector<unsigned char> _read_file;
        int _check_fifo = -1;
        int err;
        int _counter = 0;
        PipeSender();
    
    public:
        PipeSender(const std::string & filename);
        void pipeTransfer();
        ~PipeSender();
};


class PipeReceiver{
    private:
        const std::string _file_name;
        long unsigned int _buffer_size = file_data_read_size;
        int _fifo = -1;
        int bytesread = 1;
        int ret = 0;
        PipeReceiver();

    public:
        PipeReceiver(const std::string & filename);
        void pipeTransfer();
        ~PipeReceiver();
};


#endif