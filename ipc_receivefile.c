/*
 * ipc_receivefile.c
 *
 * 	USE QNX TERMINAL TO RUN IT. FOR MORE INFORMATION AND GUIDE GO TO THE "README". 
 * 
 * Available protocols:
 *		 + messages 
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
#include "iov_server.h"
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

#include "iov_server.h"

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
						"--queue    	  For receiving files with the message queue option. (*not implemented)\n"
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
		      perror ("mq_getattr()");
		      exit(EXIT_FAILURE);
		   }

		   attrs.mq_flags = O_RDONLY | O_NONBLOCK;
		   mq_setattr(msg_queue, &attrs, NULL);

			int fd = open(file_name, O_WRONLY | O_CREAT);
			if(fd==-1)
				{
					perror("open");
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

	   printf("File has been received successfully");

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
	fh = open(file_name, O_WRONLY | O_CREAT, 0644);

	// FIFO file path
	char * myfifo = "myfifo";

	// Creating the named file(FIFO)
	// mkfifo(<pathname>,<permission>)
	 mkfifo(myfifo, 0666);

	 // First open in read only and read
	 fifo = open(myfifo,O_RDONLY);
	 int read_bytes;
	 sleep(10);
	 read(fifo,receive_size,80);




	 file_size = strtol (receive_size,&ptr_end,10);

	 printf("the size%ld \n",file_size);

	 char *buffer = (char*)malloc(file_size);
	 read(fifo, buffer, file_size);
	 save_file(buffer,fh,file_size);

	 free(buffer);
	 close(fh);
	 close(fifo);
	 remove(myfifo);



}


//void msg_shm_ipc_receive(char* file_name){
//	int fd;
//	shmem_t *ptr;
//	int ret;
//	pthread_mutexattr_t mutex_attr;
//	pthread_condattr_t cond_attr;
//
//	if (argc != 2)
//	{
//			printf("ERROR: use: shmem_posix_creator shared_memory_object_name\n");
//			printf("Example: shmem_posix_creator /wally\n");
//			exit(EXIT_FAILURE);
//	}
//	if (*argv[1] != '/')
//	{
//		printf("ERROR: the shared memory name should start with a leading '/' character\n");
//		exit(EXIT_FAILURE);
//	}
//
//	printf("Creating shared memory object: '%s'\n", argv[1]);
//	fd = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, 0660);
//	if (fd == -1)
//	{
//			perror("shm_open()");
//			unlink_and_exit(argv[1]);
//	}
//
//	/* set the size of the shared memory object, allocating at least one page of memory */
//
//	ret = ftruncate(fd, sizeof(shmem_t));
//	if (ret == -1)
//		{
//			perror("ftruncate");
//			unlink_and_exit(argv[1]);
//		}
//
//	/* get a pointer to the shared memory */
//
//	ptr = mmap(0, sizeof(shmem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//	if (ptr == MAP_FAILED)
//	{
//		perror("mmap");
//		unlink_and_exit(argv[1]);
//	}
//
//	/* don't need fd anymore, so close it */
//	close(fd);
//
//	/* now set up our mutex and condvar for the synchronization and notification */
//
//	pthread_mutexattr_init(&mutex_attr);
//	pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
//	ret = pthread_mutex_init(&ptr->mutex, &mutex_attr);
//	if (ret != EOK)
//			{
//				perror("pthread_mutex_init");
//				unlink_and_exit(argv[1]);
//			}
//
//	pthread_condattr_init(&cond_attr);
//	pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
//	ret = pthread_cond_init(&ptr->cond, &cond_attr);
//	if (ret != EOK)
//	{
//			perror("pthread_cond_init");
//			unlink_and_exit(argv[1]);
//	}
//
//	printf("Shared memory created and init_flag set to let users know shared memory object is usable.\n");
//
//	while (1) {
//		sleep(1);
//
//		/* lock the mutex because we're about to update shared data */
//		ret = pthread_mutex_lock(&ptr->mutex);
//		if (ret != EOK)
//		{
//			perror("pthread_mutex_lock");
//			unlink_and_exit(argv[1]);
//		}
//
//		ptr->data_version++;
//		snprintf(ptr->text, sizeof(ptr->text), "data update: %lu", ptr->data_version);
//
//		/* finished accessing shared data, unlock the mutex */
//		ret = pthread_mutex_unlock(&ptr->mutex);
//		if (ret != EOK)
//		{
//			perror("pthread_mutex_unlock");
//			unlink_and_exit(argv[1]);
//		}
//
//		/* wake up any readers that may be waiting */
//		ret = pthread_cond_broadcast(&ptr->cond);
//		if (ret != EOK)
//		{
//			perror("pthread_cond_broadcast");
//			unlink_and_exit(argv[1]);
//		}
//	}
//
//	/* we'll never exit the above loop but here's the cleanup anyway */
//
//		/* unmap() not actually needed on termination as all memory mappings are freed on process termination */
//		if (munmap(ptr, sizeof(shmem_t)) == -1)
//		{
//			perror("munmap");
//		}
//
//		/* but the name must be removed */
//		if (shm_unlink(argv[1]) == -1)
//		{
//			perror("shm_unlink");
//		}
//
//		exit(EXIT_SUCCESS);
//
//}
//
//void unlink_and_exit(char *name)
//{
//	(void)shm_unlink(name);
//	exit(EXIT_FAILURE);
//}

void save_file(char* data,  int file,long int data_size)
{

	//write the file
	write(file,data,data_size);

}
