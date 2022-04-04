#include "linuxipclib.h"



/***
 * Protocol Enum
 * 
 * Checking the protocol name as a string
 * **/


ipcprt::Protocol IPC::protocolOptions(const std::string & input){
    if (input == "pipe") return ipcprt::Protocol::pipe;
    if (input == "msgqueue") return ipcprt::Protocol::msgqueue;
    if (input == "shm") return ipcprt::Protocol::shm;

    return ipcprt::Protocol::invalid;
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
   
   try{
     PipeSender pipes(filename);
   }catch(IPCException & e){
     std::cout<<e.what()<<std::endl;
    exit(EXIT_FAILURE);
   }
}


/**
 * @brief IPCSender Constructor for Choosing the one of the protocols
 * 
 * 
 * 
 */

IPCSender::IPCSender(const std::string & filename, const std::string  & protocol) : _file_name(filename), _protocol(protocol){
    switch (protocolOptions(_protocol))
    {
    case ipcprt::Protocol::pipe:
        ipcpipe(_file_name);
        break;
    
    default:
       std::cout<<"IPCReceiver ERROR:UNKNOWN IPC PROTOCOL"<<std::endl;
       exit(EXIT_FAILURE);
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

   try{
      PipeReceiver piper(filename);
   }catch(IPCException & e){
     std::cout<<e.what()<<std::endl;
    exit(EXIT_FAILURE);
   }
    
}

/**
 * @brief IPCReceiver Choosing Between the Protocols
 * */

IPCReceiver::IPCReceiver(const std::string & filename, const std::string  & protocol): _file_name(filename), _protocol(protocol){
     switch (protocolOptions(_protocol))
    {
    case ipcprt::Protocol::pipe:
        ipcpipe(_file_name);
        break;
    
    default:
        std::cout<<"IPCReceiver ERROR:UNKNOWN IPC PROTOCOL"<<std::endl;
        exit(EXIT_FAILURE);
        break;
    }

}
    
/**
 * @brief Command Option Class
 * 
 */

 
 CommandOption::~CommandOption(){

     _option = 0;
     optind = 0;
 }


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
			{"queue", no_argument, NULL, 'q'},
			{"pipe", no_argument, NULL, 'p'},
			{"shm", required_argument, NULL, 's'},
			{"file", required_argument, NULL, 'f'},
			{ 0, 0, 0, 0 }
      };

      _option = getopt_long(_argc, argv, "h:q:p:s:f:",longopts, &_longindex);

      if(_option == -1)
      break;

       switch (_option)
           {
              case 'h':
              printHelp();
              exit(EXIT_SUCCESS);
                break; 
              case 'p':
               _output = "pipe";
               break;  
              case 'q':
              throw IPCException("unrecognized command. please use \"--help\" for guide.\n");
               break;
              case 's':
              throw IPCException("unrecognized command. please use \"--help\" for guide.\n");
                break;
                case 'f':
                _filename = optarg;
                if ((_filename.size() < 1) || (_filename.size() >40) )
                {
                  throw IPCException("unrecognized command. the file name should be between 1 and 30 letters");
                }
                
                  

                 break;
              case ':':
                throw IPCException("unrecognized command. please use \"--help\" for guide.\n");
                break;
                case '?':
	              throw IPCException("unrecognized command. please use \"--help\" for guide.\n");
                break;
                default:
	              throw IPCException("unrecognized command. please use \"--help\" for guide.\n");
                break;
              }
    }

	}

/**
 * @brief Construct a new Command Option for Handling the terminal arguments
 * 
 * 
 * 
 * 
 */
  
/**
 * @brief Returning the ipc protocol and filename
 * 
 * 
 */
std::vector<std::string> CommandOption::getCommand(){

  if (_filename.size() == 0 || _output.size() == 0 )
  {
    throw IPCException("unrecognized command. filename and protocol should be determined. please use \"--help\" for guide.\n");
  }
  
  _options.push_back(_filename);
  _options.push_back(_output);
  

  return _options;
}

/**
 * @brief Printing the help of IPC program by inputing --help in terminal
 * 
 * 
 */

void  CommandOption::printHelp(){
  
    std::cout<<"\n\n    usage: "<< _program << "[--help] [--messages --file <path>] [--queue --file <path>][--pipe --file <path>]\n		             [shm --file <path> <buffer_size_in_kb>]\n\n\n" <<
					   _program << " is used to "<< ((_program == _ipcreceiver) ? "receive" : "send") << " files between a client "<< "(" + _ipcsender + ")" << " and server " << "(" + _ipcreceiver + ")" <<  " via different IPC methods (queue, pipe, and shm).\n" <<
						"Primary commands:\n\n" <<
            "--pipe        For "<<((_program == _ipcreceiver) ? "receiving" : "sending") <<"  files with the pipe option. (*not implemented)\n" <<
						"--queue       For "<<((_program == _ipcreceiver) ? "receiving" : "sending") <<" files with the message queue option. (*not implemented)\n" <<
						"--shm         For "<<((_program == _ipcreceiver) ? "receiving" : "sending") <<"  files by using a shared memory buffer. (*not implemented)\n\n" <<
						"--help        lists available commands and guides.\n\n";
   
 } 