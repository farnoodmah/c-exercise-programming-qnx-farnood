#include <iostream>
#include "src/lib/transferlib.h"



int main(int argc,char *argv[]) {

    std::vector<std::string> commandline;
    CommandOption cmo("ipcreceiver",argc,argv);
    commandline = cmo.getCommand();
    
    IPCReceiver ipcr(commandline[0], commandline[1]);

    sleep(5);
 

}