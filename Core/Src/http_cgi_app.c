/*
 * http_cgi_app.c
 *
 *  Created on: Mar 24, 2021
 *      Author: simon
 */

#include "http_cgi_app.h"
#include <String.h>

tCGI theCGItable[1];
bool LD1ON = false; // this variable will indicate if the LD3 LED on the board is ON or not
bool LD2ON = false; // this variable will indicate if our LD2 LED on the board is ON or not



char const *theSSItags[numSSItags] = { "tag1", "tag2" };
const tCGI LedCGI = { "/leds.cgi", LedCGIhandler };

const char* LedCGIhandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	uint32_t i = 0;
	if (iIndex == 0) {
		//turning the LED lights off
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		// we put this variable to false to indicate that the LD2 LED on the board is not ON
		LD2ON = false;
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
		// we put this variable to false to indicate that the LD* LED on the board is not ON
		LD1ON = false;
	}
	for (i = 0; i < iNumParams; i++) {
		if (strcmp(pcParam[i], "led") == 0)
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

// function to initialize CGI [= CGI #6 =]
void myCGIinit(void) {
	//add LED control CGI to the table
	theCGItable[0] = LedCGI;
	//give the table to the HTTP server
	http_set_cgi_handlers(theCGItable, 1);
} // END [= CGI #6 =]

uint16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen) {
	if (iIndex == 0) {
		if (LD1ON == false) {
			char myStr1[] = "<input value=\"1\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		} else if (LD1ON == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr1[] =
					"<input value=\"1\" name=\"led\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		}
	} else if (iIndex == 1) {
		if (LD2ON == false) {
			char myStr2[] = "<input value=\"2\" name=\"led\" type=\"checkbox\">";
			strcpy(pcInsert, myStr2);
			return strlen(myStr2);
		} else if (LD2ON == true) {
			// since the LD2 blue LED on the board is ON we make its checkbox checked!
			char myStr2[] =
					"<input value=\"2\" name=\"led\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr2);
			return strlen(myStr2);
		}
	}
	return 0;
}

void mySSIinit(void) {
	http_set_ssi_handler(mySSIHandler, (char const**) theSSItags, numSSItags);
}


