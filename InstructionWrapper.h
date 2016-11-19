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
  // Record PC at decode stage.
  unsigned short pc;

  InstructionWrapper(vector<unsigned short> raw, InstructionSet::Decoder *decoder, unsigned short pc)
      : raw(raw), decoder(decoder), instr(nullptr), pc(pc) {};

  Instruction::PipeState getState() {
    return instr ? (instr->state) : Instruction::PipeState::DECODE;
  }
  void decode() {
    instr = decoder->decode(raw, pc);
  }
  void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
    instr->execute(rf, mem);
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
  void advance() {
    instr->advance();
  }
  int getPipeIndex() {
    return instr->pipe;
  }
  string toString() {
    if (!instr) {
      stringstream ss;
      ss << "PC: " << pc << " Raw";
      return ss.str();
    }

    return instr->toStringSL();
  }

};
#endif //CSD_ASSIGNMENT2_INSTRUCTIONWRAPPER_H_H
