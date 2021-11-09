/*
 * http_cgi_app.c
 *
 *  Created on: Mar 24, 2021
 *      Author: simon
 *
 *		https://github.com/particle-iot/lwip/blob/master/contrib/examples/httpd/ssi_example/ssi_example.c
 *      https://github.com/JoeMerten/Stm32-Tools-Evaluation/tree/master/STM32Cube_FW_F4_V1.9.0/Projects/STM324xG_EVAL/Applications/LwIP/LwIP_HTTP_Server_Raw/Src
 */

/* Includes */
#include "http_cgi_app.h"
#include <String.h>
#include <stdio.h>

/* Defines */
#define numLXItags 12

/* user variables */
tCGI theCGItable[1];
bool LD1ON = false; // this variable will indicate if the LD3 LED on the board is ON or not
bool LD2ON = false; // this variable will indicate if our LD2 LED on the board is ON or not
/* define some pre-registered tags
 if one of those tags appears in a .shtml file,
 the ssi_handler is called with iIndex pointing to the location of the tag in this array
 */
const char *theLXItags[numLXItags] = { "lxi0", "lxi1", "lxi2", "lxi3", "lxi4", "lxi5", "lxi6", "lxi7", "lxi8", "lxi9", "btn0", "btn1" };
const tCGI LedCGI = { "/app.cgi", LedCGIhandler };

/* external variables */
extern uint32_t deviceIPaddr;
extern char _version_string[32];  // Firmware Version


const char* LedCGIhandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	// remember if the parameters have been sent in the URL
	// http://<host>/app.cgi?p=1&p=2   both LEDs on
	// http://<host>/app.cgi?          both LEDs off
	// http://<host>/app.cgi?p=1       only one LED on
	bool param_p1_seen = false;
	bool param_p2_seen = false;
	for (uint32_t i = 0; i < iNumParams; i++) {
		if (strcmp(pcParam[i], "p") == 0)
		{
			if (strcmp(pcValue[i], "1") == 0) {
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
				// LD3 LED (red) on the board is ON!
				LD1ON = true;
				param_p1_seen = true;
			}
			else if (strcmp(pcValue[i], "2") == 0) {
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				// LD2 LED (blue) on the board is ON!
				LD2ON = true;
				param_p2_seen = true;
			}
		}
	}
	// if the parameter is not present in the URL --> turn LED off
	if(!param_p1_seen){
		//turning the LED lights off
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		// we put this variable to false to indicate that the LD2 LED on the board is not ON
		LD2ON = false;
	}
	if(!param_p2_seen){
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
		// we put this variable to false to indicate that the LD* LED on the board is not ON
		LD1ON = false;
	}
	// the extension .shtml for SSI to work
	return "/index.shtml";
}

// function to initialize CGI
void myCGIinit(void) {
	//add LED control CGI to the table
	theCGItable[0] = LedCGI;
	//give the table to the HTTP server
	http_set_cgi_handlers(theCGItable, 1);
}

u16_t defaultSSIHandler(
#if LWIP_HTTPD_SSI_RAW
                             const char* ssi_tag_name,
#else /* LWIP_HTTPD_SSI_RAW */
                             int iIndex,
#endif /* LWIP_HTTPD_SSI_RAW */
                             char *pcInsert, int iInsertLen
#if LWIP_HTTPD_SSI_MULTIPART
                             , u16_t current_tag_part, u16_t *next_tag_part
#endif /* LWIP_HTTPD_SSI_MULTIPART */
#if defined(LWIP_HTTPD_FILE_STATE) && LWIP_HTTPD_FILE_STATE
                             , void *connection_state
#endif /* LWIP_HTTPD_FILE_STATE */
                             )
{
// uint16_t defaultSSIHandler(int iIndex, char *pcInsert, int iInsertLen) {
	if (iIndex == 0) {
		if (LD1ON == false) {
			char myStr1[] = "<input id=\"ck1\" value=\"1\" name=\"p\" type=\"checkbox\">";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		} else if (LD1ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr1[] =
					"<input id=\"ck1\" value=\"1\" name=\"p\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		}
	} else if (iIndex == 1) {
		if (LD2ON == false) {
			char myStr2[] = "<input id=\"ck2\" value=\"2\" name=\"p\" type=\"checkbox\">";
			strcpy(pcInsert, myStr2);
			return strlen(myStr2);
		} else if (LD2ON == true) {
			// since the LD2 blue LED on the board is ON we make its checkbox checked!
			char myStr2[] =
					"<input id=\"ck2\" value=\"2\" name=\"p\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr2);
			return strlen(myStr2);
		}
	}
	return 0;
}

u16_t lxiSSIHandler(
#if LWIP_HTTPD_SSI_RAW
                             const char* ssi_tag_name,
#else /* LWIP_HTTPD_SSI_RAW */
                             int iIndex,
#endif /* LWIP_HTTPD_SSI_RAW */
                             char *pcInsert, int iInsertLen
#if LWIP_HTTPD_SSI_MULTIPART
                             , u16_t current_tag_part, u16_t *next_tag_part
#endif /* LWIP_HTTPD_SSI_MULTIPART */
#if defined(LWIP_HTTPD_FILE_STATE) && LWIP_HTTPD_FILE_STATE
                             , void *connection_state
#endif /* LWIP_HTTPD_FILE_STATE */
                             )
{
// uint16_t lxiSSIHandler(int iIndex, char *pcInsert, int iInsertLen) {
	char ip_str[20];
	snprintf(ip_str, (4*3+3+1), "%d.%d.%d.%d",
			(int)(deviceIPaddr & 0xff),
			(int)((deviceIPaddr >> 8) & 0xff),
			(int)((deviceIPaddr >> 16) & 0xff),
			(int)(deviceIPaddr >> 24));

	if(iIndex == 0){
		// Serial Number
		char myStr[] = _SERIAL_NUMBER;
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 1) {
		// Firmware Version
		strcpy(pcInsert, _version_string);
		return strlen(_version_string);
	} else if(iIndex == 2){
		// IP Address
		strcpy(pcInsert, ip_str);
		return strlen(ip_str);
	} else if(iIndex == 3){
		// INSTR::IP_ADDRESS
		char myStr[64];
		snprintf(myStr, 64, "INSTR::%s", ip_str);
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if (iIndex == 4){
		// Subnet Mask
		/** @todo netmask as global vairable? */
		char myStr[] = "255.255.255.0";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if (iIndex == 5){
		// Physical MAC Address
		/** @todo MAC as global vairable? */
		char myStr[] = "00:00:00:00:00:00";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 6){
		// Gateway Address
		/** @todo gateway as global vairable? */
		char myStr[] = "192.168.1.1";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 7){
		// DHCP enabled
		char myStr[] = "true";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 8){
		// Auto IP enabled
		char myStr[] = "false";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 9){
		// Instrument Address String
		char myStr[64];
		snprintf(myStr, 64, "INSTR::%s", ip_str);
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 10){
		// btn0 from web interface
		if (LD1ON == false) {
			char myStr1[] = "<input id=\"ck1\" value=\"1\" name=\"p\" type=\"checkbox\">";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		} else if (LD1ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr1[] =
					"<input id=\"ck1\" value=\"1\" name=\"p\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		}
	} else if(iIndex == 11){
		// btn1 from web interface
		if (LD2ON == false) {
			char myStr2[] = "<input id=\"ck2\" value=\"2\" name=\"p\" type=\"checkbox\">";
			strcpy(pcInsert, myStr2);
			return strlen(myStr2);
		} else if (LD2ON == true) {
			// since the LD2 blue LED on the board is ON we make its checkbox checked!
			char myStr2[] =
					"<input id=\"ck2\" value=\"2\" name=\"p\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr2);
			return strlen(myStr2);
		}
	} else {
		// empty
		char myStr[] = "-";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	}
	return 0;
}

void mySSIinit(void) {
	// ./index.html page
	//http_set_ssi_handler(defaultSSIHandler, (char const**) theSSItags, numSSItags);

	// ./lxi/identification page
	http_set_ssi_handler(lxiSSIHandler, (char const**) theLXItags, numLXItags);

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	LD2ON = false;
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	LD1ON = false;
}


