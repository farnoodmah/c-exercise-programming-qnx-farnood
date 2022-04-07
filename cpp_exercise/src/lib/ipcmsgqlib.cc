#include "ipcmsgqlib.h"



/***
 * 
 * MsgQueueSender Class
 * 
 * 
 * **/

MsgQueueSender::MsgQueueSender(const std::string & filename): _file_name(filename){

   


    mqd_t msg_queue;
    struct mq_attr attrs;

    memset(&attrs, 0, sizeof attrs);

   attrs.mq_maxmsg = 10;
    attrs.mq_msgsize = 1027;
   FileHandler fd(_file_name);
    
    std::cout<<"The name of the file is "<<_file_name<<std::endl;

    msg_queue = mq_open("/new", O_WRONLY | O_CREAT , S_IRWXU | S_IRWXG, &attrs);
    
    std::vector<unsigned char>samplevec;
    samplevec = fd.readFile(fd.getSize());
    std::cout<<"the size is "<<fd.getSize()<<std::endl;

        
while(!samplevec.empty()){
    int i =0;
    std::vector<unsigned char> buffer;

    if(samplevec.size()<1024){
        //std::cout<<"the data is less"<<std::endl;
            buffer.insert(buffer.end(),samplevec.begin(),samplevec.end());
            samplevec.erase(samplevec.begin(),samplevec.end());
            
         }
    else if(samplevec.size()>1024){
        //std::cout<<"the data is more"<<std::endl;
        buffer.insert(buffer.end(),samplevec.begin(),samplevec.begin()+1024);
        samplevec.erase(samplevec.begin(),samplevec.begin()+1024);
     }
     else if(samplevec.size()==1024){
         std::cout<<"this one"<<std::endl;
        buffer.insert(buffer.end(),samplevec.begin(),samplevec.end());
        buffer.resize(1025);
        samplevec.erase(samplevec.begin(),samplevec.end());
     }
  

    mq_send(msg_queue, (char *)&buffer[0],buffer.size(),5);
	 	
}
   int check_empty = -1;

	 	 	// check if the queue is empty
	 	 	while (check_empty != 0)
	 	 	{   
              
	 	 		mq_getattr(msg_queue, &attrs);
	 	 		check_empty = attrs.mq_curmsgs;

	 	 	}
    
    
       int status = mq_close(msg_queue);


}




/***
 * MsgQueueReceiver Class
 * 
 * 
 * **/



MsgQueueReceiver::MsgQueueReceiver(const std::string & filename): _file_name(filename){



  mqd_t msg_queue;
 struct mq_attr attrs;
 memset(&attrs, 0, sizeof attrs); 
//   attrs.mq_flags = O_RDONLY | O_NONBLOCK;
//   mq_setattr(msg_queue, &attrs, NULL);
 
 msg_queue = mq_open("/new", O_RDONLY, &attrs);    

   
    std::vector<unsigned char> finalvec;
    unsigned int prio;

    int receive_size=3;
    const int max_msg_queue = 10;
    const int max_msg_queue_size = 4096;
    
    struct timespec abs_timeout;
     clock_gettime(CLOCK_REALTIME, &abs_timeout);
     	abs_timeout.tv_sec += 2;
          
        int check_empty = 0;

	 	 	// check if the queue is empty
	 	 	while (check_empty == 0)
	 	 	{   msg_queue = mq_open("/new", O_RDONLY , &attrs);  
	 	 		mq_getattr(msg_queue, &attrs);
	 	 		check_empty = attrs.mq_curmsgs;
                std::cout<<"check "<<check_empty<<std::endl;
	 	 	}
    
  
         std::vector<unsigned char> buffer (1027);
        int error_code = 0;
        while (true)
		   	{
		   		
                   
             receive_size = mq_receive (msg_queue, (char*)buffer.data(), 4096,&prio);
            
             if(receive_size > 0){
                 if(receive_size < 1024 ){
                      buffer.resize(receive_size);
            std::cout<<"the receive size end is "<<receive_size<<std::endl;
            std::cout<<buffer.data()<<std::endl;
            finalvec.insert(finalvec.end(),buffer.begin(),buffer.end());
            break;
                 }
            else if(receive_size == 1025) {
            buffer.resize(1024);
            std::cout<<"fuck the receive size is  "<<receive_size<<std::endl;
            std::cout<<buffer.data()<<std::endl;
            finalvec.insert(finalvec.end(),buffer.begin(),buffer.end());
            break;
            }  

            buffer.resize(receive_size);
            std::cout<<"the receive size is "<<receive_size<<std::endl;
            std::cout<<buffer.data()<<std::endl;
            finalvec.insert(finalvec.end(),buffer.begin(),buffer.end());
                 }
           
        
		   	}

           std::cout<<"final buffer "<<finalvec.data()<<std::endl;
            std::cout<<"final buffer "<<finalvec.size()<<std::endl;
        

         mq_close(msg_queue);
         mq_unlink("/new");

         FileHandler fd(_file_name);
         fd.createFile();
         fd.writeFile(finalvec,finalvec.size());   
}