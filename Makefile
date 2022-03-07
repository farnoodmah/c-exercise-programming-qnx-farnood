
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

ipc_receivefile.o: ipc_receivefile.c server.h
ipc_sendfile.o: ipc_sendfile.c server.h