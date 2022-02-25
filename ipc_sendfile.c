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




 static struct option longopts[] =

 {
			{"help", no_argument, NULL, 'h'},
			{"messages", no_argument, NULL, 'm'},
			{"queue", required_argument, NULL, 'q'},
			{"pipe", required_argument, NULL, 'p'},
			{"shm", required_argument, NULL, 's'},
			{"file", required_argument, NULL, 'f'},
			{ 0, 0, 0, 0 }
	};

char *filename;

void message_ipc_sendfile(char* filename);
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
				printf("\n\nusage: send_file [--help] [--messages –file <path>] [--queue –file <path>][--pipe –file <path>]\n"
					   "		             [shm –file <path> <buffer_size_in_kb>]\n\n\n"
					   "send_file is used to send files between a client (send_file) and server (receive_file) via different IPC methods (messages, queue, pipe, and shm).\n"
						"Primary commands:\n\n"
						"–message     For sending files with the message option.\n"
						"–queue    For sending files with the message queue option. (*not implemented)\n"
						"–pipe        For sending files with the pipe option. (*not implemented)\n"
						"–shm         For sending files by using a shared memory buffer. (*not implemented)\n\n"
						"\"send_file     –help\" lists available commands and guides.\n\n");


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

							break;
			case 'm':
				protocol = MESSAGE;
				break;
			case 'q':
				printf("message queue method is not available for now. you can use \"ipc_sendfile --messages\"\n");
				return 0;
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

		int coid; //Connection ID to server
		file_header_t hdr; //msg header will specify how many bytes of data will follow
		char incoming_message[10]; //space for server's reply
		int status; //status return value
		iov_t siov[2]; //create a 2 part iov


		// locate the server
			coid = name_open(SERVER_NAME, 0);
			if (coid == -1)
			{ //was there an error attaching to server?
			perror("ERROR: finding the name of server failed\n"); //look up error code and print
			exit(EXIT_FAILURE);
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
						exit(EXIT_FAILURE);
					}


				printf("\nData sent: %li\n", filesize);

				printf("Complete\n");

				free(buffer);

				close(fd);


}

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
