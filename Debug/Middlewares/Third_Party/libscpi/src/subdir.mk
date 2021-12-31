################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/libscpi/src/error.c \
../Middlewares/Third_Party/libscpi/src/expression.c \
../Middlewares/Third_Party/libscpi/src/fifo.c \
../Middlewares/Third_Party/libscpi/src/ieee488.c \
../Middlewares/Third_Party/libscpi/src/lexer.c \
../Middlewares/Third_Party/libscpi/src/minimal.c \
../Middlewares/Third_Party/libscpi/src/parser.c \
../Middlewares/Third_Party/libscpi/src/units.c \
../Middlewares/Third_Party/libscpi/src/utils.c 

OBJS += \
./Middlewares/Third_Party/libscpi/src/error.o \
./Middlewares/Third_Party/libscpi/src/expression.o \
./Middlewares/Third_Party/libscpi/src/fifo.o \
./Middlewares/Third_Party/libscpi/src/ieee488.o \
./Middlewares/Third_Party/libscpi/src/lexer.o \
./Middlewares/Third_Party/libscpi/src/minimal.o \
./Middlewares/Third_Party/libscpi/src/parser.o \
./Middlewares/Third_Party/libscpi/src/units.o \
./Middlewares/Third_Party/libscpi/src/utils.o 

C_DEPS += \
./Middlewares/Third_Party/libscpi/src/error.d \
./Middlewares/Third_Party/libscpi/src/expression.d \
./Middlewares/Third_Party/libscpi/src/fifo.d \
./Middlewares/Third_Party/libscpi/src/ieee488.d \
./Middlewares/Third_Party/libscpi/src/lexer.d \
./Middlewares/Third_Party/libscpi/src/minimal.d \
./Middlewares/Third_Party/libscpi/src/parser.d \
./Middlewares/Third_Party/libscpi/src/units.d \
./Middlewares/Third_Party/libscpi/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/libscpi/src/%.o: ../Middlewares/Third_Party/libscpi/src/%.c Middlewares/Third_Party/libscpi/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/apps/http -I../Middlewares/Third_Party/lwrb/include -I../Middlewares/Third_Party/libscpi/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-libscpi-2f-src

clean-Middlewares-2f-Third_Party-2f-libscpi-2f-src:
	-$(RM) ./Middlewares/Third_Party/libscpi/src/error.d ./Middlewares/Third_Party/libscpi/src/error.o ./Middlewares/Third_Party/libscpi/src/expression.d ./Middlewares/Third_Party/libscpi/src/expression.o ./Middlewares/Third_Party/libscpi/src/fifo.d ./Middlewares/Third_Party/libscpi/src/fifo.o ./Middlewares/Third_Party/libscpi/src/ieee488.d ./Middlewares/Third_Party/libscpi/src/ieee488.o ./Middlewares/Third_Party/libscpi/src/lexer.d ./Middlewares/Third_Party/libscpi/src/lexer.o ./Middlewares/Third_Party/libscpi/src/minimal.d ./Middlewares/Third_Party/libscpi/src/minimal.o ./Middlewares/Third_Party/libscpi/src/parser.d ./Middlewares/Third_Party/libscpi/src/parser.o ./Middlewares/Third_Party/libscpi/src/units.d ./Middlewares/Third_Party/libscpi/src/units.o ./Middlewares/Third_Party/libscpi/src/utils.d ./Middlewares/Third_Party/libscpi/src/utils.o

.PHONY: clean-Middlewares-2f-Third_Party-2f-libscpi-2f-src

