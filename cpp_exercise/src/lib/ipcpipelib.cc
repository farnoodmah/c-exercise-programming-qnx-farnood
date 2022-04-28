#include "ipcpipelib.h"
#include "ipcexceptionlib.h"
/***
 * PipeSender 
 * **/

volatile sig_atomic_t abort_eh;
void handler(int Sig)
{
    abort_eh = 1;
}

static void alaramHandler(int signo){
  (void)signo;
  throw IPCException("IPCSender: Disconnected form the IPCReceiver");
}

PipeSender::PipeSender(const std::string & filename): _file_name(filename){

        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCSender: PIPE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;


        _check_fifo = -1;

      

        while((_check_fifo <= 0)){
          ++_counter;

          if (_counter>5)
          {
            throw IPCException("IPCSender ERROR: Cannot connect to the IPCReceiver."); 
          }
          
          std::cout<<"          Waiting for The Receiver... Try:"<<_counter<<"/5"<<std::endl;

          _check_fifo = open(_myfifo, O_RDWR);

          sleep(5);
          
        }
   
        
}

void PipeSender::pipeTransfer(){

        struct sigaction act;
        memset(&act, 0, sizeof act);
        sigaction(SIGALRM, &act, NULL);


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

          if (_read_file.size()>0){
          
          alarm(5);

          //writing to the pipe    
          err = write(_check_fifo,_read_file.data(),_read_file.size());
          
          signal(SIGALRM, alaramHandler);

          if(err<0){
            throw IPCException("IPCSender ERROR: Cannot connect to the IPCReceiver.");
          }
            
          alarm(0);
             
          }
         
        }

    

        std::cout<<"          Successfully written to the Pipe "<<std::endl;
         
        
        std::cout<<"          Pipe is Closed  "<<std::endl;


        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCSender: PIPE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;

}

PipeSender::~PipeSender(){

        close(_check_fifo);
        remove(_myfifo);
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

        remove(_myfifo);

        //making the FIFO pipe

        int ret = mkfifo(_myfifo, 0666);

        if (ret<0){  

              std::cout<<strerror(errno)<<std::endl;
              throw IPCException("IPCReceiver ERROR: Cannot make the Pipe.");
        } 
         
      
        
}

void PipeReceiver::pipeTransfer(){


      struct sigaction sa;
      sa.sa_flags = 0;
      sa.sa_handler = handler;
      sigemptyset(&sa.sa_mask);
      sigaction(SIGALRM,&sa,0);


      alarm(10);

  
      do{
        if (0>(_fifo=open(_myfifo,O_RDONLY)))
            if(errno==EINTR){
                if(abort_eh){
                  throw IPCException("IPCSender ERROR: Cannot to the IPCSender.");
                }  
                else{
                  continue; 
                } 
            }
        }while(0);

      alarm(0); 

      FileHandler fd2(_file_name);
      fd2.createFile();
       

      //reading file in chunks of 4096 bytes from the pipe and saving them in a new file

      std::cout<<"          Starting to read the data: "<<std::endl;

      while (true){

          std::vector<unsigned char> smallbuffer(_buffer_size );
          bytesread = read(_fifo,smallbuffer.data(),_buffer_size);

          if(bytesread == 0){
             break;
          }

           else if(bytesread<0){
              std::cout<<strerror(errno)<<std::endl;
             throw IPCException("IPCReceiver ERROR: Cannot read the file");
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
  remove(_myfifo);


  
}