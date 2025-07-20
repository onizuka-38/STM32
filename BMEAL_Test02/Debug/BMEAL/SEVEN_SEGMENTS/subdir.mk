################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS.c \
../BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS_cfg.c 

OBJS += \
./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS.o \
./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS_cfg.o 

C_DEPS += \
./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS.d \
./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS_cfg.d 


# Each subdirectory must supply rules for building sources it contributes
BMEAL/SEVEN_SEGMENTS/%.o BMEAL/SEVEN_SEGMENTS/%.su BMEAL/SEVEN_SEGMENTS/%.cyclo: ../BMEAL/SEVEN_SEGMENTS/%.c BMEAL/SEVEN_SEGMENTS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BMEAL-2f-SEVEN_SEGMENTS

clean-BMEAL-2f-SEVEN_SEGMENTS:
	-$(RM) ./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS.cyclo ./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS.d ./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS.o ./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS.su ./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS_cfg.cyclo ./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS_cfg.d ./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS_cfg.o ./BMEAL/SEVEN_SEGMENTS/SEVEN_SEGMENTS_cfg.su

.PHONY: clean-BMEAL-2f-SEVEN_SEGMENTS

