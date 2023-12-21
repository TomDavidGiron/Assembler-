assembler: main.o macro.o validate_input.o fill_tables.o first_pass.o second_pass.o to_base_64.o create_files.o
	gcc -ansi -Wall -g -pedantic main.o macro.o validate_input.o fill_tables.o first_pass.o second_pass.o to_base_64.o create_files.o -o assembler

main.o: main.c data.h 
	gcc -c -ansi -Wall -g -pedantic main.c -o main.o

macro.o: macro.c data.h
	gcc -c -ansi -Wall -g -pedantic macro.c -o macro.o

validate_input.o: validate_input.c data.h
	gcc -c -ansi -Wall -g -pedantic validate_input.c -o validate_input.o

fill_tables.o: fill_tables.c data.h
	gcc -c -ansi -Wall -g -pedantic fill_tables.c -o fill_tables.o

first_pass.o: first_pass.c data.h
	gcc -c -ansi -Wall -g -pedantic first_pass.c -o first_pass.o

second_pass.o: second_pass.c data.h
	gcc -c -ansi -Wall -g -pedantic second_pass.c -o second_pass.o

to_base_64.o: to_base_64.c data.h
	gcc -c -ansi -Wall -g -pedantic to_base_64.c -o to_base_64.o

create_files.o: create_files.c data.h
	gcc -c -ansi -Wall -g -pedantic create_files.c -o create_files.o
