/*-
 * BSD 2-Clause License
 *
 * Copyright (c) 2012-2018, Jan Breuer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
	
/*******************************************************************************
  * @file    scpi-def.c
  * @brief   contains user settings and definitions for SCPI command implementation
  * @details This file implements constants, math and hardware specific 
             functionalities
  * @version 1.0
  * @author  Simon Burkhardt
  * @date    2020-07-13
  * @copyright
  * @see https://github.com/j123b567/scpi-parser/blob/master/examples/common/scpi-def.c
********************************************************************************
*/
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scpi/scpi.h"
#include "scpi-def.h"

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// #define ENABLE_SCPI_DEBUG_PRINTF

/* USER CODE END PD */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

extern uint32_t dacValue;
/* USER CODE END EV */


/**
 * @brief  save and set a desired output voltage
 */
static scpi_result_t scpi_SetVoltage(scpi_t * context) {
	double param1;
	/* read first parameter if present */
	if (!SCPI_ParamDouble(context, &param1, TRUE)) {
		return SCPI_RES_ERR;
	}
	/* input boundry check */
	if ((param1 > 3.3f) || (param1 < 0.0f)) {
		return SCPI_RES_ERR;
		// else: ignore and don't change the output
	}
	dacValue = (uint32_t)( param1 * (4096.0f / 3.3f));
	return SCPI_RES_OK;
}

static scpi_result_t scpi_SetVoltageQ(scpi_t * context) {
	double param1 = (double) ((double)dacValue / 4096.0f * 3.3f);
	// SCPI_ResultDouble(context, param1); // is this the way to do it ??
	//printf("%.4f\r\n", param1);  // 100V / 20 Bit = 100uV resolution --> .4f

	char data[16];
	size_t len = sprintf(data, "%.4f\r\n", param1);
	context->interface->write(context, data, len);

	return SCPI_RES_OK;
}

/******************************************************************************/
/*           SCPI Command Callback Handlers                                   */ 
/******************************************************************************/


/**
 * Reimplement IEEE488.2 *TST?
 *
 * Result should be 0 if everything is ok
 * Result should be 1 if something goes wrong
 *
 * Return SCPI_RES_OK
 */
static scpi_result_t My_CoreTstQ(scpi_t * context) {

    SCPI_ResultInt32(context, 0);

    return SCPI_RES_OK;
}

const scpi_command_t scpi_commands[] = {
    /* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
    { .pattern = "*CLS", .callback = SCPI_CoreCls,},
    { .pattern = "*ESE", .callback = SCPI_CoreEse,},
    { .pattern = "*ESE?", .callback = SCPI_CoreEseQ,},
    { .pattern = "*ESR?", .callback = SCPI_CoreEsrQ,},
    { .pattern = "*IDN?", .callback = SCPI_CoreIdnQ,},
    { .pattern = "*OPC", .callback = SCPI_CoreOpc,},
    { .pattern = "*OPC?", .callback = SCPI_CoreOpcQ,},
    { .pattern = "*RST", .callback = SCPI_CoreRst,},
    { .pattern = "*SRE", .callback = SCPI_CoreSre,},
    { .pattern = "*SRE?", .callback = SCPI_CoreSreQ,},
    { .pattern = "*STB?", .callback = SCPI_CoreStbQ,},
    { .pattern = "*TST?", .callback = My_CoreTstQ,},
    { .pattern = "*WAI", .callback = SCPI_CoreWai,},

    /* Required SCPI commands (SCPI std V1999.0 4.2.1) */
    {.pattern = "SYSTem:ERRor[:NEXT]?", .callback = SCPI_SystemErrorNextQ,},
    {.pattern = "SYSTem:ERRor:COUNt?", .callback = SCPI_SystemErrorCountQ,},
    {.pattern = "SYSTem:VERSion?", .callback = SCPI_SystemVersionQ,},

    /* {.pattern = "STATus:PRESet", .callback = SCPI_StatusPreset,}, */

    /* DMM */
    //{.pattern = "MEASure:VOLTage[:DC]?", .callback = scpi_etaCT_MeasureVoltageQ,},
    //{.pattern = "MEASure:CURRent[:DC]?", .callback = scpi_etaCT_MeasureCurrentQ,},
    //{.pattern = "CURRent:RANGe",         .callback = scpi_etaCT_RangeCurrent,},
    /* {.pattern = "MEASure:RESistance?", .callback = SCPI_StubQ,}, */
		
		/* SOURCE */
    {.pattern = "SOURce:VOLTage[:LEVel]", .callback = scpi_SetVoltage,},
    {.pattern = "SOURce:VOLTage[:LEVel]?", .callback = scpi_SetVoltageQ,},
		
    SCPI_CMD_LIST_END
};

scpi_interface_t scpi_interface = {
    .error = SCPI_Error,
    .write = SCPI_Write,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
};

scpi_interface_t scpi_interface_tcp = {
    .error = SCPI_Error_TCP,
    .write = SCPI_Write_TCP,
    .control = SCPI_Control_TCP,
    .flush = SCPI_Flush_TCP,
    .reset = SCPI_Reset_TCP,
};

char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];
scpi_t scpi_context;

char scpi_input_buffer_tcp[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data_tcp[SCPI_ERROR_QUEUE_SIZE];
scpi_t scpi_context_tcp;

size_t SCPI_Write(scpi_t * context, const char * data, size_t len){
	(void) context;
	return fwrite(data, 1, len, stdout);
}

int SCPI_Error(scpi_t * context, int_fast16_t err){
	return 0;
}

scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val){
	(void) context;
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t * context){
	// reset ADCs etc.
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Flush(scpi_t * context){
	(void) context;
	return SCPI_RES_OK;
}


