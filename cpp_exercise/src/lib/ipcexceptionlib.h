#include <iostream>
#include <exception>


        class IPCException : public std::exception {
            const char * _msg;
            IPCException();
        public:
           IPCException(const char * s) noexcept(true) : _msg(s) {}

         const char * what() const noexcept(true) {return _msg;}
       

        };
