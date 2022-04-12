#include "ipcpipelib.h"
#include "ipcexceptionlib.h"
/***
 * PipeSender 
 * **/


PipeSender::PipeSender(const std::string & filename): _file_name(filename){
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCSender: PIPE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;


         FileHandler fd(_file_name);

        
           _file_size = fd.getSize();
        
      

        std::cout<<"          File Name: "<<_file_name<<std::endl;
        std::cout<<"          File Size: "<<_file_size<<" bytes"<<std::endl;
        std::cout<<"          Loading the File. Please Wait..."<<std::endl;

         
           _read_file = fd.readFile(_file_size);

        std::cout<<"          The File loaded completely."<<std::endl;
         
        int err;

      

        remove(_myfifo);
         
         
         ret = mkfifo(_myfifo, 0666);
         if (ret<0)
         {  
           std::cout<<strerror(errno)<<std::endl;
           throw IPCException("IPCSender ERROR: Cannot make the Pipe.");
         }

         

         

         _check_fifo = open(_myfifo, O_WRONLY);

         if(_check_fifo < 0){
            remove(_myfifo);
            std::cout<<strerror(errno)<<std::endl;
             throw IPCException("IPCSender ERROR: Cannot open the Pipe.");
         }

         std::cout<<"          Pipe is Open: Trying to write the file  "<<std::endl;

         err = write(_check_fifo,&_read_file[0],_file_size);

          if(err < 0){
           
            remove(_myfifo);
           std::cout<<strerror(errno)<<std::endl;
             throw IPCException("IPCSender ERROR: Cannot write to the Pipe.");
         }

    

          std::cout<<"          Successfully written to the Pipe "<<std::endl;
         
         err = close(_check_fifo);

         if(err < 0){
          std::cout<<strerror(errno)<<std::endl;
             throw IPCException("IPCSender ERROR: Cannot close the Pipe.");
         }

          std::cout<<"          Pipe is Closed  "<<std::endl;


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
         
        
         //trying to to open the pipe 
          while(  _fifo == 0 || _fifo ==-1 ){

           
            std::cout<<"          Waiting For The Sender: "<<std::endl;
            _fifo = open(_myfifo,O_RDONLY);
            sleep(5);
          }

       
      
        

       //reading file in chunk of 4096 bytes
       std::cout<<"          Starting the read the data: "<<std::endl;
           while (bytesread > 0)
         {
            std::vector<unsigned char> smallbuffer(_buffer_size + 1);
           bytesread = read(_fifo,&smallbuffer[0],_buffer_size);
         
           if(bytesread<_buffer_size){
             smallbuffer.resize(bytesread);
             _readbuffer.insert(_readbuffer.end(),smallbuffer.begin(),smallbuffer.end());
              break;
             }
           if(bytesread<0){
             
         
            std::cout<<strerror(errno)<<std::endl;
             throw IPCException("IPCReceiver ERROR: Cannot read the file");
           }
            smallbuffer.resize(bytesread);
           _readbuffer.insert(_readbuffer.end(),smallbuffer.begin(),smallbuffer.end());
        
          
         }

            std::cout<<"          Received The File Successfully "<<std::endl;
      
        int err = close(_fifo);
         if(err<0){
            
            std::cout<<strerror(errno)<<std::endl;
             throw IPCException("IPCReceiver ERROR: Cannot close the file");
           }

        err = remove(_myfifo);
         if(err<0){
             std::cout<<strerror(errno)<<std::endl;
             throw IPCException("IPCReceiver ERROR: Cannot remove the file");
           }

       std::cout<<"          Save the data into the:  "<<_file_name<<std::endl;
           FileHandler fd2(_file_name);
           fd2.createFile();
             fd2.writeFile(_readbuffer,_readbuffer.size());
        
         

        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCReceiver: PIPE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        
}

  PipeReceiver::~PipeReceiver(){
  close(_fifo);
  remove(_myfifo);
}