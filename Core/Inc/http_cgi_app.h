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

/* Private macro -------------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/
void myCGIinit(void);
void mySSIinit(void);

uint16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen);
const char* LedCGIhandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);












