/*
 * ipc_sendfile.c
 *
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

void message_ipc_sendfile(char* filename);
void msg_queue_ipc_sendfile(char* filename);
//void shm_ipc_sendfile(char* filename);
void pipe_ipc_sendfile(char* filename);


long int file_size_handler(char* file_name);
int main(int argc, char **argv){

	int check_file = 0;
	protocol_t protocol = NONE; //for choosing different methods of file transferring {MESSAGE, MSG_QUEUE, PIPE, SHM, NONE}
	int option; //terminal arguments
	int longindex = 0; //getopt_long options

		while(1){

			option = getopt_long(argc, argv, "h:m:q:p:s:f:",longopts, &longindex);
			if(option==-1) break;

			switch(option){
			case 'h':
				printf("\n\nusage: send_file [--help] [--messages --file <path>] [--queue --file <path>][--pipe --file <path>]\n"
					   "		             [shm --file <path> <buffer_size_in_kb>]\n\n\n"
					   "send_file is used to send files between a client (send_file) and server (receive_file) via different IPC methods (messages, queue, pipe, and shm).\n"
						"Primary commands:\n\n"
						"--message     For sending files with the message option.\n"
						"--queue    For sending files with the message queue option. (*not implemented)\n"
						"--pipe        For sending files with the pipe option. (*not implemented)\n"
						"--shm         For sending files by using a shared memory buffer. (*not implemented)\n\n"
						"\"send_file     --help\" lists available commands and guides.\n\n");


				break;

			case 'f':

							printf(optarg);

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

							filename = optarg;
							printf("File name: \"%s\"\n",filename);
							check_file = 1;
							pipe_ipc_sendfile(filename);

							break;

			case 'q':
			protocol = MSG_QUEUE;
			break;
			case 'm':
				protocol = MESSAGE;
				break;

			case 'p':
				printf("pipe method is not available for now. you can use \"ipc_sendfile --messages\"\n");

				return 0;
			case 's':
				printf("shared memory buffer method is not available for now. you can use \"ipc_sendfile --messages\"\n");
				return 0;

			case ':':
			    printf("unrecognized command. please use \"--help\" for guide.\n");
			    return 0;
			case '?':
				printf("unrecognized command. please use \"--help\" for guide.\n");
				return 0;
			 default:
				 break;
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
				
						message_ipc_sendfile(filename);
						break;

						case MSG_QUEUE:

							if (check_file==0)
							{
								printf("you should determine a file. please use \"--help\" for guide.\n");
								exit(EXIT_FAILURE);
							}
							msg_queue_ipc_sendfile(filename);
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


void message_ipc_sendfile(char* filename){

		long int filesize = file_size_handler(filename); //finding the size of the file
		int fd;

		int coid = -1; //Connection ID to server
		file_header_t hdr; //msg header will specify how many bytes of data will follow
		char incoming_message[10]; //space for server's reply
		int status; //status return value
		iov_t siov[2]; //create a 2 part iov

			while(coid==-1){
				printf("looking for the server...\n");
				coid = name_open(SERVER_NAME, 0);
				sleep(5);

			}


			printf("\nSending the following file to the server: %s\n", filename);

			// build the header
			hdr.msg_type = FILE_IOV_MSG_TYPE;
			fd = open(filename, O_RDONLY| O_LARGEFILE, S_IRUSR | S_IWUSR );

			if(fd==-1)
				{
					perror("open\n");
					exit(EXIT_FAILURE);

				}


			hdr.data_size = filesize;


			char *buffer = (char*)malloc(filesize);


			int ret = read(fd,buffer, filesize);

				if( ret	== -1 )
				{

						perror( "Error reading\n" );
						free(buffer);
						close(fd);
						exit(EXIT_FAILURE);
				}


				// setup the message as a two part iov, first the header then the data
				SETIOV (&siov[0], &hdr, sizeof hdr);
				SETIOV (&siov[1], buffer, hdr.data_size);


				// and send the message off to the server
				status = MsgSendvs(coid, siov, 2, incoming_message, sizeof incoming_message);

				if (status == -1)
					{ //was there an error sending to server?
						perror("MsgSend\n");
							free(buffer);
							close(fd);
						exit(EXIT_FAILURE);
					}


				printf("\nData sent: %li\n", filesize);

				printf("Complete\n");

				free(buffer);

				close(fd);


}

void msg_queue_ipc_sendfile(char* filename){

	mqd_t msg_queue;
	long int file_size = file_size_handler(filename);
	long int sent_bytes = 0;
	int ret, i;
	struct mq_attr attrs;


	mq_unlink("/my_queue");

	int fd;
	msg_queue = mq_open("/my_queue", O_CREAT | O_EXCL | O_WRONLY, 0660, NULL);
	while (msg_queue == -1)
	{
		if (errno == ENOENT)
		{
			printf("the receiver is connecting...n");
			msg_queue = mq_open("/my_queue", O_CREAT | O_EXCL | O_WRONLY, 0660, NULL);
			sleep(2);
		}
		else
		{
			perror("mq_open()");
			exit(EXIT_FAILURE);
		}

	}

	 printf ("Successfully opened my_queue:\n");

	 fd = open(filename, O_RDONLY| O_LARGEFILE, S_IRUSR | S_IWUSR );

	 if(fd==-1)
	 	{
	 		perror("open");
	 		exit(EXIT_FAILURE);

	 	}

	 char *buffer = (char*)malloc(file_size);



	 while (sent_bytes < file_size)
	 	 	{
	 	 		if (buffer != NULL)
	 	 		{
	 	 			long int read_size = read(fd, buffer, MQ_MSGSIZE);
					if( read_size == -1 )
						{
							free(buffer);
							perror( "Error reading the file" );
							exit(EXIT_FAILURE);
						}
	 	 			int error_check = mq_send(msg_queue, buffer, read_size, MQ_PRIO_MAX - 1);
	 	 			if (error_check == -1)
	 	 			{
	 	 				perror("mq_send");
	 	 				free(buffer);
	 	 				exit(EXIT_FAILURE);
	 	 			}
	 	 			sent_bytes += read_size;
	 	 		}
	 	 		else
	 	 		{
	 	 			perror("malloc");
	 	 			free(buffer);
	 	 			exit(EXIT_FAILURE);
	 	 		}
	 	 	}

	 	 	free(buffer);


	 	 	printf("File is sent to the queue.\n");
	 	 	printf("Waiting for client to pick up...\n");
	 	 	int check_empty = -1;

	 	 	// check if the queue is empty
	 	 	while (check_empty != 0)
	 	 	{
	 	 		mq_getattr(msg_queue, &attrs);
	 	 		check_empty = attrs.mq_curmsgs;
	 	 	}

	 	 	printf("Client picked it up | File size = %ld byte(s).\n", file_size);

	 	 	ret = mq_close(msg_queue);
			  if (ret == -1)
			{
				  perror ("mq_close()");
					exit(EXIT_FAILURE);

			}
	 	 	ret = close(fd);
			  if (ret !=0){
				  perror ("fclose error");
				  exit(EXIT_FAILURE);
			  }

	 	if (ret == -1) {
	 	    perror ("mq_close()");
	 	    exit(EXIT_FAILURE);
	 	}

	 	exit(EXIT_FAILURE);

}

//void shm_ipc_sendfile(char* filename){
//
//	int shmid;
//	int coid;
//	int status;
//	unsigned len;
//	int mem_fd;
//	char *mem_ptr;
//	get_shmem_msg_t get_msg;
//	get_shmem_resp_t get_resp;
//	changed_shmem_msg_t changed_msg;
//	char* shm_name = "/example";
//	pthread_mutexattr_t mutex_attr;
//	pthread_condattr_t cond_attr;
//
//	long int file_size = file_size_handler(filename);
//	shmid = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, 0660);
//
//	if (shmd == -1)
//		{
//			perror("shm_open");
//			shm_unlink(shm_name);
//			exit(EXIT_FAILURE);
//		}
//
//	/* find our server */
//	coid = name_open(SHMEM_SERVER_NAME, 0);
//	if (coid == -1)
//		{
//			perror("name_open");
//			exit(EXIT_FAILURE);
//		}
//
//	ret = ftruncate64(shmid, shared_mem_size);
//	if (ret == -1)
//	{
//			perror("ftruncate");
//			shm_unlink(shm_name);
//			exit(EXIT_FAILURE);
//	}
//
//
//	printf("Set size of the shared memory object to %d byte(s).\n", shared_mem_size);
//
//	shm_ptr = (shm_data_t*)mmap64(NULL, shared_mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);
//	if (shm_ptr == MAP_FAILED)
//		{
//			perror("mmap64");
//			shm_unlink(shm_name);
//			exit(EXIT_FAILURE);
//		}
//
//	printf("Get the shared memory pointer [%p].\n", shm_ptr);
//
//	close(shmid);
//
//	pthread_mutexattr_init(&mutex_attr);
//	pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
//	ret = pthread_mutex_init(&shm_ptr->mutex, &mutex_attr);
//	if (ret != EOK)
//	{
//		perror("pthread_mutex_init");
//		shm_unlink_exit(shm_name);
//	}
//
//
//	// set up the condvar
//	pthread_condattr_init(&cond_attr);
//	pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
//	ret = pthread_cond_init(&shm_ptr->cond, &cond_attr);
//	if (ret != EOK)
//	{
//		perror("pthread_cond_init");
//		shm_unlink_exit(shm_name);
//	}
//
//	// set flags
//	shm_ptr->is_init = 1; // is initialized
//	shm_ptr->is_read = 1; // to start the sending process
//	shm_ptr->shared_mem_size = shared_mem_size;
//
//
//	while (is_end != -1)
//	{
//		lock_mutex(&shm_ptr->mutex, shm_name);
//		if (shm_ptr->is_read == 1)
//		{
//			if (sent < file_size)
//			{
//				long int read_size = read(fd, &shm_ptr->data_ap[0], data_size);
//				if(read_size != -1)
//				{
//					shm_ptr->is_read = 0;
//					shm_ptr->data_version++;
//					shm_ptr->data_size = read_size;
//					sent += read_size;
//				}
//				else
//				{
//					perror("read");
//					close(fd);
//					shm_unlink_exit(shm_name);
//				}
//			}
//			else if (sent == file_size && shm_ptr->data_size != 0)
//			{
//				shm_ptr->is_read = 0;
//				shm_ptr->data_size = 0;
//				shm_ptr->data_version++;
//				is_end = -1;
//			}
//		}
//		unlock_mutex(&shm_ptr->mutex, shm_name);
//		send_cond_broadcast(&shm_ptr->cond, shm_name);
//	}
//	if (file_size == sent)
//	{
//		printf("File sent | File size = %ld byte(s).\n", file_size);
//	}
//	else
//	{
//		printf("Connection lost. Interrupted transfer. Removed [%s]\n", shm_name);
//		close(fd);
//		shm_unlink_exit(shm_name);
//	}
//
//		get_msg.type = GET_SHMEM_MSG_TYPE;
//		get_msg.shared_mem_bytes = 8192;  // share two pages, first for data to server, second for data from server
//
//
//	if (argc != 2)
//		{
//			printf("ERROR: use: shmem_posix_user shared_memory_object_name\n");
//			printf("Example: shmem_posix_user /wally\n");
//			exit(EXIT_FAILURE);
//		}
//
//		if (*argv[1] != '/')
//		{
//			printf("ERROR: the shared memory name should start with a leading '/' character\n");
//			exit(EXIT_FAILURE);
//		}
//
//		ptr = get_shared_memory_pointer(argv[1], 100);
//		if (ptr == MAP_FAILED)
//			{
//				fprintf(stderr, "Unable to access object '%s' - was creator run with same name?\n", argv[1]);
//				exit(EXIT_FAILURE);
//			}
//
//
//		while (1) {
//				/* lock the mutex because we're about to access shared data */
//				ret = pthread_mutex_lock(&ptr->mutex);
//				if (ret != EOK)
//				{
//					perror("pthread_mutex_lock");
//					exit(EXIT_FAILURE);
//				}
//
//				/* wait for changes to the shared memory object */
//				while (last_version == ptr->data_version) {
//					ret = pthread_cond_wait(&ptr->cond, &ptr->mutex); /* does an unlock, wait, lock */
//					if (ret != EOK)
//					{
//						perror("pthread_cond_wait");
//						exit(EXIT_FAILURE);
//					}
//				}
//
//				/* update local version and data */
//				last_version = ptr->data_version;
//				strlcpy(local_data_copy, ptr->text, sizeof(local_data_copy));
//
//				/* finished accessing shared data, unlock the mutex */
//				ret = pthread_mutex_unlock(&ptr->mutex);
//				if (ret != EOK)
//				{
//					perror("pthread_mutex_unlock");
//					exit(EXIT_FAILURE);
//				}
//
//				printf("Data in shared memory was: '%s'\n", local_data_copy);
//			}
//
//			exit(EXIT_SUCCESS);
//
//}
void pipe_ipc_sendfile(char* filename){
	int pipefd;
	long int file_size = file_size_handler(filename);
	int fd;
	fd = open(filename, O_RDONLY| O_LARGEFILE, S_IRUSR | S_IWUSR );
	int ret;



	// FIFO file path
	char * myfifo = "myfifo";






	char *buffer = (char*)malloc(file_size);
	ret = read(fd,buffer, file_size);
	char *buffer2 = (char*)malloc(file_size);
	int check_fifo = -1;
	if( ret	== -1 )
	{

		perror( "Error reading\n" );
		free(buffer);
		close(fd);
		exit(EXIT_FAILURE);
	}

	close(fd);

	char arr1[80], arr2[80];


	// Open FIFO for write only
	while(check_fifo==-1){
		check_fifo = open(myfifo, O_WRONLY);

	}
		 printf("pipeopne\n");
		 sprintf(arr1, "%ld", file_size);
		 printf("the array %s\n",arr1);
		 //write the size first
		 write(check_fifo,arr1,80);

	     // Write the input arr2ing on FIFO
		 // and close it
		 write(check_fifo, buffer, file_size);
		 close(check_fifo);

		 printf("Successfully sent");

}

//void *get_shared_memory_pointer( char *name, unsigned num_retries )
//{
//	unsigned tries;
//	shmem_t *ptr;
//	int fd;
//
//	for (tries = 0;;) {
//		fd = shm_open(name, O_RDWR, 0);
//		if (fd != -1) break;
//		++tries;
//		if (tries > num_retries) {
//			perror("shmn_open");
//			return MAP_FAILED;
//		}
//		/* wait one second then try again */
//		sleep(1);
//	}
//
//	for (tries = 0;;) {
//		ptr = mmap(0, sizeof(shmem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//		if (ptr != MAP_FAILED) break;
//		++tries;
//		if (tries > num_retries) {
//			perror("mmap");
//			return MAP_FAILED;
//		}
//		/* wait one second then try again */
//		sleep(1);
//	}
//
//	/* no longer need fd */
//	(void)close(fd);
//
//	for (tries=0;;) {
//		if (ptr->init_flag) break;
//		++tries;
//		if (tries > num_retries) {
//			fprintf(stderr, "init flag never set\n");
//			(void)munmap(ptr, sizeof(shmem_t));
//			return MAP_FAILED;
//		}
//		/* wait on second then try again */
//		sleep(1);
//	}
//
//	return ptr;
//}
long int file_size_handler(char* file_name){


	int fh;


	fh = open(file_name, O_RDONLY);
	if(fh==-1)
	{
		perror("open\n");
		exit(EXIT_FAILURE);

	}
	else
	{
		struct stat file_stat;
		stat(file_name,&file_stat);
		long int size = file_stat.st_size;
		printf("\n the file size is %ld \n", file_stat.st_size);
		close(fh);
		return size;
	}

}

