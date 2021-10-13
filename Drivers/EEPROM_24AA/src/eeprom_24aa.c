/*******************************************************************************
 * @file        eeprom_24aa.c
 * @brief       C library to access the 24AA** family of EEPROM chips with
 *              unique internal EUI48 (MAC address)
 * @version
 * @author      Simon Burkhardt
 * @date        2021-10-13
 * @copyright   (c) 2020 eta systems GmbH
*******************************************************************************/

#include "main.h"
#include "eeprom_24aa.h"
#include "stm32f7xx_hal_i2c.h"

extern I2C_HandleTypeDef hi2c1;

/*
 * The 6-byte EUI-48™ node address value of the
 * 24AAXXXE48 is stored in array locations 0xFA through
 * 0xFF, as shown in Figure 9-2. The first three bytes are
 * the Organizationally Unique Identifier (OUI) assigned
 * to Microchip by the IEEE Registration Authority. The
 * remaining three bytes are the Extension Identifier, and
 * are generated by Microchip to ensure a globally
 * unique, 48-bit value.
 */
void EEPROM_GetEUI48(uint8_t adr, uint8_t* dst){
  HAL_I2C_Mem_Read(&hi2c1, adr, 0xFA, sizeof(uint8_t), adr, 6, 10);
}
