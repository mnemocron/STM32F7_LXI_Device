################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/libscpi/src/error.c \
../Middlewares/libscpi/src/expression.c \
../Middlewares/libscpi/src/fifo.c \
../Middlewares/libscpi/src/ieee488.c \
../Middlewares/libscpi/src/lexer.c \
../Middlewares/libscpi/src/minimal.c \
../Middlewares/libscpi/src/parser.c \
../Middlewares/libscpi/src/units.c \
../Middlewares/libscpi/src/utils.c 

OBJS += \
./Middlewares/libscpi/src/error.o \
./Middlewares/libscpi/src/expression.o \
./Middlewares/libscpi/src/fifo.o \
./Middlewares/libscpi/src/ieee488.o \
./Middlewares/libscpi/src/lexer.o \
./Middlewares/libscpi/src/minimal.o \
./Middlewares/libscpi/src/parser.o \
./Middlewares/libscpi/src/units.o \
./Middlewares/libscpi/src/utils.o 

C_DEPS += \
./Middlewares/libscpi/src/error.d \
./Middlewares/libscpi/src/expression.d \
./Middlewares/libscpi/src/fifo.d \
./Middlewares/libscpi/src/ieee488.d \
./Middlewares/libscpi/src/lexer.d \
./Middlewares/libscpi/src/minimal.d \
./Middlewares/libscpi/src/parser.d \
./Middlewares/libscpi/src/units.d \
./Middlewares/libscpi/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/libscpi/src/%.o: ../Middlewares/libscpi/src/%.c Middlewares/libscpi/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/EEPROM_24AA/inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/apps/http -I"C:/Users/simon/Documents/STM32/cubeide/f746zg_LXI_Device/Middlewares/lwrb/include" -I"C:/Users/simon/Documents/STM32/cubeide/f746zg_LXI_Device/Middlewares/libscpi/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

