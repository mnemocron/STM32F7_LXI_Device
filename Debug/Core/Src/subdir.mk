################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ds3231.c \
../Core/Src/eeprom_24aa.c \
../Core/Src/freertos.c \
../Core/Src/http_cgi_app.c \
../Core/Src/main.c \
../Core/Src/ntp_epochtime.c \
../Core/Src/rpc_server.c \
../Core/Src/scpi-def.c \
../Core/Src/scpi_server.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_hal_timebase_tim.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c 

OBJS += \
./Core/Src/ds3231.o \
./Core/Src/eeprom_24aa.o \
./Core/Src/freertos.o \
./Core/Src/http_cgi_app.o \
./Core/Src/main.o \
./Core/Src/ntp_epochtime.o \
./Core/Src/rpc_server.o \
./Core/Src/scpi-def.o \
./Core/Src/scpi_server.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_hal_timebase_tim.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f7xx.o 

C_DEPS += \
./Core/Src/ds3231.d \
./Core/Src/eeprom_24aa.d \
./Core/Src/freertos.d \
./Core/Src/http_cgi_app.d \
./Core/Src/main.d \
./Core/Src/ntp_epochtime.d \
./Core/Src/rpc_server.d \
./Core/Src/scpi-def.d \
./Core/Src/scpi_server.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_hal_timebase_tim.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/apps/http -I../Middlewares/Third_Party/lwrb/include -I../Middlewares/Third_Party/libscpi/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ds3231.d ./Core/Src/ds3231.o ./Core/Src/eeprom_24aa.d ./Core/Src/eeprom_24aa.o ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/http_cgi_app.d ./Core/Src/http_cgi_app.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/ntp_epochtime.d ./Core/Src/ntp_epochtime.o ./Core/Src/rpc_server.d ./Core/Src/rpc_server.o ./Core/Src/scpi-def.d ./Core/Src/scpi-def.o ./Core/Src/scpi_server.d ./Core/Src/scpi_server.o ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_timebase_tim.d ./Core/Src/stm32f7xx_hal_timebase_tim.o ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o

.PHONY: clean-Core-2f-Src

