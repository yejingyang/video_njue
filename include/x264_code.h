#ifndef X264_CODE_H_INCLUDED
#define X264_CODE_H_INCLUDED

#include "sdf.h"
#include "yuyv2yuv420p.h"


uint8 	*h264_buf;
struct	encoder *h264_encoder;


//convert picture to YUYV(422) format into I420 format
void yuyv_to_i420_format(uint8 *in);

/*
*   initialize the x264 encoder
*   @cam    :the point to the camera device structure
*   retval  :non return value
*/
void init_encoder(struct camera *cam);

/*
*   begin compress
*   @width  :the frame width
*   @height :the frame heigth
*   retval  :success is 0,others are wrong
*/
int compress_begin(int width, int height);

/*
*   compress a frame and deal with the result with the nal_handle function
*   @type       :the frame type
*   @in         :the input yuv422 picture type
*   @nal_handle :function to deal with the compress result
*   retval      :0 is success,others is failure
*/
int	compress_frame(int type, uint8 *in, nal_fun_handle nal_handle);

/*
*   compress end
*   non return value
*/
void compress_end();

/*
*   close the x264 encoder
*   non return value
*/
void uninit_encoder();

#endif // X264_CODE_H_INCLUDED
