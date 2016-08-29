//
// Created by Saipraveen B on 25/08/16.
//

#include <sstream>
#include <algorithm>
#include "InstructionSet.h"

int InstructionSet::addFactory(Instruction::Factory *factory) {
  factories.push_back(factory);
  factory->registerName(&nameset, &constructors);
  return factories.size() - 1;
}

InstructionSet::Decoder *InstructionSet::getDecoder() {
  return new InstructionSet::Decoder(this);
}

InstructionSet::InstructionSet() {
  //for( int i = 0; i < factories.size(); i++ ){
  //    factories[i]->registerName( &nameset, &constructors );
  //}
}

InstructionSet::Encoder *InstructionSet::getEncoder() {
  return new InstructionSet::Encoder(this);
}

InstructionSet::Decoder::Decoder(InstructionSet *is) {
  this->set = is;
}

Instruction *InstructionSet::Decoder::decode(vector<unsigned short> instr_raw) {
  unsigned short inum = (instr_raw[0] >> 12);
  return this->set->constructors.at(inum)->make(instr_raw);

}

InstructionSet::Encoder::Encoder(InstructionSet *is) {

  this->set = is;

}

vector<unsigned short> InstructionSet::Encoder::encode(string instr_line) {

  // Handle RISC instruction set encoding.


  // Remove the leading space.
  for (int i = 0; i < instr_line.size(); i++) {

    if (instr_line[i] == ' ') {
      instr_line[i] = ',';
      break;
    }

  }

  // Remove whitespace from the string.
  std::remove_if(instr_line.begin(), instr_line.end(), [](char c) {
    return std::isspace(static_cast<unsigned char>(c));
  });

  stringstream ss(instr_line);

  char token[100];
  vector<string> tokens;

  while (1) {
    if (ss.eof())
      break;

    ss.getline(token, 100, ',');
    tokens.push_back(string(token));
  }

  Instruction::Factory *factory = set->nameset[tokens[0]];
  vector<unsigned short> compressed = factory->encode(tokens);

  return compressed;
}

