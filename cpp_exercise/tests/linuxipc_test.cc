#include "src/lib/linuxipclib.h"
#include "src/lib/filehandlerlib.h"
#include "src/lib/ipcexceptionlib.h"


#include <map>
#include <vector>
#include<thread>

#include "gtest/gtest.h"


class FileHandlerTests : public ::testing::Test{

      public:

      protected:
      FileHandler nonexFile{"thisfiledoesnotexist.txt"};
      FileHandler txtFile{"testfile.txt"};
      FileHandler pdfFile{"pdftestfile.pdf"};
      FileHandler bigFile{"QNX.zip"};
      std::string samplestring = "Test Data";
      std::vector<unsigned char> samplevec;
      
    
 };



  TEST_F(FileHandlerTests, ReadingFromNotExistingFile){

    remove("thisfiledoesnotexist.txt");

    ASSERT_THROW(nonexFile.readFile(), IPCException);
    
  }

   TEST_F(FileHandlerTests, GettingTheCorrectSize){
     samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
    
      txtFile.createFile();
       txtFile.writeFile(samplevec,samplevec.size());
         
       ASSERT_EQ(txtFile.getSize(), samplevec.size() );

  }


   TEST_F(FileHandlerTests, ReadingAndWritingTheCorrectData){
     samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
     std::vector<unsigned char> testvec;
  
    
        txtFile.createFile();

        txtFile.writeFile(samplevec,samplevec.size());
        txtFile.openForReading();
        testvec = txtFile.readFile();
        
        
      remove("testfile.txt"); 
       ASSERT_EQ(testvec, samplevec );

  }


   TEST_F(FileHandlerTests, RemovingFile){

    samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
    
    std::vector<unsigned char> testvec;
    
        txtFile.createFile();
        txtFile.writeFile(samplevec,samplevec.size()+1);
        txtFile.removeFile();
        
        
    ASSERT_THROW( txtFile.openForReading();, IPCException);
       
    }

 

/**
 * Testing CommandOption
 * 
 */




TEST(CommandOptionTests, GettingProtocolandFileName){
  
  
 
  char cmdlineTemp[][4096] = {"","--pipe","--file","test.txt"};
  char *argv[] = {cmdlineTemp[0], cmdlineTemp[1],cmdlineTemp[2],cmdlineTemp[3], NULL};

  CommandOption co("ipcsender",4,argv);
  std::vector<std::string> outputs = co.getCommand();
  ASSERT_EQ(outputs[0],"test.txt");
  ASSERT_EQ(outputs[1],"pipe");

}

TEST(CommandOptionTests, GivingOnlyProtocolAsArgument){
  

  char cmdlineTemp[][4096] = {"","--pipe"};
  char *argv[] = {cmdlineTemp[0], cmdlineTemp[1], NULL};

 
    CommandOption co("ipcsender",2,argv);
  
ASSERT_THROW(co.getCommand(), IPCException);
 
}


TEST(CommandOptionTests, GivingWrongCommand){
  
 
  char cmdlineTemp[][4096] = {"","--wrongcommand"};
  char *argv[] = {cmdlineTemp[0], cmdlineTemp[1], NULL};
ASSERT_THROW(CommandOption co("ipcsender",2,argv), IPCException);
  
}

TEST(CommandOptionTests, GivingOnlyFileAsArgument){

  char cmdlineTemp[][4096] = {"","--file","test.txt"};
  char *argv[] = {cmdlineTemp[0], cmdlineTemp[1], cmdlineTemp[2], NULL};
  


    CommandOption co("ipcsender",3,argv);
  
  
ASSERT_THROW(co.getCommand(), IPCException);
}


TEST(CommandOptionTests, GettingMsgQueueProtocol){
  
  char * arggv[4] = {"","--msgqueue","--file","test.txt"};
 

  CommandOption co("ipcsender",4,arggv);
  std::vector<std::string> outputs = co.getCommand();
  ASSERT_EQ(outputs[0],"test.txt");
  ASSERT_EQ(outputs[1],"msgqueue");

}


/**
 * IPCException
 * 
 */

class IPCExceptionTests : public ::testing::Test{

      public:

      protected:
     void broken(){
       throw IPCException("Test Error");
     }
    
 };

TEST_F(IPCExceptionTests, CatchingException){
   ASSERT_THROW(broken(), IPCException);
 }


 /**
 * PipeSender & PipeReceiver
 * 
 */

 TEST(PipeTests, SendingSmallTextfile){

  FileHandler pf("pipesender.txt");
  FileHandler pr("pipereceiver2.txt");

  std::string samplestring = "Pipe Test Data";
  std::vector<unsigned char> samplevec;
  samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
  std::vector<unsigned char> sendvec;
  std::vector<unsigned char> recvec;
    
  pf.createFile();
  pf.writeFile(samplevec,samplevec.size());
    pf.openForReading();
   sendvec = pf.readFile();
   pf.~FileHandler();

  pid_t pid = fork();

  if (pid > 0){
    PipeSender pips("pipesender.txt");
    exit(0);
  }
  else if (pid == 0){
    PipeReceiver pipr("pipereceiver2.txt");
     pr.openForReading();
     recvec = pr.readFile();
    ASSERT_STREQ((char *)sendvec.data(),(char *)recvec.data());
    
    }

}


 
