TITLE = smvcc

CC ?= clang

OBJECTS = ./build/compiler.o \
					./build/lexer.o \
					./build/list.o \
					./build/trie.o

INCLUDES = -I ./include
CLFAGS = -std=c11 -g

# this is for the source-header pairs helper
name = newfile

default: production

# test: ${OBJECTS}
# 	@$(CC) src/main.c ${INCLUDES} ${OBJECTS} ${CLFAGS} -o ./$(TITLE).test -Dsmv_smvcc_tests

production: ${OBJECTS}
	@$(CC) src/main.c ${INCLUDES} ${OBJECTS} ${CLFAGS} -o ./$(TITLE)

# build objects
./build/%.o: ./src/%.c
	@$(CC) $< $(INCLUDES) ${CLFAGS} -o $@ -c

clean:
	@rm ./$(TITLE)
	@rm -rf ${OBJECTS}

# testing

# ./tests/bin/%: ./tests/%.c
# 	$(CC) $< $(INCLUDES) $(CFLAGS) -o $@ -c

# helper to make new source-header pairs
new:
	@printf "#ifndef smv_$(TITLE)_$(name)_h\n#define smv_$(TITLE)_$(name)_h\n\n\n#endif" > include/$(name).h
	@printf "#include \"$(name).h\"" > src/$(name).c