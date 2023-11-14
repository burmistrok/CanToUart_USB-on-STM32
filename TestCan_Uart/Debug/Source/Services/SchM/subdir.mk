################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Services/SchM/SchM.c \
../Source/Services/SchM/SchM_Callouts.c 

OBJS += \
./Source/Services/SchM/SchM.o \
./Source/Services/SchM/SchM_Callouts.o 

C_DEPS += \
./Source/Services/SchM/SchM.d \
./Source/Services/SchM/SchM_Callouts.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Services/SchM/%.o Source/Services/SchM/%.su Source/Services/SchM/%.cyclo: ../Source/Services/SchM/%.c Source/Services/SchM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F103xB -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Cdd/Led" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Common" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Inlude" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Utils" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Utils/CircularFIFOBuffer" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Services/Kernel" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Services/SchM" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Peripherals/HAL_USART" -I"C:/Users/oleg.burmistr/Documents/Repositories/CanToUart_USB-on-STM32/TestCan_Uart/Source/Peripherals/HAL_CAN" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Source-2f-Services-2f-SchM

clean-Source-2f-Services-2f-SchM:
	-$(RM) ./Source/Services/SchM/SchM.cyclo ./Source/Services/SchM/SchM.d ./Source/Services/SchM/SchM.o ./Source/Services/SchM/SchM.su ./Source/Services/SchM/SchM_Callouts.cyclo ./Source/Services/SchM/SchM_Callouts.d ./Source/Services/SchM/SchM_Callouts.o ./Source/Services/SchM/SchM_Callouts.su

.PHONY: clean-Source-2f-Services-2f-SchM

