//
// Created by Saipraveen B on 25/08/16.
//

#ifndef CSD_ASSIGNMENT2_DECODER_H
#define CSD_ASSIGNMENT2_DECODER_H

#include <iostream>
#include <vector>
#include <string>
#include "Instruction.h"

using namespace std;

class InstructionSet {
  std::vector<Instruction::Factory *> factories;
  const static unsigned short OP_TYPE_IMMEDIATE = 0x0000;
  const static unsigned short OP_TYPE_REGISTER = 0x8000;

  // Maintain the factories for every Operation string( "ADD", "SUB" etc.. )
  map<std::string, Instruction::Factory *> nameset;

  // Maintain the instruction constructors for every OpCode.
  vector<Instruction::Factory *> constructors;

 public:
  InstructionSet();
  int addFactory(Instruction::Factory *factory);

  class Decoder {
   public:
    // Construct a decoder for a given instruction set.
    Decoder(InstructionSet *is);
    // Decodes a given instruction by calling the appropriate constructor.
    Instruction *decode(vector<unsigned short> instr_raw);

    InstructionSet *set;
  };

  class Encoder {
   public:
    map<std::string, unsigned int> symbols;

    // Constructs an encoder for the given instruction set.
    Encoder(InstructionSet *);
    // encoder the instruction line by first tokenizing and then calling the appropriate factory.
    vector<unsigned short> encode(string instr_line, std::map<std::string, unsigned int> symbols);
    InstructionSet *set;

  };

  // Convenience function to build a decoder.
  Decoder *getDecoder();
  // Convenience function to build a encoder.
  Encoder *getEncoder();

};

#endif //CSD_ASSIGNMENT2_DECODER_H
