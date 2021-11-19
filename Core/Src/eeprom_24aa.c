/*******************************************************************************
 * @file        eeprom_24aa.c
 * @brief       C library to access the 24AA** family of EEPROM chips with
 *              unique internal EUI48 (MAC address)
 * @version
 * @author      Simon Burkhardt
 * @date        2021-11-19
 * @copyright   (c) 2021 eta systems GmbH
 *******************************************************************************/

#include "eeprom_24aa.h"
#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief     save one byte to the EEPROM
 * @important write cycle takes some time. Wait for a few ms before you send/request more data to the EEPROM
 * @param adr internal register address
 * @param b   data byte
 * @return    none
 */
void EEPROM_SaveByte(uint8_t adr, uint8_t b) {
	uint8_t a = 0;
	HAL_I2C_Mem_Read(&hi2c1, EEPROM24AA_ADDRESS, adr, sizeof(uint8_t), &a, 1,
			EEPROM24AA_TIMEOUT);
	if (a != b)
		HAL_I2C_Mem_Write(&hi2c1, EEPROM24AA_ADDRESS, adr, sizeof(uint8_t), &b,
				1, EEPROM24AA_TIMEOUT);

#ifdef EEPROM24AA_PRINT_DEBUG
		printf("[mem][W]: %d\n", b);
	#endif
}

/**
 * @brief     read one byte from the EEPROM
 * @important write cycle takes some time. Wait for a few ms if a write was performed prior to this function.
 * @param adr internal register address
 * @return    byte read from EEPROM
 */
uint8_t EEPROM_ReadByte(uint8_t adr) {
	uint8_t b = 0;
	HAL_I2C_Mem_Read(&hi2c1, EEPROM24AA_ADDRESS, adr, sizeof(uint8_t), &b, 1,
			EEPROM24AA_TIMEOUT);

#ifdef EEPROM24AA_PRINT_DEBUG
		printf("[mem][r]: %d\n", b);
	#endif
	return b;
}

/**
 * @brief     save one uint32_t to the EEPROM
 * @important write cycle takes some time. Wait for a few ms before you send/request more data to the EEPROM
 * @param adr internal register address
 * @param ip  one uint32_t type ip address
 * @return    none
 */
void EEPROM_SaveIP(uint8_t adr, uint32_t ip) {
	uint32_t rd = 0;
	HAL_I2C_Mem_Read(&hi2c1, EEPROM24AA_ADDRESS, adr, sizeof(uint8_t),
			(uint8_t*) &rd, 4, EEPROM24AA_TIMEOUT);
	if (rd != ip)
		HAL_I2C_Mem_Write(&hi2c1, EEPROM24AA_ADDRESS, adr, sizeof(uint8_t),
				(uint8_t*) &ip, 4, EEPROM24AA_TIMEOUT);

#ifdef EEPROM24AA_PRINT_DEBUG
	printf("[mem][W]: %d.%d.%d.%d\n", (int)(ip&0xff),(int)((ip>>8)&0xff),(int)((ip>>16)&0xff),(int)(ip>>24));
#endif
}

/**
 * @brief     read one uint32_t to the EEPROM
 * @important write cycle takes some time. Wait for a few ms if a write was performed prior to this function.
 * @param adr internal register address
 * @return    uint32_t ip address type read from EEPROM
 */
uint32_t EEPROM_ReadIP(uint8_t adr) {
	uint32_t ip = 0;
	HAL_I2C_Mem_Read(&hi2c1, EEPROM24AA_ADDRESS, adr, sizeof(uint8_t),
			(uint8_t*) &ip, 4, EEPROM24AA_TIMEOUT);
#ifdef EEPROM24AA_PRINT_DEBUG
	printf("[mem][r]: %d.%d.%d.%d\n", (int)(ip&0xff),(int)((ip>>8)&0xff),(int)((ip>>16)&0xff),(int)(ip>>24));
#endif
	return ip;
}

#ifdef __cplusplus
}
#endif
