#ifndef VIDEO_CAPTURE_H_INCLUDED
#define VIDEO_CAPTURE_H_INCLUDED

#include "sdf.h"

/*
*   open camera
*   @cam    :point to the camera structure
*   retval  :non return value
*/
void open_camera(struct camera *cam);

/*
*   close camera
*   @cam    :point to the camera structure
*   retval  :non return value
*/
void close_camera(struct camera *cam);

/*
*   start capture the picture from camera
*   @cam    :point to the camera structure
*   retval  :non return value
*/
void start_capturing(struct camera *cam);

/*
*   stop capture the picture
*   @cam    :point to the camera structure
*   retval  :non return value
*/
void stop_capturing(struct camera *cam);

/*
*   initialize the camera device
*   @cam    :point to the camera structure
*   retval  :non return value
*/
void init_camera(struct camera *cam);

/*
*   free the camera
*   @cam    :point to the camera structure
*   retval  :non return value
*/
void uninit_camera(struct camera *cam);

/*
*   map the driver memory from kernel space to user space
*   @cam    :point to the camera structure
*   retval  :non return value
*/
void init_mmap(struct camera *cam);

/*
*   set and get video capture FPS
*   @cam    :point to the camera structure
*   @fps    :the frame per second
*   retval  :non return value
*/
void set_capture_fps(struct camera *cam, int *fps);

/*
*   initialize video driver for linux system
*   @cam    :point to the camera structure
*   retval  :non return value
*/
void v4l2_init(struct camera *cam);

/*
*   close the video driver for linux system
*   @cam    :point to the camera structure
*   retval  :non return value
*/
void v4l2_close(struct camera *cam);

/*
*   read a frame from the opened camera device
*   @cam        :point to the camera structure
*   @frame_buf  :buffer for current frame
*   @length     :the frame buffer length
*   retval      :0 is success, others are failure
*/
int read_frame_from_camera(struct camera *cam, uint8 *frame_buf, int *length);

#endif // VIDEO_CAPTURE_H_INCLUDED
