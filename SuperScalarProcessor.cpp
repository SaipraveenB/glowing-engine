//
// Created by Saipraveen B on 07/10/16.
//

#include <array>
#include <queue>
#include "SuperScalarProcessor.h"
//
// Created by Saipraveen B on 25/08/16.
//

#include "InstructionWrapper.h"
#include "ALUPipe.h"
#include "InOrderALUPipe.h"

using namespace std;


#define PIPELINE_LENGTH 5
#define BATCH_SIZE 4
#define RS_SIZE 16
#define ROB_SIZE 16
#define NUM_ALUS 4

void SuperScalarProcessor::run() {

  // Create a holder for pipeline stages and their instructions.
  //std::array<InstructionWrapper *, PIPELINE_LENGTH> pipeline;
  std::vector<InstructionWrapper *> instr_load;
  std::vector<InstructionWrapper *> instr_data;
  std::vector<InstructionWrapper *> robuffer;
  std::array<ALUPipe *, NUM_ALUS> alupipes;

  //for (int i = 0; i < BATCH_SIZE; i++)
  //  instr_load[i] = nullptr;
  //for (int i = 0; i < BATCH_SIZE; i++)
  //  instr_data[i] = nullptr;

  // Add + Subtract
  alupipes[0] = new ALUPipe(2);
  // Multiply.
  alupipes[1] = new ALUPipe(4);
  // Memory operations.
  // Reservation Station for InOrderALUPipe issues in order only.
  alupipes[2] = new InOrderALUPipe(3);
  // Control ops.
  alupipes[3] = new ALUPipe(3);


  // Infinite loop till HLT;
  for (;;) {
    //TODO:printALUs();
    std::cout << "======================\n";
    std::cout << "DECODE Queue\n";
    for (auto iw : instr_load)
      std::cout << iw->toString() << std::endl;
    std::cout << "FETCH Queue\n";
    for (auto iw : instr_data)
      std::cout << iw->toString() << std::endl;

    std::cout << "ALUS:\n";
    for (auto alu : alupipes)
      std::cout << alu->toString() << std::endl << std::endl;
    //TODO:printROB();
    std::cout << "------------\n";
    std::cout << "RO Buffer:\n";
    for (auto k : robuffer)
      std::cout << k->toString() << std::endl;
    //TODO:printRegFile();
    std::cout << "-------------\n";
    std::cout << pRegFile->toString() << std::endl;

    // Check HALT register( set when the HALT instruction executes.
    if (pRegFile->spl(pRegFile->REG_HALT))
      // Stop execution
      break;



    // Advance the RO Buffer.
    // Check if the top instruction is complete and commit if true.
    for (int i = 0; i < BATCH_SIZE; i++) {

      if (robuffer.size() && robuffer.front()->getState() == Instruction::PipeState::WRITE) {
        robuffer.front()->write(pRegFile);
        robuffer.erase(robuffer.begin());
      } else
        break;

    }

    // Advance the ALUPipes
    for (ALUPipe *alupipe : alupipes) {
      alupipe->advance(false, pRegFile, pMem);
    }

    // Advance the RStations( within the pipe )
    for (ALUPipe *alupipe : alupipes) {
      alupipe->advanceRS(pRegFile, pMem);
    }

    // Advance the Fetch queue.
    for (int i = 0; i < BATCH_SIZE; i++) {

      if (robuffer.size() < ROB_SIZE && instr_data.size() && !pRegFile->spl(pRegFile->REG_STALL)) {

        InstructionWrapper *iw = instr_data.front();
        iw->fetch(pRegFile);

        // Remove from front of Instruction Decode queue.
        instr_data.erase(instr_data.begin());
        // Add to back of RO Buffer.
        robuffer.push_back(iw);
        // Add to the appropriate reseravation station.
        int idx = iw->getPipeIndex();
        alupipes[idx]->insert(iw);

        if (pRegFile->spl(pRegFile->REG_FLUSH)) {
          // Reset this.
          pRegFile->spl(pRegFile->REG_FLUSH) = 0;

          // Flush both Decode and Fetch queues. The Branch instruction is already in transit.
          instr_load.clear();
          instr_data.clear();
        }
      }

    }

    // Advance the Decode Queue.
    for (int i = 0; i < BATCH_SIZE; i++) {

      if (instr_data.size() < BATCH_SIZE && instr_load.size()) {
        InstructionWrapper *iw = instr_load.front();
        iw->decode();
        // Remove from front of Instruction Decode queue.
        instr_load.erase(instr_load.begin());
        // Add to back of Decode Queue.
        instr_data.push_back(iw);
      }

    }

    // Load new instructions.
    for (int i = 0; i < BATCH_SIZE; i++) {

      // If the Load queue has empty slots and if the pipeline is not stalled.
      if (instr_load.size() < BATCH_SIZE && !pRegFile->spl(pRegFile->REG_STALL)) {
        std::vector<unsigned short> next_instr(1);

        next_instr[0] = this->pMem->readShort(pRegFile->spl(RegisterFile<unsigned short>::REG_PC));

        InstructionWrapper
            *iw = new InstructionWrapper(next_instr, decoder, pRegFile->spl(RegisterFile<unsigned short>::REG_PC));

        instr_load.push_back(iw);

        pRegFile->spl(RegisterFile<unsigned short>::REG_PC) += 2;
      }

    }

  }

}

SuperScalarProcessor::SuperScalarProcessor(Memory<char> *mem,
                                           RegisterFile<unsigned short> *rf,
                                           InstructionSet::Decoder *decoder)
    : pMem(mem), pRegFile(rf), decoder(decoder), iStalls(0), iFlushes(0), iInstrLoaded(0) {
  rf->spl(RegisterFile<unsigned short>::REG_PC) = 0;
}

