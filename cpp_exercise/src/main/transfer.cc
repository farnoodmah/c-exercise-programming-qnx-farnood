#include <iostream>
#include "src/lib/transferlib.h"



int main(int argc,char *argv[]) {
 std::vector<std::string> commandline;
 std::cout<<"the argc  is"<<argc<<std::endl;
 std::cout<<"the argv size is "<<sizeof(argv)<<std::endl;
 std::cout<<"the argv first element"<<argv[0]<<std::endl;
 std::cout<<"the argv second element"<<argv[1]<<std::endl;
 std::cout<<"the argv third element"<<argv[2]<<std::endl;
 std::cout<<"the argv fourth element"<<argv[3]<<std::endl;

  CommandOption cmo("ipcreceiver",argc,argv);
  commandline = cmo.getCommand();
  std::cout<<"the protocol is "<<commandline[0]<<std::endl;
  std::cout<<"the filename is "<<commandline[1]<<std::endl;

}