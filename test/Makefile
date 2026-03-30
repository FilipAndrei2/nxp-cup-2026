# test/Makefile
# Compiles and runs all Catch2 + FakeIt unit tests on the host.
# Usage:
#   make          – build the test binary
#   make run      – build and run all tests
#   make clean    – remove build artefacts

CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O1 \
            -I./stubs \
            -I../include \
            -I./catch2 \
            -I./fakeit

# Source files under test (subset of ../src that has no hardware dependencies)
SRC_UNDER_TEST := \
    ../src/Vectors.cpp \
    ../src/ITrackState.cpp \
    ../src/AlgorithmStrategyImpl.cpp \
    ../src/StartingState.cpp \
    ../src/SeeingFirstFinishState.cpp \
    ../src/OnTrackState.cpp \
    ../src/SeeingFinishLineSecondTimeState.cpp \
    ../src/WaitingToApproachCubeState.cpp \
    ../src/StoppedState.cpp

# Test sources
TEST_SRCS := \
    test_main.cpp \
    test_floats.cpp \
    test_vector2.cpp \
    test_vectors.cpp \
    test_angles.cpp \
    test_states.cpp \
    test_algorithm.cpp \
    test_speed.cpp \
    catch2/catch_amalgamated.cpp

ALL_SRCS := $(TEST_SRCS) $(SRC_UNDER_TEST)
OBJS     := $(ALL_SRCS:.cpp=.o)

TARGET := run_tests

.PHONY: all run clean

all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
