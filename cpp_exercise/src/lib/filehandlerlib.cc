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
        throw IPCException("FileHandler ERROR: Cannot Open the File Correctly");
    }


    int err = close(_fd);
     if(err<0){
        throw IPCException("FileHandler ERROR: Cannot Close the File Correctly");
    }

}

/**
 * @brief Writing  a vector<unsinged char> into a file
 * 
 */

void FileHandler::writeFile(std::vector<unsigned char> & file_input, size_t write_size){

    
     _write_size = write_size;

    int err;

    _fd = open(_file_name.c_str(), O_WRONLY | O_EXCL  | O_APPEND, _mode);
     if(_fd<0){

        throw IPCException("FileHandler ERROR: Cannot Open the File Correctly");
    }

    err = write(_fd, &file_input[0], _write_size);

    if(err<0){

        close(_fd);

        throw IPCException("FileHandler ERROR: Cannot Write to the File Correctly");
    }

    err = close(_fd);

    if(err<0){

        throw IPCException("FileHandler ERROR: Cannot Close the File Correctly");
    }
}


/**
 * @brief Reading from the file
 * 
 */

std::vector<unsigned char> FileHandler::readFile(size_t read_size){
    
    
    _read_size = read_size;
  
   
    _readbuffer.resize(read_size);
    
      _fd = open(_file_name.c_str(), O_RDONLY | O_EXCL | O_APPEND, _mode);
   

    if(_fd<0){
        
        throw IPCException("FileHandler ERROR: Cannot Open the File Correctly");
    }
    size_t chunk =0;

    lseek64(_fd,0,SEEK_SET);


    while(chunk < read_size){
        size_t ph;
        ph = read(_fd, &_readbuffer[0] + chunk, 2);
         if(ph<0){
        close(_fd);
        throw IPCException("FileHandler ERROR: Cannot Read the File Correctly");
    }
        chunk = chunk + ph;
    }


    int err = close(_fd);
   
    if(err<0){
        throw IPCException("FileHandler ERROR: Cannot Close the File Correctly");
    }
    
    return _readbuffer;
}



/**
 * @brief Getting size of the file
 * 
 */


size_t FileHandler::getSize(){

_fd = open(_file_name.c_str(), O_RDONLY, _mode);
if(_fd<0){
        throw IPCException("FileHandler ERROR: Cannot Open the File Correctly");
}
struct stat file_stat;
stat(_file_name.c_str(),&file_stat);

_file_size = file_stat.st_size;
int err = close(_fd);
if(err<0){
        throw IPCException("FileHandler ERROR: Cannot Close the File Correctly");
}

return _file_size;

}

/**
 * @brief Removing the file
 * 
 */


void FileHandler::removeFile(){
   
    int err = remove(_file_name.c_str());
    if(err<0){
        throw IPCException("FileHandler ERROR: Cannot Remove the File Correctly");
    }
}

