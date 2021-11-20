/*******************************************************************************
 * @file        ntp_epochtime.c
 * @brief       C library to convert NTP timestamps to other date/time formats
 * @version
 * @author      Simon Burkhardt
 * @date        2021-11-20
 *
 * @see         https://github.com/arduino-libraries/NTPClient
 * @copyright   The MIT License (MIT)
 *              Copyright (c) 2015 by Fabrice Weinberg
 *******************************************************************************/

#include "ntp_epochtime.h"


uint32_t NTP_GetTimestamp_UNIX(uint32_t ntp){
	return ntp - SEVENZYYEARS;
}

uint32_t NTP_GetDayOfWeek(uint32_t unix){
  return (((unix / 86400L) + 4 ) % 7); //0 is Sunday
}
uint32_t NTP_GetHours(uint32_t unix){
  return ((unix % 86400L) / 3600);
}
uint32_t NTP_GetMinutes(uint32_t unix){
  return ((unix % 3600) / 60);
}
uint32_t NTP_GetSeconds(uint32_t unix){
  return (unix % 60);
}



