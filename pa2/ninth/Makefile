all: ninth

ninth: ninth.c 
	gcc -Wall -Werror -fsanitize=address -std=c11 ninth.c -o ninth -lm

clean: 
	rm -rf ninth