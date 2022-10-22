all: seventh

seventh: seventh.c 
	gcc -g -Wall -Werror -fsanitize=address -std=c11 seventh.c -o seventh -lm

clean: 
	rm -rf seventh