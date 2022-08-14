HEADERS = linked_list.h util.h pre_asm.h command_line.h encode.h asm_1.h asm_2.h label_list.h


assembler: util.o pre_asm.o assembler.o  command_line.o linked_list.o asm_1.o encode.o asm_2.o  label_list.o
	gcc -ansi -pedantic -Wall util.o command_line.o encode.o linked_list.o asm_1.o pre_asm.o asm_2.o  assembler.o label_list.o -o assembler -lm

linked_list.o: linked_list.c linked_list.h
	gcc -c -ansi -pedantic -Wall linked_list.c -o linked_list.o

pre_asm.o: pre_asm.c $(HEADERS)
	gcc -c -ansi -pedantic -Wall pre_asm.c linked_list.o -o pre_asm.o
	
assembler.o: assembler.c $(HEADERS)
	gcc -c -ansi -pedantic -Wall assembler.c -o assembler.o 
	
command_line.o: command_line.c command_line.h
	gcc -c -ansi -pedantic -Wall command_line.c -o command_line.o 
	
encode.o: encode.c $(HEADERS)
	gcc -c -ansi -pedantic -Wall encode.c -o encode.o
	
asm_1.o: asm_1.c $(HEADERS)
	gcc -c -ansi -pedantic -Wall asm_1.c -o asm_1.o
	
asm_2.o: asm_2.c $(HEADERS)
	gcc -c -ansi -pedantic -Wall asm_2.c -o asm_2.o -lm
	
util.o: util.c util.h
	gcc -c -ansi -pedantic -Wall util.c -o util.o
	
label_list.o: label_list.c label_list.h
	gcc -c -ansi -pedantic -Wall label_list.c -o label_list.o
