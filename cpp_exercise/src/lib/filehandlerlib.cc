#include "filehandlerlib.h"
#include "ipcexceptionlib.h"

/**
 *
 * @brief FileHandler Constructor for saving the file name
 * 
 * 
 */

FileHandler::FileHandler(const std::string & filename) : _file_name(filename){

}

/**
 * @brief Destroy the File Handler:: File Handler object and close the file 
 * 
 */

FileHandler::~FileHandler(){

    close(_fd);
    
}


/**
 * @brief Creating a file
 * 
 */
void FileHandler::createFile(){

    
   
    _fd = open(_file_name.c_str(), O_RDWR| O_CREAT | O_TRUNC, _mode);
    if(_fd<0){
       std::cout<<strerror(errno)<<std::endl;
        throw IPCException("FileHandler ERROR: Cannot Open the File Correctly");
    }


}

/**
 * @brief Writing  a vector<unsigned char> into a file
 * 
 */

void FileHandler::writeFile(const std::vector<unsigned char> & file_input, size_t write_size){

    
     _write_size = write_size;

    int err;
   

    _fd = open(_file_name.c_str(), O_WRONLY | O_APPEND, _mode);
      
     if(_fd<0){
         std::cout<<strerror(errno)<<std::endl;
        throw IPCException("FileHandler ERROR: Cannot Open the File Correctly");
    }

    err = write(_fd, file_input.data(), _write_size);
    
    if(err<0){

       std::cout<<strerror(errno)<<std::endl;
        throw IPCException("FileHandler ERROR: Cannot Write to the File Correctly");
    }

      err = close(_fd);

      if(err<0){
         std::cout<<strerror(errno)<<std::endl;
         throw IPCException("FileHandler ERROR: Cannot Close the File Correctly");
      }
}


/**
 * @brief Open the file for reading
 * 
 */

void FileHandler::openForReading(){

     _fd = open(_file_name.c_str(), O_RDONLY | O_APPEND| O_LARGEFILE, _mode);

      if(_fd<0){
    
        std::cout<<strerror(errno)<<std::endl;
        throw IPCException("FileHandler ERROR: Cannot Open the File Correctly");
    }
}


/**
 * @brief Reading from the file 
 * 
 */



std::vector<unsigned char> FileHandler::readFile(){


     if(_fd<=0){
    
        std::cout<<strerror(errno)<<std::endl;
        throw IPCException("FileHandler ERROR: Cannot Open the File Correctly");
    }
     
    std::vector<unsigned char> smallbuffer(_buffer_size);
    size_t ph;
    ph = read(_fd, smallbuffer.data() , _buffer_size);
    if(ph>=0){
         smallbuffer.resize(ph);
         return smallbuffer;
    }
    else if(ph<0){
        std::cout<<strerror(errno)<<std::endl;
        throw IPCException("FileHandler ERROR: Cannot Read the File Correctly");
    }
   
}


/**
 * @brief Getting size of the file
 * 
 */


size_t FileHandler::getSize(){


struct stat file_stat;
int err = stat(_file_name.c_str(),&file_stat);
if(err<0){
         if(err<0){
         std::cout<<strerror(errno)<<std::endl;
         throw IPCException("FileHandler ERROR: Cannot give the File size Correctly");
     }
}





if(file_stat.st_size<0){
         if(err<0){
         std::cout<<strerror(errno)<<std::endl;
         throw IPCException("FileHandler ERROR: Cannot give the File size Correctly");
     }
}

_file_size = file_stat.st_size;


return _file_size;

}

/**
 * @brief Removing the file
 * 
 */


void FileHandler::removeFile(){
   
    int err = remove(_file_name.c_str());
    if(err<0){
        std::cout<<strerror(errno)<<std::endl;
        throw IPCException("FileHandler ERROR: Cannot Remove the File Correctly");
    }
}

