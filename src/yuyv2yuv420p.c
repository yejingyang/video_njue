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
#include "../include/libavutil/pixfmt.h"
#include "x264.h"

#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"



void init_ctx(struct camera *cam)
{
    ctx.width = cam->width;
    ctx.height = cam->height;
    ctx.sws = sws_getContext(ctx.width, ctx.height, PIX_FMT_YUYV422, ctx.width, ctx.height, PIX_FMT_YUV420P,
                            SWS_FAST_BILINEAR, NULL, NULL, NULL);
    ctx.rows = ctx.height;
    ctx.bytesperrow = cam->bytesperrow;

    avpicture_alloc(&ctx.pic_src, PIX_FMT_YUYV422, ctx.width, ctx.height);

    printf("ctx width is %d\n", ctx.width);
    printf("ctx heigth is %d\n", ctx.height);
    printf("ctx bytesperline is %d\n", ctx.bytesperrow);
    printf("ctx rows is %d\n", ctx.rows);
}


int yuyv_to_i420p_format(uint8_t *in, x264_picture_t *pic)
{
    int rs = 0;

    ctx.pic_src.data[0] = (unsigned char *)in;
    ctx.pic_src.data[1] = ctx.pic_src.data[2] = ctx.pic_src.data[3] = NULL;
    ctx.pic_src.linesize[0] = ctx.bytesperrow;
    ctx.pic_src.linesize[1] = ctx.pic_src.linesize[2] = ctx.pic_src.linesize[3] = 0;

    rs = sws_scale(ctx.sws, ctx.pic_src.data, ctx.pic_src.linesize,
                    0, ctx.rows, pic->img.plane, pic->img.i_stride);
//    pic->img.i_plane = 3;
    return rs;
}


void uninit_ctx()
{
    avpicture_free(&ctx.pic_src);
}
