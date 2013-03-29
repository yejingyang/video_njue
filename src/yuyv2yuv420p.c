/*
 * yuyv2yuv420p.c
 *
 *  Created on: Mar 26, 2013
 *      Author: yang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdf.h"
#include "../include/yuyv2yuv420p.h"
#include "../include/libavcodec/avcodec.h"
#include "../include/libswscale/swscale.h"
#include "../include/libavutil/pixfmt.h"
#include "x264.h"


void init_ctx(struct camera *cam)
{
    ctx.width = cam->width;
    ctx.heigth = cam->height;
    ctx.sws = sws_getContext(width, height, AV_PIX_FMT_YUYV422, width, height, AV_PIX_FMT_YUV420P,
                            SWS_FAST_BILINEAR, NULL, NULL, NULL);
    ctx.rows = height;
    ctx.bytesperrow = cam->bytesperrow;
}


int yuyv_to_i420p_format(uint8_t *in, x264_picture_t *pic)
{
    int rs = 0;

    memset(ctx.pic_target, 0, sizeof(AVPicture));

    ctx.pic_src.data[0] = (unsigned char *)in;
    ctx.pic_src.data[1] = ctx.pic_src.data[2] = ctx.pic_src[3] = 0;
    ctx.pic_src.linesize[0] = ctx.bytesperrow;
    ctx.pic_src.linesize[1] = ctx.pic_src.linesize[2] = ctx.pic_src.linesize[3] = 0;

    rs = sws_scale(ctx.sws, ctx.pic_src.data, ctx.pic_src.linesize,
                    0, ctx.rows, pic->img.plane, pic->img.i_stride);
//    pic->img.i_plane = 3;
    return rs;
}
