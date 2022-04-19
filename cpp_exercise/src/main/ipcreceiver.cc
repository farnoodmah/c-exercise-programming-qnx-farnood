#include <iostream>
#include "src/lib/linuxipclib.h"
#include "src/lib/ipcexceptionlib.h"


int main(int argc,char *argv[]) {

    std::vector<std::string> commandline;
   try{

    CommandOption cmo("ipcreceiver",argc,argv);
    commandline = cmo.getCommand();
    IPCReceiver ipcr(commandline[0], commandline[1]);

   } catch(IPCException & e){
       std::cout<<e.what()<<std::endl;
       exit(EXIT_FAILURE); 
   }
  

    exit(EXIT_SUCCESS);
 

}