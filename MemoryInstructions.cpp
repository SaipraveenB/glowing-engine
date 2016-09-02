//
// Created by sauce on 2/9/16.
//

#include "MemoryInstructions.h"

// Execute this instruction
void LoadInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  unsigned short phy_addr = rf->registers[this->base_register] + register_offset;

  unsigned short transfer_half_word;

  mem->readMem(phy_addr, &transfer_half_word, sizeof(unsigned short));

  rf->registers[this->output_register] = transfer_half_word;

  return;
}

// TODO(SaipraveenB|bitesandbytes) : Check correctness.
void LoadInstruction::LoadFactory::registerName(map<string, Instruction::Factory *> *directory,
                                                vector<Instruction::Factory *> *vec) {
  directory->insert(make_pair("LD", this));

  this->INSTR_LD_BASE_OFFSET = static_cast<unsigned short>(vec->size());
  vec->push_back(this);

  return;
}

// Generates an Instruction from a raw instruction
// Assuming the 16-bit instruction is of form (4-bit each):: INSTR_ID|DEST_REG|OFFSET|BASE_REG
Instruction *LoadInstruction::LoadFactory::make(vector<unsigned short> raw_instr) {
  LoadInstruction *li = new LoadInstruction();

  // Instruction ID is bits 15-12
  unsigned short inum = raw_instr[0] >> 12;

  if (inum != this->INSTR_LD_BASE_OFFSET)
    throw std::runtime_error("inum != INSTR_LD_BASE_OFFSET");

  // Dest reg is bits 11-8
  unsigned short dest_reg = (raw_instr[0] >> 8) & 0xe;

  // Offset is bits 7-4
  unsigned short offset = (raw_instr[0] >> 4) & 0xe;

  // Base reg is bits 3-0
  unsigned short base_reg = raw_instr[0] & 0xe;

  li->base_register = base_reg;
  li->output_register = dest_reg;
  li->register_offset = offset;

  return li;
}

// TODO(bitesandbytes) : Ask SaipraveenB for input format & fill in the code.
// Encodes the string into a raw instruction
vector<unsigned short> LoadInstruction::LoadFactory::encode(vector<string> tokens) {

}

void StoreInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  unsigned short phy_addr = this->register_offset + rf->registers[this->base_register];

  unsigned short data = rf->registers[this->input_register];

  mem->writeMem(phy_addr, &data, sizeof(unsigned short));

  return;
}

void StoreInstruction::StoreFactory::registerName(map<string, Instruction::Factory *> *directory,
                                                  vector<Instruction::Factory *> *vec) {
  directory->insert(make_pair("SD", this));

  this->INSTR_SD_BASE_OFFSET = static_cast<unsigned short>(vec->size());

  vec->push_back(this);

  return;
}

// 16-bit Instruction format :: INSTR_ID|OFFSET|BASE_REG|INPUT_REG
Instruction *StoreInstruction::StoreFactory::make(vector<unsigned short> raw_instr) {
  unsigned short instr_id = raw_instr[0] >> 12;
  if (instr_id != this->INSTR_SD_BASE_OFFSET)
    throw std::runtime_error("instr_id != INSTR_SD_BASE_OFFSET");

  unsigned short offset = (raw_instr[0] >> 8) & 0xe;
  unsigned short base_reg = (raw_instr[0] >> 4) & 0xe;
  unsigned short input_reg = raw_instr[0] & 0xe;

  StoreInstruction *instr = new StoreInstruction();

  instr->input_register = input_reg;
  instr->base_register = base_reg;
  instr->register_offset = offset;

  return instr;
}

// TODO(bitesandbytes) : Ask SaipraveenB for input format & fill in the code.
// Encodes the string into a raw instruction
vector<unsigned short> StoreInstruction::StoreFactory::encode(vector<string> tokens) {

}