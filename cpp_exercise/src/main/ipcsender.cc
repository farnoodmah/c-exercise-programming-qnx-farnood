#include <iostream>
#include "src/lib/linuxipclib.h"
#include "src/lib/ipcexceptionlib.h"


int main(int argc,char *argv[]) {
    try{
        std::vector<std::string> commandline;
        CommandOption cmo("ipcsender",argc,argv);
        commandline = cmo.getCommand();
        IPCSender ipcs(commandline[0],commandline[1]);
    } catch(IPCException & e){
        std::cout<<e.what()<<std::endl;
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);

}