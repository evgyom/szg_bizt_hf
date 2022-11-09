# compile
../AFL/afl-gcc -c -o reader.o reader.c 
../AFL/afl-gcc -c -o lexer.o lexer.c
../AFL/afl-g++ -c -o parser.o parser.cpp
../AFL/afl-g++ -c -o main.o main.cpp  
../AFL/afl-g++ -o my_fuzz main.o parser.o reader.o lexer.o

# gyakon volt
echo core | sudo tee -a /proc/sys/kernel/core_pattern

# fuzz with input
../AFL/afl-fuzz -i testcase_dir -o findings_dir ./my_fuzz @@