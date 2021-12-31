/*******************************************************************************
 * @file        ntp_epochtime.h
 * @brief       C library to convert NTP timestamps to other date/time formats
 * @version
 * @author      Simon Burkhardt
 * @date        2021-11-20
 *
 * @see         https://github.com/arduino-libraries/NTPClient
 * @copyright   The MIT License (MIT)
 *              Copyright (c) 2015 by Fabrice Weinberg
 *******************************************************************************/
#include "main.h"

/* Defines */
#define SEVENZYYEARS 2208988800UL

/* Function Prototypes */
uint32_t NTP_GetTimestamp_UNIX(uint32_t ntp);
uint32_t NTP_GetDayOfWeek(uint32_t unix);
uint32_t NTP_GetHours(uint32_t unix);
uint32_t NTP_GetMinutes(uint32_t unix);
uint32_t NTP_GetSeconds(uint32_t unix);
