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
    std::cout<<"**************************************************************"<<std::endl;
    std::cout<<"****************************IPCSender: SharedMemory PROTOCOL**************************"<<std::endl;
    std::cout<<"**************************************************************"<<std::endl;
    std::cout<<"**************************************************************"<<std::endl;
    while(_shm_fd == -1){
        ++counter;
        if(counter>5){
         throw IPCException("         IPCSender ERROR: Cannot connect to the IPCSender.");
        }
        std::cout<<"          Waiting for the Receiver... Try:"<<counter<<"/5"<<std::endl;
        _shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);
        _sem_sender = sem_open(semaphoresender_name.c_str(),0);
        _sem_receiver = sem_open(semaphorereceiver_name.c_str(),0);
        sleep(3);
    }
    _err = ftruncate(_shm_fd, _shm_size);
    if(_err<0){    
        std::string errormsg = std::string(strerror(errno)) + "  IPCSender ERROR: Cannot truncate the Shared Memory. " ;
        throw IPCException(errormsg.c_str()); 
    }
    _ptr = reinterpret_cast<struct _shm_data_struct *>(mmap(0, _shm_size, PROT_WRITE, MAP_SHARED, _shm_fd, 0));
    if(_ptr == (struct _shm_data_struct *)-1){
        std::string errormsg = std::string(strerror(errno)) + "   IPCSender ERROR: Cannot map the Shared Memory. " ;
        throw IPCException(errormsg.c_str()); 
    }
}

void SharedMemorySender::shmTransfer(){
    FileHandler fd(_file_name);
    fd.openForReading();
    std::cout<<"          File Name: "<<_file_name<<std::endl;
    std::cout<<"          File Size: "<<fd.getSize()<<" bytes"<<std::endl;
    std::cout<<"          SharedMemory is Open: Trying to send the file  "<<std::endl;
    while(true){
        std::vector<unsigned char>  buffer; 
        buffer = fd.readFile();
        _shm_data_struct tempstruct;
        tempstruct.datasize = buffer.size();
        std::copy(buffer.begin(), buffer.end(),tempstruct.data);
        if (clock_gettime(CLOCK_REALTIME, &_ts) == -1){
                std::string errormsg = std::string(strerror(errno)) + "   IPCSender ERROR: Cannot get the CLOCK TIME. " ;
                throw IPCException(errormsg.c_str());
        }
        _ts.tv_sec += 15;
        int s =  sem_timedwait(_sem_receiver,&_ts);
        if(s <-1){
            if(errno == ETIMEDOUT){
                throw IPCException("IPCReceiver ERROR: Cannot Connect to the IPCReceiver.");
        }
        }
        if(_err<0){
            std::string errormsg = std::string(strerror(errno)) + "   IPCSender ERROR: Semaphore receiver. " ;
            throw IPCException(errormsg.c_str());
        }   
        memcpy(_ptr,&tempstruct, sizeof(tempstruct));
        _err =  sem_post(_sem_sender);
        if(_err<0){
            std::string errormsg = std::string(strerror(errno)) + "   IPCSender ERROR: Semaphore sender. " ;
            throw IPCException(errormsg.c_str());
        } 
        if(buffer.size()==0){ 
            break;
        }
    }
    std::cout<<"          Successfully written to the SharedMemory "<<std::endl;
    std::cout<<"**************************************************************"<<std::endl;
    std::cout<<"****************************IPCSender: SharedMemory PROTOCOL**************************"<<std::endl;
    std::cout<<"**************************************************************"<<std::endl;
    std::cout<<"**************************************************************"<<std::endl;
}

SharedMemorySender::~SharedMemorySender(){     
    sem_close(_sem_sender);
    sem_close(_sem_receiver);
    munmap(_ptr,_shm_size);
    sem_unlink(semaphoresender_name.c_str());
    sem_unlink(semaphorereceiver_name.c_str());
    shm_unlink(shm_name.c_str());
}


SharedMemoryReceiver::SharedMemoryReceiver(const std::string filename): _file_name(filename){
    std::cout<<"**************************************************************"<<std::endl;
    std::cout<<"****************************IPCReceiver: SharedMemory PROTOCOL**************************"<<std::endl;
    std::cout<<"**************************************************************"<<std::endl;
    std::cout<<"**************************************************************"<<std::endl;
    sem_unlink(semaphoresender_name.c_str());
    sem_unlink(semaphorereceiver_name.c_str());
    shm_unlink(shm_name.c_str());
    _sem_sender = sem_open(semaphoresender_name.c_str(),O_CREAT ,0660,0);
    if(_sem_sender == SEM_FAILED){
        std::string errormsg = std::string(strerror(errno)) + "   IPCReceiver ERROR:Cannot Open Semaphore Sender.  " ;
        throw IPCException(errormsg.c_str());
    } 
    _sem_receiver = sem_open(semaphorereceiver_name.c_str(),O_CREAT , 0660,1);
    if(_sem_receiver == SEM_FAILED){
        std::string errormsg = std::string(strerror(errno)) + "   IPCReceiver ERROR:Cannot Open Semaphore Receiver.   " ;
        throw IPCException(errormsg.c_str());
    }
    std::cout<<"          Waiting for the Sender..."<<std::endl;
    _shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_RDONLY, 0666);
    if(_shm_fd < 0){
        std::string errormsg = std::string(strerror(errno)) + "   IPCReceiver ERROR:Cannot Open Shared Memory.   " ;
        throw IPCException(errormsg.c_str());
    }
    _ptr = reinterpret_cast<struct _shm_data_struct *>(mmap(0, _shm_size, PROT_READ, MAP_SHARED, _shm_fd, 0));  
    if(_ptr == (struct _shm_data_struct *)-1){
        std::string errormsg = std::string(strerror(errno)) + "   IPCReceiver ERROR: Cannot map the Shared Memory.   " ;
        throw IPCException(errormsg.c_str());
    }
}

void SharedMemoryReceiver::shmTransfer(){
    FileHandler fr(_file_name);
    fr.createFile();
    std::cout<<"          Starting to read the data: "<<std::endl; 
    while(true){               
        if (clock_gettime(CLOCK_REALTIME, &_ts) == -1){
            std::string errormsg = std::string(strerror(errno)) + "   IPCReceiver ERROR: Cannot get the CLOCK TIME.   " ;
            throw IPCException(errormsg.c_str());
        }
    
        _ts.tv_sec += 15;
        int s =  sem_timedwait(_sem_sender,&_ts);
        if (s == -1){
            if (errno == ETIMEDOUT){
                std::string errormsg = std::string(strerror(errno)) + "   IPCReceiver ERROR: Cannot connect to IPCSender.   " ;
                throw IPCException(errormsg.c_str());
            }
        }
        _shm_data_struct tempstruct = *_ptr;
        if(tempstruct.datasize == 0){
            break;
        }    
        sem_post(_sem_receiver);
        std::vector<unsigned char> tempvec(_buffer_size);    
        tempvec.insert(tempvec.begin(),std::begin(tempstruct.data),std::end(tempstruct.data));
        tempvec.resize(tempstruct.datasize);
        fr.writeFile(tempvec,tempvec.size());    
    }
    std::cout<<"          Save the data into the:  "<<_file_name<<std::endl;   
    std::cout<<"**************************************************************"<<std::endl;
    std::cout<<"****************************IPCReceiver: SharedMemory PROTOCOL**************************"<<std::endl;
    std::cout<<"**************************************************************"<<std::endl;
    std::cout<<"**************************************************************"<<std::endl;
}

SharedMemoryReceiver::~SharedMemoryReceiver(){
    sem_close(_sem_sender);
    sem_close(_sem_receiver);
    sem_unlink(semaphoresender_name.c_str());
    sem_unlink(semaphorereceiver_name.c_str());
    munmap(_ptr,_shm_size); 
    shm_unlink(shm_name.c_str());
}
