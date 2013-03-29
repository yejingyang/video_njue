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

#ifdef X264
#include "stdint.h"
typedef unsigned char uint8_t;
#include "x264.h"
#endif

#ifdef ORTP
#include "ortp.h"
#endif

#define CLEAR(x) memset(&(x), 0, sizeof(x))


typedef unsigned char		uint8;
typedef unsigned short int 	uint16;
typedef	unsigned int 		uint32;
typedef unsigned long long	uint64;
typedef char				int8;
typedef short int			int16;
typedef int					int32;
typedef long long			int64;


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

//open camera
void open_camera(struct camera *cam);

//close camera
void close_camera(struct camera *cam);

//start capture the picture from camera
void start_capturing(struct camera *cam);

//stop capture the picture
void stop_capturing(struct camera *cam);

//initialize the camera device
void init_camera(struct camera *cam);

//free the camera
void uninit_camera(struct camera *cam);

//map the driver memory from kernel space to user space
void init_mmap(struct camera *cam);

//set and get video capture FPS
void set_capture_fps(struct camera *cam, int *fps);

//initialize video driver for linux system
void v4l2_init(struct camera *cam);

//close the video driver for linux system
void v4l2_close(struct camera *cam);

//read a frame from the opened camera device
int read_frame_from_camera(struct camera *cam, uint8 *frame_buf, int *length);


//#ifdef X264

struct encoder
{
	x264_param_t 	*param;
	x264_t			*handle;
	x264_picture_t	*picture;
	x264_nal_t		*nal;
};


//initialize the x264 encoder
void init_encoder(struct camera *cam);

//begin compress
int compress_begin(int width, int height);

//convert picture to YUYV(422) format into I420 format
void yuyv_to_i420_format(uint8 *in);

//compress a frame
int	compress_frame(int type, uint8 *in, uint8 *out);

//compress end
void compress_end();

//close the x264 encoder
void uninit_encoder();

//#endif


//initialize the file operator
int init_file();

//set YUV file name
int set_yuv_filename(int8 *filename);

//set h264 file name
int set_h264_filename(int8 *filename);

//save YUV format video
int	yuv_write(uint8 *src, int lenght);

//save h264 format video where be decoded by x264 encoder
int h264_write(uint8 *src, int lenght);

//close the opened file
int uninit_file();


#ifdef ORTP

struct rtp_session_mgr_t
{
	RtpSession *rtp_session;
	uint32		timestamp_inc;
	uint32		cur_timestamp;
};


//initialize the RTP protocol
int init_rtp();

//sent message to remote device
int	rtp_send(uint8 *src, int length);

//release the resource ORTP protocol took up
int uninit_rtp();

#endif

#endif /* SDF_H_ */
