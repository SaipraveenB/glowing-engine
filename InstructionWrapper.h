//
// Created by Saipraveen B on 09/10/16.
//

#ifndef CSD_ASSIGNMENT2_INSTRUCTIONWRAPPER_H_H
#define CSD_ASSIGNMENT2_INSTRUCTIONWRAPPER_H_H

#include "Instruction.h"
#include "InstructionSet.h"
class InstructionWrapper {
 public:
  Instruction *instr;
  InstructionSet::Decoder *decoder;
  //Instruction::Factory* factory;
  vector<unsigned short> raw;

  InstructionWrapper(vector<unsigned short> raw, InstructionSet::Decoder *decoder)
      : raw(raw), decoder(decoder), instr(nullptr) {};

  Instruction::PipeState getState() {
    return instr ? (instr->state) : Instruction::PipeState::DECODE;
  }
  void decode() {
    instr = decoder->decode(raw);
  }
  void execute(RegisterFile<unsigned short> *rf) {
    instr->execute(rf);
  }
  void fetch(RegisterFile<unsigned short> *rf) {
    instr->fetch(rf);
  }
  void memory(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
    instr->memory(rf, mem);
  }
  void write(RegisterFile<unsigned short> *rf) {
    instr->write(rf);
  }

  bool isHalt() {
    return instr->isHalt();
  }
};
#endif //CSD_ASSIGNMENT2_INSTRUCTIONWRAPPER_H_H
