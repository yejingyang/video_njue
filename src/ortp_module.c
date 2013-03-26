/*
 * ortp_module.c
 *
 *  Created on: Mar 20, 2013
 *      Author: yang
 */
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "../include/sdf.h"


struct rtp_session_mgr_t *rtp_session_mgr;

int		ortp_payload_type = 40;
int8	*orpt_remote_ip_address = NULL;
int		ortp_port = 8000;
int		ortp_timestamp = 3600;


//initialize the RTP protocol
int init_rtp()
{
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

	rtp_session_mgr->cur_timestamp += rtp_session_mgr->timestamp_inc;

	return sended_bytes;
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
