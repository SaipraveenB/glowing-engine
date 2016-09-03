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
  virtual void execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
    // Not implemented yet.
    return;
  }

  class Factory {

   public:

    virtual Instruction *make(vector<unsigned short> raw_instr) {
      // Not implemented yet.
      return nullptr;
    }

    virtual vector<unsigned short> encode(vector<string> tokens ) {
      vector<unsigned short> s;
      s.push_back(0);
      return s;
    }

    virtual void registerName(map<string, Instruction::Factory *> *directory, vector<Instruction::Factory *> *vec) {
      // Not implemented yet.
      // directory["ADD"] = this;
      // directory["SUB"] = this;
      // etc..

    }

  };

  // Default.
  virtual bool isHalt() {
    // Not implemented yet.
    return false;
  }

};

#endif //CSD_ASSIGNMENT2_INSTRUCTION_H
