/*
 * test_x264.c
 *
 *  Created on: Mar 25, 2013
 *      Author: yang
 */

/*
 * video_test.c
 *
 *  Created on: Mar 25, 2013
 *      Author: yang
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include "sdf.h"
#include "yuyv2yuv420p.h"
#include "ortp_module.h"


int get_and_compress_pic(struct camera *cam)
{
	int count = 1;
    int	ret;
	int length = 0;
    int out_length = 0;

    uint8 *pic;
	uint8 *pic_out;

	fd_set fds;
	struct timeval tv;

	pic = (int8 *)malloc(640 * 480 * 2);
	if (NULL == pic)
	{
		printf("can't get enough memory in main function\n");
		exit(EXIT_FAILURE);
	}

	pic_out = (int8 *)malloc(640 * 480 * 2);
	if (NULL == pic_out)
	{
		printf("can't get enough memory in main function\n");
		exit(EXIT_FAILURE);
	}

	for(;;)
	{
		length = 0;
		out_length = 0;
		memset(pic, 0, 640 * 480 * 2);
		memset(pic_out, 0, 640 * 480 * 2);

		printf("\n\n this is the %d th frame\n", count++);

		FD_ZERO(&fds);
		FD_SET(cam->camera_fd, &fds);

		tv.tv_sec = 2;
		tv.tv_usec = 0;

		ret = select(cam->camera_fd + 1, &fds, NULL, NULL, &tv);

		switch(ret)
		{
		case -1:
			break;
		case 0:
			printf("time out \n");
			break;
		default:
			read_frame_from_camera(cam, pic, &length);

			if (pic[0] == '\0')
			{
				break;
			}

			out_length = compress_frame(-1, pic, send_nalu_by_rtp);
		}
	}
}

int main()
{
	struct camera ca = {0};
	char *camera_name = "/dev/video1";

	ca.width = 640;
	ca.height = 480;
	ca.device_name = camera_name;

	printf("Now initialize the video for linux \n");

	v4l2_init(&ca);

	set_yuv_filename("test.jpg");
	init_file();
	init_encoder(&ca);
	compress_begin(ca.width, ca.height);
	init_ctx(&ca);
	init_rtp();

	get_and_compress_pic(&ca);

	uninit_file();
	v4l2_close(&ca);
}
