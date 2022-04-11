#include"ipcshmlib.h"
#include "ipcexceptionlib.h"


/**
 * @brief Shared Memory Class
 * 
 */


/**
 * Construct a new Shared Memory Sender:: Shared Memory Sender object
 * 
 * 
 */
SharedMemorySender::SharedMemorySender(const std::string filename): _file_name(filename){
    // std::string sample = "Hello World";
    
    // _shm_fd = shm_open(_shm_name.c_str(), O_CREAT | O_RDWR, 0666);

    // ftruncate(_shm_fd, _shm_size);

    // _ptr = mmap(0, _shm_size, PROT_WRITE, MAP_SHARED, _shm_fd, 0);

    // memcpy(_ptr,sample.c_str(),sample.size());

    //  munmap(_ptr,_shm_size);

//   int fd;
//   size_t len;
//   char *addr;
//   char string[3] = "he";
//   shm_unlink(_shm_name.c_str());
//   fd = shm_open(_shm_name.c_str(),O_RDWR| O_CREAT, 0);
//   ftruncate(fd,_shm_size);

//   addr = (char *) mmap(NULL, _shm_size,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);
//     close(fd);
//     memcpy(addr,string,2);
 /**
 * @brief 
 * 
 * 
 * 
 * 
 * 
 */


/* the size (in bytes) of shared memory object */
    const int SIZE = 2;
 
    /* name of the shared memory object */
    const char* name = "OS";
 
    /* strings written to shared memory */
    const char* message_0 = "Hello";
    const char* message_1 = "World";
    std::string sample = "THis is a testd";
    std::vector<char> samplevec;
    samplevec.insert(samplevec.begin(),sample.begin(),sample.end());

   /* shared memory file descriptor */
    int shm_fd;
 
    /* pointer to shared memory object */
    char * ptr;

   


    sem_t *sem_sender = sem_open(_semaphoresender_name.c_str(),_DEFAULT_SOURCE);
    sem_t *sem_receiver = sem_open(_semaphorereceiver_name.c_str(),_DEFAULT_SOURCE);

    
 

        /* create the shared memory object */
    shm_fd = -1;
    while(shm_fd == -1){
        std::cout<<"trying to open to shm"<<std::endl;
         shm_fd = shm_open(name, O_EXCL | O_RDWR, 0666);
          sem_sender = sem_open(_semaphoresender_name.c_str(),_DEFAULT_SOURCE);
     sem_receiver = sem_open(_semaphorereceiver_name.c_str(),_DEFAULT_SOURCE);

         sleep(5);
    }
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);
 
    /* memory map the shared memory object */
    ptr =(char *) mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    std::vector<unsigned char> vector1(2, '\0'); 
    
 

    while(!samplevec.empty()){

        std::vector<unsigned char>  buffer;         
        sem_wait(sem_receiver);
         if(samplevec.size()<2 ){
        memcpy(ptr,(char *)&vector1[0],vector1.size());
         buffer.insert(buffer.end(),samplevec.begin(),samplevec.end());
        samplevec.erase(samplevec.begin(),samplevec.end());
       
        memcpy(ptr,(char *)&buffer[0],buffer.size());
       
        sem_post(sem_sender);
        break;
            
        }
        
        buffer.insert(buffer.end(),samplevec.begin(),samplevec.begin()+2);
        samplevec.erase(samplevec.begin(),samplevec.begin()+2);
       
        memcpy(ptr,(char *)&buffer[0],buffer.size());
       
        sem_post(sem_sender);


    }
   
   
    if(samplevec.empty()){
        sem_wait(sem_receiver);
       
         memcpy(ptr,(char *)&vector1[0],vector1.size());
          
       sem_post(sem_sender);
        
        }

    
    /* write to the shared memory object */
    //sprintf(ptr, "%s", message_0);
    
    sem_close(sem_sender);
    sem_close(sem_receiver);

     munmap(_ptr,_shm_size);
     shm_unlink(name);

    // ptr += strlen(message_0);
    // sprintf(ptr, "%s", message_1);
    // ptr += strlen(message_1);

}

SharedMemoryReceiver::SharedMemoryReceiver(const std::string filename): _file_name(filename){
    
 /* the size (in bytes) of shared memory object */
    const int SIZE = 2;
 
    /* name of the shared memory object */
    const char* name = "OS";
 
    /* shared memory file descriptor */
    int shm_fd;
    
    const char * empty = "World";

    //setup semaphores
     sem_unlink(_semaphoresender_name.c_str());
     sem_unlink(_semaphorereceiver_name.c_str());
       /* pointer to shared memory object */
    void* ptr;
     sem_t *sem_sender = sem_open(_semaphoresender_name.c_str(),O_CREAT ,0660,0);
     if(sem_sender == SEM_FAILED){
         perror("sem_open/sender");
         exit(EXIT_FAILURE);
     }
     sem_t *sem_receiver = sem_open(_semaphorereceiver_name.c_str(),O_CREAT , 0660,1);
    /* open the shared memory object */
     if(sem_receiver == SEM_FAILED){
         perror("sem_opern/receiver");
         exit(EXIT_FAILURE);
     }

  shm_unlink(name);

   shm_fd = shm_open(name, O_CREAT | O_RDONLY, 0666);
  


  std::vector<unsigned char> buffer;
  std::vector<unsigned char> tempvec;
  
const  unsigned char * end = NULL;

     
  while(true){

      
       std::cout<<"sem sender"<<std::endl; 
    struct timespec ts;
if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
{
    /* handle error */
   std::cout<<"error"<<std::endl;
}

ts.tv_sec += 15;
    // sem_wait(sem_sender);
   int s =  sem_timedwait(sem_sender,&ts);

    if (s == -1)
{
    if (errno == ETIMEDOUT){
        sem_close(sem_sender);
    sem_close(sem_receiver);
    munmap(ptr,_shm_size); 
   shm_unlink(name);

   break;
    }
       
   
}

     ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
     std::vector<unsigned char> buffer;
     std::string  bufferstring = (char *)ptr;
      if(bufferstring.empty()){
         std::cout<<"this is the end"<<std::endl;
         break;
     }
    
    buffer.insert(buffer.begin(),bufferstring.begin(),bufferstring.end());

    std::cout<<buffer.size()<<std::endl;
    /* read from the shared memory object */
    std::cout<<buffer.data()<<std::endl;

    
    sem_post(sem_receiver);


   
    /* memory map the shared memory object */
    
  }
   
    /* remove the shared memory object */
    sem_close(sem_sender);
    sem_close(sem_receiver);
    munmap(ptr,_shm_size); 
   shm_unlink(name);
}


