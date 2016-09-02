//
// Created by sauce on 3/9/16.
//

#ifndef CSD_ASSIGNMENT2_CONTROLINSTRUCTIONS_H
#define CSD_ASSIGNMENT2_CONTROLINSTRUCTIONS_H

#include "Instruction.h"

class HaltInstruction : public Instruction {
 public:
  // Indicate HLT instruction.
  bool isHalt() {
    return true;
  }

  class HaltFactory : public Factory {
   public:
    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    Instruction *make(vector<unsigned short> raw_instr);

    vector<unsigned short> encode(vector<string> tokens);

    unsigned short INSTR_HALT;
  };
};

class ConditionalBranchInstruction : public Instruction {
 public:

  unsigned short check_reg;
  unsigned short pc_offset;

  void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem);

  class ConditionalBranchFactory : public Factory {

   public:

    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    vector<unsigned short> encode(vector<string> tokens);

    Instruction *make(vector<unsigned short> raw_instr);

    unsigned short INSTR_C_BRANCH;
  };
};

class UnconditionalBranchInstruction : public Instruction {
 public:
  unsigned short pc_offset;

  void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem);

  class UnconditionalBranchFactory : public Factory {

   public:

    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    vector<unsigned short> encode(vector<string> tokens);

    Instruction *make(vector<unsigned short> raw_instr);

    unsigned short INSTR_UC_BRANCH;
  };
};

#endif //CSD_ASSIGNMENT2_CONTROLINSTRUCTIONS_H
