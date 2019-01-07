// Palmer Robins

#include "DependencyChecker.h"
#include <iterator>

/** Creates RegisterInfo entries for each of the 32 registers and creates lists for
* dependencies and instructions.
*/
DependencyChecker::DependencyChecker(int numRegisters) {
    instCount = 0;
    RegisterInfo r;

    // Create entries for all registers
    for (int i = 0; i < numRegisters; i++)
        myCurrentState.insert(make_pair(i, r));
}

/**
*  Adds an instruction to the list of instructions and checks to see if that
* instruction results in any new data dependencies.  If new data dependencies
* are created with the addition of this instruction, appropriate entries
* are added to the list of dependences.
*/
void DependencyChecker::addInstruction(Instruction i) {
    InstType instrType = i.getInstType();
    myInstructions.push_back(i);

    switch (instrType) {
    case RTYPE:
        checkForReadDependence(i.getRT());
        checkForReadDependence(i.getRS());
        checkForWriteDependence(i.getRD());
        break;
    case ITYPE:
        checkForReadDependence(i.getRS());
        checkForWriteDependence(i.getRT());
        break;
    case JTYPE:
        break;
    default:
        break;
    }

    instCount += 1;
}

/** 
 * Determines if a read data dependence occurs when reg is read by the current
* instruction.  If so, adds an entry to the list of dependences. Also updates
* the appropriate RegisterInfo entry regardless of dependence detection.
 */
void DependencyChecker::checkForReadDependence(unsigned int reg) {
    map<unsigned int, RegisterInfo>::iterator it = myCurrentState.find(reg);

    if (it == myCurrentState.end())
        return;

    RegisterInfo matchInfo = myCurrentState.at(reg);

    if (matchInfo.accessType == WRITE) {
        Dependence depend;
        depend.dependenceType = RAW;
        depend.registerNumber = reg;
        depend.previousInstructionNumber = matchInfo.lastInstructionToAccess;
        depend.currentInstructionNumber = instCount;
        depend.prevInstruction = myInstructions.at(depend.previousInstructionNumber).getAssembly();
        depend.currInstruction = myInstructions.at(depend.currentInstructionNumber).getAssembly();
        myDependences.push_back(depend);
        stringstream dependence("");
    }

    myCurrentState.at(reg).lastInstructionToAccess = instCount;
    myCurrentState.at(reg).accessType = READ;
}

/**
* Determines if a write data dependence occurs when reg is written by the current
* instruction.  If so, adds an entry to the list of dependences. Also updates
* the appropriate RegisterInfo entry regardless of dependence detection.
*/
void DependencyChecker::checkForWriteDependence(unsigned int reg) {
    map<unsigned int, RegisterInfo>::iterator it = myCurrentState.find(reg);

    if (it == myCurrentState.end())
        return;

    RegisterInfo matchInfo = myCurrentState.at(reg);

    if (matchInfo.accessType == READ) {
        Dependence depend;
        depend.dependenceType = WAR;
        depend.registerNumber = reg;
        depend.previousInstructionNumber = matchInfo.lastInstructionToAccess;
        depend.currentInstructionNumber = instCount;
        depend.prevInstruction = myInstructions.at(depend.previousInstructionNumber).getAssembly();
        depend.currInstruction = myInstructions.at(depend.currentInstructionNumber).getAssembly();
        //myDependences.push_back(depend);
    } 
    else if (matchInfo.accessType == WRITE) {
        Dependence depend;
        depend.dependenceType = WAW;
        depend.registerNumber = reg;
        depend.previousInstructionNumber = matchInfo.lastInstructionToAccess;
        depend.currentInstructionNumber = instCount;
        depend.prevInstruction = myInstructions.at(depend.previousInstructionNumber).getAssembly();
        depend.currInstruction = myInstructions.at(depend.currentInstructionNumber).getAssembly();
        //myDependences.push_back(depend);
    }

    myCurrentState.at(reg).lastInstructionToAccess = instCount;
    myCurrentState.at(reg).accessType = WRITE;
}

/**
* Prints out the sequence of instructions followed by the sequence of data
* dependencies.
*/
void DependencyChecker::printDependences() {
    // First, print all instructions
    vector<Instruction>::iterator liter;
    int i = 0;
    cout << "INSTRUCTIONS:" << endl;
    for (liter = myInstructions.begin(); liter != myInstructions.end(); liter++) {
        cout << i << ": " << (*liter).getAssembly() << endl;
        i++;
    }

    // Second, print all dependences
    list<Dependence>::iterator diter;
    cout << "DEPENDENCES: \nType Register (FirstInstr#, SecondInstr#) " << endl;
    for (diter = myDependences.begin(); diter != myDependences.end(); diter++) {
        switch ((*diter).dependenceType) {
        case RAW:
            cout << "RAW \t";
            break;
        case WAR:
            cout << "WAR \t";
            break;
        case WAW:
            cout << "WAW \t";
            break;
        default:
            break;
        }

        cout << "$" << (*diter).registerNumber << " \t";
        cout << "(" << (*diter).previousInstructionNumber << ", ";
        cout << (*diter).currentInstructionNumber << ")" << endl;
    }
}

// Print all RAW dependence data in the instruction sequence
void DependencyChecker::printRAWDependences() {
    list<Dependence>::iterator diter;

    for (diter = myDependences.begin(); diter != myDependences.end(); diter++) {
        switch ((*diter).dependenceType) {
        case RAW:
            cout << "RAW Dependence between instruction " << (*diter).previousInstructionNumber << " " << (*diter).prevInstruction << " and " << (*diter).currentInstructionNumber << " " << (*diter).currInstruction << endl;
            break;
        case WAR:
            cout << "WAR Dependence between instruction " << (*diter).previousInstructionNumber << " " << (*diter).prevInstruction << " and " << (*diter).currentInstructionNumber << " " << (*diter).currInstruction << endl;
            break;
        case WAW:
            cout << "WAW Dependence between instruction " << (*diter).previousInstructionNumber << " " << (*diter).prevInstruction << " and " << (*diter).currentInstructionNumber << " " << (*diter).currInstruction << endl;
            break;
        default:
            break;
        }
    }
}