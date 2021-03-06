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
}

void SharedMemorySender::shmTransfer(){
    _err = ftruncate(_shm_fd, _shm_size);
    if(_err<0){    
        throw IPCException(" IPCSender ERROR: Cannot truncate the Shared Memory. " + std::string(strerror(errno)));
    }
    _ptr = reinterpret_cast<struct _shm_data_struct *>(mmap(0, _shm_size, PROT_WRITE, MAP_SHARED, _shm_fd, 0));
    if(_ptr == reinterpret_cast<struct _shm_data_struct *>(-1)){
        throw IPCException(" IPCSender ERROR: Cannot map the Shared Memory. " + std::string(strerror(errno)));
    }
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
        _err = clock_gettime(CLOCK_REALTIME, &_ts);
        if ( _err  == -1){
                throw IPCException(" IPCSender ERROR: Cannot get the CLOCK TIME. " + std::string(strerror(errno)));
        }
        _ts.tv_sec += 15;
        _err =  sem_timedwait(_sem_receiver,&_ts);
        if(_err == -1){
            if(errno == ETIMEDOUT){
                throw IPCException("IPCReceiver ERROR: Cannot Connect to the IPCReceiver." + std::string(strerror(errno)));
            }
        }
        memcpy(_ptr,&tempstruct, sizeof(tempstruct));
        _err =  sem_post(_sem_sender);
        if(_err<0){
                throw IPCException(" IPCSender ERROR: Semaphore Sender " + std::string(strerror(errno)));
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
}

void SharedMemoryReceiver::shmTransfer(){
    _sem_sender = sem_open(semaphoresender_name.c_str(),O_CREAT ,0660,0);
    if(_sem_sender == SEM_FAILED){
        throw IPCException(" IPCReceiver ERROR:Cannot Open Semaphore Sender. " + std::string(strerror(errno)));
    } 
    _sem_receiver = sem_open(semaphorereceiver_name.c_str(),O_CREAT , 0660,1);
    if(_sem_receiver == SEM_FAILED){
        throw IPCException(" IPCReceiver ERROR:Cannot Open Semaphore Receiver. " + std::string(strerror(errno)));
    }
    std::cout<<"          Waiting for the Sender..."<<std::endl;
    _shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_RDONLY, 0666);
    if(_shm_fd < 0){
        throw IPCException(" IPCReceiver ERROR:Cannot Open Shared Memory.  " + std::string(strerror(errno)));
    }
    _ptr = reinterpret_cast<struct _shm_data_struct *>(mmap(0, _shm_size, PROT_READ, MAP_SHARED, _shm_fd, 0));  
    if(_ptr == (struct _shm_data_struct *)-1){
        throw IPCException(" IPCReceiver ERROR:Cannot map the  Shared Memory.  " + std::string(strerror(errno)));
    }
    FileHandler fr(_file_name);
    fr.createFile();
    std::cout<<"          Starting to read the data: "<<std::endl; 
    while(true){        
        _err =  clock_gettime(CLOCK_REALTIME, &_ts);         
        if ( _err == -1){
            throw IPCException(" IPCReceiver ERROR: Cannot get the CLOCK TIME.  " + std::string(strerror(errno)));
        }
    
        _ts.tv_sec += 15;
        _err =  sem_timedwait(_sem_sender,&_ts);
        if (_err == -1){
            if (errno == ETIMEDOUT){
                throw IPCException(" IPCReceiver ERROR: Cannot connect to IPCSender.  " + std::string(strerror(errno)));
            }
        }
        _shm_data_struct tempstruct = *_ptr;
        if(tempstruct.datasize == 0){
            break;
        }    
        _err = sem_post(_sem_receiver);
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
    munmap(_ptr,_shm_size); 
    sem_unlink(semaphoresender_name.c_str());
    sem_unlink(semaphorereceiver_name.c_str());
    shm_unlink(shm_name.c_str());
}
