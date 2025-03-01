CFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith \
		 -Wcast-align -Wstrict-prototypes -Wstrict-overflow=4 \
		 -Wwrite-strings -Waggregate-return -Wcast-qual -Wunreachable-code \
		 -O3 -lm

objects = main.o utils.o tables.o regex.o parser.o hash-map.o 

hack-assembler: $(objects)
	cc -o hack-assembler $(objects) $(CFLAGS)

hash-map.o: hash-map.h
parser.o: parser.h regex.h utils.h tables.h 
regex.o: regex.h 
tables.o: tables.h utils.h
utils.o: utils.h
main.o: parser.h tables.h utils.h

.PHONY: clean
clean: 
	rm hack-assembler $(objects) 2>/dev/null || true

