//
// Created by Saipraveen B on 25/08/16.
//

#ifndef CSD_ASSIGNMENT2_PROCESSOR_H
#define CSD_ASSIGNMENT2_PROCESSOR_H

#include "Memory.h"
#include "RegisterFile.h"
#include "InstructionSet.h"

// Processor Class to simulate a processor.
class Processor {
 private:

  // Byte-addressable memory of the processor.
  Memory<char> *pMem;

  // Register File for the processor.
  RegisterFile<unsigned short> *pRegFile;

 public:

  // Initialization of processor
  Processor(Memory<char> *mem, RegisterFile<unsigned short> *rf, InstructionSet::Decoder *decoder);

  // Run the processors i.e. execute instructions.
  void run();

  // Decoder for the instruction.
  InstructionSet::Decoder *decoder;
};

#endif //CSD_ASSIGNMENT2_PROCESSOR_H
