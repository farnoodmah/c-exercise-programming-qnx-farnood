#ifndef _IOV_SERVER_H_
#define _IOV_SERVER_H_

#include <sys/iomsg.h>
#include <pthread.h>
#include <stdint.h>

//messages protocol server name
#define SERVER_NAME "filetransfer"
#define MAX_FILE_NAME_SIZE 1024
#define FILE_IOV_MSG_TYPE (_IO_MAX + 2)

// message queue: message size & max number of message
#define MQ_MAXMSG 1024
#define MQ_MSGSIZE 4096


//layout of msg's should always defined by a struct, and ID'd by a msg type
// number as the first member
typedef struct
{
	uint16_t msg_type;
	unsigned data_size;
} file_header_t;
// the data follows the above header in the message, it's data_size long



typedef enum {
	MESSAGE, MSG_QUEUE, PIPE, SHM, NONE
} protocol_t;

#endif //_IOV_SERVER_H_
