/*
 * yuyv2yuv420p.h
 *
 *  Created on: Mar 26, 2013
 *      Author: yang
 */

#ifndef YUYV2YUV420P_H_
#define YUYV2YUV420P_H_
#include <stdint.h>
#include "libavcodec/avcodec.h"

#include "libswscale/swscale.h"
#include "sdf.h"
#include "x264.h"

struct Ctx
{
	int 	vid;
	int 	width;
	int 	height;
	struct SwsContext *sws;
	int 	rows;
	int 	bytesperrow;

	//test
//	AVPicture pic_src;
    x264_picture_t pic_xsrc;
};

struct Ctx ctx;

int yuyv_to_i420p_format(uint8_t *in, x264_picture_t *pic);

void init_ctx(struct camera *cam);

void uninit_ctx();


#endif /* YUYV2YUV420P_H_ */
