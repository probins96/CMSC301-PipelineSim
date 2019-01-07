# makefile
# use to compile and test the assembler ASM for the STACK computer and
# its various components

DEBUG_FLAG= -DDEBUG -g -Wall
CFLAGS=-DDEBUG -g -Wall -std=c++11

.SUFFIXES: .cpp .o

.cpp.o:
	g++ $(CFLAGS) -c $<


PIPESIM: PipelineSim.o DependencyChecker.o Instruction.o OpcodeTable.o RegisterTable.o Pipeline.o ASMParser.o BinaryParser.o
	g++ -o PIPESIM DependencyChecker.o PipelineSim.o OpcodeTable.o ASMParser.o BinaryParser.o RegisterTable.o Instruction.o Pipeline.o

DependencyChecker.o: DependencyChecker.h OpcodeTable.h RegisterTable.h Instruction.h Pipeline.h

Pipeline.o: Pipeline.h ASMParser.h DependencyChecker.h

ASMParser.o: ASMParser.h OpcodeTable.h RegisterTable.h Instruction.h 

BinaryParser.o: BinaryParser.h OpcodeTable.h RegisterTable.h Instruction.h

Instruction.o: OpcodeTable.h RegisterTable.h Instruction.h 

OpcodeTable.o: OpcodeTable.h 

RegisterTable.o: RegisterTable.h  

clean:
	/bin/rm -f PIPESIM *.o core