//
// Created by sauce on 2/9/16.
//

#include <sstream>
#include "MemoryInstructions.h"

namespace {
void split(const std::string &s, const char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  char item[100];
  while (ss.getline( item, 100, delim)) {
    elems.push_back(string(item));
    //std::cerr << "item :: " << item << "\n";
  }
}

std::vector<std::string> split(const std::string &s, const char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}
}


void LoadInstruction::LoadFactory::registerName(map<string, Instruction::Factory *> *directory,
                                                vector<Instruction::Factory *> *vec) {
  directory->insert(make_pair("LD", this));

  this->INSTR_LD_BASE_OFFSET = 12;

  (*vec)[this->INSTR_LD_BASE_OFFSET] = this;
  return;
}

// Generates an Instruction from a raw instruction
// 16-bit instruction :: INSTR_ID|DEST_REG|OFFSET|BASE_REG
Instruction *LoadInstruction::LoadFactory::make(vector<unsigned short> raw_instr) {
  LoadInstruction *li = new LoadInstruction();

  // Instruction ID is bits 15-12
  unsigned short inum = raw_instr[0] >> 12;

  if (inum != this->INSTR_LD_BASE_OFFSET)
    throw std::runtime_error("inum != INSTR_LD_BASE_OFFSET");

  // Dest reg is bits 11-8
  unsigned short dest_reg = (raw_instr[0] >> 8) & 0x0F;

  // Offset is bits 7-4
  unsigned short offset = (raw_instr[0] >> 4) & 0x0F;

  // Base reg is bits 3-0
  unsigned short base_reg = raw_instr[0] & 0x0F;

  li->base_register = base_reg;
  li->output_register = dest_reg;
  li->register_offset = offset;

  li->marker_pab = nullptr;
  li->marker_pao = nullptr;
  return li;
}

// Encodes the string into a raw instruction
vector<unsigned short> LoadInstruction::LoadFactory::encode(vector<string> tokens,
                                                            std::map<std::string, unsigned int> symbols) {
  // The first token is "LD"
  if (tokens[0] != "LD")
    throw std::runtime_error("tokens[0] != LD");

  unsigned short instr = 0;
  instr |= INSTR_LD_BASE_OFFSET;
  instr <<= 12;

  unsigned short dest_reg = static_cast<unsigned short>(std::stoi(tokens[1].substr(1)));
  const auto &strs = split(tokens[2], '[');
  unsigned short offset = static_cast<unsigned short>(std::stoi(strs[0].substr(1)));
  unsigned short base_reg = static_cast<unsigned short>(std::stoi(split(strs[1].substr(1), ']').at(0)));
  instr |= (dest_reg & 0x0F) << 8;
  instr |= (offset & 0x0F) << 4;
  instr |= (base_reg & 0x0F);

  return std::vector<unsigned short>(1, instr);
}

void StoreInstruction::StoreFactory::registerName(map<string, Instruction::Factory *> *directory,
                                                  vector<Instruction::Factory *> *vec) {
  directory->insert(make_pair("SD", this));

  this->INSTR_SD_BASE_OFFSET = 13;

  (*vec)[this->INSTR_SD_BASE_OFFSET] = this;

  return;
}

// 16-bit Instruction format :: INSTR_ID|OFFSET|BASE_REG|INPUT_REG
Instruction *StoreInstruction::StoreFactory::make(vector<unsigned short> raw_instr) {
  unsigned short instr_id = raw_instr[0] >> 12;
  if (instr_id != this->INSTR_SD_BASE_OFFSET)
    throw std::runtime_error("instr_id != INSTR_SD_BASE_OFFSET");

  unsigned short offset = (raw_instr[0] >> 8) & 0x0F;
  unsigned short base_reg = (raw_instr[0] >> 4) & 0x0F;
  unsigned short input_reg = raw_instr[0] & 0x0F;

  StoreInstruction *instr = new StoreInstruction();

  instr->input_register = input_reg;
  instr->base_register = base_reg;
  instr->register_offset = offset;
  instr->marker_data = nullptr;
  instr->marker_pao = nullptr;
  instr->marker_pab = nullptr;

  return instr;
}

// Encodes the string into a raw instruction
vector<unsigned short> StoreInstruction::StoreFactory::encode(vector<string> tokens,
                                                              std::map<std::string, unsigned int> symbols) {
  // The first token is "SD"
  if (tokens[0] != "SD")
    throw std::runtime_error("tokens[0] != SD");

  unsigned short instr = 0;
  instr |= INSTR_SD_BASE_OFFSET;
  instr <<= 12;

  const auto &strs = split(tokens[1], '[');
  unsigned short offset = static_cast<unsigned short>(std::stoi(strs[0].substr(1)));
  unsigned short base_reg = static_cast<unsigned short>(std::stoi(strs[1].substr(1)));
  unsigned short input_reg = static_cast<unsigned short>(std::stoi(tokens[2].substr(1)));
  instr |= (offset & 0x0F) << 8;
  instr |= (base_reg & 0x0F) << 4;
  instr |= (input_reg & 0x0F);

  return std::vector<unsigned short>(1, instr);
}
void StoreInstruction::execute(RegisterFile<unsigned short> *rf) {
  advance();
}
void StoreInstruction::fetch(RegisterFile<unsigned short> *rf) {
  unsigned short phy_addr_offset;// = rf->get(this->register_offset);
  unsigned short phy_addr_base;// = rf->get(this->base_register);
  if ((marker_pao = rf->try_get(this->register_offset, phy_addr_offset, marker_pao)) != nullptr) {
    // Stall.
    return;
  }

  if ((marker_pab = rf->try_get(this->base_register, phy_addr_base, marker_pab)) != nullptr) {
    // Stall.
    return;
  }

  phy_addr = phy_addr_base + phy_addr_offset;
  data = rf->get(this->input_register);

  if ((marker_data = rf->try_get(this->input_register, data, marker_data)) != nullptr) {
    // Stall.
    return;
  }

  advance();

}
void StoreInstruction::memory(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  mem->writeShort(phy_addr, data);
  advance();
}
void StoreInstruction::write(RegisterFile<unsigned short> *rf) {
  advance();
}

void LoadInstruction::execute(RegisterFile<unsigned short> *rf) {
  advance();
}
void LoadInstruction::fetch(RegisterFile<unsigned short> *rf) {
  unsigned short phy_addr_offset;// = rf->get(this->register_offset);
  unsigned short phy_addr_base;// = rf->get(this->base_register);
  if ((marker_pao = rf->try_get(this->register_offset, phy_addr_offset, marker_pao)) != nullptr) {
    // Stall.
    return;
  }

  if ((marker_pab = rf->try_get(this->base_register, phy_addr_base, marker_pab)) != nullptr) {
    // Stall.
    return;
  }

  phy_addr = phy_addr_base + phy_addr_offset;
  rf->mark_volatile(this->output_register, this);

  advance();
}
void LoadInstruction::memory(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  this->transfer_half_word = mem->readShort(phy_addr);
  rf->forward_operand(this->output_register, this, this->transfer_half_word);
  advance();
}
void LoadInstruction::write(RegisterFile<unsigned short> *rf) {
  rf->set(this->output_register, transfer_half_word);
  advance();
}
