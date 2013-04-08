/*
 * yuyv2yuv420p.h
 *
 *  Created on: Mar 26, 2013
 *      Author: yang
 */

#ifndef YUYV2YUV420P_H_
#define YUYV2YUV420P_H_

#include <stdint.h>

#include "sdf.h"
#include "libswscale/swscale.h"


struct Ctx ctx;

/*
*   change picture from one format to another(here is yuv420)
*   @in     :source picture with the flat memory space
*   @pic    :point to target picture
*   retval  :0 is success, others are failure
*/
int yuyv_to_i420p_format(uint8_t *in, x264_picture_t *pic);

/*
*   initialize the ctx structure
*   @cam    :point to the camera structure
*   retval  :0 is success, others are failure
*/
void init_ctx(struct camera *cam);

/*
*   uninitialize the ctx structure
*   non return value
*/
void uninit_ctx();


#endif /* YUYV2YUV420P_H_ */
