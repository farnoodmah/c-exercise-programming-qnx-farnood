#ifndef _IOV_SERVER_H_
#define _IOV_SERVER_H_

#include <sys/iomsg.h>


#define SERVER_NAME "filetransfer"
#define MAX_FILE_NAME_SIZE 1024
#define FILE_IOV_MSG_TYPE (_IO_MAX + 2)

//layout of msg's should always defined by a struct, and ID'd by a msg type 
// number as the first member
typedef struct
{
	uint16_t msg_type;
	unsigned data_size;
} file_header_t;
// the data follows the above header in the message, it's data_size long

// checksum reply is an int

typedef enum {
	MESSAGE, MSG_QUEUE, PIPE, SHM, NONE
} protocol_t;

#endif //_IOV_SERVER_H_
