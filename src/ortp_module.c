/*
 * ortp_module.c
 *
 *  Created on: Mar 20, 2013
 *      Author: yang
 */
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "sdf.h"
#include "ortp_module.h"


//initialize the RTP protocol
int init_rtp()
{
    ortp_payload_type   = 96;
    ortp_port           = 1234;
    ortp_timestamp      = 3600;
    orpt_remote_ip_address = "172.17.13.132";

	rtp_session_mgr = NULL;
	char	*m_SSRC = NULL;

	rtp_session_mgr = (struct rtp_session_mgr_t *)malloc(sizeof(struct rtp_session_mgr_t));
	if (NULL == rtp_session_mgr)
	{
		printf("out of memory when initial RTP\n");
		return MEMORYERROR;
	}

	memset(rtp_session_mgr, 0, sizeof(struct rtp_session_mgr_t));

	ortp_init();
	ortp_scheduler_init();

	rtp_session_mgr->rtp_session = rtp_session_new(RTP_SESSION_SENDONLY);
	if (NULL == rtp_session_mgr->rtp_session)
	{
		printf("out of memory when initial RTP Session\n");
		return MEMORYERROR;
	}

	rtp_session_set_scheduling_mode(rtp_session_mgr->rtp_session, 1);
	rtp_session_set_blocking_mode(rtp_session_mgr->rtp_session, 1);
	rtp_session_set_remote_addr(rtp_session_mgr->rtp_session, orpt_remote_ip_address, ortp_port);
	rtp_session_set_payload_type(rtp_session_mgr->rtp_session, ortp_payload_type);

	m_SSRC = getenv("SSRC");
	if (NULL != m_SSRC)
	{
		rtp_session_set_ssrc(rtp_session_mgr->rtp_session, atoi(m_SSRC));
	}

	rtp_session_mgr->cur_timestamp = 0;
	rtp_session_mgr->timestamp_inc = ortp_timestamp;

	printf("RTP initialize successfully\n");

	return SUCCESS;
}

//sent message to remote device
int	rtp_send(uint8 *src, int length)
{
	int	sended_bytes = 0;

	//input parameter check
	if (NULL == src || 0 >= length)
	{
		printf("input parameter error RPT send function\n");
		return sended_bytes;
	}

	sended_bytes = rtp_session_send_with_ts(rtp_session_mgr->rtp_session,
											(uint8 *)src,
											length,
											rtp_session_mgr->cur_timestamp);

	return sended_bytes;
}

//update the rtp timestamp
void rtp_update_timestamp()
{
    rtp_session_mgr->cur_timestamp += rtp_session_mgr->timestamp_inc;
}

//release the resource ORTP protocol took up
int uninit_rtp()
{
	rtp_session_destroy(rtp_session_mgr->rtp_session);
	rtp_session_mgr->rtp_session = NULL;

	free(rtp_session_mgr);
	rtp_session_mgr = NULL;

	return SUCCESS;
}



//deal with the nalu and cut it to adapt with the MTU
int send_nalu_by_rtp(x264_nal_t *nal, int nNal)
{
    int     i = 0;
    int     j = 0;
    int     fu_count = 0;
    int     payload_count = 0;
    int     current_bytes = 0;

    uint8   flag = 0;//original nalu header flag
    uint8   fu_flag = 0;
    uint8   nal_flag = 0;
    uint8   *tmp = NULL;
    uint8   *buf = nal_buf;

    for (i = 0; i < nNal; i++)
    {
        //abandon the original payload header
        if (*(nal[i].p_payload + 2) & 0x01)
        {
            tmp = nal[i].p_payload + 3;
            payload_count = nal[i].i_payload - 3;
        }
        else
        {
            tmp = nal[i].p_payload + 4;
            payload_count = nal[i].i_payload - 4;
        }

        //judge if the payload shuld cut or not
        if (MTU < payload_count)
        {
            flag = tmp[0];
            fu_flag = 0;
            nal_flag = 0;
            tmp = tmp + 1;
            payload_count = payload_count - 1;
            fu_count = payload_count / MTU;

            //FU-A is 28 ,B11100000 is 224
            fu_flag = ((flag & 224) | 28);

            for ( j = 0; j < fu_count; j++)
            {
                //set the nalu type by original nalu flag
                nal_flag = (flag & 31);
                current_bytes = MTU;

                if ( j == 0)
                {
                    //start of the FU-A group
                    nal_flag = (nal_flag | 128);
                }
                else if (j == (fu_count - 1))
                {
                    //end of the FU-A group
                    nal_flag = (nal_flag | 64);
                    current_bytes = payload_count;
                }

                *buf = fu_flag;
                *(buf + 1) = nal_flag;
                memcpy(buf + 2, tmp, current_bytes);
                tmp += current_bytes;

                rtp_send(buf, current_bytes + 2);

                payload_count -= current_bytes;
            }
        }
        else
        {
            rtp_send(tmp, payload_count);
        }
    }

    //update the rtp timestamp
    rtp_update_timestamp();
}
