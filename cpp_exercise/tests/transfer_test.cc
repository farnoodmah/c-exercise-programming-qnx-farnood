#include "src/lib/transferlib.h"

#include <map>
#include <vector>
#include<thread>

#include "gtest/gtest.h"



// class FileHandlerTest : public ::testing::Test{

//     public:

//     protected:
// //     FileHandler txtFileHandler{"testfile.txt"};
// //     FileHandler pdfFileHandler{"pdftestfile.pdf"};
    
// };



//  TEST_F(FileHandlerTest, OpeningFile){
     
//      txtFileHandler.openFile();
//  }

//  TEST_F(FileHandlerTest, WritingFile){
//      const std::string tInput = "Test Message";
//      txtFileHandler.writeFile(tInput,strlen(tInput.c_str()));
//  }

//  TEST_F(FileHandlerTest, ReadingFile){

//      size_t readSize = 12;
//      const std::string tInput = "Test Message";
//      std::string tOutPut = txtFileHandler.readFile(readSize);
//      ASSERT_EQ(tInput, tOutPut);
    
//  }

//  TEST_F(FileHandlerTest, GettingSizeOfFile){
//      size_t filesize = txtFileHandler.getSize();
//     ASSERT_EQ(12, filesize);
//  }

//  TEST_F(FileHandlerTest, GettingSizeOfBigFile){
//      size_t filesize = pdfFileHandler.getSize();
//      std::cout<<filesize<<std::endl;
//  }

     // TEST_F(FileHandlerTest, OpeningTextFile){
     //      FileHandler fdt("testfile.txt");
     //      fdt.createFile();
     // }


    //  TEST_F(FileHandlerTest, GettingaVectorByReading){
    //       FileHandler fd2("testfile.txt");
    //       size_t  filesize = fd2.getSize();
    //       std::vector<unsigned char> testbuffer(filesize);
    //       testbuffer = fd2.readFile(filesize);
    //       std::cout<<&testbuffer[0]<<std::endl;
    //  }

     // TEST_F(FileHandlerTest, WritingPdfFileAfterReading){
     //      std::vector<unsigned char> testbuffer = pdfFileHandler.readFile(pdfFileHandler.getSize());
     //      std::cout<<&testbuffer[0]<<std::endl;
     //      const mode_t  _mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
     //      int fd = open("pdffinal.pdf", O_WRONLY | O_CREAT | O_TRUNC, _mode);
     //      write(fd,&testbuffer[0],pdfFileHandler.getSize());
     //      close(fd);
     // }

     // TEST_F(FileHandlerTest, WritingFunctionGettingVector){

     //      std::vector<unsigned char> testbuffer = pdfFileHandler.readFile(pdfFileHandler.getSize());
     //      FileHandler test_fd("newpdf.pdf");
     //      test_fd.writeFile(testbuffer,pdfFileHandler.getSize());
     // }



 /*** 
  * IPCSenderPipeTests
  * 
  * 
  * **/

// void ipcsendtest(){
//      IPCSender ipcs("binarytest.dat", "pipe");
// }
// void ipcreceivetest(){
  
//      IPCReceiver ipcr("binaryreceived.dat", "pipe");
// }

// TEST(IPCSenderPipeTests, WritingSmallFileInPipe){
//      std::thread server(ipcreceivetest);
     
//      std::thread client(ipcsendtest);
     
//      server.join();
//      client.join();
     
     

     
     

// }

/*** 
  * IPCReceiverPipeTests
  * 
  * 
  * **/


// TEST(IPCReceiverPipeTests, CallingPipeFunction){

//          IPCReceiver ipcr("testingreceiver.txt", "pipe");
//  }




// in refactoring phase it should be deleted because of instantioation duplicate
// TEST(CheckoutTests, CanInstantiateCheckout){
   
//     Checkout co;
// }

// TEST(CheckoutTests, CanAddItemPrice){
//     Checkout co;
//     co.addItemPrice("1",12);

// }

// TEST(CheckoutTests, CanAddItem){
//     Checkout co;
//     co.addItem("a");
// }

// TEST_F(CheckoutTests, CanAddItemPrice){
//     checkOut.addItemPrice("1",12);

// }

// TEST_F(CheckoutTests, CanAddItem){
    
//     checkOut.addItem("a");
// }

// TEST_F(CheckoutTests, CanCalculateTotal){
//      checkOut.addItemPrice("a",1);
//     checkOut.addItem("a");
   
//     int total= checkOut.calculateTotal();
//     ASSERT_EQ(1, total);

// }

// TEST_F(CheckoutTests, CanAddMultipleItems){
//     checkOut.addItemPrice("a",1);
//     checkOut.addItemPrice("b",2);
//     checkOut.addItem("a");
//     checkOut.addItem("b");

//     int total = checkOut.calculateTotal();
//     ASSERT_EQ(3, total);

// }

// TEST_F(CheckoutTests, ItemWithNoPriceThrowsException){
//     ASSERT_THROW(checkOut.addItem("a"), std::invalid_argument);
// }


/**
 * Testing CommandOption
 * 
 */

// TEST(CommandOptionTests, InitializingCommandOptionObj){
  
//   CommandOption co("ipcsender");
// }





// TEST(CommandOptionTests, GettingPipeString){
//   //  char * arggv[2];
//   //  arggv[0]="";
//   //  arggv[1]="--pipe";

//   char *cmdline[100] = {"","--pipe"};

//   char *arggv[] = {cmdline[0],cmdline[1],NULL}; 

//   CommandOption co("ipcsender",3,arggv);
//   std::string output = co.getCommand();
//   ASSERT_EQ(output,"pipe");

  
// }



TEST(CommandOptionTests, GettingProtocolandFileName){

  char * arggv[4];
  arggv[0] = "";
  arggv[1]="--pipe";
  arggv[2] = "--file";
  arggv[3] = "test.txt";


  CommandOption co("ipcsender",4,arggv);
  std::vector<std::string> outputs = co.getCommand();
  ASSERT_EQ(outputs[0],"pipe");
  ASSERT_EQ(outputs[1],"test.txt");

}


