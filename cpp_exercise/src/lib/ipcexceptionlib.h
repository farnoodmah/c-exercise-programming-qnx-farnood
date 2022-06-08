#include <iostream>
#include <exception>

class IPCException : public std::runtime_error{
        public:
               using std::runtime_error::runtime_error;
};
