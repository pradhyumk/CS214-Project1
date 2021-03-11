all: ww

ww: ww.c
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined ww.c -o ww.out

clean:
	rm -f ww
