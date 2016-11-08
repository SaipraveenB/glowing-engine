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

  // Register/Immediate values.
  unsigned short in0;
  unsigned short in1;
  unsigned short out;
  // Operation ID.
  unsigned short op;

  // Based on mode and op, this function either adds subtracts or multiplies the operands.
  void execute(RegisterFile<unsigned short> *rf);
  void fetch(RegisterFile<unsigned short> *rf);
  void memory(RegisterFile<unsigned short> *rf, Memory<char> *mem);
  void write(RegisterFile<unsigned short> *rf);

  class ArithmeticFactory : public Factory {

   public:
    // Implements the stub in Instruction() to register names like "ADD", "SUB", "MUL" etc.
    // Also defines a mapping from instruction number to factory.
    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    // Encodes the instructions given "ADD", "SUB" or "MUL" into it's binary form. symbol table not used.
    vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols);

    // Decodes the given array of shorts. Creates an Arithmetic Instruction object and sets it's mode, in0, in1, out and op to their respective values.
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

  short val0;
  short val1;
  unsigned short res;
  void *marker0;
  void *marker1;
};

#endif //CSD_ASSIGNMENT2_ARITHMETICINSTRUCTION_H
