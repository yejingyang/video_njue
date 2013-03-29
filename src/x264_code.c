/*
 * x264_code.c
 *
 *  Created on: Mar 20, 2013
 *      Author: yang
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include "../include/yuyv2yuv420p.h"
#include "../include/x264.h"
#include "../include/sdf.h"


uint8 	*h264_buf;
struct	encoder *h264_encoder;

//initialize the x264 encoder
void init_encoder(struct camera *cam)
{
	h264_encoder = NULL;

	//get memory for the encoder
	if (NULL == (h264_encoder =
			(struct encoder *)malloc(sizeof(struct encoder))))
	{
		printf("can't get memory for X264 encoder struct\n");
		exit(EXIT_FAILURE);
	}

	compress_begin(FRAME_WIDTH, FRAME_HEIGHT);

	h264_buf = NULL;

	//get memory for h264 buffer field
	if (NULL == ( h264_buf =
			(uint8 *)malloc(sizeof(uint8) * cam->width * cam->height * 3)))
	{
		printf("out of memory\n");
		exit(EXIT_FAILURE);
	}
}


//begin compress
int compress_begin(int width, int height)
{
	struct encoder *en = h264_encoder;

	//parameters check
	if ((width % 16 != 0) || (height % 16 != 0))
	{
		return PARAMERROR;
	}

	//get memory for encoder parameter structure
	if (NULL == ( en->param = (x264_param_t *)malloc(sizeof(x264_param_t))))
	{
		printf("out of memory when get memory for encoder parameters\n");
		return MEMORYERROR;
	}

	//get memory for encoder picture structure
	if (NULL ==( en->picture =
			(x264_picture_t *)malloc(sizeof(x264_picture_t))))
	{
		printf("out of memory when get memory for encoder picture\n");
		return MEMORYERROR;
	}

	//set the encoder parameter by default
	x264_param_default(en->param);

	//set the frame width and height
	en->param->i_width = width;
	en->param->i_height = height;

	en->param->rc.i_lookahead = 0;

	en->param->i_fps_num = FPS;
	en->param->i_fps_den = 1;

	//set the maximum quantization step length to 26
	en->param->rc.i_qp_max = 26;
	//disable the CABAC encode method
	en->param->b_cabac = 0;
	//enable diamond analysis search method
	en->param->analyse.i_me_method = X264_ME_DIA;
	//set reference frame with 1 frame
	en->param->i_frame_reference = 1;

	/*
	 * set the maximum of picture slice to 1400 to adapt with
	 * the net MTU(1500)
	 */
	en->param->i_slice_max_size = 1400;

	//use base line profile
	x264_param_apply_profile(en->param, x264_profile_names[0]);

	if (NULL == (en->handle = x264_encoder_open(en->param)))
	{
		return BASICERROR;
	}

	//create a new picture
	x264_picture_alloc(en->picture, X264_CSP_I420, en->param->i_width, en->param->i_height);
	en->picture->img.i_csp = X264_CSP_I420;
	en->picture->img.i_plane = 3;

	return SUCCESS;
}



//compress a frame
int	compress_frame(int type, uint8 *in, uint8 *out)
{
	struct encoder *en = h264_encoder;

	x264_picture_t	pic_out;
	int 	nNal = -1;
	int 	result = 0;
	int		i = 0;

	uint8	*p_out = out;


	//picture format conversion and save it in the picture planar field
	yuyv_to_i420p_format(in, en->picture);

	switch (type)
	{
	case 0:
		en->picture->i_type = X264_TYPE_P;
		break;

	case 1:
		en->picture->i_type = X264_TYPE_IDR;
		break;

	case 2:
		en->picture->i_type = X264_TYPE_I;
		break;

	default:
		en->picture->i_type = X264_TYPE_AUTO;
		break;
	}

	//encode a picture
	if (x264_encoder_encode(en->handle, &(en->nal),
			&nNal, en->picture, &pic_out) < 0)
	{
		printf("x264_encoder_encode failure\n");

		return BASICERROR;
	}

	//do with the encode result
	for (i = 0; i < nNal; i++)
	{
		memcpy(p_out, en->nal[i].p_payload, en->nal[i].i_payload);
		p_out += en->nal[i].i_payload;

		result += en->nal[i].i_payload;
	}

	return result;
}


/* test  compress_frame
int	compress_frame(int type, uint8 *in, uint8 *out)
{
	struct encoder *en = h264_encoder;
	x264_picture_t pic_out;
	int nNal = -1;
	int result = 0;
	int i = 0;
	uint8_t *p_out = out;

	char *y = en->picture->img.plane[0];
	char *u = en->picture->img.plane[1];
	char *v = en->picture->img.plane[2];

	int is_y = 1, is_u = 1;
	int y_index = 0, u_index = 0, v_index = 0;

	int yuv422_length = 2 * en->param->i_width * en->param->i_height;

	//序列为YU YV YU YV，一个yuv422帧的长度 width * height * 2 个字节
	for (i = 0; i < yuv422_length; ++i) {
		if (is_y) {
			*(y + y_index) = *(in + i);
			++y_index;
			is_y = 0;
		} else {
			if (is_u) {
				*(u + u_index) = *(in + i);
				++u_index;
				is_u = 0;
			} else {
				*(v + v_index) = *(in + i);
				++v_index;
				is_u = 1;
			}
			is_y = 1;
		}
	}

	switch (type) {
	case 0:
		en->picture->i_type = X264_TYPE_P;
		break;
	case 1:
		en->picture->i_type = X264_TYPE_IDR;
		break;
	case 2:
		en->picture->i_type = X264_TYPE_I;
		break;
	default:
		en->picture->i_type = X264_TYPE_AUTO;
		break;
	}

	if (x264_encoder_encode(en->handle, &(en->nal), &nNal, en->picture,
			&pic_out) < 0) {
		return -1;
	}

	for (i = 0; i < nNal; i++) {
		memcpy(p_out, en->nal[i].p_payload, en->nal[i].i_payload);
		p_out += en->nal[i].i_payload;
		result += en->nal[i].i_payload;
	}

	return result;
}
*/



//compress end
void compress_end()
{
	struct encoder *en = h264_encoder;

	if (en->picture)
	{
		x264_picture_clean(en->picture);

		en->picture = NULL;
	}

	if (en->param)
	{
		free(en->param);
		en->param = NULL;
	}

	if (en->handle)
	{
		x264_encoder_close(en->handle);
	}

	free(en);
	en = NULL;
}


//close the x264 encoder
void uninit_encoder()
{
	compress_end();

	free(h264_buf);
	h264_buf = NULL;
}
