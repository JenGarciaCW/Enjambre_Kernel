################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Joystick.cpp \
../src/joystickclass.cpp \
../src/socketmessage.cpp 

OBJS += \
./src/Joystick.o \
./src/joystickclass.o \
./src/socketmessage.o 

CPP_DEPS += \
./src/Joystick.d \
./src/joystickclass.d \
./src/socketmessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


