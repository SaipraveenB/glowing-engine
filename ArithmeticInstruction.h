//
// Created by Saipraveen B on 26/08/16.
//

#ifndef CSD_ASSIGNMENT2_ARITHMETICINSTRUCTION_H
#define CSD_ASSIGNMENT2_ARITHMETICINSTRUCTION_H

#include "Instruction.h"

class ArithmeticInstruction : public Instruction {
 public:
  // Hold register/immediate values.
  unsigned short mode;

  unsigned short in0;
  unsigned short in1;
  unsigned short out;
  unsigned short op;

  void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem);

  class ArithmeticFactory : public Factory {

   public:

    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols);

    Instruction *make(vector<unsigned short> raw_instr);

    unsigned short INSTR_ADD;
    unsigned short INSTR_ADD_IMM0;
    unsigned short INSTR_ADD_IMM1;

    unsigned short INSTR_MUL;
    unsigned short INSTR_MUL_IMM0;
    unsigned short INSTR_MUL_IMM1;

    unsigned short INSTR_SUB;
    unsigned short INSTR_SUB_IMM0;
    unsigned short INSTR_SUB_IMM1;

  };

};

#endif //CSD_ASSIGNMENT2_ARITHMETICINSTRUCTION_H
