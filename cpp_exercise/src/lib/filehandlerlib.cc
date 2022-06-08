#include "filehandlerlib.h"
#include "ipcexceptionlib.h"

/**
 *
 * @brief FileHandler Constructor for saving the file name
 * 
 * 
 */

FileHandler::FileHandler(const std::string & filename) : _file_name(filename){}
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
       throw IPCException(" FileHandler ERROR: Cannot Open the File Correctly " + std::string(strerror(errno)));
    }
}
/**
 * @brief Writing  a vector<unsigned char> into a file
 * 
 */
void FileHandler::writeFile(const std::vector<unsigned char> & file_input, size_t write_size){
    int err;
    _fd = open(_file_name.c_str(), O_WRONLY | O_APPEND, _mode);
    if(_fd<0){
        throw IPCException(" FileHandler ERROR: Cannot Open the File Correctly " + std::string(strerror(errno)));
    }
    err = write(_fd, file_input.data(), write_size);
    if(err<0){
       throw IPCException(" FileHandler ERROR: Cannot Write the File Correctly " + std::string(strerror(errno)));
    }
    err = close(_fd);
    if(err<0){
         throw IPCException(" FileHandler ERROR: Cannot Close the File Correctly " + std::string(strerror(errno)));
    }
}
/**
 * @brief Open the file for reading
 * 
 */
void FileHandler::openForReading(){
    _fd = open(_file_name.c_str(), O_RDONLY | O_APPEND| O_LARGEFILE, _mode);
    if(_fd<0){
        throw IPCException(" FileHandler ERROR: Cannot  Open the File Correctly " + std::string(strerror(errno)));
    }
}
/**
 * @brief Reading from the file 
 * 
 */
std::vector<unsigned char> FileHandler::readFile(){
    if(_fd<=0){
        throw IPCException(" FileHandler ERROR: Cannot Open the File Correctly " + std::string(strerror(errno)));
    } 
    std::vector<unsigned char> smallbuffer(_buffer_size);
    size_t ph;
    ph = read(_fd, smallbuffer.data() , _buffer_size);
    if(ph>=0){
        smallbuffer.resize(ph);
        return smallbuffer;
    }
    else{
        throw IPCException(" FileHandler ERROR: Cannot  Read the File Correctly " + std::string(strerror(errno)));
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
        throw IPCException(" FileHandler ERROR: Cannot  give the File size Correctly " + std::string(strerror(errno)));
    }
    if(file_stat.st_size<0){  
        throw IPCException(" FileHandler ERROR: Cannot  give the File size Correctly " + std::string(strerror(errno)));        
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
        throw IPCException(" FileHandler ERROR: Cannot  give the File size Correctly " + std::string(strerror(errno)));
    }
}

