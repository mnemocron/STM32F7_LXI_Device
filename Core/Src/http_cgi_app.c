/*
 * http_cgi_app.c
 *
 *  Created on: Mar 24, 2021
 *      Author: simon
 *
 *
 *      https://github.com/JoeMerten/Stm32-Tools-Evaluation/tree/master/STM32Cube_FW_F4_V1.9.0/Projects/STM324xG_EVAL/Applications/LwIP/LwIP_HTTP_Server_Raw/Src
 */

#include "http_cgi_app.h"
#include <String.h>

tCGI theCGItable[1];
bool LD1ON = false; // this variable will indicate if the LD3 LED on the board is ON or not
bool LD2ON = false; // this variable will indicate if our LD2 LED on the board is ON or not

char const *theSSItags[numSSItags] = { "tag1", "tag2" };
char const *theLXItags[numLXItags] = { "lxi0", "lxi1", "lxi2", "lxi3", "lxi4", "lxi5", "lxi6", "lxi7", "lxi8", "lxi9" };

const tCGI LedCGI = { "/app.cgi", LedCGIhandler };

const char* LedCGIhandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	uint32_t i = 0;

	for (i = 0; i < iNumParams; i++) {
		if (strcmp(pcParam[i], "p") == 0)
		{
			if (strcmp(pcValue[i], "1") == 0) {
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
				// LD3 LED (red) on the board is ON!
				LD1ON = true;
			}
			else if (strcmp(pcValue[i], "2") == 0) {
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				// LD2 LED (blue) on the board is ON!
				LD2ON = true;
			}
		}
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

uint16_t defaultSSIHandler(int iIndex, char *pcInsert, int iInsertLen) {
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

uint16_t lxiSSIHandler(int iIndex, char *pcInsert, int iInsertLen) {
	if(iIndex == 0){
		// Serial Number
		char myStr[] = "S/N.12345";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 1) {
		// Firmware Version
		char myStr[] = "V1.0.0";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 2){
		// IP Address
		char myStr[] = "192.168.1.179";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 3){
		// INSTR::IP_ADDRESS
		char myStr[] = "INSTR::192.168.1.179";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if (iIndex == 4){
		// Subnet Mask
		char myStr[] = "255.255.255.0";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if (iIndex == 5){
		// Physical MAC Address
		char myStr[] = "00:00:00:00:00:00";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
	} else if(iIndex == 6){
		// Gateway Address
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
		char myStr[] = "INSTR::192.168.1.179";
		strcpy(pcInsert, myStr);
		return strlen(myStr);
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


