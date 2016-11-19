#include <iostream>
#include <sstream>
#include <regex>
#include <fstream>
#include "Processor.h"
#include "ArithmeticInstruction.h"
#include "ControlInstructions.h"
#include "MemoryInstructions.h"
#include "SuperScalarProcessor.h"

void processCommand(string buf, Memory<char> *, RegisterFile<unsigned short> *);

void extractLR(string basic_string, unsigned short *pInt, unsigned short *pInt1);

int main( int argc, char** argv ) {

  // 8-bit stride , 1KB memory
  Memory<char> *pMem = new Memory<char>(1024);
  // 16 16-bit register file. + 4 default registers for special purposes( Program counter, FLAGS etc.. )
  RegisterFile<unsigned short> *pRegFile = new RegisterFile<unsigned short>(16);

  InstructionSet *isa = new InstructionSet();
  isa->addFactory(new ArithmeticInstruction::ArithmeticFactory());
  isa->addFactory(new LoadInstruction::LoadFactory());
  isa->addFactory(new StoreInstruction::StoreFactory());
  isa->addFactory(new ConditionalBranchInstruction::ConditionalBranchFactory());
  isa->addFactory(new UnconditionalBranchInstruction::UnconditionalBranchFactory());
  isa->addFactory(new HaltInstruction::HaltFactory());

  SuperScalarProcessor *p = new SuperScalarProcessor(pMem, pRegFile, isa->getDecoder());

  InstructionSet::Encoder *encoder = isa->getEncoder();

  // Load instructions into the memory at location 0.
  char buf[1000];
  unsigned short memPos = 0;

  char totalbuf[10000];
  stringstream program(totalbuf);

  ifstream fin(argv[1]);
  while (fin.getline(buf, 1000)) {
    //if (string(buf).compare(";") == 0)
    //  break;

    string bstr = string(buf);
    bstr = std::regex_replace(bstr, std::regex("^ +| +$|( ) +"), "$1");
    if (bstr.find(':') != string::npos) {

      stringstream ss(bstr);
      char c[100];
      ss.getline(c, 100, ':');
      string x = string(c);
      encoder->symbols[x] = memPos;

      ss.getline(c, 100, '\n');
      std::string str(c);
      str = std::regex_replace(str, std::regex("^ +| +$|( ) +"), "$1");
      program << str << "\n";
      memPos += 2;
    } else if (buf[0] == '$') {
      // Initialisation command.
      processCommand(string(buf), pMem, pRegFile);

    } else if (bstr[0] == '#') {
      // A comment.
    } else if (bstr[0] == ' ') {
      // Not a valid instruction.

    } else {
      // Valid instruction.

      program << bstr << "\n";
      memPos += 2;
    }
  }

  //std::cout << "Total program size: " << memPos << std::endl;
  // Reset to start.
  memPos = 0;
  while (program.getline(buf, 1000)) {
    encoder->symbols["_PC_"] = memPos;
    vector<unsigned short> bin_instr = encoder->encode(buf, encoder->symbols);
    pMem->writeShort(memPos, bin_instr[0]);
    //pMem->writeMem(memPos, bin_instr.data(), 2);
    // Ww're only putting in instructions that take up exactly 16 bits.
    memPos += 2;
  }

  // Run the processor.
  p->run();

  ofstream fout(argv[2]);
  fout << pMem->dumpMem();
  fout.close();
  std::cout << "Stalls: " << p->iStalls << std::endl;
  std::cout << "Flushes: " << p->iFlushes << std::endl;
  std::cout << "Total Instructions: " << p->iInstrLoaded << std::endl;
  std::cout << "Total efficiency: " << 1 - ((p->iStalls + p->iFlushes * 2) / (float) p->iInstrLoaded) << std::endl;

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
    pMem->writeShort(l, r);
    //pMem->writeMem(l, &r, 2);
  }
}

void extractLR(string basic_string, unsigned short *p0, unsigned short *p1) {

  stringstream ss(basic_string);
  char sl[20];
  char sr[20];

  ss.getline(sl, 20, '$');
  ss >> sr;

  *p0 = static_cast<unsigned short> ( stoi(string(sl)));
  *p1 = static_cast<unsigned short> ( stoi(string(sr)));

}