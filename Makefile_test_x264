TOP_DIR := $(shell pwd)
APP = $(TOP_DIR)/bin/test_x264

CC = gcc
CFLAGS = -g 
LIBS = -lpthread -lx264 -lm -lavcodec -lswscale -lavutil 
DEP_LIBS = -L$(TOP_DIR)/lib
HEADER = -I$(TOP_DIR)/include
OBJS = src/test_x264.c src/x264_code.c src/video_capture.c src/file_operate.c src/yuyv2yuv420p.c

all:  $(OBJS)
	$(CC) -g -o $(APP) $(OBJS) $(HEADER) $(LIBS) $(DEP_LIBS) -DX264

clean:
	rm -f *.o a.out $(APP) core *~
