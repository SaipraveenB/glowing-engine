//
// Created by Saipraveen B on 11/11/16.
//

#include "InOrderALUPipe.h"
InOrderALUPipe::InOrderALUPipe(int pipe_size) : ALUPipe(pipe_size) {

}
InstructionWrapper *InOrderALUPipe::nextInstr() {
  // If the top instruction is ready for execution.
  if (this->rstation.size() && this->rstation[0]->getState() == Instruction::PipeState::EXECUTE)
    return this->rstation[0];

  return nullptr;
}
