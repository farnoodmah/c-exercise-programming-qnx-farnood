# C Programming Hands ON

Target OS: QNX Neutrino

Programs: ipc_receivefile ipc_sendfile 

Goal: Transferring  the files between two processes. "ipc_sendfile" program reads the file and send it to "ipc_receivefile". Then, "ipc_receivefile" gets the file and save it.

The file transfer happens via one of these four protocols:

MESSAGE, MESSAGE QUEUE, PIPE and SHARED MEMORY

## Progress

- [x] --help implementation
- [x] MESSAGES transport type
- [x] PIPE transport type
- [x] QUEUE transport type
- [x] PIPE transport type
- [ ] SHARED_MEMORY transport type

## How to

1. open the folder with Momentics IDE.
2. open the QNX target on your QEMU.
3. run the ipc_sendfile and ipc_receivefile on Momentics to get the binary codes.
4. copy the binary codes and the file you want to transfer to the "tmp" folder of the QNX target.
5. in you qnx terminal go the "/tmp/" folder.
6. run the ipc_receive file in this format:
"/tmp/ipc_receivefile --messages --file "savefile"
7. in another qnx terminal run the ipc_sendfile:
"/tmp/ipc_sendfile --messages --file "sendfile".

if you did it correctly the file transfers successfully and appears with the new name in the tmp folder.



