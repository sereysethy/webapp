SRC=webapp.c utils.c
BIN=webapp
HEADER=header.h

.PHONY: all
.PHONY: clean

all: ${SRC} ${HEADER}
	gcc -g ${SRC} -o ${BIN}

clean:
	rm -rf *.o ${BIN}
