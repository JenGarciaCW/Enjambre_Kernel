################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/UART.cpp \
../src/socketmessage.cpp 

OBJS += \
./src/UART.o \
./src/socketmessage.o 

CPP_DEPS += \
./src/UART.d \
./src/socketmessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -pthread -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103 -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -I/home/kevin/Desktop/BlackLib-master -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


