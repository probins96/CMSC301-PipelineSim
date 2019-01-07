// Palmer Robins

#ifndef __ASMPARSER_H__
#define __ASMPARSER_H__

#include "Instruction.h"
#include "OpcodeTable.h"
#include "RegisterTable.h"

#include <fstream>
#include <vector>

using namespace std;

/* This class reads in a MIPS assembly file and checks its syntax.  If
 * the file is syntactically correct, this class will retain a list 
 * of Instructions (one for each instruction from the file).  This
 * list of Instructions can be iterated through.
 */
class ASMParser {

public:

    // Specify a text file containing MIPS assembly instructions. Function
    // checks syntactic correctness of file and creates a list of Instructions.
    ASMParser(string filename);

    // Returns true if the file specified was syntactically correct. Otherwise,
    // returns false.
    bool isFormatCorrect() { return myFormatCorrect; };

    // Iterator that returns the next Instruction in the list of Instructions.
    Instruction getNextInstruction();

protected:

    vector<Instruction> myInstructions;

    int myIndex; // iterator index
    bool myFormatCorrect;

    RegisterTable registers; // encodings for registers
    OpcodeTable opcodes; // encodings of opcodes

private:

    int myLabelAddress; // Used to assign labels addresses

    // Decomposes a line of assembly code into strings for the opcode field and operands,
    // checking for syntax errors and counting the number of operands.
    void getTokens(string line, string& opcode, string* operand, int& num_operands);

    // Given an Opcode, a string representing the operands, and the number of operands,
    // breaks operands apart and stores fields into Instruction.
    bool getOperands(Instruction& i, Opcode o, string* operand, int operand_count);

    // Returns true if character is white space
    bool isWhitespace(char c) { return (c == ' ' || c == '\t'); };

    // Returns true if character is a digit
    bool isDigit(char c) { return (c >= '0' && c <= '9'); };

    // Returns true if character is an uppercase letter
    bool isAlphaUpper(char c) { return (c >= 'A' && c <= 'Z'); };

    // Returns true if character is a lowercase letter
    bool isAlphaLower(char c) { return (c >= 'a' && c <= 'z'); };

    // Returns true if character is a plus or minus
    bool isSign(char c) { return (c == '-' || c == '+'); };

    // Returns true if character is an alphabetic character
    bool isAlpha(char c) { return (isAlphaUpper(c) || isAlphaLower(c)); };

    // Returns true if s represents a valid decimal integer
    bool isNumberString(string s);

    // Converts a string to an integer.  Assumes s is something like "-231" and produces -231
    int cvtNumString2Number(string s);
    
};

#endif
