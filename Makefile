CC=cc
CFLAGS=-std=c99 -w -pedantic -o clc -lm -lreadline

clc: src/main.c src/lexer.c src/parser.c
	@$(CC) $(CFLAGS) src/*.c

install:
	@install -s clc /bin/clc

clean:
	@rm clc

uninstall:
	@rm /bin/clc
