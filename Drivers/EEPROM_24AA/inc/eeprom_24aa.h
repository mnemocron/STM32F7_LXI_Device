/*******************************************************************************
 * @file        eeprom_24aa.h
 * @brief       C library to access the 24AA** family of EEPROM chips with
 *              unique internal EUI48 (MAC address)
 * @version
 * @author      Simon Burkhardt
 * @date        2021-10-13
 * @copyright   (c) 2020 eta systems GmbH
*******************************************************************************/

#ifndef _EEPROM_24AA_H
#define	_EEPROM_24AA_H

#define EEPROM24AA_ADDRESS 0x60


void EEPROM_GetEUI48(uint8_t adr, uint8_t* dst);

#endif	/* _EEPROM_24AA_H */
