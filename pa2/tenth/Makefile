all: tenth

tenth: tenth.c 
	gcc -Wall -Werror -fsanitize=address -std=c11 tenth.c -o tenth -lm

clean: 
	rm -rf tenth