/*******************************************************************************
  * @file    rpc_server.c
  * @brief   contains an implementation of the UDP/RPC/Portmap/VXI-11 stack
  *          that is necessary for the VXI-11 autodiscovery protocol
  * @version 1.0
  * @author  Simon Burkhardt
  * @date    2021-06-25
  * @see     https://datatracker.ietf.org/doc/html/rfc8167
  * @see     http://www.ultimaserial.com/avr_lwip_udp.html
  * @note    use Wireshark to debug packets
********************************************************************************
*/

/** Example of a correct RPC call
 * [00 - 03] 60 d5 9b e8   <-- XID from Caller
 * [04 - 07] 00 00 00 00   <-- message type (0) = Call
 * [08 - 11] 00 00 00 02   <-- RPC Version (2)
 * [12 - 15] 00 01 86 a0   <-- Program: Portmap (100000)
 * [16 - 19] 00 00 00 02   <-- Program Version (2)
 * [20 - 23] 00 00 00 03   <-- Procedure (3) = Get Port
 * [24 - 27] 00 00 00 00   <-- Credentials: Author
 * [28 - 31] 00 00 00 00   <-- Credentials: Length (0)
 * [32 - 35] 00 00 00 00   <-- Verifier: Author
 * [36 - 39] 00 00 00 00   <-- Verifier: Length (0)
 * ---
 * [00 - 03] 00 06 07 af   <-- Program: VXI-11 Core (395183)
 * [04 - 07] 00 00 00 01   <-- Program Version (1)
 * [07 - 11] 00 00 00 06   <-- Protocol (6) = TCP
 * [12 - 15] 00 00 00 04   <-- Port (4)
 * [16 - 20] 00 00 00 00   <-- Data
 * */

/** Example of a correct RPC Reply (RIGOL)
 * [00 - 03] 60 d5 9b e8   <-- XID from Caller
 * [04 - 07] 00 00 00 01   <-- message type (1) = Reply
 * [08 - 11] 00 00 00 00   <-- Reply State is accepted (0)
 * [12 - 15] 00 00 00 00   <-- Verifier: Author
 * [16 - 19] 00 00 00 00   <-- Verifier: Length (0)
 * [20 - 23] 00 00 00 00   <-- Accept State: (0) = RPC successful
 * ---
 * [24 - 27] 00 00 02 6a   <-- Portmap GETPORT Reply Port (618)
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lwip/tcpip.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/udp.h"
#include "lwip/inet.h"

#include "lwip/api.h"
#include "queue.h"
#include "rpc_server.h"

#define RPC_THREAD_PRIO osPriorityNormal // (tskIDLE_PRIORITY + 2)

/**
 * @brief UDP recieve callback function
 * @note  ip4_current_src_addr() only works within this udp_recv() function
 *        pcb->remote_ip and pcb->local_ip are both 0.0.0.0 (empty)
 * @see   https://github.com/lxi-tools/liblxi/blob/master/src/vxi11.c#L57
 */
udp_recv_fn udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
	if (p != NULL) {
		printf("[LXI discovery request received]\n");
		// build a reply package
		// rfc8167 - The XID of a Reply always matches that of the initiating Call.
		char msg[28];
		for(int i=0; i<4; i++){
			msg[i] = ((char*) p->payload)[i];
		}
		pbuf_free(p);
		for(int i=4; i<28; i++){
			msg[i] = 0;
		}
		/** @todo what is the correct reply? should this be 618 as by Rigol? */
		msg[7] = 1;  // message type = Reply
		msg[26] = 0x13; // Port 5025
		msg[27] = 0xa1;

		struct pbuf *p;
		p = pbuf_alloc(PBUF_TRANSPORT, 28*sizeof(char), PBUF_RAM);
		memcpy(p->payload, msg, sizeof(msg));
		udp_sendto(pcb, p, ip4_current_src_addr(), port); // reply to source IP + port
		pbuf_free(p); //De-allocate packet buffer
	}
}

void rpc_server_thread( void *arg ) {
	struct udp_pcb *ptel_pcb;

	(void) arg;

	ptel_pcb = udp_new();
	udp_bind(ptel_pcb, IP_ADDR_ANY, 111);
	udp_recv(ptel_pcb, udp_echo_recv, NULL);

	while (1) {
		vTaskDelay(200); //some delay!
	}
}

void rpc_server_init(void) {
    TaskHandle_t xHandle = NULL;
    BaseType_t xReturned;
    xReturned = xTaskCreate(rpc_server_thread, "RPC", DEFAULT_THREAD_STACKSIZE, NULL, RPC_THREAD_PRIO, &xHandle);
    LWIP_ASSERT("rpc_server_init failed", xReturned == pdPASS);
}

