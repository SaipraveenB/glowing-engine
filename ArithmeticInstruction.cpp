//
// Created by Saipraveen B on 26/08/16.
//

#include "ArithmeticInstruction.h"

void ArithmeticInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {

  unsigned short val0;
  unsigned short val1;

  unsigned short dest = out;
  if (mode == 0) {
    val0 = rf->reg(in0);
    val1 = rf->reg(in1);
  } else if (mode == 1) {

    val0 = in0; // TODO(SaipraveenB): Convert from 2s complement 4-bit to 2s complement 16-bit.
    val1 = rf->reg(in1);

  } else if (mode == 2) {
    val1 = in1;
    val0 = rf->reg(in0);
  }

  switch (op) {
    case 0:rf->reg(dest) = val1 + val0;
      break;
    case 1:rf->reg(dest) = val1 - val0;
      break;
    case 2:rf->reg(dest) = val1 * val0;
      break;
  }

}

void ArithmeticInstruction::ArithmeticFactory::registerName(map<string, Instruction::Factory *> *directory,
                                                            vector<Instruction::Factory *> *vec) {


  // Register names.
  directory->insert(make_pair("ADD", this));
  directory->insert(make_pair("SUB", this));
  directory->insert(make_pair("MUL", this));

  // Retreive instruction numbers.
  INSTR_ADD = (unsigned short) vec->size();
  vec->push_back(this);
  INSTR_ADD_IMM0 = (unsigned short) vec->size();
  vec->push_back(this);
  INSTR_ADD_IMM1 = (unsigned short) vec->size();
  vec->push_back(this);

  INSTR_MUL = (unsigned short) vec->size();
  vec->push_back(this);
  INSTR_MUL_IMM0 = (unsigned short) vec->size();
  vec->push_back(this);
  INSTR_MUL_IMM1 = (unsigned short) vec->size();
  vec->push_back(this);

  INSTR_SUB = (unsigned short) vec->size();
  vec->push_back(this);
  INSTR_SUB_IMM0 = (unsigned short) vec->size();
  vec->push_back(this);
  INSTR_SUB_IMM1 = (unsigned short) vec->size();
  vec->push_back(this);

}

vector<unsigned short>
ArithmeticInstruction::ArithmeticFactory::encode(vector<string> tokens) {

  // Check for immediate operands.

  string instr_name = tokens[0];
  string in0 = tokens[1];
  string in1 = tokens[2];
  string out = tokens[3];

  unsigned short instr_offset = 0;

  unsigned short si0;
  unsigned short si1;
  unsigned short so;

  si0 = (unsigned short) std::stoi(in0.substr(1));
  si1 = (unsigned short) std::stoi(in1.substr(1));

  if (in0[0] == '#') {
    instr_offset = 1;
    // TODO(SaipraveenB): 2s complement si0 here.
  } else if (in1[0] == '#') {
    instr_offset = 2;
    // TODO(SaipraveenB): 2s complement si1 here.
  }

  so = (unsigned short) std::stoi(out.substr(1));

  unsigned short instr_base = 0;
  if (instr_name.compare("ADD") == 0) {
    instr_base = INSTR_ADD;
  } else if (instr_name.compare("SUB") == 0) {
    instr_base = INSTR_SUB;
  } else if (instr_name.compare("MUL") == 0) {
    instr_base = INSTR_MUL;
  }

  unsigned short instr_full = instr_base + instr_offset;

  unsigned short full = (instr_full << 12) | (so << 8) | (si0 << 4) | (si1);
  vector<unsigned short> v;
  v.push_back(full);
  return v;

}

Instruction *ArithmeticInstruction::ArithmeticFactory::make(vector<unsigned short> raw_instr) {

  ArithmeticInstruction *ai = new ArithmeticInstruction();

  unsigned short inum = raw_instr[0] >> 12;

  if (inum == INSTR_ADD || inum == INSTR_ADD_IMM1 || inum == INSTR_ADD_IMM0) {
    ai->op = 0;
    ai->mode = inum - INSTR_ADD;
  }

  if (inum == INSTR_SUB || inum == INSTR_SUB_IMM1 || inum == INSTR_SUB_IMM0) {
    ai->op = 1;
    ai->mode = inum - INSTR_SUB;
  }

  if (inum == INSTR_MUL || inum == INSTR_MUL_IMM1 || inum == INSTR_MUL_IMM0) {
    ai->op = 2;
    ai->mode = inum - INSTR_MUL;
  }

  ai->out = (unsigned short) ((raw_instr[0] >> 8) & 0x0F);

  ai->in0 = (unsigned short) ((raw_instr[0] >> 4) & 0x0F);

  ai->in1 = (unsigned short) ((raw_instr[0]) & 0x0F);

  return ai;
}
