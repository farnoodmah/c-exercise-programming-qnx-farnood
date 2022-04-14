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
       
       
      

        remove(_myfifo);
         
         //making the FIFO pipe

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
       
        //opening for the file the be read
         fd.openForReading();


         while (true)
         {  
           
           //reading the file 
           _read_file = fd.readFile();

           //the end of the file
           if(_read_file.size()==0){
             break;
           }

           if (_read_file.size()>0){

          //writing to the pipe    
          err = write(_check_fifo,_read_file.data(),_read_file.size());

          if(err < 0){
           
            remove(_myfifo);
           std::cout<<strerror(errno)<<std::endl;
             throw IPCException("IPCSender ERROR: Cannot write to the Pipe.");
         }
             
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

       
       

       //reading file in chunks of 4096 bytes from the pipe and saving them in a new file

       std::cout<<"          Starting the read the data: "<<std::endl;

           while (bytesread > 0)
         {
            std::vector<unsigned char> smallbuffer(_buffer_size + 1);
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