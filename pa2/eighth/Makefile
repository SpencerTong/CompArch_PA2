all: eighth

eighth: eighth.c 
	gcc -Wall -Werror -fsanitize=address -std=c11 eighth.c -o eighth -lm

clean: 
	rm -rf eighth