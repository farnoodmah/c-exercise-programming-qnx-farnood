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

        //removing and unlinking from old shared memories

        remove(_msg_queue_name.c_str());
        ret = mq_unlink(_msg_queue_name.c_str());
   
        //setting the max number of msg and msg size
        _attrs.mq_maxmsg = _msg_queue_maxnummsg;
        _attrs.mq_msgsize = _msg_queue_msgsize;



        //opening the file
        FileHandler fd(_file_name);
        fd.openForReading();
    
        std::cout<<"          File Name: "<<_file_name<<std::endl;
        std::cout<<"          File Size: "<<fd.getSize()<<" bytes"<<std::endl;
        std::cout<<"          Loading the File. Please Wait..."<<std::endl;



    

        //opening the msg queue
        _msg_queue = mq_open(_msg_queue_name.c_str(), O_WRONLY | O_CREAT , S_IRWXU | S_IRWXG, &_attrs);


        if(_msg_queue<0){
            
            std::cout<<strerror(errno)<<std::endl;
            throw IPCException("IPCSender ERROR: Cannot open the MSGQueue.");

        }
   
        std::cout<<"          MSGQueue is Open: Trying to send the file  "<<std::endl;
 
  

        

        while (true){

            //reading and sending the data of file in smaller parts of 4096
            std::vector<unsigned char> buffer;
            buffer = fd.readFile();
  

            mq_send(_msg_queue, reinterpret_cast<char *>(buffer.data()),buffer.size(),_priority);

            //End of the file
            if(buffer.size()==0){

            break;

            }

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


	 	// check if the queue is open and empty
	 	while (_check_empty == 0)
	 	{ 
            std::cout<<"          Waiting for the Sender..."<<std::endl;     
            _msg_queue = mq_open(_msg_queue_name.c_str(), O_RDONLY , &_attrs); 

            if(_msg_queue>=0){

                 mq_getattr(_msg_queue, &_attrs);
	 	 		_check_empty = _attrs.mq_curmsgs;
            }
            sleep(5);
	 	}
              
    
        std::cout<<"          Starting to read the data: "<<std::endl;   
         
        FileHandler fd(_file_name);
        fd.createFile();

        while (true){

            //receiving and writing the data in 4096 size

            std::vector<unsigned char> buffer (4096);
		   	  clock_gettime(CLOCK_REALTIME, &_ts);

            //setting the time for 10 seconds so if no data received after 10 seconds it will break the loop   
     	    _ts.tv_sec += 10;	
                   
            _receive_size = mq_timedreceive(_msg_queue,reinterpret_cast<char *>(buffer.data()),_msg_queue_msgsize,&_priority,&_ts);


            if(_receive_size<=0){

                break;
            }
            else if(_receive_size>0){

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