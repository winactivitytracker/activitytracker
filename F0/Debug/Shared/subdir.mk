################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/School/IOT/KBS/GitHub/IoT/activitytracker/Shared/aes.c 

CPP_SRCS += \
C:/School/IOT/KBS/GitHub/IoT/activitytracker/Shared/radioAPI.cpp \
C:/School/IOT/KBS/GitHub/IoT/activitytracker/Shared/receiver.cpp \
C:/School/IOT/KBS/GitHub/IoT/activitytracker/Shared/transmitter.cpp 

C_DEPS += \
./Shared/aes.d 

OBJS += \
./Shared/aes.o \
./Shared/radioAPI.o \
./Shared/receiver.o \
./Shared/transmitter.o 

CPP_DEPS += \
./Shared/radioAPI.d \
./Shared/receiver.d \
./Shared/transmitter.d 


# Each subdirectory must supply rules for building sources it contributes
Shared/aes.o: C:/School/IOT/KBS/GitHub/IoT/activitytracker/Shared/aes.c Shared/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/School/IOT/KBS/GitHub/IoT/activitytracker/F0/Core/Src" -I"../../Shared" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Shared/aes.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Shared/radioAPI.o: C:/School/IOT/KBS/GitHub/IoT/activitytracker/Shared/radioAPI.cpp Shared/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/School/IOT/KBS/GitHub/IoT/activitytracker/F0/Core/Src" -I"../../Shared" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Shared/radioAPI.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Shared/receiver.o: C:/School/IOT/KBS/GitHub/IoT/activitytracker/Shared/receiver.cpp Shared/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/School/IOT/KBS/GitHub/IoT/activitytracker/F0/Core/Src" -I"../../Shared" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Shared/receiver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Shared/transmitter.o: C:/School/IOT/KBS/GitHub/IoT/activitytracker/Shared/transmitter.cpp Shared/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/School/IOT/KBS/GitHub/IoT/activitytracker/F0/Core/Src" -I"../../Shared" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Shared/transmitter.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

