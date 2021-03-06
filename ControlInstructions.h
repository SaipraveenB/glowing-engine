//
// Created by sauce on 3/9/16.
//

#ifndef CSD_ASSIGNMENT2_CONTROLINSTRUCTIONS_H
#define CSD_ASSIGNMENT2_CONTROLINSTRUCTIONS_H

#include "Instruction.h"

// Halt Instruction (HLT)
class HaltInstruction : public Instruction {
 public:
  // Indicate HLT instruction.
  bool isHalt() {
    return true;
  }

  string toStringSL();
  void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem);
  void fetch(RegisterFile<unsigned short> *rf);
  void memory(RegisterFile<unsigned short> *rf, Memory<char> *mem);
  void write(RegisterFile<unsigned short> *rf);

  // Factory to register the encoders and decoders for HLT instruction.
  class HaltFactory : public Factory {
   public:

    // Registers the instruction HLT with the instruction factory.
    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    // Converts raw bytes into an Instruction* for execution.
    Instruction *make(vector<unsigned short> raw_instr, unsigned short pc);

    // Converts a raw string description into raw bytes (machine code)
    vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols);

    // OPCODE for HLT instruction.
    unsigned short INSTR_HALT;
  };

};

// Conditional Branch Instruction (BEQZ)
class ConditionalBranchInstruction : public Instruction {
 public:

  // Holds the register number that will be checked for zero.
  unsigned short check_reg;

  // Offset from PC that will be used (register)
  short pc_offset;
  string toStringSL();
  // Executes a conditional branch instruction.
  void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem);
  void fetch(RegisterFile<unsigned short> *rf);
  void memory(RegisterFile<unsigned short> *rf, Memory<char> *mem);
  void write(RegisterFile<unsigned short> *rf);

  // Registering conditional branch instruction BEQZ with instruction factory.
  class ConditionalBranchFactory : public Factory {

   public:

    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols);

    Instruction *make(vector<unsigned short> raw_instr, unsigned short pc);

    unsigned short INSTR_C_BRANCH;
  };
  unsigned short target;
  void *marker_target;
};

// Unconditional Branch Instruction (JMP)
class UnconditionalBranchInstruction : public Instruction {
 public:

  // Offset from PC that the PC will be set to.
  short pc_offset;
  string toStringSL();
  void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem);
  void fetch(RegisterFile<unsigned short> *rf);
  void memory(RegisterFile<unsigned short> *rf, Memory<char> *mem);
  void write(RegisterFile<unsigned short> *rf);

  // Registering the Unconditional Branch Instruction with Instruction Factory.
  class UnconditionalBranchFactory : public Factory {

   public:

    void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec);

    vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols);

    Instruction *make(vector<unsigned short> raw_instr, unsigned short pc);

    unsigned short INSTR_UC_BRANCH;
  };
};

#endif //CSD_ASSIGNMENT2_CONTROLINSTRUCTIONS_H
