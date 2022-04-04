# C++ Programming Hands ON

Target OS: Linux

Programs: ipcreceiver ipcsender

Goal: Transferring  the files between two processes. "ipcsender" program reads the file and send it to "ipcreceiver". Then, "ipcreceiver" gets the file and save it.

The file transfer happens via one of these three protocols:

PIPE, MESSAGE QUEUE, and SHARED MEMORY

## Progress

- [x] FileHandler Class Implementation
- [x] IPC Abstract Class Implementation
- [x] IPCSender Implementation
- [x] IPCReceiver Implementation
- [x] Pipe Sender Implementation
- [x] Pipe Receiver Implementation
- [ ] Message Queue Receiver Implementation
- [ ] Message Queue Sender Implementation
- [ ] SHM Sender Implementation
- [ ] SHM Receiver Implementation

## How to

1. Run the "environment_setup.sh" to install the required tools.
2. Run the "build_and_tests.sh" to build to binary files of "ipcreceiver" and "ipcsender" and to test the program.
3. The test log can be accessed in "test_output" folder and the binary files can be accessed in "binary_output".
4. Run the "ipcsender" and "ipcreceiver" via your terminal in this format:

```terminal
ipcreceiver --pipe --file sendingfilename

ipcsender --pipe --file receivingfilename
```
5. For more information and guide use "--help".



