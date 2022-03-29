#ifndef TRANSFERLIB_H
#define TRANSFERLIB_H

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

namespace ipcprt {

    enum Protocol {
        invalid,
        pipe,
        shm,
        msgqueue
    };
}

#define PIPE_FIFO_NAME "/tmp/myfifo"

/**
 *      FileHandler Class
 */
class FileHandler{
private:
const std::string _file_name; //input file name
uint8_t _fd;  //file desriptor
std::string _file_input; // the data that should be written into the file
size_t _write_size; // the size of data that should be written
size_t _read_size; // the size of data that should be read
size_t _file_size; // the size of the file
std::string _file_output; // the read data from file

const mode_t  _mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // mode for opening the file

public:
FileHandler(const std::string & filename);
~FileHandler();
void createFile();
void writeFile(std::vector<unsigned char> & file_input, size_t write_size);
std::vector<unsigned char> readFile(size_t read_size);
size_t getSize();
  
};
 

/**
 *      IPC Class
 */

class IPC{
    
    protected:
    IPC(){};
    virtual ~IPC(){};
    virtual void ipcpipe(const std::string & filename)=0;
    virtual void ipcshm()=0;
    virtual void ipcmsgqueue()=0;
    ipcprt::Protocol protocolOptions(const std::string & input);

};


class IPCSender: public IPC{
    private:
    const std::string _file_name;
    const std::string _protocol;
    void ipcpipe(const std::string & filename) override;
    void ipcshm() override{};
    void ipcmsgqueue() override{};
    IPCSender();

    public:
    IPCSender(const std::string & filename, const std::string  & protocol); 
    ~IPCSender(){};
  

};


class IPCReceiver: public IPC{

    private:
    const std::string _file_name;
    const std::string _protocol;
    void ipcpipe(const std::string & filename) override;
    void ipcshm() override{};
    void ipcmsgqueue() override{};
    IPCReceiver();

    public:
    IPCReceiver(const std::string & filename, const std::string  & protocol); 
    ~IPCReceiver(){};

};


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
    ~PipeSender(){};

};


class PipeReceiver{
    private:
    const std::string _file_name;
    size_t _file_size;
    const char * _myfifo = PIPE_FIFO_NAME;
    int ret;
    std::string _read_file;
    int _check_fifo = -1;
    
    uint16_t fd;
    PipeReceiver();
    

    public:
    PipeReceiver(const std::string & filename);
    ~PipeReceiver(){};

};

class CommandOption{
    private:
    static struct option longopts[];
    const std::string _ipcreceiver = "ipcreceiver";
    const std::string _ipcsender = "ipcsender";
    const std::string _program;
    std::string _filename;
    int _longindex = 0;
    int _argc;
     int _option;
    std::vector<std::string> _options;
    std::string _help_message;
    std::string _output;

    CommandOption();
    
    std::string printHelp();
    
    public:
   
    CommandOption(const std::string & program, int argc, char *argv[]);
    std::vector<std::string> getCommand();
    ~CommandOption();

};

#endif