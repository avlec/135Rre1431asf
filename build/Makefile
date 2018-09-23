COMPILER=gcc
COMPILER_FLAGS=-Wall -Werror

PMan: ../source/PMan.c CommandParser ProcessLinkedList Tokenizer
	$(COMPILER) $(COMPILER_FLAGS) ../source/PMan.c CommandParser.o ProcessLinkedList.o Tokenizer.o -o PMan

CommandParser: ../source/CommandParser.c ../source/header/CommandParser.h ../source/header/CommandTypes.h
	$(COMPILER) $(COMPILER_FLAGS) -c ../source/CommandParser.c -o CommandParser.o

ProcessLinkedList: ../source/ProcessLinkedList.c ../source/header/ProcessLinkedList.h ../source/header/CommandTypes.h
	$(COMPILER) $(COMPILER_FLAGS) -c ../source/ProcessLinkedList.c -o ProcessLinkedList.o

Tokenizer: ../source/Tokenizer.c ../source/header/Tokenizer.h
	$(COMPILER) $(COMPILER_FLAGS) -c ../source/Tokenizer.c -o Tokenizer.o

clean:
	rm *.o
