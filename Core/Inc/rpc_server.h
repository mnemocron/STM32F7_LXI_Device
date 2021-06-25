/*
 * rcp_server.h
 *
 *  Created on: Jun 23, 2021
 *      Author: simon
 */

#ifndef _RCP_SERVER_H_
#define _RCP_SERVER_H_

#include <stdint.h>

void rpc_server_init(void);
void rpc_echo_init(void);

void BonkTask(void *argument);

#endif /* _RCP_SERVER_H_ */
