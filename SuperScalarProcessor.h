//
// Created by Saipraveen B on 25/08/16.
//

#ifndef CSD_ASSIGNMENT2_PIPELINEDPROCESSOR_H
#define CSD_ASSIGNMENT2_PIPELINEDPROCESSOR_H

#include "Memory.h"
#include "RegisterFile.h"
#include "InstructionSet.h"

// Processor Class to simulate a processor.
class SuperScalarProcessor {
 private:

  // Byte-addressable memory of the processor.
  Memory<char> *pMem;

  // Register File for the processor.
  RegisterFile<unsigned short> *pRegFile;

 public:

  // Initialization of processor
  SuperScalarProcessor(Memory<char> *mem, RegisterFile<unsigned short> *rf, InstructionSet::Decoder *decoder);

  // Run the processors i.e. execute instructions.
  void run();

  // Decoder for the instruction.
  InstructionSet::Decoder *decoder;

  // Number of stalls, flushes and loaded instructuons.
  int iStalls, iFlushes, iInstrLoaded;

  void printALUs();
};

#endif //CSD_ASSIGNMENT2_PROCESSOR_H
