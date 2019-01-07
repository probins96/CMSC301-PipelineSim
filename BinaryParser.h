// Palmer Robins

#ifndef __BINARYPARSER_H__
#define __BINARYPARSER_H__

#include "Instruction.h"
#include "OpcodeTable.h"
#include "RegisterTable.h"

#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

/**
 *  This class reads in a MIPS assembly file and checks its syntax.  If
 * the file is syntactically correct, this class will retain a list 
 * of Instructions (one for each instruction from the file).  This
 * list of Instructions can be iterated through.
 */
class BinaryParser {

public:

    // Specify a text file containing 32b encodings. Function
    // checks syntactic correctness of file and creates a list of Instructions.
    BinaryParser(string filename);

    // Returns true if the file specified was syntactically correct. Otherwise,
    // returns false.
    bool isFormatCorrect() { return myFormatCorrect; };

    // Iterator that returns the next Instruction in the list of Instructions.
    Instruction getNextInstruction();

private:

    vector<Instruction> myInstructions; // list of Instructions
    
    int myIndex; // iterator index
    bool myFormatCorrect;

    const static int ENCODED_INST_LENGTH = 32; // The length of an encoded MIPS instruction
    const static int OPCODE_LENGTH = 6; // Length of an opcode is 6 bits
    const static int REGISTER_LENGTH = 5; // Length of encoded register is 5 bits
    const static int FUNC_FIELD_LOCATION = 26; // The encoded function field begins at bit 26

    OpcodeTable myOpcodes; // encodings of opcodes

    // This function checks the syntax of a binary MIPS instruction
    // Receives a string representing a single instruction
    // Returns a boolean which is true if syntax is correct
    bool checkInstSyntax(string inst);

    // This function receives a 32 bit line representing a MIPS instruction,
    // and returns the opcode field of that instruction
    string getOpcodeField(string line);

    // This function receives a 32 bit line representing a MIPS instruction
    // and returns the last 6 bits, where the function field of an RTYPE
    // command is located.
    string getFuncField(string line);

    // This function receives a 32b line representing a MIPS instruction,
    // and returns the instruction without the 6b opcode.
    string getLineNoOpcode(string line);

    // This function returns a string representing the assembly code of
    // a single MIPS instruction
    string createAssemblyCode(Instruction i);

    // This function takes in one instruction, its opcode, and 
    // its line and determines which instruction type's decode
    // function needs to be called
    // Fields stored in an instruction instance
    bool decode(Instruction& i, Opcode opcode, string& line);

    // This function separates an RType instruction into the fields needed to
    // print the assembly representation.
    // Fields stored in an Instruction instance
    bool decodeRType(Instruction& i, Opcode opcode, string& lineWithoutOpcode);

    // This function separates an IType instruction into the fields needed to
    // print the assembly representation.
    // Fields stored in an Instruction instance
    bool decodeIType(Instruction& i, Opcode opcode, string& lineWithoutOpcode);

    // This function separates an JType instruction into the fields needed to
    // print the assembly representation.
    // Fields stored in an Instruction instance
    bool decodeJType(Instruction& i, Opcode opcode, string& lineWithoutOpcode);

    // This function uses the RType fields to set the values of an instruction data type
    // Returns a string containing the instruction in MIPS assembly
    string writeRTypeDecoded(Instruction i);

    // This function uses the IType fields to set the values of an instruction data type
    // Returns a string containing the instruction in MIPS assembly
    string writeITypeDecoded(Instruction i);

    // This function uses the JType fields to set the values of an instruction data type
    // Returns a string containing the instruction in MIPS assembly
    string writeJTypeDecoded(Instruction i);

    // This function converts a binary register value into a decimal string
    Register convertRegisterToAssembly(string& reg, int start, int finish);

    // This function converts a binary string in two's complement
    // into a decimal value (as a string). Used in IType conversion.
    int convertTwosComplementToDecimal(string str);

    // Returns true if character is a digit
    bool isOneOrZero(char c) { return (c == '0' || c == '1'); };
    
};

#endif