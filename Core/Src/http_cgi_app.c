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
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* Defines */
#define N_SSI_TAGS 12  // total number of different SSI tags present on the system (all tags from different pages)

/* user variables */
tCGI theCGItable[1];
bool webConf_switch_LED1 = false; // this variable will indicate if the LD3 LED on the board is ON or not
bool webConf_switch_Function42 = false; // this variable will indicate if our LD2 LED on the board is ON or not
/* define some pre-registered tags
 if one of those tags appears in a .shtml (or /lxi/identification/index.xml) file,
 the ssi_handler is called with iIndex pointing to the location of the tag in this array
 */
const char *SSI_tagList[N_SSI_TAGS] = { "lxi0", "lxi1", "lxi2", "lxi3", "lxi4",
		"lxi5", "lxi6", "lxi7", "lxi8", "lxi9", "btn0", "btn1" };
const tCGI settingsCGI = { "/settings.cgi", settings_CGI_Handler };

/* external variables */
extern uint32_t deviceStatus_IPv4Addr;
extern uint32_t deviceStatus_IPv4Mask;
extern uint32_t deviceStatus_IPv4Gate;
extern bool deviceStatus_DHCPenabled;

extern uint8_t MACAddrUser[6];

extern char _version_string[32];  // Firmware Version

const char* settings_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]) {
	// remember if the parameters have been sent in the URL
	// http://<host>/settings.cgi?p=1&p=2   both switches on
	// http://<host>/settings.cgi?          both switches off
	// http://<host>/settings.cgi?p=1       switch '1' is on, switch '2' is off
	bool param_p1_seen = false;
	bool param_p2_seen = false;
	for (uint32_t i = 0; i < iNumParams; i++) {
		if (strcmp(pcParam[i], "p") == 0) {
			if (strcmp(pcValue[i], "1") == 0) {
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
				webConf_switch_LED1 = true;
				param_p1_seen = true;
			} else if (strcmp(pcValue[i], "2") == 0) {
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				webConf_switch_Function42 = true;
				param_p2_seen = true;
			}
		}
	}
	// if the parameter is not present in the URL --> this parameter is 'off'
	if (!param_p1_seen) {
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
		webConf_switch_LED1 = false;
	}
	if (!param_p2_seen) {
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		webConf_switch_Function42 = false;
	}
	// the extension .shtml for SSI to work
	return "/index.shtml";
}

void webCGIinit(void) {
	//add LED control CGI to the table
	theCGItable[0] = settingsCGI;
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
		) {
// uint16_t defaultSSIHandler(int iIndex, char *pcInsert, int iInsertLen) {
	if (iIndex == 0) {
		if (webConf_switch_LED1 == false) {
			char myStr1[] =
					"<input id=\"ck1\" value=\"1\" name=\"p\" type=\"checkbox\">";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		} else if (webConf_switch_LED1 == true) {
			// since the LD3 red LED on the board is ON we make its checkbox checked!
			char myStr1[] =
					"<input id=\"ck1\" value=\"1\" name=\"p\" type=\"checkbox\" checked>";
			strcpy(pcInsert, myStr1);
			return strlen(myStr1);
		}
	} else if (iIndex == 1) {
		if (webConf_switch_Function42 == false) {
			char myStr2[] =
					"<input id=\"ck2\" value=\"2\" name=\"p\" type=\"checkbox\">";
			strcpy(pcInsert, myStr2);
			return strlen(myStr2);
		} else if (webConf_switch_Function42 == true) {
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
		) {
// uint16_t lxiSSIHandler(int iIndex, char *pcInsert, int iInsertLen) {
	char str_ipv4_addr[20];
	snprintf(str_ipv4_addr, (4 * 3 + 3 + 1), "%d.%d.%d.%d",
			(int) (deviceStatus_IPv4Addr & 0xff),
			(int) ((deviceStatus_IPv4Addr >> 8) & 0xff),
			(int) ((deviceStatus_IPv4Addr >> 16) & 0xff),
			(int) (deviceStatus_IPv4Addr >> 24));
	char str_ipv4_gate[20];
		snprintf(str_ipv4_gate, (4 * 3 + 3 + 1), "%d.%d.%d.%d",
			(int) (deviceStatus_IPv4Gate & 0xff),
			(int) ((deviceStatus_IPv4Gate >> 8) & 0xff),
			(int) ((deviceStatus_IPv4Gate >> 16) & 0xff),
			(int) (deviceStatus_IPv4Gate >> 24));
	char str_ipv4_mask[20];
	snprintf(str_ipv4_mask, (4 * 3 + 3 + 1), "%d.%d.%d.%d",
			(int) (deviceStatus_IPv4Mask & 0xff),
			(int) ((deviceStatus_IPv4Mask >> 8) & 0xff),
			(int) ((deviceStatus_IPv4Mask >> 16) & 0xff),
			(int) (deviceStatus_IPv4Mask >> 24));


	char str_tmp[STRLEN_CGI_SSI_TEMP];

	if (iIndex == 0) {
		// Serial Number
		snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, _SERIAL_NUMBER);
		strcpy(pcInsert, str_tmp);
		return strlen(str_tmp);
	} else if (iIndex == 1) {
		// Firmware Version
		// @todo lxi/identification firmware version contains build date
		strcpy(pcInsert, _version_string);
		return strlen(_version_string);
	} else if (iIndex == 2) {
		// IP Address
		strcpy(pcInsert, str_ipv4_addr);
		return strlen(str_ipv4_addr);
	} else if (iIndex == 3) {
		// INSTR::IP_ADDRESS
		snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "TCPIP0::%s", str_ipv4_addr);
		strcpy(pcInsert, str_tmp);
		return strlen(str_tmp);
	} else if (iIndex == 4) {
		// Subnet Mask
		snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "%s", str_ipv4_mask);
		strcpy(pcInsert, str_tmp);
		return strlen(str_tmp);
	} else if (iIndex == 5) {
		// Physical MAC Address
		snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "%02X:%02X:%02X:%02X:%02X:%02X\n", MACAddrUser[0],
				MACAddrUser[1], MACAddrUser[2], MACAddrUser[3],
				MACAddrUser[4], MACAddrUser[5]);
		strcpy(pcInsert, str_tmp);
		return strlen(str_tmp);
	} else if (iIndex == 6) {
		// Gateway Address
		snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "%s", str_ipv4_gate);
		strcpy(pcInsert, str_tmp);
		return strlen(str_tmp);
	} else if (iIndex == 7) {
		// DHCP enabled
		if(deviceStatus_DHCPenabled){
			snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "true");
		} else {
			snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "false");
		}
		strcpy(pcInsert, str_tmp);
		return strlen(str_tmp);
	} else if (iIndex == 8) {
		// Auto IP enabled
		// @todo Auto IP status in lxi/identification
		snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "true");
		strcpy(pcInsert, str_tmp);
		return strlen(str_tmp);
	} else if (iIndex == 9) {
		// Instrument Address String
		snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "INSTR::%s", str_ipv4_addr);
		strcpy(pcInsert, str_tmp);
		return strlen(str_tmp);
	} else if (iIndex == 10) {
		// btn0 from web interface
		if (webConf_switch_LED1 == false) {
			snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "<input id=\"ck1\" value=\"1\" name=\"p\" type=\"checkbox\">");
			strcpy(pcInsert, str_tmp);
			return strlen(str_tmp);
		} else if (webConf_switch_LED1 == true) {
			snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "<input id=\"ck1\" value=\"1\" name=\"p\" type=\"checkbox\" checked>");
			strcpy(pcInsert, str_tmp);
			return strlen(str_tmp);
		}
	} else if (iIndex == 11) {
		if (webConf_switch_Function42 == false) {
			snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "<input id=\"ck2\" value=\"2\" name=\"p\" type=\"checkbox\">");
			strcpy(pcInsert, str_tmp);
			return strlen(str_tmp);
		} else if (webConf_switch_Function42 == true) {
			snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "<input id=\"ck2\" value=\"2\" name=\"p\" type=\"checkbox\" checked>");
			strcpy(pcInsert, str_tmp);
			return strlen(str_tmp);
		}
	} else {
		// empty
		snprintf(str_tmp, STRLEN_CGI_SSI_TEMP, "-");
		strcpy(pcInsert, str_tmp);
		return strlen(str_tmp);
	}
	return 0;
}

void webSSIinit(void) {
	// ./index.html page
	//http_set_ssi_handler(defaultSSIHandler, (char const**) theSSItags, numSSItags);

	// ./lxi/identification page
	http_set_ssi_handler(lxiSSIHandler, (char const**) SSI_tagList, N_SSI_TAGS);

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	webConf_switch_Function42 = false;
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	webConf_switch_LED1 = false;
}

