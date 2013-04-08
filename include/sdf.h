/*
 * sdf.h
 *
 *  Created on: Mar 20, 2013
 *      Author: yang
 */

#ifndef SDF_H_
#define SDF_H_

#include <linux/videodev2.h>
#include <string.h>

#include "stdint.h"
#include "x264.h"
#include "../include/ortp/ortp.h"


#define CLEAR(x) memset(&(x), 0, sizeof(x))

//define the function execute status
#define BASICERROR			1
#define SUCCESS				0
#define PARAMERROR			BASICERROR<<1
#define MEMORYERROR			BASICERROR<<2
#define FILEERROR			BASICERROR<<3

#define	FPS		15
#define FRAME_WIDTH		640
#define FRAME_HEIGHT	480

#define FILELENGTH		255
#define YUVFILENAME		"video.yuv"
#define H264FILENAME	"video.h264"

#define MTU 1400

typedef unsigned char		uint8;
typedef unsigned short int 	uint16;
typedef	unsigned int 		uint32;
typedef unsigned long long	uint64;
typedef char				int8;
typedef short int			int16;
typedef int					int32;
typedef long long			int64;

typedef unsigned char uint8_t;

typedef int (*nal_fun_handle)(x264_nal_t *, int );


struct buffer
{
	void	*start;
	size_t	length;
};

struct camera
{
	char	*device_name;
	int		camera_fd;
	int		width;
	int		height;
	int 	image_size;
	int     bytesperrow;
	int 	frame_number;
	struct	v4l2_capability v4l2_cap;
	struct 	v4l2_format		v4l2_fmt;
	struct	v4l2_crop 		crop;
	struct	buffer			*buffers;
};

struct encoder
{
	x264_param_t 	*param;
	x264_t			*handle;
	x264_picture_t	*picture;
	x264_nal_t		*nal;
};

struct rtp_session_mgr_t
{
	RtpSession *rtp_session;
	uint32		timestamp_inc;
	uint32		cur_timestamp;
};

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

#endif /* SDF_H_ */
