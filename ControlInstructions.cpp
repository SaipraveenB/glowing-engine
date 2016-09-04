//
// Created by sauce on 3/9/16.
//
#include <sstream>
#include <regex>
#include "ControlInstructions.h"

namespace {
void split(const std::string &s, const char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  char item[100];
  while (ss.getline(item, 100, delim)) {
    elems.push_back(string(item));
  }
}

std::vector<std::string> split(const std::string &s, const char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
  //std::cerr;
}
}

void HaltInstruction::HaltFactory::registerName(map<string, Instruction::Factory *> *directory,
                                                vector<Instruction::Factory *> *vec) {
  directory->insert(make_pair("HLT", this));

  this->INSTR_HALT = 6;

  (*vec)[this->INSTR_HALT] = this;
}

Instruction *HaltInstruction::HaltFactory::make(vector<unsigned short> raw_instr) {
  unsigned short inum = raw_instr[0] >> 12;
  if (inum != this->INSTR_HALT)
    throw std::runtime_error("inum != INSTR_HALT");

  HaltInstruction *instr = new HaltInstruction();

  return instr;
}

vector<unsigned short> HaltInstruction::HaltFactory::encode(vector<string> tokens,
                                                            std::map<std::string, unsigned int> symbols) {
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
  else
    rf->spl(RegisterFile<unsigned short>::REG_PC) += 2;

  return;
}

void ConditionalBranchInstruction::ConditionalBranchFactory::registerName(map<string,
                                                                              Instruction::Factory *> *directory,
                                                                          vector<Instruction::Factory *> *vec) {
  directory->insert(make_pair("BEQZ", this));

  this->INSTR_C_BRANCH = 8;

  (*vec)[this->INSTR_C_BRANCH] = this;

  return;
}

vector<unsigned short> ConditionalBranchInstruction::ConditionalBranchFactory::encode(vector<string> tokens,
                                                                                      std::map<std::string,
                                                                                               unsigned int> symbols) {
  if (tokens[0] != "BEQZ")
    throw std::runtime_error("tokens[0] != BEQZ");

  tokens[1].erase(std::remove(tokens[1].begin(), tokens[1].end(), '('), tokens[1].end());
  tokens[1].erase(std::remove(tokens[1].begin(), tokens[1].end(), ')'), tokens[1].end());

  unsigned short reg_num = static_cast<unsigned short>(std::stoi(tokens[1].substr(1)));
  short pc_offset = static_cast<short>(symbols[tokens[2]] - symbols["_PC_"]);

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
  short pc_offset = static_cast<short>(raw_instr[0] & 0xFF);

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

  this->INSTR_UC_BRANCH = 10;

  (*vec)[this->INSTR_UC_BRANCH] = this;

  return;
}

vector<unsigned short> UnconditionalBranchInstruction::UnconditionalBranchFactory::encode(vector<string> tokens,
                                                                                          std::map<std::string,
                                                                                                   unsigned int> symbols) {
  if (tokens[0] != "JMP")
    throw std::runtime_error("tokens[0] != JMP");

  short pc_offset = static_cast<short>(symbols[tokens[1]] - symbols["_PC_"]);

  unsigned short instr = (this->INSTR_UC_BRANCH & 0x0F) << 12;
  // 8-bit PC offset.
  instr |= (static_cast<signed char>(pc_offset) & 0xFF);

  return std::vector<unsigned short>(1, instr);
}

Instruction *UnconditionalBranchInstruction::UnconditionalBranchFactory::make(vector<unsigned short> raw_instr) {
  ConditionalBranchInstruction *instr = new ConditionalBranchInstruction();
  unsigned short inum = raw_instr[0] >> 12;

  if (inum != this->INSTR_UC_BRANCH)
    throw std::runtime_error("inum != this->INSTR_UC_BRANCH");

  short pc_offset = static_cast<short>(raw_instr[0] & 0xFF);

  instr->pc_offset = pc_offset;

  return instr;
}