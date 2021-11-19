/*
 * http_cgi_app.c
 *
 *  Created on: Mar 24, 2021
 *      Author: simon
 */

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "lwip/apps/httpd.h"
#include <Stdbool.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define STRLEN_CGI_SSI_TEMP (64 + 32)

/* Private macro -------------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/
void webCGIinit(void);
void webSSIinit(void);

uint16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen);
const char* settings_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);












