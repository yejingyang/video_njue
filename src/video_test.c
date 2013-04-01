/*
 * video_test.c
 *
 *  Created on: Mar 25, 2013
 *      Author: yang
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "../include/sdf.h"
#include "../include/yuyv2yuv420p.h"


int main()
{
	int	 length = 0;
	int8 *pic;
	struct camera ca = {0};
	char *camera_name = "/dev/video1";

	ca.width = 640;
	ca.height = 480;
	ca.device_name = camera_name;

	pic = (int8 *)malloc(640 * 480 * 2);
	if (NULL == pic)
	{
		printf("can't get enough memory in main function\n");
		exit(EXIT_FAILURE);
	}

	printf("Now initialize the video for linux \n");

	v4l2_init(&ca);

	set_yuv_filename("test.jpg");
	init_file();

	init_ctx(&ca);


	read_frame_from_camera(&ca, pic, &length);
	yuv_write(pic, length);

	uninit_file();
	v4l2_close(&ca);
}
