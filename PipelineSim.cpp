// Name: Palmer Robins

#include "ASMParser.h"
#include "BinaryParser.h"
#include "Pipeline.h"

using namespace std;

// This template function receives either a Binary or ASM Parser
// It executes syntax checking and the simulation of the pipeline
template 
<class ParserType> 
void addInstructions(ParserType&& parser);

/**
 * This file reads in a file contains assembly or binary code
 * If the file is correct syntactically, the ideal, with stalling, 
 * and with data forwarding execution times will be calculated
 * to stdout.
 */
int main(int argc, char *argv[]) {
    // Check for a command line argument
    if (argc != 2) {
        cerr << "You need to specify a binary or assembly file to translate." << endl;
        exit(1);
    }

    // Get the input file extension
    string filename = argv[1];
    size_t fileExtension = filename.find('.');
    string fileFormat = filename.substr(fileExtension, filename.size());

    // Determine if the input is in assembly or binary
    if (fileFormat == ".asm")
        addInstructions <ASMParser> (ASMParser(argv[1]));
    else if (fileFormat == ".mach")
        addInstructions <BinaryParser> (BinaryParser(argv[1]));
    else {
        cerr << "The input file needs to be in '.asm' or '.mach' format." << endl;
        exit(1);
    }
}

// This template function receives either a Binary or ASM Parser
// It executes syntax checking and the simulation of the pipeline
template <class ParserType> 
void addInstructions(ParserType&& parser) {

    // Check for a correct format
    if (parser.isFormatCorrect() == false) {
        cerr << "The file format is incorrect." << endl;
        exit(1);
    }

    // Create instances of all three pipelines
    Pipeline pipeline;
    unique_ptr<StallPipeline> stall(new StallPipeline());
    unique_ptr<ForwardPipeline> forwarding(new ForwardPipeline());

    // Assemble lists of instructions
    Instruction i;
    i = parser.getNextInstruction();
    while (i.getOpcode() != UNDEFINED) {
        pipeline.addInstruction(i);
        stall->addInstruction(i);
        forwarding->addInstruction(i);
        i = parser.getNextInstruction();
    }
    
    // Simulate the Pipeline
    cout << "Instr#\tCompletionTime\tMnemonic" << endl;
    pipeline.runPipeline();
    stall->runPipeline();
    forwarding->runPipeline();

}