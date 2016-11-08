//
// Created by Saipraveen B on 07/10/16.
//

#include <array>
#include "PipelinedProcessor.h"
//
// Created by Saipraveen B on 25/08/16.
//

#include "InstructionWrapper.h"

using namespace std;

#define PIPELINE_LENGTH 5
void PipelinedProcessor::run() {

  // Create a holder for pipeline stages and their instructions.
  std::array<InstructionWrapper *, PIPELINE_LENGTH> pipeline;
  for (int i = 0; i < PIPELINE_LENGTH; i++)
    pipeline[i] = nullptr;

  bool halted = false;

  // Infinite loop till HLT;
  for (;;) {
    // 2-Byte instruction buffer.

    std::vector<unsigned short> next_instr(1);

    // Iterate through all the stages and attempt to advance the instructions.
    for (int i = PIPELINE_LENGTH - 1; i >= 0; i--) {
      InstructionWrapper *instr = pipeline[i];

      if (!instr)
        continue;

      if (i != PIPELINE_LENGTH - 1 && (pipeline[i + 1] != nullptr)) {
        // Stall this instruction.
        continue;
      }

      switch (pipeline[i]->getState()) {

        case Instruction::PipeState::DECODE:
          instr->decode();
          break;
        case Instruction::PipeState::FETCH:
          instr->fetch(pRegFile);
          break;
        case Instruction::PipeState::EXECUTE:
          instr->execute(pRegFile);
          break;
        case Instruction::PipeState::MEMORY:
          instr->memory(pRegFile, pMem);
          break;
        case Instruction::PipeState::WRITE:
          instr->write(pRegFile);
          break;

      }

      // Handle branching.
      if (pRegFile->spl(RegisterFile<unsigned short>::REG_BSR) != 0) {
        // Branch.
        // Flush stages before execute.
        for (int i = 0; i < 3; i++) {
          pipeline[i] = nullptr;
          // Reset volatile registers for this statement.
          pRegFile->remove_volatility(pipeline[i]);
          iFlushes++;
        }

        // the pipeline can't be halted if a branch just happened.
        halted = false;
        //pRegFile->clear_volatility();

        // Reset branch state register( BSR ).
        pRegFile->spl(RegisterFile<unsigned short>::REG_BSR) = 0;
      }

      int new_stage = static_cast<int>( instr->getState());
      if (new_stage >= PIPELINE_LENGTH) {
        // Clear the stage.( Discard the instruction. )
        pipeline[i] = 0;
      }

      // Attach the instruction to the new pipe stage
      pipeline[new_stage] = pipeline[i];
      if (i != new_stage)
        // Clear the old stage.
        pipeline[i] = nullptr;
      else
        iStalls++;

    }

    // Tick the clock on the forwarding table.
    pRegFile->tick();

    // Termination condition.
    // End-of-pipe instruction is a halt instruction.
    if (pipeline[PIPELINE_LENGTH - 1] && pipeline[PIPELINE_LENGTH - 1]->isHalt())
      break;

    if (pipeline[1] && pipeline[1]->isHalt())
      halted = true;

    // Check if the processor has encountered a halt and do not load any more instructions.
    if (halted) {
      // Do this anyway or else branching instructions will fail.
      pRegFile->spl(RegisterFile<unsigned short>::REG_PC) += 2;
      continue;
    }

    auto pc = pRegFile->spl(RegisterFile<unsigned short>::REG_PC);
    // If PC is the highest possible value, no instruction can be here and it's thus a halt signal.
    if (pc == ~static_cast<unsigned short>(0))
      return;

    // Otherwise check if the pipe is free at the DECODE stage.
    if (pipeline[0] != nullptr)
      continue;

    // The first stage is clear. Go.
    next_instr[0] = this->pMem->readShort(pRegFile->spl(RegisterFile<unsigned short>::REG_PC));

    //Instruction *instr = decoder->decode(next_instr);
    InstructionWrapper *iw = new InstructionWrapper(next_instr, decoder);
    pipeline[0] = iw;

    iInstrLoaded++;
    // Extract the next instruction.
    pRegFile->spl(RegisterFile<unsigned short>::REG_PC) += 2;

  }

}

PipelinedProcessor::PipelinedProcessor(Memory<char> *mem,
                                       RegisterFile<unsigned short> *rf,
                                       InstructionSet::Decoder *decoder)
    : pMem(mem), pRegFile(rf), decoder(decoder), iStalls(0), iFlushes(0), iInstrLoaded(0) {
  rf->spl(RegisterFile<unsigned short>::REG_PC) = 0;
}
