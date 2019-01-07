// Name: Palmer Robins

#include "Pipeline.h"

// The "ideal" pipeline constructor
Pipeline::Pipeline() {

    // Initialize the dependency checker
    checker = new DependencyChecker();

    cycleCounter = 0;
    instructionCounter = 0;

    // Each stage begins as empty
    initializeStages();
}

// Pipeline deconstructor
Pipeline::~Pipeline() {
    delete checker;
}

// Add a given instruction to the list of instructions
// @param i - The instruction to add to myInstructions
void Pipeline::addInstruction(Instruction i) {
    //Add an instruction to our instr vector
    myInstructions.push_back(i);
    //And to our dependency checker
    checker->addInstruction(i);
}

// Print the pipeline, given the type of pipeline
void Pipeline::printPipeline(string pipelineType) {
    cout << pipelineType << endl;
    checker->printRAWDependences();
    cout << "Instr#\tCompletionTime\tMnemonic" << endl;
    
    for (unsigned int i = 0; i < instStrings.size(); i++)
        cout << instStrings.at(i) << endl;

    // Print the total time taken in the pipeline
    cout << "Total time is " << to_string(cycleCounter) << endl;
    cout << endl;
}

// As an instruction leaves the pipeline,
// construct the string to print
void Pipeline::constructLine() {
    Instruction complete = getWriteBack();
    string line = to_string(instructionCounter);
    line += "\t";
    line += to_string(cycleCounter);
    line += "\t|";
    line += complete.getAssembly();
    instructionCounter += 1;
    instStrings.push_back(line);
}

// Set the stages of the pipeline to null
// at the beginning
void Pipeline::initializeStages() {
    inFetch = nullptr;
    inDecode = nullptr;
    inExecute = nullptr;
    inMemory = nullptr;
    inWriteBack = nullptr;
}

// Execute the pipeline simulation
void Pipeline::runPipeline() {

    numInstructions = myInstructions.size();

    // Put the first instruction into the fetch stage
    if (myInstructions.size() >0)
        setFetch(&myInstructions.at(cycleCounter));
    else {
        cerr << "Instructions didn't read correctly. Check input file." << endl;
        exit(1);
    }

    // Simulate the pipeline
    while (instructionCounter < numInstructions) {
        
        // Each loop iteration serves as a cycle
        cycleCounter += 1;

        // Not counting dependences, just move each instruction
        // through the five cycles
        if (inWriteBack)
            constructLine(); // instr is leaving pipeline
        if (inMemory)
            setWriteBack(inMemory);
        if (inExecute)
            setMemory(inExecute);
        if (inDecode)
            setExecute(inDecode);
        if (inFetch)
            setDecode(inFetch);

        // If the cycle counter is less than the total instr count,
        // Keep inserting an instruction to fetch
        if (cycleCounter < numInstructions)
            setFetch(&myInstructions.at(cycleCounter));
    }
    printPipeline("IDEAL:");
}

// Override the runPipeline method
void StallPipeline::runPipeline() {

    numInstructions = myInstructions.size();
    instrCycled = cycleCounter;

    list<Dependence> dependences = checker->getDependencies();
    list<Dependence>::iterator it;
    
    // Put the first instruction into the fetch stage
    setFetch(&myInstructions.at(instrCycled));

    // Simulate the pipeline
    while (instructionCounter < numInstructions) {
        cycleCounter += 1;
        instrCycled += 1;

        // Determine the stall length as we leave the pipeline
        if (inWriteBack) {
            for (it = dependences.begin(); it != dependences.end(); it++) {
                // There is a dependence where the instr in write back is the currInstr
                if (it->currInstruction == getWriteBack().getAssembly()) {
                    int currInstNumber = it->currentInstructionNumber;
                    int prevInstNumber = it->previousInstructionNumber;
                    // If the instructions are separated
                    if (currInstNumber - prevInstNumber == 2)
                        cycleCounter += 1;
                    // The instructions are back to back
                    else if (currInstNumber - prevInstNumber == 1)
                        cycleCounter += 2;
                }
            }
            constructLine(); // instr is leaving pipeline
            // Account for time taken to determine jump location
            if (getWriteBack().getInstType() == JTYPE)
                cycleCounter += 1;
        }

        // Advance stages
        if (inMemory)
            setWriteBack(inMemory);
        if (inExecute)
            setMemory(inExecute);
        if (inDecode)
            setExecute(inDecode);
        if (inFetch)
            setDecode(inFetch);

        // Insert next instruction, if necessary
        if (instrCycled < numInstructions)
            setFetch(&myInstructions.at(instrCycled));
    }
    printPipeline("STALL:");
}

// Override the runPipeline method
void ForwardPipeline::runPipeline() {

    numInstructions = myInstructions.size();
    instrCycled = cycleCounter;

    list<Dependence> dependences = checker->getDependencies();
    list<Dependence>::iterator it;

    // Put the first instruction into the fetch stage
    setFetch(&myInstructions.at(instrCycled));

    // Simulate the pipeline
    while (instructionCounter < numInstructions) {
        cycleCounter += 1;
        instrCycled += 1;

        // Determine any stalls here
        if (inWriteBack) {
            for (it = dependences.begin(); it != dependences.end(); it++) {
                // There is a dependence where the instr in write back is the currInstr
                if (it->currInstruction == getWriteBack().getAssembly()) {
                    int currInstNumber = it->currentInstructionNumber;
                    int prevInstNumber = it->previousInstructionNumber;
                    // If the instructions are back to back
                    // Memory instructions still require a stall
                    if (currInstNumber - prevInstNumber == 1 && myInstructions[prevInstNumber].getOpcode() == LB) {
                        cycleCounter += 1;
                    }
                }
            }
            constructLine(); // instr is leaving pipeline
            // Account for time taken to determine jump location
            if (getWriteBack().getInstType() == JTYPE)
                cycleCounter += 1;
        }

        // Advance stages of pipeline
        if (inMemory)
            setWriteBack(inMemory);
        if (inExecute)
            setMemory(inExecute);
        if (inDecode)
            setExecute(inDecode);
        if (inFetch)
            setDecode(inFetch);

        // Insert next instruction, if necessary
        if (instrCycled < numInstructions)
            setFetch(&myInstructions.at(instrCycled));
    }
    printPipeline("FORWARDING:");
}