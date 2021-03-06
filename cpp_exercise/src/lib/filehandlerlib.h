#ifndef FILEHANDLERLIB_H
#define FILEHANDLERLIB_H


#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <cerrno>
#include <errno.h>



/**
 * @brief FileHandler Class for creating, writing, reading, and removing
 * 
 */

const size_t file_data_read_size = 4096;
class FileHandler{

    private:
        const std::string _file_name; //input file name
        int _fd;  //file descriptor
        std::string _file_input; // the data that should be written into the file
        size_t _write_size; // the size of data that should be written
        size_t _read_size; // the size of data that should be read
        size_t _file_size; // the size of the file
        size_t _buffer_size = 4096; //the size of the chunks in reading the file
        std::vector<unsigned char> _readbuffer; // the read data from  file
        const mode_t  _mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // mode for opening the file

    public:
        FileHandler(const std::string & filename);
        ~FileHandler();
        void createFile();
        void writeFile(const std::vector<unsigned char> & file_input, size_t write_size);
        void removeFile();
        std::vector<unsigned char> readFile();
        void openForReading();
        size_t getSize();  
};

#endif