//
// Created by Saipraveen B on 25/08/16.
//

#include "Processor.h"

using namespace std;

void Processor::run() {

  // Infinite loop till HLT;
  for (;;) {

    // 2-Byte instruction buffer.
    std::vector<unsigned short> next_instr(1);

    this->pMem->readMem(pRegFile->spl(RegisterFile<unsigned short>::REG_PC), next_instr.data(), 2);

    Instruction *instr = decoder->decode(next_instr);
    if (instr->isHalt())
      return;

    instr->execute(pRegFile, pMem);

  }

}

Processor::Processor(Memory<char> *mem, RegisterFile<unsigned short> *rf, InstructionSet::Decoder *decoder)
    : pMem(mem), pRegFile(rf), decoder(decoder) {
  // Set special register Program Counter to 0.
  rf->spl(RegisterFile<unsigned short>::REG_PC) = 0;
}
