#include "ipcpipelib.h"
/***
 * PipeSender 
 * **/


PipeSender::PipeSender(const std::string & filename): _file_name(filename){
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCSender: PIPE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;


         FileHandler fd(_file_name);

         try{
           _file_size = fd.getSize();
         }catch(IPCException & e){
          std::cout<<e.what()<<std::endl;
          exit(EXIT_FAILURE);
         }

        std::cout<<"          File Name: "<<_file_name<<std::endl;
        std::cout<<"          File Size: "<<_file_size<<" bytes"<<std::endl;

         try{
           _read_file = fd.readFile(_file_size);

         }catch(IPCException & e){
          std::cout<<e.what()<<std::endl;
          exit(EXIT_FAILURE);
         }
         
        int err;

      

        remove(_myfifo);
         
         
         ret = mkfifo(_myfifo, 0666);
         if (ret<0)
         {
           throw IPCException("IPCSender ERROR: Cannot make the Pipe.");
         }

         

         

         _check_fifo = open(_myfifo, O_WRONLY);

         if(_check_fifo < 0){
            remove(_myfifo);
             throw IPCException("IPCSender ERROR: Cannot open the Pipe.");
         }

         std::cout<<"          Pipe is Open: Trying to write the file  "<<std::endl;

         err = write(_check_fifo,&_read_file[0],_file_size);

          if(err < 0){
            close(_check_fifo);
            remove(_myfifo);
             throw IPCException("IPCSender ERROR: Cannot write to the Pipe.");
         }

    

          std::cout<<"          Successfully written to the Pipe "<<std::endl;
         
         err = close(_check_fifo);

         if(err < 0){
          
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

        FileHandler fd2(_file_name);
        try{
          fd2.createFile();
        }catch(IPCException & e){
          remove(_myfifo);
          std::cout<<e.what()<<std::endl;
          exit(EXIT_FAILURE);
        }
      
         std::cout<<"          File Name: "<<_file_name<<std::endl;
         
        
         //trying to to open the pipe 
          while(  _fifo == 0 || _fifo ==-1 ){

            sleep(5);
            std::cout<<"          Waiting For The Sender: "<<std::endl;
            _fifo = open(_myfifo,O_RDONLY);

          }

        //reading file in chunk of 2 bytes
        unsigned char mybuffer[2];
        size_t bytesread = 1;

       
           while ((bytesread = read(_fifo,mybuffer, 2)) > 0)
         {

           if(bytesread<0){
             close(_fifo);
             remove(_myfifo);
             throw IPCException("IPCReceiver ERROR: Cannot read the file");
           }

          //  for(int i = 0; i <2 ;i++){
          //   _readbuffer.push_back(mybuffer[i]);
          // }
            _readbuffer.insert(_readbuffer.end(),std::begin(mybuffer), std::end(mybuffer));
          
          
         }

            std::cout<<"          Received The File Successfully "<<std::endl;
      
        int err = close(_fifo);
         if(err<0){
             remove(_myfifo);
             throw IPCException("IPCReceiver ERROR: Cannot close the file");
           }

        err = remove(_myfifo);
         if(err<0){
             
             throw IPCException("IPCReceiver ERROR: Cannot remove the file");
           }

       
        
       
        try{
           fd2.writeFile(_readbuffer,_readbuffer.size());
        }catch(IPCException & e){
          std::cout<<e.what()<<std::endl;
          exit(EXIT_FAILURE);
        }
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