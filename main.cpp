#include <iostream>
#include <sstream>
#include "Processor.h"
#include "ArithmeticInstruction.h"
#include "ControlInstructions.h"

void processCommand(string buf, Memory<char> *, RegisterFile<unsigned short> *);

void extractLR(string basic_string, unsigned short *pInt, unsigned short *pInt1);

int main() {

  // 8-bit stride , 1KB memory
  Memory<char> *pMem = new Memory<char>(1024);
  // 16 16-bit register file. + 4 default registers for special purposes( Program counter, FLAGS etc.. )
  RegisterFile<unsigned short> *pRegFile = new RegisterFile<unsigned short>(16);

  InstructionSet *isa = new InstructionSet();
  isa->addFactory(new ArithmeticInstruction::ArithmeticFactory());

  Processor *p = new Processor(pMem, pRegFile, isa->getDecoder());

  InstructionSet::Encoder *encoder = isa->getEncoder();



  // Load instructions into the memory at location 0.
  char buf[1000];
  unsigned int memPos = 0;

  char totalbuf[10000];
  stringstream program(totalbuf);

  while( cin.getline(buf, 1000) ){
    if (string(buf).compare(";") == 0)
      break;

    string bstr = string(buf);
    if( bstr.find(':') != string::npos ){

      stringstream ss(bstr);
      char c[100];
      ss.getline(c,100, ':');
      string x = string(c);
      encoder->symbols[x] = memPos;


      ss.getline(c,100,'\n');
      program << c << "\n";
      memPos += 2;
    }else if (buf[0] == '$') {
      // Initialisation command.
      processCommand(string(buf), pMem, pRegFile);

    }else if( bstr[0] == '#' ) {
      // A comment.
    }else if( bstr[0] == ' ' ) {
      // Not a valid instruction.
    }else{
      // Valid instruction.
      program << bstr;
      memPos += 2;
    }
  }

  std::cout<<"Total program size: " << memPos <<  std::endl;
  // Reset to start.
  memPos = 0;
  while (program.getline(buf, 1000)) {


    encoder->symbols["_PC_"] = memPos;

    vector<unsigned short> bin_instr = encoder->encode(buf);
    pMem->writeMem(memPos, bin_instr.data(), 2);
    // Ww're only putting in instructions that take up exactly 16 bits.
    memPos += 2;

  }

  // Run the processor.
  p->run();

  return 0;
}

void processCommand(string buf, Memory<char> *pMem, RegisterFile<unsigned short> *pRegFile) {
  string command = buf.substr(1);
  if (command[0] == 'R') {
    // Register fill command.
    string rcommand = command.substr(1);
    unsigned short l, r;
    extractLR(rcommand, &l, &r);

    pRegFile->reg(l) = r;
  } else {
    // Memory fill command.
    unsigned short l, r;
    extractLR(command, &l, &r);

    // Write 2 bytes into the memory.
    pMem->writeMem(l, &r, 2);
  }
}

void extractLR(string basic_string, unsigned short *p0, unsigned short *p1) {

  stringstream ss(basic_string);
  char sl[20];
  char sr[20];

  ss.getline(sl, 20, '$');
  ss >> sr;

  *p0 = (unsigned short) stoi(sl);
  *p1 = (unsigned short) stoi(sr);

}