################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Cdd/Led/Led.c 

OBJS += \
./Source/Cdd/Led/Led.o 

C_DEPS += \
./Source/Cdd/Led/Led.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Cdd/Led/%.o Source/Cdd/Led/%.su Source/Cdd/Led/%.cyclo: ../Source/Cdd/Led/%.c Source/Cdd/Led/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Cdd/Led" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Common" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Inlude" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Utils" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Utils/CircularFIFOBuffer" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Services/Kernel" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Services/SchM" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Peripherals/HAL_USART" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Peripherals/HAL_CAN" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Source-2f-Cdd-2f-Led

clean-Source-2f-Cdd-2f-Led:
	-$(RM) ./Source/Cdd/Led/Led.cyclo ./Source/Cdd/Led/Led.d ./Source/Cdd/Led/Led.o ./Source/Cdd/Led/Led.su

.PHONY: clean-Source-2f-Cdd-2f-Led

