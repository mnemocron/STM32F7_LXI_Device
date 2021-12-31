################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/core/ipv4/%.o: ../Middlewares/Third_Party/LwIP/src/core/ipv4/%.c Middlewares/Third_Party/LwIP/src/core/ipv4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/apps/http -I../Middlewares/Third_Party/lwrb/include -I../Middlewares/Third_Party/libscpi/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv4

clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv4:
	-$(RM) ./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.o

.PHONY: clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv4

