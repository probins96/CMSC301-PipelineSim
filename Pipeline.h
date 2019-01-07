// Name: Palmer Robins

#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "Instruction.h"
#include "DependencyChecker.h"
#include "OpcodeTable.h"

using namespace std;

/** 
 * Pipeline Base Class
 * This class simulates a pipeline without considering
 * data hazards that slow down execution time
 */
class Pipeline {

    public:

        // The "ideal" pipeline constructor
        Pipeline();

        // Pipeline deconstructor
        ~Pipeline();

        // Add a given instruction to the list of instructions
        // @param i - The instruction to add to myInstructions
        void addInstruction(Instruction i);

        // Execute the pipeline simulation
        virtual void runPipeline();

    protected:

        // Print the pipeline, given the type of pipeline
        void printPipeline(string pipelineType);

        // Set the stages of the pipeline to null 
        // at the beginning
        void initializeStages();

        // As an instruction leaves the pipeline,
        // construct the string to print
        void constructLine();

        // Given an instruction, place it in the fetch stage
        void setFetch(Instruction* i) { inFetch = i; }
        // Given an instruction, place it in the decode stage
        void setDecode(Instruction* i) { inDecode = i; }
        // Given an instruction, place it in the execute stage
        void setExecute(Instruction* i) { inExecute = i; }
        // Given an instruction, place it in the memory stage
        void setMemory(Instruction* i) { inMemory = i; }
        // Given an instruction, place it in the write back stage
        void setWriteBack(Instruction* i) { inWriteBack = i; }

        // Get the instruction in the fetch stage
        Instruction getFetch() { return *inFetch; }
        // Get the instruction in the decode stage
        Instruction getDecode() { return *inDecode; }
        // Get the instruction in the execute stage
        Instruction getExecute() { return *inExecute; }
        // Get the instruction in the memory stage
        Instruction getMemory() { return *inMemory; }
        // Get the instruction in the write back stage
        Instruction getWriteBack() { return *inWriteBack; }

        vector<Instruction> myInstructions; // list of instruction

        vector<string> instStrings; // Stores information needed for printing, as strings

        // Each variable here stores the instruction in that stage of pipeline
        Instruction *inFetch;
        Instruction *inDecode;
        Instruction *inExecute;
        Instruction *inMemory;
        Instruction *inWriteBack;

        int numInstructions; // total number of instructions
        int cycleCounter, instructionCounter; // Keeps track of cycles and instructions

        DependencyChecker* checker; // Use dependency checker to identify dependences

        OpcodeTable myOpcodes;

};

/** 
 * Stall Pipeline Class
 * This class simulates a pipeline  considering
 * data hazards. Pipeline stalls until hazard is resolved.
 */
class StallPipeline : public Pipeline {

    public:

        // Stall pipeline constructor
        StallPipeline() : Pipeline() {}

        // Virtual deconstructor
        virtual ~StallPipeline() {}

        // Override the runPipeline method
        void runPipeline();
    
    protected:

        int instrCycled; // Track the number of instr to enter pipeline

};

/** 
 * Stall Pipeline Class
 * This class simulates a pipeline  considering
 * data hazards. Utilizes data forwarding to improve performance.
 */
class ForwardPipeline : public StallPipeline {

    public:

        // Data forwarding pipeline constructor
        ForwardPipeline() : StallPipeline() {}

        // Virtual deconstructor
        virtual ~ForwardPipeline() {}

        // Override the runPipeline method
        void runPipeline();

};

#endif