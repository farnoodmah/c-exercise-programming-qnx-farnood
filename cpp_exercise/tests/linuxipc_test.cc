#include "src/lib/linuxipclib.h"
#include "src/lib/filehandlerlib.h"
#include "src/lib/ipcexceptionlib.h"


#include <map>
#include <vector>
#include<thread>
#include <fstream>


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
        
        
      ASSERT_THROW( txtFile.openForReading(), IPCException);
       
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

  ASSERT_THROW( CommandOption co("ipcsender",2,argv), IPCException);
 
}


TEST(CommandOptionTests, GivingWrongCommand){
  
 
  char cmdlineTemp[][4096] = {"","--wrongcommand"};
  char *argv[] = {cmdlineTemp[0], cmdlineTemp[1], NULL};
  ASSERT_THROW(CommandOption co("ipcsender",2,argv), IPCException);
  
}

TEST(CommandOptionTests, GivingOnlyFileAsArgument){

  char cmdlineTemp[][4096] = {"","--file","test.txt"};
  char *argv[] = {cmdlineTemp[0], cmdlineTemp[1],cmdlineTemp[2],cmdlineTemp[3], NULL};
  
  ASSERT_THROW( CommandOption co("ipcsender",4,argv), IPCException);

}


TEST(CommandOptionTests, GettingMsgQueueProtocol){
  
 char cmdlineTemp[][4096] = {"","--msgqueue","--file","test.txt"};
 char *argv[] = {cmdlineTemp[0], cmdlineTemp[1], cmdlineTemp[2], cmdlineTemp[3], NULL};
  

  CommandOption co("ipcsender",4,argv);
  std::vector<std::string> outputs = co.getCommand();
  ASSERT_EQ(outputs[0],"test.txt");
  ASSERT_EQ(outputs[1],"msgqueue");

}

TEST(CommandOptionTests, GettingSHMProtocol){
  
  char cmdlineTemp[][4096] = {"","--shm","--file","test.txt"};
  char *argv[] = {cmdlineTemp[0], cmdlineTemp[1], cmdlineTemp[2], cmdlineTemp[3], NULL};

  CommandOption co("ipcsender",4,argv);
  std::vector<std::string> outputs = co.getCommand();
  ASSERT_EQ(outputs[0],"test.txt");
  ASSERT_EQ(outputs[1],"shm");

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

 void pipeSenderTest(){
          FileHandler pf("pipesender.txt");
          std::string samplestring = "Pipe Test Data";
          std::vector<unsigned char> samplevec;
          samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
          pf.createFile();
          pf.writeFile(samplevec,samplevec.size());
          PipeSender pipes("pipesender.txt");
          pipes.pipeTransfer();

  }

  void  pipeReceiverTest(){
          PipeReceiver piper("pipereceiver.txt");
          piper.pipeTransfer();
     }

TEST(PipeTests, SendingSmallTextfile){

   std::thread th1(pipeSenderTest);
   std::thread th2(pipeReceiverTest);

   th1.join();
   th2.join();

   std::ifstream f1("pipesender.txt", std::ifstream::binary|std::ifstream::ate);
   std::ifstream f2("pipereceiver.txt", std::ifstream::binary|std::ifstream::ate);

   bool check = std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));

   ASSERT_EQ(1,check);

}

TEST(PipeTests, OpeningOnlyPipeSender){

  
  ASSERT_THROW(PipeSender pipes("pipesender.txt"), IPCException);

}

TEST(PipeTests, OpeningOnlyPipeReceiver){

  PipeReceiver piper("pipereceiver.txt");
  ASSERT_THROW(piper.pipeTransfer(), IPCException);

}

/**
 * MSGQueueSender & MSGQueueReceiver
 * 
 */


 void msgqSenderTest(){
          FileHandler pf("msgqsender.txt");
          std::string samplestring = "MSGQueue Test Data";
          std::vector<unsigned char> samplevec;
          samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
    
          pf.createFile();
          pf.writeFile(samplevec,samplevec.size());
          MsgQueueSender msgqs("msgqsender.txt");
          msgqs.msgqTransfer();
  }

  void  msgqReceiverTest(){
          MsgQueueReceiver msgqr("msgqreceiver.txt");
          msgqr.msgqTransfer();
     }
    

 TEST(MSGQueueTests, SendingSmallTextfile){

   std::thread th1(msgqSenderTest);
   std::thread th2(msgqReceiverTest);

   th1.join();
   th2.join();

   std::ifstream f1("msgqsender.txt", std::ifstream::binary|std::ifstream::ate);
   std::ifstream f2("msgqreceiver.txt", std::ifstream::binary|std::ifstream::ate);

   bool check = std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));

   ASSERT_EQ(1,check);

}

TEST(MSGQueueTests, OpeningOnlyMSGQSender){

   MsgQueueSender msgqs("msgqsender.txt");
  
   ASSERT_THROW(msgqs.msgqTransfer(), IPCException);

}

TEST(MSGQueueTests, OpeningOnlyMSGQReceiver){

  
   ASSERT_THROW(MsgQueueReceiver msgr("msgqreceiver.txt"), IPCException);

}

/**
 * SharedMemorySender & SharedMemoryReceiver
 * 
 */


 void shmSenderTest(){
          FileHandler pf("shmsender.txt");
          std::string samplestring = "SHM Test Data";
          std::vector<unsigned char> samplevec;
          samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
    
          pf.createFile();
          pf.writeFile(samplevec,samplevec.size());
          SharedMemorySender shms("shmsender.txt");
          shms.shmTransfer();
     }

     void  shmReceiverTest(){
          SharedMemoryReceiver shmr("shmreceiver.txt");
          shmr.shmTransfer();
     }
    

 TEST(SharedMemoryTests, SendingSmallTextfile){

   std::thread th1(shmSenderTest);
   std::thread th2(shmReceiverTest);

  th1.join();
  th2.join();

  std::ifstream f1("shmsender.txt", std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2("shmreceiver.txt", std::ifstream::binary|std::ifstream::ate);

   bool check = std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));

  ASSERT_EQ(1,check);

}

TEST(SharedMemoryTests, OpeningOnlySHMSender){

  
  ASSERT_THROW(SharedMemorySender shms("shmsender.txt"), IPCException);

}

TEST(SharedMemoryTests, OpeningOnlySHMReceiver){

  SharedMemoryReceiver shmr("shmreceiver.txt");  
  ASSERT_THROW(shmr.shmTransfer(), IPCException);

}


