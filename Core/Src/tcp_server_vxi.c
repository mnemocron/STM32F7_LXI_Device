/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIsrv OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGsrv (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Christiaan Simons rewrote this file to get a more stable echo application.
 *
 **/

 // This file was modified by ST
 // This file way modified by Simon Burkhardt

#include "tcp_server_vxi.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "main.h"
#include "scpi/scpi.h"
#include "scpi-def.h"

#if LWIP_TCP

extern scpi_t scpi_context_tcp;

static struct tcp_pcb *tcp_vxiserver_pcb;
struct tcp_vxiserver_struct *scpi_server;
struct tcp_pcb *scpi_tpcb;

// ECHO protocol states
enum tcp_vxiserver_states
{
  VXI_NONE = 0,
  VXI_ACCEPTED,
  VXI_RECEIVED,
  VXI_CLOSING
};

// structure for maintaing connection infos to be passed as argument to LwIP callbacks
struct tcp_vxiserver_struct
{
  u8_t state;             // current connection state
  u8_t retries;
  struct tcp_pcb *pcb;    // pointer on the current tcp_pcb
  struct pbuf *bufrx;     // pointer on the received/to be transmitted pbuf
  struct pbuf *buftx;     // pointer on the transmitted pbuf
};

static err_t tcp_vxi_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_vxi_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *bufrx, err_t err);
static void tcp_vxi_error(void *arg, err_t err);
static err_t tcp_vxi_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_vxi_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_vxi_send(struct tcp_pcb *tpcb, struct tcp_vxiserver_struct *srv);
static void tcp_vxi_connection_close(struct tcp_pcb *tpcb, struct tcp_vxiserver_struct *srv);

/**
  * @brief  Initializ the tcp echo server
  * @param  None
  * @retval None
  */
void tcp_server_vxi_init(void)
{
  tcp_vxiserver_pcb = tcp_new();  // create new tcp pcb
  if (tcp_vxiserver_pcb != NULL){
    err_t err;
    // bind _pcb to protocol specific port
    err = tcp_bind(tcp_vxiserver_pcb, IP_ADDR_ANY, TCP_PORT_SCPI_RAW);
    if (err == ERR_OK){
      // start tcp listening for _pcb
      tcp_vxiserver_pcb = tcp_listen(tcp_vxiserver_pcb);
      // initialize LwIP tcp_accept callback function
      tcp_accept(tcp_vxiserver_pcb, tcp_vxi_accept);
    }
    else{
      // deallocate the pcb
      memp_free(MEMP_TCP_PCB, tcp_vxiserver_pcb);
    }
  }
}

/**
  * @brief  This function is the implementation of tcp_accept LwIP callback
  * @param  arg: not used
  * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
  * @param  err: not used
  * @retval err_t: error status
  */
static err_t tcp_vxi_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  err_t ret_err;
  struct tcp_vxiserver_struct *srv;
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);
  // set priority for the newly accepted tcp connection newpcb
  tcp_setprio(newpcb, TCP_PRIO_MIN);
  // allocate structure srv to maintain tcp connection informations
  srv = (struct tcp_vxiserver_struct *)mem_malloc(sizeof(struct tcp_vxiserver_struct));
  scpi_server = srv;
  if (srv != NULL){
    srv->state = VXI_ACCEPTED;
    srv->pcb = newpcb;
    srv->retries = 0;
    srv->bufrx = NULL;
    // pass newly allocated srv structure as argument to newpcb
    tcp_arg(newpcb, srv);
    // initialize lwip tcp_recv callback function for newpcb 
    tcp_recv(newpcb, tcp_vxi_recv);
    // initialize lwip tcp_err callback function for newpcb 
    tcp_err(newpcb, tcp_vxi_error);
    // initialize lwip tcp_poll callback function for newpcb
    tcp_poll(newpcb, tcp_vxi_poll, 0);
    ret_err = ERR_OK;
  }
  else{
    tcp_vxi_connection_close(newpcb, srv);
    ret_err = ERR_MEM; // return memory error
  }
  return ret_err;
}


/**
  * @brief  This function is the implementation for tcp_recv LwIP callback
  * @param  arg: pointer on a argument for the tcp_pcb connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  pbuf: pointer on the received pbuf
  * @param  err: error information regarding the reveived pbuf
  * @retval err_t: error code
  */
static err_t tcp_vxi_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *bufrx, err_t err)
{
  struct tcp_vxiserver_struct *srv;
  err_t ret_err;
  LWIP_ASSERT("arg != NULL",arg != NULL);
  srv = (struct tcp_vxiserver_struct *)arg;
  // if we receive an empty tcp frame from client => close connection
  if (bufrx == NULL){
    srv->state = VXI_CLOSING; // remote host closed connection
    if(srv->buftx == NULL){   // @NOTE was bufrx
       tcp_vxi_connection_close(tpcb, srv); // we're done sending
    }
    else{
      tcp_sent(tpcb, tcp_vxi_sent); // we're not done yet - ack received packet
      tcp_vxi_send(tpcb, srv);      // send remaining data
    }
    ret_err = ERR_OK;
  }
  // else : a non empty frame was received from client but for some reason err != ERR_OK
  else if(err != ERR_OK){
    if (bufrx != NULL){
      srv->bufrx = NULL;
      pbuf_free(bufrx);   // free received pbuf
    }
    ret_err = err;
  }
  else if(srv->state == VXI_ACCEPTED){
    srv->state = VXI_RECEIVED; // first data chunk in bufrx->payload
    srv->bufrx = bufrx;        // store reference to incoming pbuf (chain)
    tcp_sent(tpcb, tcp_vxi_sent); // initialize LwIP tcp_sent callback function
    SCPI_Input(&scpi_context_tcp, srv->bufrx->payload, srv->bufrx->len);
    srv->bufrx = NULL;
    pbuf_free(bufrx);   // free received pbuf
    // tcp_vxi_send(tpcb, srv);   // send back the received data (echo)
    ret_err = ERR_OK;
  }
  else if (srv->state == VXI_RECEIVED){
    // more data received from client and previous data has been already sent*/
    if(srv->bufrx == NULL){
      srv->bufrx = bufrx;
      SCPI_Input(&scpi_context_tcp, srv->bufrx->payload, srv->bufrx->len);
      srv->bufrx = NULL;
      pbuf_free(bufrx);   // free received pbuf
      // tcp_vxi_send(tpcb, srv); // send back received data
    }
    else{
      struct pbuf *ptr;
      ptr = srv->bufrx; // chain pbufs to the end of what we recv'ed previously 
      pbuf_chain(ptr,bufrx);
    }
    ret_err = ERR_OK;
  }
  else if(srv->state == VXI_CLOSING){
    // odd case, remote side closing twice, trash data
    tcp_recved(tpcb, bufrx->tot_len);
    srv->bufrx = NULL;
    pbuf_free(bufrx);
    ret_err = ERR_OK;
  }
  else{
    // unkown srv->state, trash data 
    tcp_recved(tpcb, bufrx->tot_len);
    srv->bufrx = NULL;
    pbuf_free(bufrx);
    ret_err = ERR_OK;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_err callback function (called
  *         when a fatal tcp_connection error occurs.
  * @param  arg: pointer on argument parameter
  * @param  err: not used
  * @retval None
  */
static void tcp_vxi_error(void *arg, err_t err)
{
  struct tcp_vxiserver_struct *srv;
  LWIP_UNUSED_ARG(err);
  scpi_server = NULL;
  srv = (struct tcp_vxiserver_struct *)arg;
  if (srv != NULL){
    mem_free(srv);  //  free srv structure
  }
}

/**
  * @brief  This function implements the tcp_poll LwIP callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
  * @retval err_t: error code
  */
static err_t tcp_vxi_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct tcp_vxiserver_struct *srv;
  srv = (struct tcp_vxiserver_struct *)arg;
  if (srv != NULL){
    if (srv->buftx != NULL){ // @NOTE was bufrx
      // there is a remaining pbuf (chain) , try to send data
      tcp_sent(tpcb, tcp_vxi_sent);
      tcp_vxi_send(tpcb, srv);
    }
    else{
      // no remaining pbuf (chain) 
      if(srv->state == VXI_CLOSING){
        tcp_vxi_connection_close(tpcb, srv);
      }
    }
    ret_err = ERR_OK;
  }
  else{
    tcp_abort(tpcb);     // nothing to be done
    ret_err = ERR_ABRT;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data)
  * @param  None
  * @retval None
  */
static err_t tcp_vxi_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct tcp_vxiserver_struct *srv;
  LWIP_UNUSED_ARG(len);
  srv = (struct tcp_vxiserver_struct *)arg;
  srv->retries = 0;
  if(srv->buftx != NULL){ // @NOTE was bufrx
    // still got pbufs to send
    tcp_sent(tpcb, tcp_vxi_sent);
    tcp_vxi_send(tpcb, srv);
  }
  else{
    // if no more data to send and client closed connection*/
    if(srv->state == VXI_CLOSING)
      tcp_vxi_connection_close(tpcb, srv);
  }
  return ERR_OK;
}


/**
  * @brief  This function is used to send data for tcp connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
static void tcp_vxi_send(struct tcp_pcb *tpcb, struct tcp_vxiserver_struct *srv)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;
  // @note was bufrx
  while ((wr_err == ERR_OK) &&
         (srv->buftx != NULL) &&
         (srv->buftx->len <= tcp_sndbuf(tpcb)))
  {
    ptr = srv->buftx;  // get pointer on pbuf from srv structure
    // enqueue data for transmission
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
    if (wr_err == ERR_OK){
      u16_t plen;
      u8_t freed;
      plen = ptr->len;
      // continue with next pbuf in chain (if any)
      srv->buftx = ptr->next;
      if(srv->buftx != NULL){
        pbuf_ref(srv->buftx);  // increment reference count for srv->p
      }
      do{ // chop first pbuf from chain
        freed = pbuf_free(ptr); // try hard to free pbuf
      }while(freed == 0);
     tcp_recved(tpcb, plen);    // we can read more data now
   }
   else if(wr_err == ERR_MEM){
      // we are low on memory, try later / harder, defer to poll
     srv->buftx = ptr;
   }
   else
   {
     // other problem ??
   }
  }
}

/**
  * @brief  This functions clossrv the tcp connection
  * @param  tcp_pcb: pointer on the tcp connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
static void tcp_vxi_connection_close(struct tcp_pcb *tpcb, struct tcp_vxiserver_struct *srv)
{
  // remove all callbacks
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);
  if (srv != NULL){
    mem_free(srv);   // delete srv structure
  }
  tcp_close(tpcb);
}








size_t SCPI_Write_TCP(scpi_t * context, const char * data, size_t len){
	(void) context;
	struct pbuf *msg;
	// allocate structure srv to maintain tcp connection informations

	/*
	msg = (struct pbuf *)mem_malloc(sizeof(struct pbuf) + len*sizeof(char));
	memcpy(msg->payload, data, len);
	msg->len = len;
	scpi_server->buftx = msg;

	if(scpi_server->buftx == NULL){
		scpi_server->buftx = msg;
	} else {
		struct pbuf *ptr;
		ptr = scpi_server->buftx; // chain pbufs to the end of what we recv'ed previously
		pbuf_chain(ptr, msg);
		// pbuf_chain(scpi_server->buftx, msg);
	}
	tcp_vxi_send(scpi_server->pcb, scpi_server);
	*/
	return fwrite(data, 1, len, stdout);
}

int SCPI_Error_TCP(scpi_t * context, int_fast16_t err){
	return 0;
}

scpi_result_t SCPI_Control_TCP(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val){
	(void) context;
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset_TCP(scpi_t * context){
	// reset ADCs etc.
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Flush_TCP(scpi_t * context){
	(void) context;
	return SCPI_RES_OK;
}

#endif // LWIP_TCP
