CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -Wl,-rpath=$(LIB) -L$(LIB) -ligraph -lse2

example.o: $(INCLUDE)/speak_easy_2.h

check: example
	./example

clean:
	-find -type f -not -name '*.c' -not -name 'Makefile' -exec rm {} \;
