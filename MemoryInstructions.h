//
// Created by sauce on 2/9/16.
//

#ifndef CSD_ASSIGNMENT2_MEMORYINSTRUCTIONS_H
#define CSD_ASSIGNMENT2_MEMORYINSTRUCTIONS_H

#include "Instruction.h"

class LoadInstruction : public Instruction {
 public:
  unsigned short base_register;
  unsigned short register_offset;

  unsigned short output_register;

  void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem);

  class LoadFactory : public Factory {
   public:
    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    Instruction *make(vector<unsigned short> raw_instr);

    vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols);

    unsigned short INSTR_LD_BASE_OFFSET;
  };
};

class StoreInstruction : public Instruction {
 public:
  unsigned short base_register;
  unsigned short register_offset;

  unsigned short input_register;

  void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem);

  class StoreFactory : public Factory {
   public:
    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    Instruction *make(vector<unsigned short> raw_instr);

    vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols);

    unsigned short INSTR_SD_BASE_OFFSET;
  };
};

#endif //CSD_ASSIGNMENT2_MEMORYINSTRUCTIONS_H
