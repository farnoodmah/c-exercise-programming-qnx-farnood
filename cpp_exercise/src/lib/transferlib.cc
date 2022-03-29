#include "transferlib.h"



/**
 *
 * FileHandler Methods 
 * 
 * 
 */

FileHandler::FileHandler(const std::string & filename) : _file_name(filename){

}

/**
 * @brief Destroy the File Handler:: File Handler object
 * 
 */

FileHandler::~FileHandler(){
    
}


//opening and creating the files
void FileHandler::createFile(){

    
   
    _fd = open(_file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, _mode);

    close(_fd);
}


void FileHandler::writeFile(std::vector<unsigned char> & file_input, size_t write_size){

    
     _write_size = write_size;
    std::cout<<&file_input[0]<<std::endl;
    _fd = open(_file_name.c_str(), O_WRONLY | O_CREAT  | O_TRUNC, _mode);
    write(_fd, &file_input[0], _write_size);

    close(_fd);
    
}

std::vector<unsigned char> FileHandler::readFile(size_t read_size){
    
    std::cout<<"the file name is "<<_file_name<<std::endl;
    _read_size = read_size;
    std::cout<<"the file size"<< _read_size<<std::endl;
    std::vector<unsigned char> readbuffer;
    readbuffer.resize(read_size);
    size_t already_size ;
      _fd = open(_file_name.c_str(), O_RDONLY | O_LARGEFILE, _mode);
    off_t chunk =0;
    lseek64(_fd,0,SEEK_SET);

    printf("the file des %d \n", _fd);

      
    
    while(chunk < read_size){
        size_t ph;
        ph = read(_fd, &readbuffer[0] + chunk, 2);
        chunk = chunk + ph;
    }

    std::cout<<"the first element of the vector is "<<readbuffer[0]<<std::endl;
    std::cout<<"reading" << chunk<< "bytes"<<std::endl;

    close(_fd);
    
    
    return readbuffer;
}



size_t FileHandler::getSize(){

_fd = open(_file_name.c_str(), O_RDONLY, _mode);
struct stat file_stat;
stat(_file_name.c_str(),&file_stat);

_file_size = file_stat.st_size;
close(_fd);

std::cout<<_file_size<<std::endl;

return _file_size;

}

/***
 * IPC
 * **/


ipcprt::Protocol IPC::protocolOptions(const std::string & input){
    if (input == "pipe") return ipcprt::Protocol::pipe;
    if (input == "shm") return ipcprt::Protocol::shm;
    if (input == "shm") return ipcprt::Protocol::shm;

    return ipcprt::Protocol::invalid;
}

/***
 * IPCSender
 * **/

void IPCSender::ipcpipe(const std::string & filename){
   std::cout<<"the IPCSender protocol is pipe"<<std::endl;
   std::cout<<"the IPCSender file name is "<<filename<<std::endl;

   PipeSender pipes(filename);
}


IPCSender::IPCSender(const std::string & filename, const std::string  & protocol) : _file_name(filename), _protocol(protocol){
    switch (protocolOptions(_protocol))
    {
    case ipcprt::Protocol::pipe:
        ipcpipe(_file_name);
        break;
    
    default:
        std::cout<<"wtf"<<std::endl;
        break;
    }
}


/***
 * IPCReceiver
 * **/

void IPCReceiver::ipcpipe(const std::string & filename){

        std::cout<<"the IPCReceiver protocol is pipe"<<std::endl;
        std::cout<<"the IPCReceiver file name is "<<filename<<std::endl; 

        PipeReceiver piper(filename);
    
}


IPCReceiver::IPCReceiver(const std::string & filename, const std::string  & protocol): _file_name(filename), _protocol(protocol){
     switch (protocolOptions(_protocol))
    {
    case ipcprt::Protocol::pipe:
        ipcpipe(_file_name);
        break;
    
    default:
        std::cout<<"wtf"<<std::endl;
        break;
    }

}
    

/***
 * PipeSender 
 * **/


PipeSender::PipeSender(const std::string & filename): _file_name(filename){

         FileHandler fd(_file_name);
         _file_size = fd.getSize();
         _read_file = fd.readFile(_file_size);
    

         std::cout<<"the reader fille is "<<&_read_file[0]<<std::endl;
         remove(_myfifo);
         ret = mkfifo(_myfifo, 0666);
         std::cout<<"the ret is "<<ret<<std::endl;
         _check_fifo = open(_myfifo, O_WRONLY);
         if(_check_fifo == -1){
             perror("open");
             exit(EXIT_FAILURE);
         }
         std::cout<<"Pipe is Open "<< std::endl;
         write(_check_fifo,&_read_file[0],_file_size);
         std::cout<<"Successfully sent"<<std::endl;
         
         close(_check_fifo);

}

/***
 * PipeSender 
 * **/

PipeReceiver::PipeReceiver(const std::string & filename): _file_name(filename){

        FileHandler fd2(_file_name);
        fd2.createFile();
        std::vector<unsigned char> buffer;
         int set=0;
          int fifo = -1;
         sleep(5);

          while(fifo==-1 || fifo == 0){
            sleep(5);
            std::cout<<"receiver opened the pipe"<<std::endl;
            fifo = open(_myfifo,O_RDONLY);

          }

        unsigned char mybuffer[2];
        ssize_t bytesread = 1;

           while ((bytesread = read(fifo,mybuffer, 2)) > 0)
         {
          for(int i = 0; i <2 ;i++){
            buffer.push_back(mybuffer[i]);
          }

          
         }
        

        
        // while(set==0){
            
        //     std::cout<<"reading from the file"<<std::endl;
        //     set = read(fifo, &buffer[0], 20);
        //     std::cout<<"the  read receiver ret is "<<ret<<std::endl;
        // }
        close(fifo);
        remove(_myfifo);
        std::cout<<"the buffer is "<<&buffer[0]<<std::endl;
        
        fd2.writeFile(buffer,20);
       
        
}



// Checkout::Checkout():total(0){

//   //ToDO 
// }

// void Checkout::addItemPrice(std::string item, int price){
//   prices[item] = price;
// }

// void Checkout::addItem(std::string item){
//   total += prices[item];
//   //this is not implemented correctly
//   if(total==0){
//     throw std::invalid_argument("Invalid Item. No price");
//   }
// }

// int Checkout::calculateTotal(){
//   return  total;
// }

// void Checkout::addDiscount(std::string item, int nbrOfItems, int discountPrice){
// ;

// }
 
 CommandOption::~CommandOption(){

     _option = 0;
 }


CommandOption::CommandOption(const std::string & program, int argc, char *argv[]): _program(program), _argc(argc){
    
     
   optind = 0;
   
    std::cout<<"the in put command is "<< argv[1]<< std::endl;

    

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
              _output = printHelp();
              std::cout<<"help"<<std::endl;
                break; 
              case 'p':
               _output = "pipe";
               std::cout<<"pipe"<<std::endl;
               break;  
              case 'q':
               break;
              case 's':
                break;
                case 'f':
                  _filename = optarg;
                 break;
              case ':':
                  std::cout<<"unrecognized command. please use \"--help\" for guide.\n";
	              
                break;
                case '?':
	            std::cout<<"unrecognized command. please use \"--help\" for guide.\n";
                break;
                default:
	              std::cout<<"unrecognized command. please use \"--help\" for guide.\n";

                    break;
              }
    }

	}


  

std::vector<std::string> CommandOption::getCommand(){

  _options.push_back(_filename);
  _options.push_back(_output);
  

  return _options;


   
}

std::string  CommandOption::printHelp(){
  
    std::cout<<"\n\n    usage: "<< _program << "[--help] [--messages --file <path>] [--queue --file <path>][--pipe --file <path>]\n		             [shm --file <path> <buffer_size_in_kb>]\n\n\n" <<
					   _program << " is used to "<< ((_program == _ipcreceiver) ? "receive" : "send") << " files between a client "<< "(" + _ipcsender + ")" << " and server " << "(" + _ipcreceiver + ")" <<  " via different IPC methods (queue, pipe, and shm).\n" <<
						"Primary commands:\n\n" <<
						"--queue       For "<<((_program == _ipcreceiver) ? "receiving" : "sending") <<" files with the message queue option. (*not implemented)\n" <<
						"--pipe        For "<<((_program == _ipcreceiver) ? "receiving" : "sending") <<"  files with the pipe option. (*not implemented)\n" <<
						"--shm         For "<<((_program == _ipcreceiver) ? "receiving" : "sending") <<"  files by using a shared memory buffer. (*not implemented)\n\n" <<
						"--help        lists available commands and guides.\n\n";
   return "";
 } 