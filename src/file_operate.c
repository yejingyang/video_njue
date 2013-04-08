/*
 * file_operate.c
 *
 *  Created on: Mar 20, 2013
 *      Author: yang
 */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/sdf.h"


int8 *yuv_file = YUVFILENAME;
int8 *h264_file = H264FILENAME;

FILE *yuv_fd;
FILE *h264_fd;


//initialize the file operator
int init_file()
{
	int	ret_status = BASICERROR;

	yuv_fd = NULL;
	h264_fd = NULL;

	do{
		if (NULL == (yuv_fd = fopen(yuv_file, "w")))
		{
			printf("YUV file open failure\n");
			ret_status = FILEERROR;
			break;
		}

		if (NULL == (h264_fd = fopen(h264_file, "w")))
		{
			printf("H264 file open failure\n");
			ret_status = FILEERROR;
			break;
		}

		ret_status = SUCCESS;
	}while (0);

	return ret_status;
}


//set YUV file name
int set_yuv_filename(int8 *filename)
{
	return set_filename(filename, yuv_file);
}


//set h264 file name
int set_h264_filename(int8 *filename)
{
	return set_filename(filename, h264_file);
}


//set file name
int set_filename(int8 *filename, int8 *file_operator)
{
	int file_length = strlen(filename);
	int	 ret_status = BASICERROR;

	do{
		//check the input parameter
		if (strlen(filename) < 1)
		{
			printf("set filename error!\n");
			ret_status = PARAMERROR;
			break;
		}

		//malloc memory for file operator
		if (NULL == (file_operator = (int8 *)malloc(file_length + 1)))
		{
			printf("out of memory for file name buffer\n");
			ret_status = MEMORYERROR;
			break;
		}

		memcpy(file_operator, filename, file_length);

		ret_status = SUCCESS;
	}while(0);

	return ret_status;
}


//save YUV format video
int	yuv_write(uint8 *src, int lenght)
{
	return file_write(src, lenght, yuv_fd);
}


//save h264 format video where be decoded by x264 encoder
int h264_write(uint8 *src, int lenght)
{
	return file_write(src, lenght, h264_fd);
}


//file write
int file_write(uint8 *src, int length, FILE *fd)
{
	int ret_status = BASICERROR;

	do{
		//check parameter
		if (fd == NULL)
		{
			printf("wrong file operator in file write function\n");
			ret_status = PARAMERROR;
			break;
		}

		if (NULL == src || length < 1)
		{
			printf("wrong source input into file write function\n");
			ret_status = PARAMERROR;
			break;
		}

		//write to file
		if (0 == fwrite(src, length, 1, fd))
		{
			printf("write file wrong\n");
			ret_status = FILEERROR;
			break;
		}

		ret_status = SUCCESS;
	}while (0);

	return ret_status;
}


//save the original nal unit to the file system
int save_nalu_to_filesystem(x264_nal_t *nal, int nNal)
{
    int     i = 0;

    for (i = 0; i < nNal; i++)
    {
        h264_write(nal[i].p_payload, nal[i].i_payload);
    }

    return SUCCESS;
}


//close the opened file
int uninit_file()
{
	int ret_status = BASICERROR;

	do{
		if (fclose(yuv_fd))
		{
			ret_status = FILEERROR;
			break;
		}
		yuv_fd = NULL;

		if (fclose(h264_fd))
		{
			ret_status = FILEERROR;
			break;
		}
		h264_fd = NULL;

		ret_status = SUCCESS;
	}while (0);

	return ret_status;
}
