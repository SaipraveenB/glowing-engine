//
// Created by Saipraveen B on 25/08/16.
//

#ifndef CSD_ASSIGNMENT2_INSTRUCTION_H
#define CSD_ASSIGNMENT2_INSTRUCTION_H
#include <iostream>
#include <string>
#include <vector>
#include "Memory.h"
#include "RegisterFile.h"

using namespace std;

class Instruction {


 public:
  // Possible pipe states in order.
  enum PipeState {
    DECODE, FETCH, EXECUTE, MEMORY, WRITE, DONE
  };
  PipeState state;
  // Advance this instruction to the next state.
  void advance() {
    int next = static_cast<int>( state ) + 1;
    state = static_cast<PipeState>( next );
  }
  Instruction() {
    // Initialize to FETHC
    state = PipeState::FETCH;
  }

  // Runs the instruction given access to the register file and memory.
  virtual void execute(RegisterFile<unsigned short> *rf) {
    // Not implemented yet.
    return;
  }

  // Get register values and identify which values are volatile( for operand forwarding )
  virtual void fetch(RegisterFile<unsigned short> *rf) {
    // Not implemented yet.
  }

  // Write/Read values from memory here.
  virtual void memory(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
    // Not implemented yet.
  }

  // Write back to register file.
  virtual void write(RegisterFile<unsigned short> *rf) {
    // Not implemented yet.
  }

  class Factory {

   public:

    virtual Instruction *make(vector<unsigned short> raw_instr) {
      // Not implemented yet.
      return nullptr;
    }
    // Stub for the encoding functions to convert the tokens to binary.
    virtual vector<unsigned short> encode(vector<string> tokens, std::map<std::string, unsigned int> symbols) {
      vector<unsigned short> s;
      s.push_back(0);
      return s;
    }
    // Registers the names and reserves instruction numbers here.
    virtual void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec) {
      // Not implemented yet.
      // directory["ADD"] = this;
      // directory["SUB"] = this;
      // etc..

    }

  };

  // Defines if an instruction is a HALT instruction. Tells the processor to stop.
  virtual bool isHalt() {
    // Not implemented yet.
    return false;
  }
};

#endif //CSD_ASSIGNMENT2_INSTRUCTION_H
