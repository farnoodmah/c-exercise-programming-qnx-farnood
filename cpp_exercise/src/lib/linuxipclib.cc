#include "linuxipclib.h"
#include "ipcexceptionlib.h"



/***
 * Protocol Enum
 * 
 * Checking the protocol name as a string
 * **/


Protocol IPC::protocolOptions(const std::string & input){
    if (input == "pipe") return Protocol::Pipe;
    if (input == "msgqueue") return Protocol::MsgQueue;
    if (input == "shm") return Protocol::SHM;
    return Protocol::Invalid;
}

/**
 * @brief IPCSender Class
 * */



/**
 * @brief IPCSender Pipe Protocol
 * 
 * 
 * 
 */


void IPCSender::ipcpipe(const std::string & filename){
     PipeSender pipes(filename);
     pipes.pipeTransfer();
}

/**
 * @brief IPCSender MsgQueue Protocol
 * 
 * 
 * 
 */


void IPCSender::ipcmsgqueue(const std::string & filename){
      MsgQueueSender msgqs(filename);
      msgqs.msgqTransfer();
}

/**
 * @brief IPCSender SharedMemorySender Protocol
 * 
 * 
 * 
 */

void IPCSender::ipcshm(const std::string & filename){
      SharedMemorySender shms(filename);
      shms.shmTransfer();
}

/**
 * @brief IPCSender Constructor for Choosing the one of the protocols
 * 
 * 
 * 
 */

IPCSender::IPCSender(const std::string & filename, const std::string  & protocol) : _file_name(filename), _protocol(protocol){
    switch (protocolOptions(_protocol)){
      case Protocol::Pipe:
        ipcpipe(_file_name);
        break;
      case Protocol::MsgQueue:
        ipcmsgqueue(_file_name);
        break;
      case Protocol::SHM:
        ipcshm(_file_name);
        break; 
      default:
        throw IPCException("IPCReceiver ERROR:UNKNOWN IPC PROTOCOL");
        break;
    }
}


/**
 * @brief IPCReceiver Class
 * 
 * 
 * 
 */

/**
 * @brief IPCReceiver Pipe Protocol
 * */

void IPCReceiver::ipcpipe(const std::string & filename){      
      PipeReceiver piper(filename);  
      piper.pipeTransfer();
}

/**
 * @brief IPCReceiver MsgQueue Protocol
 * */

void IPCReceiver::ipcmsgqueue(const std::string & filename){
      MsgQueueReceiver msgqr(filename);
      msgqr.msgqTransfer();
}

/**
 * @brief IPCReceiver SharedMemory Protocol
 * */

void IPCReceiver::ipcshm(const std::string & filename){
      SharedMemoryReceiver shmr(filename);
      shmr.shmTransfer();
}


/**
 * @brief IPCReceiver Choosing Between the Protocols
 * */

IPCReceiver::IPCReceiver(const std::string & filename, const std::string  & protocol): _file_name(filename), _protocol(protocol){
    switch (protocolOptions(_protocol)){
        case Protocol::Pipe:
             ipcpipe(_file_name);
             break;
        case Protocol::MsgQueue:
            ipcmsgqueue(_file_name);
            break;
        case Protocol::SHM: 
            ipcshm(_file_name);
            break;
        default:
            throw IPCException("IPCReceiver ERROR:UNKNOWN IPC PROTOCOL");
            break;
    }
}
    
/**
 * @brief Command Option Class
 * 
 */


/**
 * @brief Construct a new Command Option for Handling the terminal arguments
 * 
 * 
 * 
 * 
 */

CommandOption::CommandOption(const std::string & program, int argc, char *argv[]): _program(program), _argc(argc){
    optind = 0;
    while(1){   
        _longindex = 0;
        option longopts[]= {
			  {"help", no_argument, NULL, 'h'},
			  {"msgqueue", no_argument, NULL, 'q'},
			  {"pipe", no_argument, NULL, 'p'},
			  {"shm",  no_argument, NULL, 's'},
			  {"file", required_argument, NULL, 'f'},
			  { 0, 0, 0, 0 }
        };
        _option = getopt_long(_argc, argv, "h:q:p:s:f:",longopts, &_longindex);
        if(_option == -1){
          break;
        }
        switch (_option){
              case 'h':
              printHelp();
              exit(EXIT_SUCCESS);
                break; 
              case 'p':
               _output = "pipe";
               break;  
              case 'q':
              _output = "msgqueue";
               break; 
              case 's':
              _output = "shm";
                break;
              case 'f':
                _filename = optarg;
                if ((_filename.size() < 1) || (_filename.size() >40)){
                  throw IPCException("unrecognized command. the file name should be between 1 and 30 letters");
                }
                break;
              case ':':
                throw IPCException("unrecognized command. please use \"--help\" for guide.\n");
              case '?':
	              throw IPCException("unrecognized command. please use \"--help\" for guide.\n");
              default:
	              throw IPCException("unrecognized command. please use \"--help\" for guide.\n");
              }
      }
      if (_filename.size() == 0 || _output.size() == 0 ){
          throw IPCException("unrecognized command. filename and protocol should be determined. please use \"--help\" for guide.\n");
      }
      _options.push_back(_filename);
      _options.push_back(_output);
	}

 CommandOption::~CommandOption(){
   optind = 0;
 } 
  
/**
 * @brief Returning the ipc protocol and filename
 * 
 * 
 */
std::vector<std::string> CommandOption::getCommand(){
      return _options;
}

/**
 * @brief Printing the help of IPC program by inputing --help in terminal
 * 
 * 
 */

void  CommandOption::printHelp(){
            std::cout<<"\n\n    usage: "<< _program << "[--help] [--messages --file <path>] [--queue --file <path>][--pipe --file <path>]\n		             [shm --file <path>]\n\n\n" <<
					   _program << " is used to "<< ((_program == _ipcreceiver) ? "receive" : "send") << " files between a client "<< "(" + _ipcsender + ")" << " and server " << "(" + _ipcreceiver + ")" <<  " via different IPC methods (queue, pipe, and shm).\n" <<
						"Primary commands:\n\n" <<
            "--pipe        For "<<((_program == _ipcreceiver) ? "receiving" : "sending") <<"  files with the pipe option.\n" <<
						"--msgqueue       For "<<((_program == _ipcreceiver) ? "receiving" : "sending") <<" files with the message queue option.\n" <<
						"--shm         For "<<((_program == _ipcreceiver) ? "receiving" : "sending") <<"  files by using a shared memory buffer.\n\n" <<
						"--help        lists available commands and guides.\n\n";
 } 