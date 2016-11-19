//
// Created by Saipraveen B on 26/08/16.
//


#include "ArithmeticInstruction.h"
void ArithmeticInstruction::ArithmeticFactory::registerName(map<string, Instruction::Factory *> *directory,
                                                            vector<Instruction::Factory *> *vec) {


  // Register names.
  directory->insert(make_pair("ADD", this));
  directory->insert(make_pair("SUB", this));
  directory->insert(make_pair("MUL", this));

  // Retrieve instruction numbers.
  INSTR_ADD = 0;
  (*vec)[INSTR_ADD] = this;
  INSTR_ADD_IMM0 = 1;
  (*vec)[INSTR_ADD_IMM0] = this;
  INSTR_ADD_IMM1 = 3;
  (*vec)[INSTR_ADD_IMM1] = this;

  INSTR_MUL = 2;
  (*vec)[INSTR_MUL] = this;
  INSTR_MUL_IMM0 = 5;
  (*vec)[INSTR_MUL_IMM0] = this;
  INSTR_MUL_IMM1 = 7;
  (*vec)[INSTR_MUL_IMM1] = this;

  INSTR_SUB = 4;
  (*vec)[INSTR_SUB] = this;
  INSTR_SUB_IMM0 = 9;
  (*vec)[INSTR_SUB_IMM0] = this;
  INSTR_SUB_IMM1 = 11;
  (*vec)[INSTR_SUB_IMM1] = this;
}

vector<unsigned short>
ArithmeticInstruction::ArithmeticFactory::encode(vector<string> tokens, std::map<std::string, unsigned int> symbols) {

  // Check for immediate operands.
  string instr_name = tokens[0];
  string in0 = tokens[2];
  string in1 = tokens[3];
  string out = tokens[1];

  unsigned short instr_offset = 0;

  unsigned short si0;
  unsigned short si1;
  unsigned short so;

  si0 = (unsigned short) std::stoi(in0.substr(1));
  si1 = (unsigned short) std::stoi(in1.substr(1));



  so = (unsigned short) std::stoi(out.substr(1));
  si0 &= 0xF;
  si1 &= 0xF;
  so &= 0xF;

  unsigned short instr_base = 0;
  unsigned short instr_full = 0;
  if (instr_name.compare("ADD") == 0) {
    instr_full = INSTR_ADD;
    if (in0[0] == '#') {
      instr_full = INSTR_ADD_IMM0;

    } else if (in1[0] == '#') {
      instr_full = INSTR_ADD_IMM1;

    }
  } else if (instr_name.compare("SUB") == 0) {
    instr_full = INSTR_SUB;
    if (in0[0] == '#') {
      instr_full = INSTR_SUB_IMM0;

    } else if (in1[0] == '#') {
      instr_full = INSTR_SUB_IMM1;

    }
  } else if (instr_name.compare("MUL") == 0) {
    instr_full = INSTR_MUL;
    if (in0[0] == '#') {
      instr_full = INSTR_MUL_IMM0;

    } else if (in1[0] == '#') {
      instr_full = INSTR_MUL_IMM1;

    }
  }

  //unsigned short instr_full = instr_base + instr_offset;

  unsigned short full = (instr_full << 12) | (so << 8) | (si0 << 4) | (si1);
  vector<unsigned short> v;
  v.push_back(full);
  return v;

}

Instruction *ArithmeticInstruction::ArithmeticFactory::make(vector<unsigned short> raw_instr, unsigned short pc) {

  ArithmeticInstruction *ai = new ArithmeticInstruction();

  unsigned short inum = raw_instr[0] >> 12;

  if (inum == INSTR_ADD || inum == INSTR_ADD_IMM1 || inum == INSTR_ADD_IMM0) {
    ai->op = 0;
    ai->pipe = 0;
    if (inum == INSTR_ADD)
      ai->mode = 0;
    else if (inum == INSTR_ADD_IMM0)
      ai->mode = 1;
    else
      ai->mode = 2;
  }

  if (inum == INSTR_SUB || inum == INSTR_SUB_IMM1 || inum == INSTR_SUB_IMM0) {
    ai->op = 1;
    ai->pipe = 0;
    if (inum == INSTR_SUB)
      ai->mode = 0;
    else if (inum == INSTR_SUB_IMM0)
      ai->mode = 1;
    else
      ai->mode = 2;
  }

  if (inum == INSTR_MUL || inum == INSTR_MUL_IMM1 || inum == INSTR_MUL_IMM0) {
    ai->op = 2;
    ai->pipe = 1;
    if (inum == INSTR_MUL)
      ai->mode = 0;
    else if (inum == INSTR_MUL_IMM0)
      ai->mode = 1;
    else
      ai->mode = 2;
  }

  ai->out = (unsigned short) ((raw_instr[0] >> 8) & 0x0F);

  ai->in0 = (unsigned short) ((raw_instr[0] >> 4) & 0x0F);

  ai->in1 = (unsigned short) ((raw_instr[0]) & 0x0F);

  // Initialize the operand forwarding markers
  ai->marker0 = nullptr;
  ai->marker1 = nullptr;
  ai->marked = false;
  ai->pc = pc;
  return ai;

}

void ArithmeticInstruction::execute(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  switch (op) {
    case 0:res = static_cast<unsigned short>( val1 + val0 );
      break;
    case 1:res = static_cast<unsigned short>( val0 - val1 );
      break;
    case 2:res = static_cast<unsigned short>( val1 * val0 );
      break;
  }

  // Forward operand. Later commit it.
  rf->forward_operand(this->out, this, res);

  advance();
}
void ArithmeticInstruction::fetch(RegisterFile<unsigned short> *rf) {

  unsigned short regval0;
  unsigned short regval1;

  // Mark the targets volatile even before we get the data so that instructions in the decode queue work properly.

  bool stall = false;
  if (mode == 0) {
    if (!this->done0 && (marker0 = rf->try_get(in0, regval0, marker0)) != nullptr) {
      // Stall.
      // return;
      stall = true;
    } else {
      if (!this->done0)
        val0 = static_cast<short>( regval0 );
      this->done0 = true;
    }

    if (!this->done1 && (marker1 = rf->try_get(in1, regval1, marker1)) != nullptr) {
      // Stall.
      // return;
      stall = true;
    } else {
      if (!this->done1)
        val1 = static_cast<short>( regval1 );
      this->done1 = true;
    }

    if (!marked) {
      rf->mark_volatile(out, this);
      marked = true;
    }

    if (stall) return;

  } else if (mode == 1) {
    val0 = static_cast<short>(in0 >> 3 ? in0 | 0xFFF0 : in0);
    if (!this->done1 && (marker1 = rf->try_get(in1, regval1, marker1)) != nullptr) {
      // Stall.
      // return;
      stall = true;
    } else {
      if (!this->done1)
        val1 = static_cast<short>( regval1 );
      this->done1 = true;

    }

    if (!marked) {
      rf->mark_volatile(out, this);
      marked = true;
    }
    if (stall) return;

  } else if (mode == 2) {

    val1 = static_cast<short>(in1 >> 3 ? in1 | 0xFFF0 : in1);
    if (!done0 && (marker0 = rf->try_get(in0, regval0, marker0)) != nullptr) {
      // Stall.
      //return;
      stall = true;
    } else {
      if (!this->done0)
        val0 = static_cast<short>( regval0 );

      this->done0 = true;

    }

    if (!marked) {
      rf->mark_volatile(out, this);
      marked = true;
    }
    if (stall) return;


  }


  advance();
}
void ArithmeticInstruction::memory(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  advance();
}

void ArithmeticInstruction::write(RegisterFile<unsigned short> *rf) {
  //rf->set(this->out, res);
  rf->commit_operands(this);
  advance();
}
string ArithmeticInstruction::toStringSL() {
  string sup = Instruction::toStringSL();
  stringstream ss;
  ss << sup << " ArithmeticInstr" << " val0-wait: " << pcOf(marker0) << " val1-wait: " << pcOf(marker1) << " val0: "
     << val0 << " val1: " << val1;
  return ss.str();
}
