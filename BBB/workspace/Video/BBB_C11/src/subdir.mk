################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/OCV_S.cpp \
../src/socketmessage.cpp 

OBJS += \
./src/OCV_S.o \
./src/socketmessage.o 

CPP_DEPS += \
./src/OCV_S.d \
./src/socketmessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -I/home/kevin/Desktop/BlackLib-master -I/home/kevin/opencv_build/build/install/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


