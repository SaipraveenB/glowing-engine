//
// Created by sauce on 3/9/16.
//
#include <sstream>
#include "ControlInstructions.h"

namespace {
void split(const std::string &s, const char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  char item[100];
  while (ss.getline( item, 100, delim)) {
    elems.push_back(string(item));
    std::cerr << "item :: " << item << "\n";
  }
}

std::vector<std::string> split(const std::string &s, const char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}
}

void HaltInstruction::HaltFactory::registerName(map<string, Instruction::Factory *> *directory,
                                                vector<Instruction::Factory *> *vec) {
  directory->insert(make_pair("HLT", this));

  this->INSTR_HALT = static_cast<unsigned short>(vec->size());

  vec->push_back(this);
}

Instruction *HaltInstruction::HaltFactory::make(vector<unsigned short> raw_instr) {
  unsigned short inum = raw_instr[0] >> 12;
  if (inum != this->INSTR_HALT)
    throw std::runtime_error("inum != INSTR_HALT");

  HaltInstruction *instr = new HaltInstruction();

  return instr;
}

vector<unsigned short> HaltInstruction::HaltFactory::encode(vector<string> tokens) {
  if (tokens[0] != "HLT")
    throw std::runtime_error("tokens[0] != HLT");

  unsigned short instr = 0;
  instr |= this->INSTR_HALT;
  instr <<= 12;

  return std::vector<unsigned short>(1, instr);
}

void ConditionalBranchInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  if (rf->reg(this->check_reg) == 0)
    rf->spl(RegisterFile<unsigned short>::REG_PC) += this->pc_offset;

  return;
}

void ConditionalBranchInstruction::ConditionalBranchFactory::registerName(map<string,
                                                                              Instruction::Factory *> *directory,
                                                                          vector<Instruction::Factory *> *vec) {
  directory->insert(make_pair("BEQZ", this));

  this->INSTR_C_BRANCH = static_cast<unsigned short>(vec->size());

  vec->push_back(this);

  return;
}

vector<unsigned short> ConditionalBranchInstruction::ConditionalBranchFactory::encode(vector<string> tokens) {
  if (tokens[0] != "BEQZ")
    throw std::runtime_error("tokens[0] != BEQZ");

  const auto &strs = split(tokens[1], '(');

  // TODO(bitesandbytes) : DEBUG : check for size of strs.
  unsigned short reg_num = static_cast<unsigned short>(std::stoi(strs[0]));
  unsigned short pc_offset = static_cast<unsigned short>(std::stoi(tokens[2]));

  unsigned short instr = (this->INSTR_C_BRANCH & 0x0F) << 12;
  instr |= (reg_num & 0x0F) << 8;

  // 8-bit PC offset.
  instr |= (pc_offset & 0xFF);

  return std::vector<unsigned short>(1, instr);
}

Instruction *ConditionalBranchInstruction::ConditionalBranchFactory::make(vector<unsigned short> raw_instr) {
  ConditionalBranchInstruction *instr = new ConditionalBranchInstruction();
  unsigned short inum = raw_instr[0] >> 12;

  if (inum != this->INSTR_C_BRANCH)
    throw std::runtime_error("inum != this->INSTR_C_BRANCH");

  unsigned short check_reg = (raw_instr[0] >> 8) & 0x0F;
  unsigned short pc_offset = (raw_instr[0] & 0xFF);

  instr->check_reg = check_reg;
  instr->pc_offset = pc_offset;

  return instr;
}
void UnconditionalBranchInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  rf->spl(RegisterFile<unsigned short>::REG_PC) += this->pc_offset;
  return;
}

void UnconditionalBranchInstruction::UnconditionalBranchFactory::registerName(map<string,
                                                                                  Instruction::Factory *> *directory,
                                                                              vector<Instruction::Factory *> *vec) {
  directory->insert(make_pair("JMP", this));

  this->INSTR_UC_BRANCH = static_cast<unsigned short>(vec->size());

  vec->push_back(this);

  return;
}

vector<unsigned short> UnconditionalBranchInstruction::UnconditionalBranchFactory::encode(vector<string> tokens) {
  if (tokens[0] != "JMP")
    throw std::runtime_error("tokens[0] != JMP");

  unsigned short pc_offset = static_cast<unsigned short>(std::stoi(tokens[1]));

  unsigned short instr = (this->INSTR_UC_BRANCH & 0x0F) << 12;
  // 8-bit PC offset.
  instr |= (pc_offset & 0xFF);

  return std::vector<unsigned short>(1, instr);
}

Instruction *UnconditionalBranchInstruction::UnconditionalBranchFactory::make(vector<unsigned short> raw_instr) {
  ConditionalBranchInstruction *instr = new ConditionalBranchInstruction();
  unsigned short inum = raw_instr[0] >> 12;

  if (inum != this->INSTR_UC_BRANCH)
    throw std::runtime_error("inum != this->INSTR_UC_BRANCH");

  unsigned short pc_offset = (raw_instr[0] & 0xFF);

  instr->pc_offset = pc_offset;

  return instr;
}