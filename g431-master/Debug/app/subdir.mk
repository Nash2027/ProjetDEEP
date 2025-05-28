################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/compteur.c \
../app/main.c \
../app/sim808.c 

OBJS += \
./app/compteur.o \
./app/main.o \
./app/sim808.o 

C_DEPS += \
./app/compteur.d \
./app/main.d \
./app/sim808.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su app/%.cyclo: ../app/%.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../core/Inc -I../drivers/stm32g4xx_hal/Inc -I../drivers/stm32g4xx_hal/Inc/Legacy -I../drivers/cmsis/Device/ST/STM32G4xx/Include -I../drivers/cmsis/Include -I../app -I../drivers/bsp -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/compteur.cyclo ./app/compteur.d ./app/compteur.o ./app/compteur.su ./app/main.cyclo ./app/main.d ./app/main.o ./app/main.su ./app/sim808.cyclo ./app/sim808.d ./app/sim808.o ./app/sim808.su

.PHONY: clean-app

