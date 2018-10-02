COMPILER=gcc
COMPILER_FLAGS=-Wall -Werror -g

PMan: ../source/PMan.c CommandCheck SystemCallWrapper ProcessLinkedList Tokenizer
	$(COMPILER) $(COMPILER_FLAGS) ../source/PMan.c CommandCheck.o SystemCallWrapper.o ProcessLinkedList.o Tokenizer.o -o PMan

CommandCheck: ../source/CommandCheck.c ../source/header/CommandCheck.h
	$(COMPILER) $(COMPILER_FLAGS) -c ../source/CommandCheck.c -o CommandCheck.o

SystemCallWrapper: ../source/SystemCallWrapper.c ../source/header/SystemCallWrapper.h
	$(COMPILER) $(COMPILER_FLAGS) -c ../source/SystemCallWrapper.c -o SystemCallWrapper.o

ProcessLinkedList: ../source/ProcessLinkedList.c ../source/header/ProcessLinkedList.h ../source/header/CommandTypes.h
	$(COMPILER) $(COMPILER_FLAGS) -c ../source/ProcessLinkedList.c -o ProcessLinkedList.o

Tokenizer: ../source/Tokenizer.c ../source/header/Tokenizer.h
	$(COMPILER) $(COMPILER_FLAGS) -c ../source/Tokenizer.c -o Tokenizer.o

clean:
	rm *.o PMan
