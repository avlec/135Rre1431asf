COMPILER=gcc
COMPILER_FLAGS=-Wall -Werror

PMan: ../source/PMan.c ProcessLinkedList Tokenizer Inf
	$(COMPILER) $(COMPILER_FLAGS) ../source/PMan.c ProcessLinkedList.o Tokenizer.o -o PMan

ProcessLinkedList: ../source/ProcessLinkedList.c ../source/header/ProcessLinkedList.h ../source/header/CommandTypes.h
	$(COMPILER) $(COMPILER_FLAGS) -c ../source/ProcessLinkedList.c -o ProcessLinkedList.o

Tokenizer: ../source/Tokenizer.c ../source/header/Tokenizer.h
	$(COMPILER) $(COMPILER_FLAGS) -c ../source/Tokenizer.c -o Tokenizer.o

Inf: ../source/inf.c
	$(COMPILER) $(COMPILER_FLAGS) ../source/inf.c -o Inf

clean:
	rm *.o
