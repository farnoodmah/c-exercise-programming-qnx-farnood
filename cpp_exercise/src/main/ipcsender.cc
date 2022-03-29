#include <iostream>
#include "src/lib/transferlib.h"



int main(int argc,char *argv[]) {
    
    std::vector<std::string> commandline;
    CommandOption cmo("ipcsender",argc,argv);
    commandline = cmo.getCommand();
  

    IPCSender ipcs(commandline[0],commandline[1]);
    sleep(5);
    

}