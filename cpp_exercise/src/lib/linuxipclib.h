#ifndef LINUXIPCLIB_H
#define LINUXIPCLIB_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <getopt.h>
#include "ipcpipelib.h"
#include "ipcmsgqlib.h"
#include "ipcshmlib.h"


enum class Protocol{Invalid, Pipe, SHM, MsgQueue};
/**
 *      IPC Class
 */

class IPC{

    protected:
        IPC(){};
        virtual ~IPC(){};
        virtual void ipcpipe(const std::string & filename)=0;
        virtual void ipcmsgqueue(const std::string & filename)=0;
        virtual void ipcshm(const std::string & filename)=0;
        Protocol protocolOptions(const std::string & input);
};
class IPCSender: public IPC{
    private:
        const std::string _file_name;
        const std::string _protocol;
        void ipcpipe(const std::string & filename) override;
        void ipcmsgqueue(const std::string & filename) override;
        void ipcshm(const std::string & filename)   override;
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
        void ipcmsgqueue(const std::string & filename) override;
        void ipcshm(const std::string & filename) override;
        IPCReceiver();

    public:
        IPCReceiver(const std::string & filename, const std::string  & protocol); 
        ~IPCReceiver(){};
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
    void printHelp();
    
    public:
    CommandOption(const std::string & program, int argc, char *argv[]);
    std::vector<std::string> getCommand();
    ~CommandOption();

};

#endif