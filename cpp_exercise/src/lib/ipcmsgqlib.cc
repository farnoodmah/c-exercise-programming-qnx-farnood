#include "ipcmsgqlib.h"
#include "ipcexceptionlib.h"

/***
 * 
 * MsgQueueSender Class
 * 
 * 
 * **/

MsgQueueSender::MsgQueueSender(const std::string & filename): _file_name(filename){
        //unlinking from old shared memories
        mq_unlink(msg_queue_name.c_str());
        //setting the max number of msg and msg size
        _attrs.mq_maxmsg = _msg_queue_maxnummsg;
        _attrs.mq_msgsize = _msg_queue_msgsize;
        //opening the msg queue
        _msg_queue = mq_open(msg_queue_name.c_str(), O_WRONLY | O_CREAT , S_IRWXU | S_IRWXG, &_attrs);
        if(_msg_queue<0){
                std::string errormsg = std::string(strerror(errno)) + "   IPCSender ERROR: Cannot open the MSGQueue.   " ;
                throw IPCException(errormsg.c_str());
        }
}

void MsgQueueSender::msgqTransfer(){
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"****************************IPCSender: MESSAGE QUEUE PROTOCOL**************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        std::cout<<"**************************************************************"<<std::endl;
        //opening the file
        FileHandler fd(_file_name);
        fd.openForReading();
        std::cout<<"          File Name: "<<_file_name<<std::endl;
        std::cout<<"          File Size: "<<fd.getSize()<<" bytes"<<std::endl;
        std::cout<<"          Loading the File. Please Wait..."<<std::endl;
        std::cout<<"          MSGQueue is Open: Trying to send the file  "<<std::endl;
        while (true){
            //reading and sending the data of file in smaller parts of 4096
            std::vector<unsigned char> buffer;
            buffer = fd.readFile();
            clock_gettime(CLOCK_REALTIME, &_ts);
            //setting the time for 10 seconds so if no data received after 10 seconds it will break the loop   
     	    _ts.tv_sec += 10;	
            ret = mq_timedsend(_msg_queue,(char*)buffer.data(),buffer.size(),_priority, &_ts);
            if(ret<0){
                if (errno == ETIMEDOUT){
                std::string errormsg = std::string(strerror(errno)) + "   IPCSender ERROR: Cannot Connect to the Receiver   " ;
                throw IPCException(errormsg.c_str());
            }
            std::string errormsg = std::string(strerror(errno)) + "   IPCSender ERROR: MsgQueueSend Error  " ;
            throw IPCException(errormsg.c_str());
            }
            //End of the file
            if(buffer.size()==0){
            break;
            }
       }
       checkingMsgQueueEmpty();
       std::cout<<"          Successfully written to the MSGQueue "<<std::endl;
       std::cout<<"**************************************************************"<<std::endl;
       std::cout<<"****************************IPCSender: MESSAGE QUEUE PROTOCOL**************************"<<std::endl;
       std::cout<<"**************************************************************"<<std::endl;
       std::cout<<"**************************************************************"<<std::endl;
}

void MsgQueueSender::checkingMsgQueueEmpty(){
        while(_check_empty!=0){
            ++counter;
            if(counter > 5){
                throw IPCException("IPCSender ERROR: IPCReceiver did not receive the file successfully");
            }
                std::cout<<"          Checking the IPCReceiver received the file completely... Try:"<<counter<<"/5"<<std::endl;
                mq_getattr(_msg_queue, &_attrs);
	 	 		_check_empty = _attrs.mq_curmsgs;
                if(_check_empty == 0){
                    break;
                }
                sleep(2);
        }
}

MsgQueueSender::~MsgQueueSender(){  
       mq_close(_msg_queue);
       mq_unlink(msg_queue_name.c_str());
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
            ++counter;
            if(counter > 5){
                throw IPCException("IPCSender ERROR: Cannot connect to the IPCSender");
            }
            std::cout<<"          Waiting for the Sender...Try:"<<counter<<"/5"<<std::endl;     
            _msg_queue = mq_open(msg_queue_name.c_str(), O_RDONLY , &_attrs); 
            if(_msg_queue>=0){
                mq_getattr(_msg_queue, &_attrs);
	 	 		_check_empty = _attrs.mq_curmsgs;
            }
            sleep(3);
	 	}
}

void MsgQueueReceiver::msgqTransfer(){
        std::cout<<"          Starting to read the data: "<<std::endl;    
        FileHandler fd(_file_name);
        fd.createFile();
        while (true){
            sleep(1);
            //receiving and writing the data in 4096 size
            std::vector<unsigned char> buffer (4096);
		    clock_gettime(CLOCK_REALTIME, &_ts);
            //setting the time for 10 seconds so if no data received after 10 seconds it will break the loop   
     	     _ts.tv_sec += 10;	
            _receive_size = mq_timedreceive(_msg_queue,(char*)buffer.data(),_msg_queue_msgsize,&_priority,&_ts);
            if(_receive_size<=0){
                if (errno == ETIMEDOUT){
                std::string errormsg = std::string(strerror(errno)) + "   IPCSender ERROR: Cannot Connect to the Sender  " ;
                throw IPCException(errormsg.c_str());
            }
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
        mq_unlink(msg_queue_name.c_str());
}