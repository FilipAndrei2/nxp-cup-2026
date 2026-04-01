################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ATrackState.cpp \
../src/AlgorithmStrategyImpl.cpp \
../src/AlgorithmStrategyTest.cpp \
../src/BreakingState.cpp \
../src/Car.cpp \
../src/CarBuilder.cpp \
../src/FinishedState.cpp \
../src/ITrackState.cpp \
../src/OnTrackState.cpp \
../src/PixyCamControllerImpl.cpp \
../src/SeeingFirstFinishState.cpp \
../src/SeeingSecondFinishState.cpp \
../src/ServoControllerImpl.cpp \
../src/SpeedControllerImpl.cpp \
../src/StartingState.cpp \
../src/TestState.cpp \
../src/UltrasoundSensorController.cpp \
../src/Vectors.cpp \
../src/WaitingCubeState.cpp \
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
./src/ATrackState.o \
./src/AlgorithmStrategyImpl.o \
./src/AlgorithmStrategyTest.o \
./src/BreakingState.o \
./src/Car.o \
./src/CarBuilder.o \
./src/FinishedState.o \
./src/ITrackState.o \
./src/OnTrackState.o \
./src/PixyCamControllerImpl.o \
./src/SeeingFirstFinishState.o \
./src/SeeingSecondFinishState.o \
./src/ServoControllerImpl.o \
./src/SpeedControllerImpl.o \
./src/StartingState.o \
./src/TestState.o \
./src/UltrasoundSensorController.o \
./src/Vectors.o \
./src/WaitingCubeState.o \
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
./src/ATrackState.d \
./src/AlgorithmStrategyImpl.d \
./src/AlgorithmStrategyTest.d \
./src/BreakingState.d \
./src/Car.d \
./src/CarBuilder.d \
./src/FinishedState.d \
./src/ITrackState.d \
./src/OnTrackState.d \
./src/PixyCamControllerImpl.d \
./src/SeeingFirstFinishState.d \
./src/SeeingSecondFinishState.d \
./src/ServoControllerImpl.d \
./src/SpeedControllerImpl.d \
./src/StartingState.d \
./src/TestState.d \
./src/UltrasoundSensorController.d \
./src/Vectors.d \
./src/WaitingCubeState.d \
./src/funcs.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C++ Compiler'
	arm-none-eabi-g++ "@src/ATrackState.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/display.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


