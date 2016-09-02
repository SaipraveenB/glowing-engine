cmake_minimum_required(VERSION 3.0)
project(CSD_Assignment2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Memory.cpp Memory.h RegisterFile.cpp RegisterFile.h Processor.cpp Processor.h InstructionSet.cpp InstructionSet.h Instruction.cpp Instruction.h ArithmeticInstruction.cpp ArithmeticInstruction.h MemoryInstructions.h MemoryInstructions.cpp ControlInstructions.h ControlInstructions.cpp)
add_executable(CSD_Assignment2 ${SOURCE_FILES})