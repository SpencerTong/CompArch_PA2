all: third

third: third.c 
	gcc -Wall -Werror -fsanitize=address -std=c11 third.c -o third -lm

clean: 
	rm -rf third