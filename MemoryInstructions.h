//
// Created by sauce on 2/9/16.
//

#ifndef CSD_ASSIGNMENT2_MEMORYINSTRUCTIONS_H
#define CSD_ASSIGNMENT2_MEMORYINSTRUCTIONS_H

#include "Instruction.h"

// Load Instruction (LD)
class LoadInstruction : public Instruction {
 public:
  // Register containing base address.
  unsigned short base_register;

  // Register containing offset.
  unsigned short register_offset;

  // Register that will contain the output of LD instruction.
  unsigned short output_register;

  void execute(RegisterFile<unsigned short> *rf);
  void fetch(RegisterFile<unsigned short> *rf);
  void memory(RegisterFile<unsigned short> *rf, Memory<char> *mem);
  void write(RegisterFile<unsigned short> *rf);

  // Register Load Instruction with Instruction Factory.
  class LoadFactory : public Factory {
   public:
    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    Instruction *make(vector<unsigned short> raw_instr);

    vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols);

    unsigned short INSTR_LD_BASE_OFFSET;
  };
  void *marker_pao;
  void *marker_pab;
  unsigned short phy_addr;
  unsigned short transfer_half_word;

};

// Store Instruction (SD)
class StoreInstruction : public Instruction {
 public:

  // Register containing base address.
  unsigned short base_register;

  // Register containing offset from base address.
  unsigned short register_offset;

  // Register containing the source of data.
  unsigned short input_register;

  void execute(RegisterFile<unsigned short> *rf);
  void fetch(RegisterFile<unsigned short> *rf);
  void memory(RegisterFile<unsigned short> *rf, Memory<char> *mem);
  void write(RegisterFile<unsigned short> *rf);

  // Register Store Instruction with Instruction Factory.
  class StoreFactory : public Factory {
   public:
    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    Instruction *make(vector<unsigned short> raw_instr);

    vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols);

    unsigned short INSTR_SD_BASE_OFFSET;
  };
  unsigned short phy_addr;
  unsigned short data;

  void *marker_pao;
  void *marker_pab;
  void *marker_data;
};

#endif //CSD_ASSIGNMENT2_MEMORYINSTRUCTIONS_H
