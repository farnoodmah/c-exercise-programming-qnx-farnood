## Possible Problems

__________________

### Wrong or incomplete terminal arguments for IPCSender and IPCReceiver [SOLVED]

This programs is able to respond properly to users' wrong or incomplete arguments.

__________________


### Sending  a not existing File [SOLVED]

This program is able to respond properly via a proper exception.
__________________


### Sending large files through IPC protocols [SOLVED NO UNIT TEST]

Since the application splits the file data into smaller parts (4096), it can handle sending large files.
__________________

### IPCSender or IPCReceiver runs indefinitely when only one of them is launched [SOLVED]

All the protocols would run for a limited of time when only one of them (IPCSender or IPCReceiver)  launches. It also throws an exception because of not connecting to the client or server.
__________________

### SharedMemory and MsgQueue destructors do not work correctly [SOLVED]

All the transaction functions of protocols moved to another function and only the initialization remained in the constructor.
__________________

### Terminating one of the programs in the middle of the file transfer has not effect on the other side [SOLVED]

If one of the programs get terminated during the file transaction, the other side would throw an exception.

__________________

### Giving different protocols as arguments
__________________

### Giving the same file name of IPCSender to the IPCReceiver as an argument
__________________

### Program is not stable in filehandling

__________________


