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

#include "tcp_server_scpi_raw.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "main.h"
#include "scpi/scpi.h"
#include "scpi-def.h"

#if LWIP_TCP

extern scpi_t scpi_context_vxi;

static struct tcp_pcb *tcp_scpirawserver_pcb;
extern struct tcp_scpirawserver_struct *scpi_server;
extern struct tcp_pcb *scpi_tpcb;

// ECHO protocol states
enum tcp_scpirawserver_states
{
  scpiraw_NONE = 0,
  scpiraw_ACCEPTED,
  scpiraw_RECEIVED,
  scpiraw_CLOSING
};

// structure for maintaing connection infos to be passed as argument to LwIP callbacks
struct tcp_scpirawserver_struct
{
  u8_t state;             // current connection state
  u8_t retries;
  struct tcp_pcb *pcb;    // pointer on the current tcp_pcb
};

static err_t tcp_scpiraw_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_scpiraw_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *bufrx, err_t err);
static void tcp_scpiraw_error(void *arg, err_t err);
static err_t tcp_scpiraw_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_scpiraw_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_scpiraw_send(struct tcp_pcb *tpcb, struct tcp_scpirawserver_struct *srv);
static void tcp_scpiraw_connection_close(struct tcp_pcb *tpcb, struct tcp_scpirawserver_struct *srv);

/**
  * @brief  Initializ the tcp echo server
  * @param  None
  * @retval None
  */
void tcp_server_scpiraw_init(void)
{
  tcp_scpirawserver_pcb = tcp_new();  // create new tcp pcb
  if (tcp_scpirawserver_pcb != NULL){
    err_t err;
    // bind _pcb to protocol specific port
    err = tcp_bind(tcp_scpirawserver_pcb, IP_ADDR_ANY, TCP_PORT_SCPI_RAW);
    if (err == ERR_OK){
      // start tcp listening for _pcb
      tcp_scpirawserver_pcb = tcp_listen(tcp_scpirawserver_pcb);
      // initialize LwIP tcp_accept callback function
      tcp_accept(tcp_scpirawserver_pcb, tcp_scpiraw_accept);
    }
    else{
      // deallocate the pcb
      memp_free(MEMP_TCP_PCB, tcp_scpirawserver_pcb);
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
static err_t tcp_scpiraw_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  err_t ret_err;
  struct tcp_scpirawserver_struct *srv;
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);
  // set priority for the newly accepted tcp connection newpcb
  tcp_setprio(newpcb, TCP_PRIO_MIN);
  // allocate structure srv to maintain tcp connection informations
  srv = (struct tcp_scpirawserver_struct *)mem_malloc(sizeof(struct tcp_scpirawserver_struct));
  scpi_server = srv;
  scpi_tpcb = newpcb;
  if (srv != NULL){
    srv->state = scpiraw_ACCEPTED;
    srv->pcb = newpcb;
    srv->retries = 0;
    tcp_arg(newpcb, srv);              // pass newly allocated srv structure as argument to newpcb
    tcp_recv(newpcb, tcp_scpiraw_recv);    // initialize lwip tcp_recv callback function for newpcb
    tcp_err(newpcb, tcp_scpiraw_error);    // initialize lwip tcp_err callback function for newpcb
    tcp_poll(newpcb, tcp_scpiraw_poll, 0); // initialize lwip tcp_poll callback function for newpcb
    ret_err = ERR_OK;
  }
  else{
    tcp_scpiraw_connection_close(newpcb, srv);
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
static err_t tcp_scpiraw_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *bufrx, err_t err)
{
  struct tcp_scpirawserver_struct *srv;
  err_t ret_err;
  LWIP_ASSERT("arg != NULL",arg != NULL);
  srv = (struct tcp_scpirawserver_struct *)arg;
  // if we receive an empty tcp frame from client => close connection
  if (bufrx == NULL){
    srv->state = scpiraw_CLOSING; // remote host closed connection
       tcp_scpiraw_connection_close(tpcb, srv); // we're done sending
       ret_err = ERR_OK;
  }
  // else : a non empty frame was received from client but for some reason err != ERR_OK
  else if(err != ERR_OK){
    if (bufrx != NULL){
      pbuf_free(bufrx);   // free received pbuf
    }
    ret_err = err;
  }
  else if(srv->state == scpiraw_ACCEPTED){
    srv->state = scpiraw_RECEIVED; // first data chunk in bufrx->payload
    tcp_sent(tpcb, tcp_scpiraw_sent); // initialize LwIP tcp_sent callback function
    SCPI_Input(&scpi_context_vxi, bufrx->payload, bufrx->len);
    pbuf_free(bufrx);   // free received pbuf
    ret_err = ERR_OK;
  }
  else if (srv->state == scpiraw_RECEIVED){
    // more data received from client and previous data has been already sent*/
      SCPI_Input(&scpi_context_vxi, bufrx->payload, bufrx->len);
      pbuf_free(bufrx);   // free received pbuf
      ret_err = ERR_OK;
  }
  else if(srv->state == scpiraw_CLOSING){
    // odd case, remote side closing twice, trash data
    tcp_recved(tpcb, bufrx->tot_len);
    pbuf_free(bufrx);
    ret_err = ERR_OK;
  }
  else{
    // unkown srv->state, trash data 
    tcp_recved(tpcb, bufrx->tot_len);
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
static void tcp_scpiraw_error(void *arg, err_t err)
{
  struct tcp_scpirawserver_struct *srv;
  LWIP_UNUSED_ARG(err);
  scpi_server = NULL;
  scpi_tpcb = NULL;
  srv = (struct tcp_scpirawserver_struct *)arg;
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
static err_t tcp_scpiraw_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct tcp_scpirawserver_struct *srv;
  srv = (struct tcp_scpirawserver_struct *)arg;
  if (srv != NULL){
    if(srv->state == scpiraw_CLOSING){
    tcp_scpiraw_connection_close(tpcb, srv);
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
static err_t tcp_scpiraw_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct tcp_scpirawserver_struct *srv;
  LWIP_UNUSED_ARG(len);
  srv = (struct tcp_scpirawserver_struct *)arg;
  srv->retries = 0;
  //if(srv->buftx != NULL){ // @NOTE was bufrx
    // still got pbufs to send
    //tcp_sent(tpcb, tcp_scpiraw_sent);
    //tcp_scpiraw_send(tpcb, srv);
  //} else{
    // if no more data to send and client closed connection*/
    if(srv->state == scpiraw_CLOSING)
      tcp_scpiraw_connection_close(tpcb, srv);
  //}
  return ERR_OK;
}

/**
  * @brief  This functions clossrv the tcp connection
  * @param  tcp_pcb: pointer on the tcp connection
  * @param  srv: pointer on echo_state structure
  * @retval None
  */
static void tcp_scpiraw_connection_close(struct tcp_pcb *tpcb, struct tcp_scpirawserver_struct *srv)
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
  scpi_server = NULL;
  scpi_tpcb = NULL;
}

#endif // LWIP_TCP
