// Palmer Robins

#include "BinaryParser.h"

// Specify a text file containing MIPS machine instructions. Function
// checks syntactic correctness of file and creates a list of Instructions.
BinaryParser::BinaryParser(string filename) {

    Instruction i;
    myFormatCorrect = true;

    // Try to open the input file
    ifstream in;
    in.open(filename.c_str());

    // There was a problem opening the file
    if (in.bad())
        myFormatCorrect = false;
    else {
        string line;
        //For every instruction in the input file
        while (getline(in, line)) {
            // Check the syntax of the line
            if (!checkInstSyntax(line)) {
                myFormatCorrect = false;
                break;
            }

            //Get the opcode field and function field
            string opcode_field = getOpcodeField(line);
            string func_field = getFuncField(line);

            // Get the opcode as an enum Opcode & check its validity
            Opcode opcode = myOpcodes.getOpcode_fromBinary(opcode_field, func_field);
            if (opcode == UNDEFINED) {
                myFormatCorrect = false;
                break;
            }

            // Decode the line
            bool success = decode(i, opcode, line);

            // Did the decoding process work correctly
            if (!success) {
                myFormatCorrect = false;
                break;
            }

            // Create MIPS assembly string
            string assemblyInstruction = createAssemblyCode(i);

            // Set the assembly string into the instruction instance
            i.setAssembly(assemblyInstruction);

            // Add it to our vector of instructions
            myInstructions.push_back(i);
        }
    }
    in.close();
    myIndex = 0;
}

// This function checks the syntax of a binary MIPS instruction
// Receives a string representing a single instruction
// Returns a boolean which is true if syntax is correct
bool BinaryParser::checkInstSyntax(string inst) {

    // All lines must be 32 bits long
    int len = inst.length();
    if (len != ENCODED_INST_LENGTH)
        return false;

    // If any of those bits aren't a 1 or 0, return false
    int pos = 0;
    while (pos < len) {
        char c = inst.at(pos);
        if (!isOneOrZero(c))
            return false;
        pos++;
    }
    return true;
}

// This function receives a 32 bit line representing a MIPS instruction,
// and returns the opcode field of that instruction
string BinaryParser::getOpcodeField(string line) {
    int pos = 0; // position in line

    // create opcode field string
    string opcodeField = "";
    while (pos < OPCODE_LENGTH) {
        opcodeField += line.at(pos);
        pos++;
    }
    return opcodeField;
}

// This function receives a 32 bit line representing a MIPS instruction
// and returns the last 6 bits, where the function field of an RTYPE
// command is located.
string BinaryParser::getFuncField(string line) {
    unsigned int pos = FUNC_FIELD_LOCATION; // position in line

    // create function field string
    string funcField = "";
    while (pos < line.length()) {
        funcField += line.at(pos);
        pos++;
    }
    return funcField;
}

// This function receives a 32b line representing a MIPS instruction,
// and returns the instruction without the 6b opcode.
string BinaryParser::getLineNoOpcode(string line) {

    string instWithoutOpcode = "";
    unsigned int pos = OPCODE_LENGTH; // Position in line

    // Create the line with no opcode
    while (pos < line.length()) {
        instWithoutOpcode += line.at(pos);
        pos++;
    }
    return instWithoutOpcode;
}

// This function takes in one instruction, its opcode, and
// its line and determines which instruction type's decode
// function needs to be called.
// Fields stored in an instruction instance
bool BinaryParser::decode(Instruction& i, Opcode opcode, string& line) {

    // Use the opcode to get the name of the opcode in
    // string form.
    string opcodeName = myOpcodes.getOpcodeName(opcode);

    // The instruction type determines how we decode the binary string
    // Use the opcode to determine instruction type
    InstType instType = myOpcodes.getInstType(opcode);

    // Get the line (w/o the opocode) of the instruction, to pass into the decode function
    string lineWithoutOpcode = getLineNoOpcode(line); //naming

    bool success;
    switch (instType) {
    case RTYPE:
        success = decodeRType(i, opcode, lineWithoutOpcode);
        break;
    case ITYPE:
        success = decodeIType(i, opcode, lineWithoutOpcode);
        break;
    case JTYPE:
        success = decodeJType(i, opcode, lineWithoutOpcode);
        break;
    }
    return success;
}

// This function separates an RType instruction into the fields needed to
// print the assembly representation.
// Fields stored in an instruction instance
bool BinaryParser::decodeRType(Instruction& i, Opcode opcode, string& lineWithoutOpcode) {
    Register rd, rs, rt = -1;
    string imm = "";
    int imm_r = -1;

    // Each register's position in this instruction
    int rd_p = myOpcodes.RDposition(opcode);
    int rs_p = myOpcodes.RSposition(opcode);
    int rt_p = myOpcodes.RTposition(opcode);
    int imm_p = myOpcodes.IMMposition(opcode);

    // At this step, we create strings containing the binary value of each register, given its
    // present in our command
    unsigned int pos = 0;

    // If the command includes the register, convert that register to decimal
    // If the position is -1, it isn't included in this example
    if (rs_p != -1)
        rs = convertRegisterToAssembly(lineWithoutOpcode, pos, REGISTER_LENGTH);
    pos += REGISTER_LENGTH;

    if (rt_p != -1)
        rt = convertRegisterToAssembly(lineWithoutOpcode, pos, pos + REGISTER_LENGTH);
    pos += REGISTER_LENGTH;

    if (rd_p != -1)
        rd = convertRegisterToAssembly(lineWithoutOpcode, pos, pos + REGISTER_LENGTH);
    pos += REGISTER_LENGTH;

    // If present, get the binary string of the integer and convert it to an integer
    if (imm_p != -1) {
        while (pos < lineWithoutOpcode.length() - OPCODE_LENGTH) { //opcode length = funcfield length
            imm += lineWithoutOpcode.at(pos);
            pos++;
        }
        // function stoi() converts a binary string to an integer
        imm_r = stoi(imm, nullptr, 2);
    }

    // set the values of the instruction instance corresponding to this line
    i.setValues(opcode, rs, rt, rd, imm_r);

    return true;
}

// This function separates an IType instruction into the fields needed to
// print the assembly representation.
// Fields stored in an instruction instance
bool BinaryParser::decodeIType(Instruction& i, Opcode opcode, string& lineWithoutOpcode) {
    string imm;
    int rt, rs, rd = -1;
    int imm_r = 0;

    // No rd register in ITYPE commands

    // Get register and imm positions based on this instruction's opcode
    int rs_p = myOpcodes.RSposition(opcode);
    int rt_p = myOpcodes.RTposition(opcode);
    int imm_p = myOpcodes.IMMposition(opcode);

    // At this step, we create strings containing the binary value of each register, given its
    // present in our command
    unsigned int pos = 0;
    if (rs_p != -1)
        rs = convertRegisterToAssembly(lineWithoutOpcode, pos, REGISTER_LENGTH);
    pos += REGISTER_LENGTH;

    if (rt_p != -1)
        rt = convertRegisterToAssembly(lineWithoutOpcode, pos, (pos + REGISTER_LENGTH));
    pos += REGISTER_LENGTH;

    // If present, get the string of the immediate field and convert it from two's complement to decimal
    if (imm_p != -1) {
        while (pos < lineWithoutOpcode.length()) {
            imm += lineWithoutOpcode.at(pos);
            pos++;
        }
        imm_r = convertTwosComplementToDecimal(imm);
    }

    i.setValues(opcode, rs, rt, rd, imm_r);

    return true;
}

// This function separates an JType instruction into the fields needed to
// print the assembly representation.
// Fields stored in an instruction instance
bool BinaryParser::decodeJType(Instruction& i, Opcode opcode, string& lineWithoutOpcode) { 
    //use reference for string
    int rd = -1;
    int rt = -1; 
    int rs = -1;

    string imm = "";
    int imm_r = 0;

    int imm_p = myOpcodes.IMMposition(opcode);
    unsigned int pos = 0;

    if (imm_p != -1) {
        while (pos < lineWithoutOpcode.length()) {
            imm += lineWithoutOpcode.at(pos);
            pos++;
        }
        imm_r = stoi(imm, nullptr, 2);
    }

    i.setValues(opcode, rs, rt, rd, imm_r);

    return true;
}

// This function returns a string representing the assembly code of
// a single MIPS instruction
string BinaryParser::createAssemblyCode(Instruction i) {
    Opcode opcode = i.getOpcode();
    string myOpcodestring = myOpcodes.getOpcodeName(opcode);
    InstType type = myOpcodes.getInstType(opcode);

    switch (type) {
    case RTYPE:
        return writeRTypeDecoded(i); // put all necessary info into the instruction instance
    case JTYPE:
        return writeJTypeDecoded(i);
    case ITYPE:
        return writeITypeDecoded(i);
    default:
        return "";
    }
}

// This function uses the RType fields to set the values of an instruction data type
// Returns a string containing the instruction in MIPS assembly
string BinaryParser::writeRTypeDecoded(Instruction i) {
    Opcode opcode = i.getOpcode();
    string assembly = myOpcodes.getOpcodeName(opcode) + ' ';

    // If register is present, add it to the assembly string.
    if (myOpcodes.RDposition(opcode) != -1) {
        Register rd = i.getRD();
        // If it is first, print with a space preceeding and a comma afterwards
        if (myOpcodes.RDposition(opcode) != 0)
            assembly += " $" + to_string(rd) + ',';
        // Otherwise, simply print with a comma afterwards
        else
            assembly += "$" + to_string(rd) + ',';
    }
    if (myOpcodes.RSposition(opcode) != -1) {
        Register rs = i.getRS();
        if (myOpcodes.RSposition(opcode) != 0)
            assembly += " $" + to_string(rs) + ',';
        else
            assembly += "S" + to_string(rs) + ',';
    }
    if (myOpcodes.RTposition(opcode) != -1) {
        Register rt = i.getRT();
        if (myOpcodes.RTposition(opcode) != 0)
            assembly += " $" + to_string(rt) + ',';
        else
            assembly += "$" + to_string(rt) + ',';
    }

    if (myOpcodes.IMMposition(opcode) != -1) {
        int imm = i.getImmediate();
        assembly += " " + to_string(imm); // Immediate would always be last if present
    }

    // If there is a stray comma at the end, remove it
    if (assembly[assembly.length() - 1] == ',')
        assembly.pop_back();

    return assembly;
}

// This function uses the IType fields to set the values of an instruction data type
// Returns a string containing the instruction in MIPS assembly
string BinaryParser::writeITypeDecoded(Instruction i) {
    Opcode opcode = i.getOpcode();
    stringstream assembly;
    assembly << myOpcodes.getOpcodeName(opcode) << " ";
    Register rt, rs;

    if (myOpcodes.RTposition(opcode) != -1) {
        rt = i.getRT();
        if (myOpcodes.RTposition(opcode) == 2)
            assembly << '$' << to_string(rt);
        else
            assembly << '$' << to_string(rt) << ", ";
    }

    if (myOpcodes.RSposition(opcode) != -1) {
        rs = i.getRS();
        if (myOpcodes.RSposition(opcode) == 2)
            assembly << '$' << to_string(rs);
        else
            assembly << '$' << to_string(rs) << ", ";
    }

    string assemblyString;
    if (myOpcodes.IMMposition(opcode) != -1) {
        int imm = i.getImmediate();

        // Case to handle instructions that handle memory
        if (myOpcodes.isMemoryInstr(opcode)) {
            string immString = to_string(imm);
            assembly << immString;
            assemblyString = assembly.str();
            string::iterator it;
            int index = assemblyString.length() - 1;
            for (it = assemblyString.end(); it > assemblyString.begin(); it--, index--) {
                if (assemblyString.at(index) == ' ') {
                    assemblyString.insert(index, ' ' + immString + "($" + to_string(rs) + ')');
                    break;
                } else
                    assemblyString.pop_back();
            }
            return assemblyString;
        }
        // Case to handle instructions that have a label in the imm field
        else if (myOpcodes.isIMMLabel(opcode)) {
            imm *= 4;
            // Convert integer representing address to hexadecimal
            assembly << "0x" << hex << imm;
        }
        // Any other cases simply require us to add the immediate to the assembly string
        else {
            assembly << imm;
        }
    }
    return assembly.str();
}

// This function uses the JType fields to set the values of an instruction data type
// Returns a string containing the instruction in MIPS assembly
string BinaryParser::writeJTypeDecoded(Instruction i) {
    Opcode opcode = i.getOpcode();
    stringstream assembly;
    assembly << myOpcodes.getOpcodeName(opcode) << " ";

    if (myOpcodes.IMMposition(opcode) != -1) {
        int imm = i.getImmediate();
        imm *= 4;

        // Convert integer representing address to hexadecimal
        assembly << "0x" << hex << imm;
    }
    return assembly.str();
}

// This function converts a binary register value into a decimal string
Register BinaryParser::convertRegisterToAssembly(string& reg, int start, int finish) {
    int pos = start;
    string binaryRegister = "";

    while (pos < finish) {
        binaryRegister += reg.at(pos);
        pos++;
    }

    int decimalRegister = stoi(binaryRegister, nullptr, 2);
    string assemblyRegister = "$" + to_string(decimalRegister);

    return decimalRegister;
}

// This function converts a binary string in two's complement
// into a decimal value (as a string). Used in IType conversion.
int BinaryParser::convertTwosComplementToDecimal(string str) {
    char sign = str[0]; // first bit of this format

    // if the value is negative, we swap the bits
    if (sign == '1') {
        for (unsigned int i = 0; i < str.length(); i++) {
            if (str[i] == '0')
                str[i] = '1';
            else
                str[i] = '0';
        }
        // This function converts a binary string to decimal
        // Add one and return the negative
        int decimal = stoi(str, nullptr, 2) + 1;
        return -decimal;
    }
    // if positive, we only need to convert to binary
    int decimal = stoi(str, nullptr, 2);
    return decimal;
}

// Iterator that returns the next Instruction in the list of Instructions
Instruction BinaryParser::getNextInstruction() {
    if (myIndex < (int)(myInstructions.size())) {
        myIndex++;
        return myInstructions[myIndex - 1];
    }

    Instruction i;
    return i;
}