/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  *
  * @see https://lwip.fandom.com/wiki/IPv4
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
#include "scpi/scpi.h"          // SCPI Library
#include "scpi-def.h"           // SCPI User Code
#include "eeprom_24aa.h"        // EEPROM macros
#include "lwrb/lwrb.h"          // light weight ring buffer for UART
#include "http_cgi_app.h"       // http User Code
#include "lwip/apps/httpd.h"    // http Library
#include "scpi_server.h"        // TCP/IP server for SCPI input
#include "rpc_server.h"         // LXI discovery service
#include "lwip/apps/sntp.h"     // SNTP API for time keeping
#include "ds3231.h"             // precision RTC on I2C
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ENABLE_DEBUG_PRINTF 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart3;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
/** @note : Stack Size must be large enough to not cause a stack overflow --> HardFault_Handler() */
lwrb_t ringbuffer;
uint8_t uart_rx_buffer_data[128];
char temp[128];  // to pass UART Rx Ringbuffer to SCPI Input
bool flag_uart_cmd_available = false;

/* external API interfaces */
extern scpi_interface_t scpi_interface_vxi;
extern scpi_interface_t scpi_interface_serial;
extern struct netif gnetif;

/* global variables to save new device settings */
uint8_t MACAddrUser[6];
uint32_t newSettings_IPv4Addr = 0;
uint32_t newSettings_IPv4Gate = 0;
uint32_t newSettings_IPv4Mask = 0;
bool newSettings_DHCPenabled = false;

/* global flags */
bool applySettings_DHCP     = false;
bool applySettings_IPv4Addr = false;
bool applySettings_IPv4Mask = false;
bool applySettings_IPv4Gate = false;

/* global device status variables */
uint32_t deviceStatus_IPv4Addr = 0;
uint32_t deviceStatus_IPv4Mask = 0;
uint32_t deviceStatus_IPv4Gate = 0;
bool deviceStatus_DHCPenabled = false;
char _version_string[32];  // Firmware Version

ip4_addr_t ipv4_temp;

void printr(uint8_t reg) {
	printf("Reg 0x%02x = ", reg);
	uint8_t val;
	HAL_I2C_Master_Transmit(_ds3231_ui2c, DS3231_I2C_ADDR << 1, &reg, 1, DS3231_TIMEOUT);
	HAL_StatusTypeDef s = HAL_I2C_Master_Receive(_ds3231_ui2c,
			DS3231_I2C_ADDR << 1, &val, 1, DS3231_TIMEOUT);
	for (uint8_t i = 0; i < 8; i++) {
		printf("%d", (val >> (7 - i)) & 1);
	}
	printf(" With status %d", s);
	printf("\n");
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */
void I2C_ScanAddresses(I2C_HandleTypeDef *hi2c);
void LXI_LCI_Mechanism(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  MACAddrUser[0] = 0x00;
  MACAddrUser[1] = 0x80;
  MACAddrUser[2] = 0xE1;
  MACAddrUser[3] = 0xc0;
  MACAddrUser[4] = 0xff;
  MACAddrUser[5] = 0xee;

  // read EUI48 compatible MAC address from EEPROM before initializing the LwIP stack
  _EEPROM_GetEUI48(EEPROM24AA_ADDRESS, MACAddrUser);

  // I2C_ScanAddresses(&hi2c1);

  /** @todo - test with working module
  DS3231_Init(&hi2c1);
  DS3231_SetFullTime(11, 15, 50);
  DS3231_SetFullDate(19, 11, 5, 2021);
  */

  lwrb_init(&ringbuffer, uart_rx_buffer_data, sizeof(uart_rx_buffer_data));

  // initialize SCPI Interface for UART Connection
  SCPI_Init(&scpi_context_serial,
	     	scpi_commands,
	     	&scpi_interface_serial,
	     	scpi_units_def,
	     	SCPI_IDN1, SCPI_IDN2, SCPI_IDN3, SCPI_IDN4,
	     	(char*)&scpi_input_buffer_serial, SCPI_INPUT_BUFFER_LENGTH,
	     	scpi_error_queue_data_serial, SCPI_ERROR_QUEUE_SIZE);
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);  // must be enabled again
  // __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);  // @Bug : gets stuck in TIM1 interrupt. Priority issue because of systick?

  SCPI_Input(&scpi_context_serial, "*IDN?\n", 6);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  scpi_server_init();
  rpc_server_init();
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00808CD2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|TRIG_OUT_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : TRIG_OUT_Pin */
  GPIO_InitStruct.Pin = TRIG_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(TRIG_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_VBUS_Pin */
  GPIO_InitStruct.Pin = USB_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 * @see    https://github.com/STMicroelectronics/STM32CubeF4/blob/master/Projects/STM32F401RE-Nucleo/Examples/UART/UART_Printf/Src/main.c
 */
PUTCHAR_PROTOTYPE {
	/* Place your implementation of fputc here */
	/* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
#ifdef ENABLE_DEBUG_PRINTF
	HAL_UART_Transmit(&huart3, (uint8_t*) &ch, 1, 1);
#endif
	// while(!(CDC_Transmit_FS((uint8_t*)&ch, 1) == USBD_BUSY));
	return ch;
}

/**
 * @brief callback funtion to set system time from NTP server
 * @param sec timestamp in seconds
 * @param us  additional microseconds
 * @see   sntp.c:139
 * @see   sntp_opts.h:56
 * @todo  implement RTC callback
 */
void SNTP_RTC_callback(uint32_t sec, uint32_t us){
	printf("NTP: %d\n", (int)sec);
}

/**
 * @note LXI Standard v1.5 - 2.4.5 LAN Configuration Initialize (LCI)
 * @note LXI Standard v1.5 - 2.4.5.1 LCI Mechanism ("LAN RESET" menu entry that, when activated, places its network settings in a default state)
 * @todo implement network default settings
 */
void LXI_LCI_Mechanism(void)
{
  deviceStatus_DHCPenabled = true;
  //MX_LWIP_Deinit();
  //MX_LWIP_Reinit_DHCP();
  printf("LCI: Init DHCP On\n");
}

void I2C_ScanAddresses(I2C_HandleTypeDef *hi2c){
    uint8_t error, address;
    uint16_t nDevices;
    nDevices = 0;
    printf("Scanning for available I2C devices...");
    for(address = 0; address < 255; address++ )
    {
        HAL_Delay(10);
        error = HAL_I2C_Master_Transmit(hi2c, address, 0x00, 1, 1);
        //error = HAL_I2C_Master_Receive(hi2c, address, 0x00, 1, 1);
        if (error == HAL_OK)
        {
            printf("I2C device found at address 0x%02X!\n", address);
            nDevices++;
        }
    }
    if (nDevices == 0)
        printf("No I2C devices found\n");
    else
        printf("done\n");
}

void IP_Settings_Apply(bool* flag, uint32_t* ip, uint8_t reg)
{
	if(*flag){
		*flag = false;
		ipv4_temp.addr = *ip;  // store new ip to temp struct
		if(reg == EEPROM24AA_REG_IP){
			EEPROM_SaveIP(EEPROM24AA_REG_IP, *ip);
			netif_set_ipaddr(&gnetif, &ipv4_temp);
			//ipAddressPrinted = false;
		}
		if(reg == EEPROM24AA_REG_GATEWAY){
			EEPROM_SaveIP(EEPROM24AA_REG_GATEWAY, *ip);
			netif_set_gw(&gnetif, &ipv4_temp);
			//ipAddressPrinted = false;
		}
		if(reg == EEPROM24AA_REG_SUBNET){
			EEPROM_SaveIP(EEPROM24AA_REG_SUBNET, *ip);
			netif_set_netmask(&gnetif, &ipv4_temp);
			//ipAddressPrinted = false;
		}
		printf("[set] %d.%d.%d.%d\n", (int)(*ip&0xff),(int)((*ip>>8)&0xff),(int)((*ip>>16)&0xff),(int)(*ip>>24));
	}
}

void DHCP_Settings_Apply(bool* flag, bool* en, uint8_t reg)
{
	if(*flag){
		*flag = false;
		//ipAddressPrinted = false;
		EEPROM_SaveByte(reg, *en);
		if (*en) {
			deviceStatus_DHCPenabled = true;
			dhcp_start(&gnetif);
		} else {
			deviceStatus_DHCPenabled = false;
			dhcp_stop(&gnetif);  // will reset IP + mask + GW

			osDelay(5);
			newSettings_IPv4Addr = EEPROM_ReadIP(EEPROM24AA_REG_IP);
			osDelay(5);
			newSettings_IPv4Mask = EEPROM_ReadIP(EEPROM24AA_REG_SUBNET);
			osDelay(5);
			newSettings_IPv4Gate = EEPROM_ReadIP(EEPROM24AA_REG_GATEWAY);

			applySettings_IPv4Addr = true;
			applySettings_IPv4Gate = true;
			applySettings_IPv4Mask = true;
		}
	}
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN 5 */
	deviceStatus_DHCPenabled = true;
	// if DHCP was previously disabled, use the settings from the EEPROM
	osDelay(5);
	deviceStatus_DHCPenabled = EEPROM_ReadByte(EEPROM24AA_REG_DHCP_EN);
	if(!deviceStatus_DHCPenabled){
		dhcp_stop(&gnetif);
		osDelay(5);
		newSettings_IPv4Addr = EEPROM_ReadIP(EEPROM24AA_REG_IP);
		osDelay(5);
		newSettings_IPv4Gate = EEPROM_ReadIP(EEPROM24AA_REG_GATEWAY);
		osDelay(5);
		newSettings_IPv4Mask = EEPROM_ReadIP(EEPROM24AA_REG_SUBNET);
		// @todo add further validation for IP / gateway addresses
		if(ip4_addr_netmask_valid(newSettings_IPv4Mask)){
			applySettings_IPv4Mask = true;
			applySettings_IPv4Addr = true;
			applySettings_IPv4Gate = true;
		} else {
			// deviceStatus_DHCPenabled = true;  // revert back to DHCP
		}
	}

	bool ipAddressPrinted = false;
	httpd_init();
	webCGIinit();
	webSSIinit();

	/* Configure and start the SNTP client */
	/* @see https://www.pool.ntp.org/zone/ch */
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, "pool.ntp.org");
	sntp_setservername(1, "2.ch.pool.ntp.org");
	sntp_init();

	// build firmware version string
#ifdef INCLUDE_DATE_IN_VERSION_STRING
	snprintf(_version_string, _LEN_VERSION_STR, "v%d.%d.%d (%s %s)",
			_VERSION_MAJOR, _VERSION_MINOR, _VERSION_PATCH, __DATE__, __TIME__);
#else
	snprintf(_version_string, _LEN_VERSION_STR, "v%d.%d.%d",
			_VERSION_MAJOR, _VERSION_MINOR, _VERSION_PATCH);
#endif

#ifdef PRINT_ETA_SERIAL_STRING
	printf("PCB rev. %s\n", _PCB_REVISION);
#endif

#ifdef PRINT_ETA_VERSION_STRING
	printf("Firmware %s\n", _version_string);
#endif

	/* Infinite loop */
	for (;;) {
		// osDelay(1);

		// Handle any changes in the Network settings
		// save to EEPROM to recover settings after reboot
		IP_Settings_Apply(&applySettings_IPv4Addr, &newSettings_IPv4Addr, EEPROM24AA_REG_IP     );
		IP_Settings_Apply(&applySettings_IPv4Mask, &newSettings_IPv4Mask, EEPROM24AA_REG_SUBNET );
		IP_Settings_Apply(&applySettings_IPv4Gate, &newSettings_IPv4Gate, EEPROM24AA_REG_GATEWAY);
		DHCP_Settings_Apply(&applySettings_DHCP  , &newSettings_DHCPenabled, EEPROM24AA_REG_DHCP_EN);

		if (!ipAddressPrinted) {
			if (!ip4_addr_isany(netif_ip4_addr(&gnetif))) {
				deviceStatus_IPv4Addr = ip4_addr_get_u32(netif_ip4_addr(&gnetif));
				deviceStatus_IPv4Gate = ip4_addr_get_u32(netif_ip4_gw(&gnetif));
				deviceStatus_IPv4Mask = ip4_addr_get_u32(netif_ip4_netmask(&gnetif));
				// print IP address
				printf("IP %d.%d.%d.%d\n", (int) (deviceStatus_IPv4Addr & 0xff),
						(int) ((deviceStatus_IPv4Addr >> 8) & 0xff),
						(int) ((deviceStatus_IPv4Addr >> 16) & 0xff),
						(int) (deviceStatus_IPv4Addr >> 24));
				ipAddressPrinted = true;

				// print PHY/MAC address
				printf("MAC %02x:%02x:%02x:%02x:%02x:%02x\n", MACAddrUser[0],
						MACAddrUser[1], MACAddrUser[2], MACAddrUser[3],
						MACAddrUser[4], MACAddrUser[5]);
			}
		}

		// Feed UART Rx data to SCPI_Input when a line end is detected in UART IRQ
		if (flag_uart_cmd_available) {
			flag_uart_cmd_available = false;
			/** @todo MUTEX for ringbuffer? */

			uint8_t len = lwrb_get_full(&ringbuffer);
			lwrb_read(&ringbuffer, temp, len);
			SCPI_Input(&scpi_context_serial, temp, len);

			/** @todo : does this work without the temp buffer ? */
			// first character appears to be a '\n'  --> start at buff[1] does not work either
			// lwrb_get_linear_block_read_address()
			//SCPI_Input(&scpi_context_serial, (uint8_t*)&ringbuffer.buff[0], lwrb_get_full(&ringbuffer));
			//lwrb_reset(&ringbuffer);
		} else {
			__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE); // make absolutely sure, this is enabled
		}
	}
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
