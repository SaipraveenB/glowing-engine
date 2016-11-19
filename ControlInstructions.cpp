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

Instruction *HaltInstruction::HaltFactory::make(vector<unsigned short> raw_instr, unsigned short pc) {
  unsigned short inum = raw_instr[0] >> 12;
  if (inum != this->INSTR_HALT)
    throw std::runtime_error("inum != INSTR_HALT");

  HaltInstruction *instr = new HaltInstruction();
  instr->pc = pc;
  // Use the 4th pipe for Control instructions.
  instr->pipe = 3;
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
void HaltInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {

  advance();
}
void HaltInstruction::fetch(RegisterFile<unsigned short> *rf) {
  // Stall the pipeline.
  rf->spl(rf->REG_STALL) = 1;
  advance();
}

void HaltInstruction::memory(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  // Not used anymore.
  advance();
}
void HaltInstruction::write(RegisterFile<unsigned short> *rf) {
  // Stop the processor.
  rf->spl(rf->REG_HALT) = 1;
  //rf->spl(RegisterFile<unsigned short>::REG_PC) = ~static_cast<unsigned short>(0);
  advance();
}

string HaltInstruction::toStringSL() {
  string sup = Instruction::toStringSL();
  stringstream ss;
  ss << sup << " HaltInstr";
  return ss.str();
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

Instruction *ConditionalBranchInstruction::ConditionalBranchFactory::make(vector<unsigned short> raw_instr,
                                                                          unsigned short pc) {
  ConditionalBranchInstruction *instr = new ConditionalBranchInstruction();
  unsigned short inum = raw_instr[0] >> 12;

  if (inum != this->INSTR_C_BRANCH)
    throw std::runtime_error("inum != this->INSTR_C_BRANCH");

  unsigned short check_reg = (raw_instr[0] >> 8) & 0x0F;
  short pc_offset = static_cast<short>(raw_instr[0] & 0xFF);

  instr->check_reg = check_reg;
  instr->pc_offset = pc_offset;
  instr->marker_target = nullptr;
  instr->pc = pc;
  instr->pipe = 3;
  return instr;
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

Instruction *UnconditionalBranchInstruction::UnconditionalBranchFactory::make(vector<unsigned short> raw_instr,
                                                                              unsigned short pc) {
  UnconditionalBranchInstruction *instr = new UnconditionalBranchInstruction();
  unsigned short inum = raw_instr[0] >> 12;

  if (inum != this->INSTR_UC_BRANCH)
    throw std::runtime_error("inum != this->INSTR_UC_BRANCH");

  short pc_offset = static_cast<short> ( static_cast<char>(raw_instr[0] & 0xFF) );

  instr->pc_offset = pc_offset;
  instr->pc = pc;
  instr->pipe = 3;

  return instr;
}

/*void ConditionalBranchInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  if (target == 0)
    rf->spl(RegisterFile<unsigned short>::REG_PC) += this->pc_offset;
  else
    rf->spl(RegisterFile<unsigned short>::REG_PC) += 2;

  return;
}*/

void ConditionalBranchInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  if (target == 0) {

    // Reset the PC.
    rf->spl(RegisterFile<unsigned short>::REG_PC) = this->pc_offset + this->pc;

  } else
    // Reset the PC.
    rf->spl(RegisterFile<unsigned short>::REG_PC) = this->pc + (unsigned short) 2;

  // Unstall the pipeline.
  rf->spl(RegisterFile<unsigned short>::REG_STALL) = 0;

  advance();
}

void ConditionalBranchInstruction::fetch(RegisterFile<unsigned short> *rf) {
  // If the pipeline is not already stalled, flush it.
  // Flush the Decode and Fetch batches.
  if (rf->spl(rf->REG_STALL) == 0)
    rf->spl(rf->REG_FLUSH) = 1;

  // Stall the pipeline.
  rf->spl(RegisterFile<unsigned short>::REG_STALL) = 1;



  //target = rf->reg(this->check_reg);
  if ((marker_target = rf->try_get(this->check_reg, target, marker_target)) != nullptr) {
    // Stall.
    return;
  }

  advance();
}
void ConditionalBranchInstruction::memory(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  advance();
}
void ConditionalBranchInstruction::write(RegisterFile<unsigned short> *rf) {
  advance();
}
string ConditionalBranchInstruction::toStringSL() {
  string sup = Instruction::toStringSL();
  stringstream ss;
  ss << sup << " CondBranchInstr" << " target-wait:" << pcOf(marker_target) << " target:" << target;
  return ss.str();

}
void UnconditionalBranchInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {

  // Reset the PC to the appropriate value.
  rf->spl(RegisterFile<unsigned short>::REG_PC) = this->pc_offset + this->pc;

  // Revoke the stall and let the instructions load.
  rf->spl(RegisterFile<unsigned short>::REG_STALL) = 0;

  advance();
}
void UnconditionalBranchInstruction::fetch(RegisterFile<unsigned short> *rf) {

  // Stall the pipeline.
  rf->spl(RegisterFile<unsigned short>::REG_STALL) = 1;
  // Flush the Decode and Fetch batches.
  rf->spl(rf->REG_FLUSH) = 1;

  advance();
}
void UnconditionalBranchInstruction::memory(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  advance();
}
void UnconditionalBranchInstruction::write(RegisterFile<unsigned short> *rf) {
  advance();
}
string UnconditionalBranchInstruction::toStringSL() {
  string sup = Instruction::toStringSL();
  stringstream ss;
  ss << sup << " UnCondBranchInstr";
  return ss.str();
}
