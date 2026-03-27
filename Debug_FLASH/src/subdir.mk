################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AlgorithmStrategyImpl.cpp \
../src/Car.cpp \
../src/CarBuilder.cpp \
../src/ITrackState.cpp \
../src/OnTrackState.cpp \
../src/PixyCamControllerImpl.cpp \
../src/SeeingFinishFirstTimeState.cpp \
../src/SeeingFinishLineSecondTimeState.cpp \
../src/ServoControllerImpl.cpp \
../src/SpeedControllerImpl.cpp \
../src/StartingBeforeFinishLineState.cpp \
../src/StoppedState.cpp \
../src/UltrasoundSensorController.cpp \
../src/Vectors.cpp \
../src/WaitingToApproachCubeState.cpp \
../src/funcs.cpp \
../src/main.cpp 

C_SRCS += \
../src/display.c \
../src/esc.c \
../src/hbridge.c \
../src/linear_camera.c \
../src/main_functions.c \
../src/pixy2.c \
../src/receiver.c \
../src/servo.c 

OBJS += \
./src/AlgorithmStrategyImpl.o \
./src/Car.o \
./src/CarBuilder.o \
./src/ITrackState.o \
./src/OnTrackState.o \
./src/PixyCamControllerImpl.o \
./src/SeeingFinishFirstTimeState.o \
./src/SeeingFinishLineSecondTimeState.o \
./src/ServoControllerImpl.o \
./src/SpeedControllerImpl.o \
./src/StartingBeforeFinishLineState.o \
./src/StoppedState.o \
./src/UltrasoundSensorController.o \
./src/Vectors.o \
./src/WaitingToApproachCubeState.o \
./src/display.o \
./src/esc.o \
./src/funcs.o \
./src/hbridge.o \
./src/linear_camera.o \
./src/main.o \
./src/main_functions.o \
./src/pixy2.o \
./src/receiver.o \
./src/servo.o 

C_DEPS += \
./src/display.d \
./src/esc.d \
./src/hbridge.d \
./src/linear_camera.d \
./src/main_functions.d \
./src/pixy2.d \
./src/receiver.d \
./src/servo.d 

CPP_DEPS += \
./src/AlgorithmStrategyImpl.d \
./src/Car.d \
./src/CarBuilder.d \
./src/ITrackState.d \
./src/OnTrackState.d \
./src/PixyCamControllerImpl.d \
./src/SeeingFinishFirstTimeState.d \
./src/SeeingFinishLineSecondTimeState.d \
./src/ServoControllerImpl.d \
./src/SpeedControllerImpl.d \
./src/StartingBeforeFinishLineState.d \
./src/StoppedState.d \
./src/UltrasoundSensorController.d \
./src/Vectors.d \
./src/WaitingToApproachCubeState.d \
./src/funcs.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C++ Compiler'
	arm-none-eabi-g++ "@src/AlgorithmStrategyImpl.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/display.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


