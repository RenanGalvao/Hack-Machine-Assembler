CFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith \
		 -Wcast-align -Wstrict-prototypes -Wstrict-overflow=4 \
		 -Wwrite-strings -Waggregate-return -Wcast-qual -Wunreachable-code \
		 -O3 -lm

objects = main.o utils.o tables.o regex.o parser.o \
		  hash-map.o dynamic-string-array.o

hack-assembler: $(objects)
	cc -o hack-assembler $(objects) $(CFLAGS)

dynamic-string-array.o: dynamic-string-array.h regex.h utils.h
hash-map.o: hash-map.h
parser.o: parser.h regex.h utils.h tables.h 
regex.o: regex.h 
tables.o: tables.h utils.h
utils.o: utils.h dynamic-string-array.h
main.o: parser.h tables.h dynamic-string-array.h utils.h

.PHONY: clean
clean: 
	rm hack-assembler $(objects) 2>/dev/null || true

