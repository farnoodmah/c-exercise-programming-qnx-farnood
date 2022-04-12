#include "src/lib/linuxipclib.h"
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
    std::string exception;

     try{
       nonexFile.readFile(10);
       
       }catch(IPCException & e){
          exception = e.what();
          ASSERT_EQ("FileHandler ERROR: Cannot Close the File Correctly", exception);
          
       }

    
  }

   TEST_F(FileHandlerTests, GettingTheCorrectSize){
     samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
    try{
        txtFile.createFile();
        txtFile.writeFile(samplevec,samplevec.size());
    } catch(IPCException & e){}
         
       ASSERT_EQ(txtFile.getSize(), samplevec.size() );

  }


   TEST_F(FileHandlerTests, ReadingAndWritingTheCorrectData){
     samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
     std::vector<unsigned char> testvec;
  
    try{
        txtFile.createFile();

        txtFile.writeFile(samplevec,samplevec.size());
        testvec = txtFile.readFile(samplevec.size());
    } catch(IPCException & e){}
      remove("testfile.txt"); 
       ASSERT_EQ(testvec, samplevec );

  }

TEST_F(FileHandlerTests, ReadingBigFileAndWriting){
    FileHandler fhpd("new.zip");
    std::vector<unsigned char> testvec;
    try{
      fhpd.createFile();
        
      testvec = bigFile.readFile(bigFile.getSize());
      fhpd.writeFile(testvec,testvec.size());
    } catch(IPCException & e){}
      

  }


   TEST_F(FileHandlerTests, RemovingFile){

    samplevec.insert(samplevec.begin(), samplestring.begin(), samplestring.end());
    std::string exception;
    std::vector<unsigned char> testvec;
    try{
        txtFile.createFile();
        txtFile.writeFile(samplevec,samplevec.size());
        txtFile.removeFile();
        testvec = txtFile.readFile(samplevec.size());
    } catch(IPCException & e){
          exception = e.what();
          ASSERT_EQ("FileHandler ERROR: Cannot Close the File Correctly", exception);
    }
       
    }

 

/**
 * Testing CommandOption
 * 
 */




TEST(CommandOptionTests, GettingProtocolandFileName){
  
  char * arggv[4] = {"","--pipe","--file","test.txt"};
 

  CommandOption co("ipcsender",4,arggv);
  std::vector<std::string> outputs = co.getCommand();
  ASSERT_EQ(outputs[0],"test.txt");
  ASSERT_EQ(outputs[1],"pipe");

}

TEST(CommandOptionTests, GivingOnlyProtocolAsArgument){
  
  char * arggv[2] = {"","--pipe"};
  std::string exception;
 try{
    CommandOption co("ipcsender",2,arggv);
  std::vector<std::string> outputs = co.getCommand();

 } catch(IPCException & e){
   exception = e.what();
   
   ASSERT_EQ("unrecognized command. filename and protocol should be determined. please use \"--help\" for guide.\n",exception);
 }  
  
}


TEST(CommandOptionTests, GivingWrongCommand){
  
  char * arggv[2] = {"","--wrongcommand"};
  std::string exception;
 try{
    CommandOption co("ipcsender",2,arggv);
  std::vector<std::string> outputs = co.getCommand();

 } catch(IPCException & e){
   exception = e.what();
   ASSERT_EQ("unrecognized command. please use \"--help\" for guide.\n",exception);
 }  
  
}

TEST(CommandOptionTests, GivingOnlyFileAsArgument){
  
  char * arggv[3] = {"","--file","test.txt"};
  std::string exception;
 try{
    CommandOption co("ipcsender",3,arggv);
  std::vector<std::string> outputs = co.getCommand();

 } catch(IPCException & e){
   exception = e.what();
   
   ASSERT_EQ("unrecognized command. filename and protocol should be determined. please use \"--help\" for guide.\n",exception);
 }  
  
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
   try{
    broken();
       }catch(IPCException & e){
     ASSERT_EQ("Test Error",e.what());
   }
 }