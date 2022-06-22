#include "ipcpipelib.h"
#include "ipcexceptionlib.h"
/***
 * PipeSender 
 * **/
volatile sig_atomic_t abort_eh = 0;
void handler(int Sig){
    abort_eh = 1;
}

PipeSender::PipeSender(const std::string & filename): _file_name(filename){
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCSender: PIPE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        _check_fifo = -1;
        int errf = -1;
        while((errf< 0)){
            ++_counter;
            if (_counter>5)
            {
              throw IPCException("IPCSender ERROR: Cannot connect to the IPCReceiver."); 
            }
            std::cout<<"          Waiting for The Receiver... Try:"<<_counter<<"/5"<<std::endl;
            //checking the existence of fifo
            errf = access( pipe_fifo_name.c_str(), F_OK ) ;
            sleep(2);
        }
}

void PipeSender::pipeTransfer(){
        struct sigaction sa;
        sa.sa_flags = 0;
        sa.sa_handler = handler;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGALRM,&sa,0);
        alarm(5);
        _check_fifo = open(pipe_fifo_name.c_str(), O_WRONLY);
        if(_check_fifo < 0){
            if(errno==EINTR){
                if(abort_eh){
                  close(_check_fifo);
                  remove(pipe_fifo_name.c_str());
                  throw IPCException("IPCSender ERROR: Cannot connect to the IPCReceiver.");
                }  
            }
            else{
                throw IPCException("IPCSender ERROR: Cannot open the pipe");
            }
        }
        alarm(0);
        FileHandler fd(_file_name);
        _file_size = fd.getSize();
        std::cout<<"          File Name: "<<_file_name<<std::endl;
        std::cout<<"          File Size: "<<_file_size<<" bytes"<<std::endl;
        //opening for the file the be read
        fd.openForReading();
        while (true){ 
          //reading the file 
          _read_file = fd.readFile();
           //the end of the file
          if(_read_file.size()==0){
             break;
          }
          if(_read_file.size()>0){
            sigaction(SIGPIPE,&sa,0);
            //writing to the pipe    
            err = write(_check_fifo,_read_file.data(),_read_file.size());
            if(err<0){
                throw IPCException("IPCSender ERROR: Cannot connect to the IPCReceiver.");
            }
          }
        }
        close(_check_fifo);
        _check_fifo = access( pipe_fifo_name.c_str(), F_OK );
        _counter = 0;
        while(_check_fifo == 0){
              ++_counter;
              if (_counter > 5){
                  throw IPCException("    IPCSender ERROR: IPCReceiver did not receive the file successfully");
              }
              std::cout<<"    Checking the IPCReceiver received the file completely... Try:" << _counter << "/5" << std::endl;
              _check_fifo = access( pipe_fifo_name.c_str(), F_OK );
              if(_check_fifo == -1){
                break;
              }
              sleep(5);
        }
        std::cout<<"          Successfully written to the Pipe "<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCSender: PIPE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
}

PipeSender::~PipeSender(){
        close(_check_fifo);
}

/**
 * @brief PipeReceiver
 * 
 * 
 */
PipeReceiver::PipeReceiver(const std::string & filename): _file_name(filename){
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCReceiver: PIPE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"          File Name: "<<_file_name<<std::endl;
        remove(pipe_fifo_name.c_str());
        //making the FIFO pipe
        ret = mkfifo(pipe_fifo_name.c_str(), 0666);
        if (ret<0){
            throw IPCException(" IPCReceiver ERROR: Cannot make the pipe. " + std::string(strerror(errno)));
        } 
}

void PipeReceiver::pipeTransfer(){
      struct sigaction sa;
      sa.sa_flags = 0;
      sa.sa_handler = handler;
      sigemptyset(&sa.sa_mask);
      sigaction(SIGALRM,&sa,0);
      alarm(10);
      _fifo = open(pipe_fifo_name.c_str(),O_RDONLY);
      if(_fifo < 0){
            if(errno==EINTR){
                if(abort_eh){
                  throw IPCException("IPCReceiver ERROR: Cannot connect to the IPCSender. ");
                }  
            }
            else{
                throw IPCException("IPCReceiver ERROR: Cannot open  the pipe.");
            } 
      }
      alarm(0); 
      FileHandler fd2(_file_name);
      fd2.createFile();
      //reading file in chunks of 4096 bytes from the pipe and saving them in a new file
      std::cout<<"          Starting to read the data: "<<std::endl;
      while (true){
          std::vector<unsigned char> smallbuffer(_buffer_size);
          bytesread = read(_fifo,smallbuffer.data(),_buffer_size);
          if(bytesread<0){ 
            throw IPCException(" IPCReceiver ERROR: Cannot read the pipe. " + std::string(strerror(errno)));
          }
          else if(bytesread == 0){
             break;
          }
          smallbuffer.resize(bytesread);
          fd2.writeFile(smallbuffer,smallbuffer.size());
      }
      std::cout<<"          Received The File Successfully "<<std::endl; 
      std::cout<<"          Save the data into the:  "<<_file_name<<std::endl;
      std::cout<<"**************************************************************"<<std::endl;
      std::cout<<"****************************IPCReceiver: PIPE PROTOCOL**************************"<<std::endl;
      std::cout<<"**************************************************************"<<std::endl;
      std::cout<<"**************************************************************"<<std::endl;
}

PipeReceiver::~PipeReceiver(){ 
      close(_fifo);
      remove(pipe_fifo_name.c_str());
}