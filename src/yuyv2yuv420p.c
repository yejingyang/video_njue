/*
 * yuyv2yuv420p.c
 *
 *  Created on: Mar 26, 2013
 *      Author: yang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/yuyv2yuv420p.h"
#include "../include/libavcodec/avcodec.h"
#include "../include/libswscale/swscale.h"
#include "../include/libavutil/pixfmt.h"


void init_ctx(int width, int height,int bytesperline)
{
    ctx.width = width;
    ctx.heigth = height;
    ctx.sws = sws_getContext(width, height, AV_PIX_FMT_NONE, width, height, AV_PIX_FMT_YUV420P,
                            SWS_FAST_BILINEAR, 0, 0, 0);
    ctx.rows = height;
    ctx.bytesperrow = bytesperline;
    avpicture_alloc(ctx.pic_target, AV_PIX_FMT_YUV420P, ctx.width, ctx.heigth);
}


int yuyv_to_i420p_format(uint8_t *in)
{
    int rs = 0;

    memset(ctx.pic_target, 0, sizeof(AVPicture));

    ctx.pic_src.data[0] = (unsigned char *)in;
    ctx.pic_src.data[1] = ctx.pic_src.data[2] = ctx.pic_src[3] = 0;
    ctx.pic_src.linesize[0] = ctx.bytesperrow;
    ctx.pic_src.linesize[1] = ctx.pic_src.linesize[2] = ctx.pic_src.linesize[3] = 0;

    rs = sws_scale(ctx.sws, ctx.pic_src.data, ctx.pic_src.linesize,
                    0, ctx.rows, ctx.pic_target->data, ctx.pic_target->linesize);
}
