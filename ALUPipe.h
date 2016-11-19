//
// Created by Saipraveen B on 09/11/16.
//

#ifndef CSD_ASSIGNMENT2_ALUPIPE_H
#define CSD_ASSIGNMENT2_ALUPIPE_H

#include <array>
#include "InstructionWrapper.h"
#define RS_SIZE 4
class ALUPipe {
 public:
  std::vector<InstructionWrapper *> pipe;
  int pipe_size;
  std::vector<InstructionWrapper *> rstation;

  ALUPipe(int pipe_size);

  bool advance(bool blocked, RegisterFile<unsigned short> *rf, Memory<char> *mem);
  virtual InstructionWrapper *nextInstr();
  void insert(InstructionWrapper *iw);
  bool isRSFull();
  string toString();
  void advanceRS(RegisterFile<unsigned short> *rf, Memory<char> *mem);
};

#endif //CSD_ASSIGNMENT2_ALUPIPE_H
