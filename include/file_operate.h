#ifndef FILE_OPERATE_H_INCLUDED
#define FILE_OPERATE_H_INCLUDED

#include "sdf.h"

/*
*   initialize the file operator
*   retval  :0 is success, others are failure
*/
int init_file();

/*
*   set YUV file name
*   @filename   :input the yuv file filename
*   retval      :0 is success, others are failure
*/
int set_yuv_filename(int8 *filename);

/*
*   set h264 file name
*   @filename   :input the h264 file filename
*   retval      :0 is success, others are failure
*/
int set_h264_filename(int8 *filename);

/*
*   save YUV format video
*   @src    :the buffer address
*   @length :the buffer length
*   retval  :0 is success, others are failure
*/
int	yuv_write(uint8 *src, int lenght);

/*
*   save h264 format video where be decoded by x264 encoder
*   @src    :the buffer address
*   @length :the buffer length
*   retval  :0 is success, others are failure
*/
int h264_write(uint8 *src, int lenght);

/*
*   save the original nal unit to the file system
*   with the FU-A cut function
*   @nal    :point to source nal unit
*   @nNal   :number of the nalu
*   retval  :0 is success,others are failure
*/
int save_nalu_to_filesystem(x264_nal_t *nal, int nNal);

/*
*   close the opened file
*   retval  :0 is success, others are failure
*/
int uninit_file();

#endif // FILE_OPERATE_H_INCLUDED
