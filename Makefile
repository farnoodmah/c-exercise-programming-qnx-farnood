
#
#	Makefile for c-exercise-programming
#



DEBUG = -g
CC = qcc
LD = qcc

TARGET = -Vgcc_ntox86_64
#TARGET = -Vgcc_ntoarmv7le
#TARGET = -Vgcc_ntoaarch64le

CFLAGS += $(DEBUG) $(TARGET) -Wall
LDFLAGS+= $(DEBUG) $(TARGET)

BINS = ipc_sendfile ipc_receivefile

all:	$(BINS)


# dependencies

iov_server.o: ipc_receivefile.c iov_server.h
iov_client.o: ipc_sendfile.c iov_server.h