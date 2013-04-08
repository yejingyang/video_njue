#ifndef ORTP_MODULE_H_INCLUDED
#define ORTP_MODULE_H_INCLUDED

#include "sdf.h"
#include "x264_code.h"


int		ortp_payload_type;
int		ortp_port;
int		ortp_timestamp;
int8	*orpt_remote_ip_address;
uint8   nal_buf[MTU+2];
struct  rtp_session_mgr_t   *rtp_session_mgr;


/*
*   initialize the RTP protocol
*   non return value
*/
int init_rtp();

/*
*   sent message to remote device
*   @src    :send buffer
*   @length :send buffer length
*   retval  :sended bytes by oRTP
*/
int	rtp_send(uint8 *src, int length);

/*
*   release the resource ORTP protocol took up
*   no return value
*/
int uninit_rtp();

/*
*   update the rtp timestamp
*   non return value
*/
void rtp_update_timestamp();

/*
*   deal with the nalu and cut it to adapt with the MTU
*   with the FU-A cut function
*   @nal    :point to source nal unit
*   @nNal   :number of the nalu
*   retval  :0 is success,others are failure
*/
int send_nalu_by_rtp(x264_nal_t *nal, int nNal);

#endif // ORTP_MODULE_H_INCLUDED
