//
// Created by Saipraveen B on 11/11/16.
//

#ifndef CSD_ASSIGNMENT2_INORDERALUPIPE_H
#define CSD_ASSIGNMENT2_INORDERALUPIPE_H

#include "ALUPipe.h"
class InOrderALUPipe : public ALUPipe {
 public:
  InOrderALUPipe(int pipe_size);

  InstructionWrapper *nextInstr();
};

#endif //CSD_ASSIGNMENT2_INORDERALUPIPE_H
