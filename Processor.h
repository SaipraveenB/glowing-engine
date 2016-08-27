//
// Created by Saipraveen B on 25/08/16.
//

#ifndef CSD_ASSIGNMENT2_PROCESSOR_H
#define CSD_ASSIGNMENT2_PROCESSOR_H


#include "Memory.h"
#include "RegisterFile.h"
#include "InstructionSet.h"

class Processor {
private:


    Memory<char>* pMem;
    RegisterFile<unsigned short>* pRegFile;

public:

    Processor(Memory<char> *mem, RegisterFile<unsigned short> *rf, InstructionSet::Decoder *decoder);
    void run();

    InstructionSet::Decoder *decoder;
};

#endif //CSD_ASSIGNMENT2_PROCESSOR_H
