################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f030c8tx.s 

S_DEPS += \
./Core/Startup/startup_stm32f030c8tx.d 

OBJS += \
./Core/Startup/startup_stm32f030c8tx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32f030c8tx.o: ../Core/Startup/startup_stm32f030c8tx.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0 -g3 -c -I"C:/School/IOT/KBS/GitHub/IoT/activitytracker/F0/Core/Src" -I"../../Shared" -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32f030c8tx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

