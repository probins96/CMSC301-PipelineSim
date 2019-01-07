// Palmer Robins

#include "Instruction.h"

// Creates a default instruction that has the opcode UNDEFINED
Instruction::Instruction() {
    myOpcode = UNDEFINED;
    myRS = myRT = myRD = NumRegisters;
    myAssembly = "";
}

// Constructs new instruction and initializes fields according to arguments:
// opcode, first source register, second source register, destination
// register, and immediate value
Instruction::Instruction(Opcode op, Register rs, Register rt, Register rd, int imm) {
    setValues(op, rs, rt, rd, imm);
}

// Allows you to set all the fields of the Instruction:
// opcode, first source register, second source register, destination
// register, and immediate value
void Instruction::setValues(Opcode op, Register rs, Register rt, Register rd, int imm) {

    myOpcode = op;
    if (op < 0 || op >= UNDEFINED)
        myOpcode = UNDEFINED;

    myRS = rs;
    if (rs < 0 || rs >= NumRegisters)
        myRS = NumRegisters;

    myRT = rt;
    if (rt < 0 || rt >= NumRegisters)
        myRT = NumRegisters;

    myRD = rd;
    if (rd < 0 || rd >= NumRegisters)
        myRD = NumRegisters;

    myImmediate = imm;
    if (!((imm & 0xFFFF0000) << 1)) // make sure it has nothing in upper 16 bits
        myImmediate = imm;
}

// Returns the type of instruction
InstType Instruction::getInstType() {
    OpcodeTable opTable;
    return opTable.getInstType(myOpcode);
}
