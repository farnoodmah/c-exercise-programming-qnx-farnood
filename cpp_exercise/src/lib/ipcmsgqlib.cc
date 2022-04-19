#include "ipcmsgqlib.h"
#include "ipcexceptionlib.h"


/***
 * 
 * MsgQueueSender Class
 * 
 * 
 * **/

MsgQueueSender::MsgQueueSender(const std::string & filename): _file_name(filename){
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCSender: MESSAGE QUEUE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;

   

    int err;
    remove(_msg_queue_name.c_str());
        ret = mq_unlink(_msg_queue_name.c_str());

    
     memset(&_attrs, 0, sizeof _attrs);
   

    _attrs.mq_maxmsg = _msg_queue_maxnummsg;
    _attrs.mq_msgsize = _msg_queue_msgsize;

   FileHandler fd(_file_name);
    
    std::cout<<"          File Name: "<<_file_name<<std::endl;
    std::cout<<"          File Size: "<<fd.getSize()<<" bytes"<<std::endl;
        std::cout<<"          Loading the File. Please Wait..."<<std::endl;


    


    _msg_queue = mq_open(_msg_queue_name.c_str(), O_WRONLY | O_CREAT , S_IRWXU | S_IRWXG, &_attrs);
     if(_msg_queue<0){
         std::cout<<strerror(errno)<<std::endl;
         throw IPCException("IPCSender ERROR: Cannot open the MSGQueue.");

    }
   
 std::cout<<"          MSGQueue is Open: Trying to send the file  "<<std::endl;
 
 fd.openForReading();

        

while (true)
{
    std::vector<unsigned char> buffer;
    buffer = fd.readFile();
  

    err = mq_send(_msg_queue, (char *)&buffer[0],buffer.size(),_priority);
      if(buffer.size()==0){
        break;
    }
}

   

	 	 	// check if the queue is empty
	 	 	while (_check_empty != 0)
	 	 	{   
	 	 		mq_getattr(_msg_queue, &_attrs);
	 	 		_check_empty = _attrs.mq_curmsgs;

	 	 	}
    
    
      

       std::cout<<"          Successfully written to the MSGQueue "<<std::endl;
       std::cout<<"          MSGQueue is Closed  "<<std::endl;

        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCSender: MESSAGE QUEUE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
}

MsgQueueSender::~MsgQueueSender(){
      mq_close(_msg_queue);
      
    
    mq_unlink(_msg_queue_name.c_str());

}

/***
 * MsgQueueReceiver Class
 * 
 * 
 * **/


MsgQueueReceiver::MsgQueueReceiver(const std::string & filename): _file_name(filename){
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCReceiver: MESSAGE QUEUE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;


  

        memset(&_attrs, 0, sizeof _attrs); 


	 	 	// check if the queue is empty
	 	 	while (_check_empty == 0)
	 	 	{ 
                std::cout<<"          Waiting for the Sender..."<<std::endl;     
               _msg_queue = mq_open(_msg_queue_name.c_str(), O_RDONLY , &_attrs);  
	 	 		mq_getattr(_msg_queue, &_attrs);
	 	 		_check_empty = _attrs.mq_curmsgs;
                  sleep(5);
	 	 	}
              
    
        std::cout<<"          Starting the read the data: "<<std::endl;   
         
       FileHandler fd(_file_name);
        fd.createFile();
        while (true)
		   	{
                   std::vector<unsigned char> buffer (4097);
		   	clock_gettime(CLOCK_REALTIME, &_ts);
     	    _ts.tv_sec += 10;	
                   
             _receive_size = mq_timedreceive(_msg_queue,(char*)buffer.data(),_msg_queue_msgsize,&_priority,&_ts);
             if(_receive_size<0){
                 break;
             }
             if(_receive_size>0){
            buffer.resize(_receive_size);
              fd.writeFile(buffer,buffer.size()); 
           
             }
        
		   	}

         std::cout<<"          Received "<<fd.getSize()<<" Bytes Successfully "<<std::endl;


        

         std::cout<<"          Save the data into the:  "<<_file_name<<std::endl;   
         
  
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCReceiver: MESSAGE QUEUE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;  
}

MsgQueueReceiver::~MsgQueueReceiver(){
     mq_close(_msg_queue);    
    mq_unlink(_msg_queue_name.c_str());
    remove(_msg_queue_name.c_str());
}