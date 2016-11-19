//
// Created by Saipraveen B on 09/11/16.
//

#include "ALUPipe.h"

ALUPipe::ALUPipe(int pipe_size) : pipe_size(pipe_size) {

  for (int i = 0; i < pipe_size; i++)
    pipe.push_back(nullptr);

}

string ALUPipe::toString() {
  stringstream ss;
  ss << "Pipe Size: " << pipe_size << "\n";
  ss << "Reservation Station:\n";
  for (auto iw : rstation)
    ss << (iw ? iw->toString() : "*") << "\n";

  ss << "\nPipe:\n";
  for (auto iw : pipe)
    ss << (iw ? iw->toString() : "*") << "\n";

  return ss.str();
}
bool ALUPipe::isRSFull() {
  return rstation.size() >= RS_SIZE;
}

void ALUPipe::insert(InstructionWrapper *iw) {
  rstation.push_back(iw);
}

InstructionWrapper *ALUPipe::nextInstr() {

  InstructionWrapper *inp = nullptr;
  for (int i = rstation.size() - 1; i >= 0; i--) {
    // If in execute, ready transfer.
    if (rstation[i]->getState() == Instruction::PipeState::EXECUTE) {
      inp = rstation[i];
    }
  }
  return inp;

}

void ALUPipe::advanceRS(RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  for (int i = rstation.size() - 1; i >= 0; i--) {
    // If still in FETCH, fetch.
    if (rstation[i]->getState() == Instruction::PipeState::FETCH)
      rstation[i]->fetch(rf);
  }
}
bool ALUPipe::advance(bool blocked, RegisterFile<unsigned short> *rf, Memory<char> *mem) {
  InstructionWrapper *inp = nullptr;

  inp = nextInstr();

  InstructionWrapper *iw_ret = nullptr;
  for (int i = pipe_size - 1; i >= 0; i--) {
    if (i == pipe_size - 1 && pipe[i] && !blocked) {
      iw_ret = pipe[i];
      //pipe[i]->execute(rf);
      pipe[i] = nullptr;
      iw_ret->execute(rf, mem);
      //iw_ret->advance();
      continue;
    }
    if (i == pipe_size - 1) continue;
    if (pipe[i + 1] == nullptr) {
      pipe[i + 1] = pipe[i];
      pipe[i] = nullptr;
    }

  }

  if (pipe[0] == nullptr && inp) {
    pipe[0] = inp;
    // Throw away inp from the reservation station.
    rstation.erase(std::remove(rstation.begin(), rstation.end(), inp), rstation.end());
    return true;
  } else
    return false;

}