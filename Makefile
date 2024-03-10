TITLE = smvcc

CC ?= clang

OBJECTS = ./build/lexer.o \
  ./build/parser.o \
	./build/list.o 

INCLUDES = -I ./include
CLFAGS = -std=c11 -g

TESTS_PATH = ./tests

# this is for the source-header pairs helper
name = newfile

.PHONY: clean test production new

default: test # for now

test: ${OBJECTS}
	@$(CC) main.c ${INCLUDES} ${OBJECTS} ${CLFAGS} -o ./$(TITLE).test -Dsmv_smvcc_tests

production: ${OBJECTS}
	@$(CC) main.c ${INCLUDES} ${OBJECTS} ${CLFAGS} -o ./$(TITLE)

# build objects
./build/%.o: ./src/%.c
	@$(CC) $< $(INCLUDES) ${CLFAGS} -o $@ -c

clean:
	@rm ./$(TITLE)
	@rm -rf ${OBJECTS}

# testing

./tests/bin/%: ./tests/%.c
	$(CC) $< $(INCLUDES) $(CFLAGS) -o $@ -c

# helper to make new source-header pairs
new:
	@printf "#ifndef smv_$(TITLE)_$(name)_h\n#define smv_$(TITLE)_$(name)_h\n\n\n#endif" > include/$(name).h
	@printf "#include \"$(name).h\"" > src/$(name).c