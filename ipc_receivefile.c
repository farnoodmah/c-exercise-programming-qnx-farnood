/*
 * ipc_receivefile.c
 *
 * 	USE QNX TERMINAL TO RUN IT. FOR MORE INFORMATION AND GUIDE GO TO THE "README". 
 * 
 * Available protocols:
 *		 + messages 
 *		 + queue
 *------------------------------------------------------
 
 *  Created on: Feb 24, 2022
 *   Author: Farnood Mahboubi
 */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <errno.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/resmgr.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/netmgr.h>     // #define for ND_LOCAL_NODE is in here
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/stat.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <mqueue.h>
#include <time.h>
#include <process.h>
#include <sys/mman.h>

#include "server.h"
#include "server.h"

typedef union
{
	uint16_t msg_type;
	struct _pulse pulse;
	file_header_t msg_file_hdr;
} msg_buf_t;


//typedef union
//{
//	uint16_t type;
//	struct _pulse pulse;
//	get_shmem_msg_t get_shmem;
//	changed_shmem_msg_t changed_shmem;
//} shm_buf_t;

static struct option longopts[] =

 {
			{"help", no_argument, NULL, 'h'},
			{"messages", no_argument, NULL, 'm'},
			{"queue", no_argument, NULL, 'q'},
			{"pipe", required_argument, NULL, 'p'},
			{"shm", required_argument, NULL, 's'},
			{"file", required_argument, NULL, 'f'},
			{ 0, 0, 0, 0 }
	};


char *filename;
msg_buf_t filemsg;
int file;


void messages_ipc_receive(char* file_name);
void msg_queue_ipc_receive(char* file_name);
//void msg_shm_ipc_receive(char* file_name);
void pipe_ipc_receive(char* file_name);

void save_file(char* data, int file,long int data_size);

int main(int argc, char **argv){

	int check_file = 0;

	protocol_t protocol = NONE; //for choosing different methods of file transferring {MESSAGE, MSG_QUEUE, PIPE, SHM, NONE}
	int option;	//terminal arguments
	int longindex = 0; //getopt_long options

		while(1){

			option = getopt_long(argc, argv, "h:m:q:p:s:f:",longopts, &longindex);
			if(option==-1) break;

			switch(option){
			case 'h':
				printf("\n\n usage: receive_file [--help] [--messages --file <path>] [--queue --file <path>][--pipe --file <path>]\n"
					   "		             [shm --file <path> <buffer_size_in_kb>]\n\n\n"
					   "receive_file is used to receive the files from a client (send_file) via different IPC methods (messages, queue, pipe, and shm).\n"
						"Primary commands:\n\n"
						"--message     For receiving files with the message option.\n"
						"--queue    	  For receiving files with the message queue option.\n"
						"--pipe        For receiving files with the pipe option. (*not implemented)\n"
						"--shm         For receiving files by using a shared memory buffer. (*not implemented)\n\n"
						"\"receive_file     --help\" lists available commands and guides.\n");


				break;

			case 'f':
				if (strlen(optarg)>MAX_FILE_NAME_SIZE)
					{
						perror("ERROR: the name of the file is too long); \n");
						exit(EXIT_FAILURE);
					}

				if (strlen(optarg)==0)
					{
						perror("ERROR: the name of the file should be specified); \n");
						exit(EXIT_FAILURE);
					}

				filename=optarg;
				printf("file name: \"%s\"\n",filename);
				check_file = 1;
				pipe_ipc_receive(filename);
				break;

			case 'm':
				protocol = MESSAGE;
				break;
			case 'q':
				protocol = MSG_QUEUE;
				break;
			case 'p':
				printf("pipe method is not available for now. you can use \"--messages\"\n");
				return 0;
			case 's':
				printf("shared memory buffer method is not available for now. you can use \"--messages\"\n");
				return 0;
			case ':':
				printf("unrecognized command. please use \"--help\" for guide.\n");
				return 0;
			case '?':
				printf("unrecognized command. please use \"--help\" for guide.\n");
				return 0;
			default:
				printf("unrecognized command. please use \"--help\" for guide.\n");
				return 0;
			}
		}

		//selecting between different ipc protocols
		switch(protocol){

				case MESSAGE:
				if (check_file==0)
				{
				printf("you should determine a file. please use \"--help\" for guide.\n");
				exit(EXIT_FAILURE);
				}
				
				messages_ipc_receive(filename);
				break;

				case MSG_QUEUE:
				if (check_file==0)
				{
					printf("you should determine a file. please use \"--help\" for guide.\n");
					exit(EXIT_FAILURE);
				}
				msg_queue_ipc_receive(filename);
				break;

				case NONE:
				printf("unrecognized command. please use \"--help\" for guide.\n");
				exit(EXIT_FAILURE);

				default:
				printf("unrecognized command. please use \"--help\" for guide.\n");
				exit(EXIT_FAILURE);

							}




	return 0;
}


void messages_ipc_receive(char* file_name)
{

	int fh;
	int rcvid = 0; //Connection ID to sender
	name_attach_t* attach; //naming the server
	msg_buf_t msg;
	int status;
	char checksum[] = "success";

	attach = name_attach(NULL, SERVER_NAME, 0);
	if (attach == NULL)
			{ //was there an error creating the channel?
				perror("name_attach\n"); //look up the errno code and print
				name_detach(attach, 0);
				exit(EXIT_FAILURE);
			}


	while (1)
			{
				printf("Waiting for a file via the message method...\n");
				fh = open(file_name, O_WRONLY | O_CREAT, 0644);
				while(rcvid == 0){
					printf("Waiting for the file...\n");
					rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

				}
				
				if (rcvid == -1)
				{ //was there an error receiving msg?
					perror("MsgReceive\n"); //look up errno code and print
					close(file);
					exit(EXIT_FAILURE); //give up
				}

				else if (rcvid > 0)
				{

					switch (msg.msg_type)
					{
					case FILE_IOV_MSG_TYPE:
						printf("Received a file. the data size is %d bytes\n",
								msg.msg_file_hdr.data_size);

						char *data = (char*)malloc(msg.msg_file_hdr.data_size);

						printf("allocating %d bytes\n", msg.msg_file_hdr.data_size);

						if (data == NULL)
						{
							if (MsgError(rcvid, ENOMEM ) == -1)
							{
								perror("MsgError\n");
								free(data);
								close(file);
								exit(EXIT_FAILURE);
							}
						}
						else
						{

							status = MsgRead(rcvid, data, msg.msg_file_hdr.data_size, sizeof(file_header_t));
							if (status == -1)
								{
								perror("MsgRead\n");
								free(data);
								close(file);
								exit(EXIT_FAILURE);
								}


							save_file(data, fh, msg.msg_file_hdr.data_size);
							free(data);

							status = MsgReply(rcvid, EOK, checksum, sizeof(checksum));
							if (status == -1)
							{
								perror("MsgReply\n");
								close(file);
								exit(EXIT_FAILURE);
							}
						}

						break;
					default:
						if (MsgError(rcvid, ENOSYS) == -1)
						{
							perror("MsgError\n");
							close(file);
							exit(EXIT_FAILURE);
						}
						break;
					}

							close(file);
							if (status !=0)
							{
								perror("fclose error\n");
								exit(EXIT_FAILURE);
							}
							name_detach(attach, 0);


							exit(EXIT_SUCCESS);

				}
				else if (rcvid == 0)
				{ //pulse
					switch (msg.pulse.code)
					{
					case _PULSE_CODE_DISCONNECT:
						printf("Received disconnect pulse. Transferring the file completed.\n");
						if (ConnectDetach(msg.pulse.scoid) == -1)
						{
							perror("ConnectDetach");
						}
						break;
					case _PULSE_CODE_UNBLOCK:
						printf("Received unblock pulse\n");
						if (MsgError(msg.pulse.value.sival_int, -1) == -1)
						{
							perror("MsgError\n");
						}
						break;

					default:
						printf("unknown pulse received, code = %d\n", msg.pulse.code);

					}
				}
				else
				{
					printf("Receive returned an unexpected value: %d\n", rcvid);
				}
			}

		

}


void msg_queue_ipc_receive(char* file_name){

	mqd_t msg_queue=-1;
	int read_size = 0;
	int error_code = 0;
	struct mq_attr attrs;
	int ret;


	printf("Waiting for the sender queue...\n");

	while (msg_queue == -1)
		{
			msg_queue = mq_open("/my_queue", O_RDONLY, 0660, NULL);
		}

	printf ("Successfully opened my_queue:\n");

	sleep(2);


		   /* Get the queue's attributes. */

		   ret = mq_getattr (msg_queue, &attrs);
		   if (ret == -1) {
		      perror ("mq_getattr()\n");
		      exit(EXIT_FAILURE);
		   }

		   attrs.mq_flags = O_RDONLY | O_NONBLOCK;
		   mq_setattr(msg_queue, &attrs, NULL);

			int fd = open(file_name, O_WRONLY | O_CREAT);
			if(fd==-1)
				{
					perror("open\n");
					exit(EXIT_FAILURE);

				}


		   char *data = malloc(MQ_MSGSIZE);
		   	while (error_code != EAGAIN)
		   	{
		   		if (data != NULL)
		   		{
		   			read_size = mq_receive(msg_queue, data, MQ_MSGSIZE, NULL);
		   			error_code = errno;
		   			if (read_size > 0)
		   				save_file(data, fd, read_size);
		   		}
		   		else
		   		{
		   			perror("malloc");
		   			free(data);
		   			mq_close(msg_queue);
		   			mq_unlink("/my_queue");
		   			exit(EXIT_FAILURE);
		   		}
		   	}


	   free(data);
	   close(fd);

	   printf("File has been received successfully\n");

	   /* Unlink and then close the message queue. */
	   ret = mq_unlink ("/my_queue");
	   if (ret == -1) {
	      perror ("mq_unlink()");
	      exit(EXIT_FAILURE);
	   }

	   ret = mq_close (msg_queue);
	   if (ret == -1) {
	      perror ("mq_close()");
	      exit(EXIT_FAILURE);
	   }
	   exit(0);
}

void pipe_ipc_receive(char* file_name){


	int fh;
	int fifo;
	char  receive_size[80];
	long int file_size;
	char * ptr_end;
	int ret = -1;
	//opening an empty file to write and save the data
	fh = open(file_name, O_WRONLY | O_CREAT, 0644);

	if(fh==-1)
		 	{
		 		perror("open");
		 		exit(EXIT_FAILURE);

		 	}

	//FIFO (PIPE) directory
	char * myfifo = FIFO_NAME;

	// Creating the named file(FIFO)
	// mkfifo(<pathname>,<permission>)
	ret = mkfifo(myfifo, 0666);
	if(ret==-1)
		 	{
		 		perror("mkfifo");
				 close(fh);
		 		exit(EXIT_FAILURE);

		 	}


	 // open the FIFO in read only
	 fifo = open(myfifo,O_RDONLY);

	if(fifo==-1)
		 	{
		 		perror("openfifo");
				close(fh);
		 		exit(EXIT_FAILURE);

		 	}

	 int read_bytes;
	 while (ret ==-1){
		 printf("waiting for the connection ...\n");
		 printf("getting the size of the receiving file\n");
		 ret = read(fifo,receive_size,80);
	 }

	 printf("read the data successfully \n");

	 // changing the received size of the file from string to long int
	 file_size = strtol (receive_size,&ptr_end,10);

	 printf("the size of the file is %ld \n",file_size);

	 char *buffer = (char*)malloc(file_size);
	 read(fifo, buffer, file_size);

	 // writing the received the data
	 save_file(buffer,fh,file_size);

	 free(buffer);
	 ret = close(fh);
	 	if (ret !=0){
	 			 perror ("fclose error");
	 			free(buffer);
	 		exit(EXIT_FAILURE);
	 			  }
	 close(fifo);
	 remove(myfifo);



}



void save_file(char* data,  int file,long int data_size)
{

	//write the file
	write(file,data,data_size);

}
