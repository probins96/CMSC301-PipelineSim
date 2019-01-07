// Palmer Robins

#include "OpcodeTable.h"

// Initializes all the fields for every instruction in Opcode enum
OpcodeTable::OpcodeTable() {

    myArray[ADD].name = "add";
    myArray[ADD].numOps = 3;
    myArray[ADD].rdPos = 0;
    myArray[ADD].rsPos = 1;
    myArray[ADD].rtPos = 2;
    myArray[ADD].immPos = -1;
    myArray[ADD].instType = RTYPE;
    myArray[ADD].op_field = "000000";
    myArray[ADD].funct_field = "100000";

    myArray[ADDI].name = "addi";
    myArray[ADDI].numOps = 3;
    myArray[ADDI].rdPos = -1;
    myArray[ADDI].rsPos = 1;
    myArray[ADDI].rtPos = 0;
    myArray[ADDI].immPos = 2;
    myArray[ADDI].instType = ITYPE;
    myArray[ADDI].op_field = "001000";

    myArray[XOR].name = "xor";
    myArray[XOR].numOps = 3;
    myArray[XOR].rdPos = 0;
    myArray[XOR].rsPos = 1;
    myArray[XOR].rtPos = 2;
    myArray[XOR].immPos = -1;
    myArray[XOR].instType = RTYPE;
    myArray[XOR].op_field = "000000";
    myArray[XOR].funct_field = "100110";

    myArray[MULT].name = "mult";
    myArray[MULT].numOps = 2;
    myArray[MULT].rdPos = -1;
    myArray[MULT].rsPos = 0;
    myArray[MULT].rtPos = 1;
    myArray[MULT].immPos = -1;
    myArray[MULT].instType = RTYPE;
    myArray[MULT].op_field = "000000";
    myArray[MULT].funct_field = "011000";

    myArray[MFLO].name = "mflo";
    myArray[MFLO].numOps = 1;
    myArray[MFLO].rdPos = 0;
    myArray[MFLO].rsPos = -1;
    myArray[MFLO].rtPos = -1;
    myArray[MFLO].immPos = -1;
    myArray[MFLO].instType = RTYPE;
    myArray[MFLO].op_field = "000000";
    myArray[MFLO].funct_field = "010010";

    myArray[SLL].name = "sll";
    myArray[SLL].numOps = 3;
    myArray[SLL].rdPos = 0;
    myArray[SLL].rsPos = -1;
    myArray[SLL].rtPos = 1;
    myArray[SLL].immPos = 2;
    myArray[SLL].instType = RTYPE;
    myArray[SLL].op_field = "000000";
    myArray[SLL].funct_field = "000000";

    myArray[SLT].name = "slt";
    myArray[SLT].numOps = 3;
    myArray[SLT].rdPos = 0;
    myArray[SLT].rsPos = 1;
    myArray[SLT].rtPos = 2;
    myArray[SLT].immPos = -1;
    myArray[SLT].instType = RTYPE;
    myArray[SLT].op_field = "000000";
    myArray[SLT].funct_field = "101010";

    myArray[SLTI].name = "slti";
    myArray[SLTI].numOps = 3;
    myArray[SLTI].rdPos = -1;
    myArray[SLTI].rsPos = 1;
    myArray[SLTI].rtPos = 0;
    myArray[SLTI].immPos = 2;
    myArray[SLTI].instType = ITYPE;
    myArray[SLTI].op_field = "001010";

    myArray[LB].name = "lb";
    myArray[LB].numOps = 3;
    myArray[LB].rdPos = -1;
    myArray[LB].rsPos = 2;
    myArray[LB].rtPos = 0;
    myArray[LB].immPos = 1;
    myArray[LB].instType = ITYPE;
    myArray[LB].isMemoryInstr = true;
    myArray[LB].op_field = "100000";

    myArray[J].name = "j";
    myArray[J].numOps = 1;
    myArray[J].rdPos = -1;
    myArray[J].rsPos = -1;
    myArray[J].rtPos = -1;
    myArray[J].immPos = 0;
    myArray[J].instType = JTYPE;
    myArray[J].immLabel = true;
    myArray[J].op_field = "000010";

    myArray[BEQ].name = "beq";
    myArray[BEQ].rdPos = -1;
    myArray[BEQ].rsPos = 0;
    myArray[BEQ].rtPos = 1;
    myArray[BEQ].immPos = 2;
    myArray[BEQ].instType = ITYPE;
    myArray[BEQ].immLabel = true;
    myArray[BEQ].op_field = "000100";
}

// Given a valid MIPS assembly mnemonic, returns an Opcode which represents a
// template for that instruction.
Opcode OpcodeTable::getOpcode(string str) {
    for (int i = 0; i < (int)UNDEFINED; i++) {
        if (myArray[i].name == str) {
            return (Opcode)i;
        }
    }
    return UNDEFINED;
}
// Given a valid MIPS opcode field, returns an Opcode which represents a
// template for that instruction.
// Function field parameter is used if the instruction is an RTYPE
Opcode OpcodeTable::getOpcode_fromBinary(string opcode_field, string func_field) {
    
    // If the instruction is not RTYPE, it can be identified through its unique opcode field
    int matches = 0; // keeps track of how many entries in the array match the opcode_field
    int lastMatched = 0;
    for (int i = 0; i < (int)UNDEFINED; i++)
        if (myArray[i].op_field == opcode_field) {
            matches++;
            lastMatched = i; // store the last entry that matched
        }

    // If there was only 1 match, return that opcode
    if (matches == 1)
        return (Opcode)lastMatched;
    else if (matches == 0)
        return UNDEFINED;

    // If the instruction is an RTYPE, we can use the function field as the str
    for (int i = 0; i < (int)UNDEFINED; i++)
        if (myArray[i].funct_field == func_field)
            return (Opcode)i;

    return UNDEFINED;
}

// Given a valid MIPS opcode field, returns the corresponding Opcode name
string OpcodeTable::getOpcodeName(Opcode o) {
    return myArray[o].name;
}

// Given an Opcode, returns number of expected operands.
int OpcodeTable::numOperands(Opcode o) {
    if (o < 0 || o >= UNDEFINED)
        return -1;

    return myArray[o].numOps;
}

// Given an Opcode, returns the position of RS field.  If field is not
// appropriate for this Opcode, returns -1.
int OpcodeTable::RSposition(Opcode o) {
    if (o < 0 || o >= UNDEFINED)
        return -1;

    return myArray[o].rsPos;
}

// Given an Opcode, returns the position of RT field.  If field is not
// appropriate for this Opcode, returns -1.
int OpcodeTable::RTposition(Opcode o) {
    if (o < 0 || o >= UNDEFINED)
        return -1;

    return myArray[o].rtPos;
}

// Given an Opcode, returns the position of RD field.  If field is not
// appropriate for this Opcode, returns -1.
int OpcodeTable::RDposition(Opcode o) {
    if (o < 0 || o >= UNDEFINED)
        return -1;

    return myArray[o].rdPos;
}

// Given an Opcode, returns the position of IMM field.  If field is not
// appropriate for this Opcode, returns -1.
int OpcodeTable::IMMposition(Opcode o) {
    if (o < 0 || o >= UNDEFINED)
        return -1;

    return myArray[o].immPos;
}

// Given an Opcode, returns instruction type.
InstType OpcodeTable::getInstType(Opcode o) {
    if (o < 0 || o > UNDEFINED)
        return (InstType) - 1;

    return myArray[o].instType;
}

// Given an Opcode, returns a string representing the binary encoding of the opcode
// field.
string OpcodeTable::getOpcodeField(Opcode o) {
    if (o < 0 || o > UNDEFINED)
        return string("");

    return myArray[o].op_field;
}

// Given an Opcode, returns a string representing the binary encoding of the function
// field.
string OpcodeTable::getFunctField(Opcode o) {
    if (o < 0 || o > UNDEFINED)
        return string("");

    return myArray[o].funct_field;
}

// Given an Opcode, returns true if instruction expects a label in the instruction.
// See "J".
bool OpcodeTable::isIMMLabel(Opcode o) {
    if (o < 0 || o > UNDEFINED)
        return false;

    return myArray[o].immLabel;
}

// Given an opcode, returns true if instruction loads or writes to memory
// Example: "lb"
bool OpcodeTable::isMemoryInstr(Opcode o) {
    if (o < 0 || o > UNDEFINED)
        return false;

    return myArray[o].isMemoryInstr;
}