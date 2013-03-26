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

struct Ctx
{
	int 	vid;
	int 	width;
	int 	heigth;
	struct SwsContext *sws;
	int 	rows;
	int 	bytesperrow;
	AVPicture	pic_src;
	AVPicture	pic_target;
};


int yuyv_to_i420p_format();

#endif /* YUYV2YUV420P_H_ */
