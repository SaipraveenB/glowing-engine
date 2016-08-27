//
// Created by Saipraveen B on 25/08/16.
//

#ifndef CSD_ASSIGNMENT2_DECODER_H
#define CSD_ASSIGNMENT2_DECODER_H

#include <iostream>
#include <vector>
#include <string>
#include "Instruction.h"

using namespace std;


class InstructionSet {
    std::vector<Instruction::Factory*> factories;
    const static unsigned short OP_TYPE_IMMEDIATE = 0x0000;
    const static unsigned short OP_TYPE_REGISTER = 0x8000;

    map<std::string, Instruction::Factory*> nameset;
    vector<Instruction::Factory *> constructors;

public:
    InstructionSet();
    int addFactory( Instruction::Factory* factory );

    class Decoder{
    public:
        Decoder(InstructionSet* is);
        Instruction* decode( vector<unsigned short> instr_raw );

        InstructionSet *set;
    };

    class Encoder{
    public:
        Encoder(InstructionSet* );
        vector<unsigned short> encode( string instr_line );

        InstructionSet *set;

    };

    Decoder* getDecoder();
    Encoder* getEncoder();

};


#endif //CSD_ASSIGNMENT2_DECODER_H
