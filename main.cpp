#include <iostream>
#include "Processor.h"
#include "Instruction.h"
#include "ArithmeticInstruction.h"


int main() {

    // 8-bit stride , 1KB memory
    Memory<char>* pMem = new Memory<char>(1024);
    // 16 16-bit register file. + 4 default registers for special purposes( Program counter, FLAGS etc.. )
    RegisterFile<unsigned short>* pRegFile = new RegisterFile<unsigned short>(16);

    InstructionSet* isa = new InstructionSet();
    isa->addFactory(new ArithmeticInstruction::ArithmeticFactory());

    Processor* p = new Processor( pMem, pRegFile, isa->getDecoder() );

    InstructionSet::Encoder *encoder = isa->getEncoder();

    // Load instructions into the memory at location 0.
    char buf[1000];
    int memPos = 0;
    while( cin.getline( buf, 1000 ) ){

        if( string(buf).compare(";") == 0 )
            break;


        vector<unsigned short> bin_instr = encoder->encode(buf);
        pMem->writeMem(memPos, bin_instr.data(), 2 );
        // Ww're only putting in instructions that take up exactly 16 bits.
        memPos += 1;

    }

    // Run the processor.
    p->run();

    return 0;
}