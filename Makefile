TITLE = smvcc

CC = clang

OBJECTS = ./build/compiler.o \
					./build/lexer.o \
					./build/list.o \
					./build/token.o

INCLUDES = -I ./include
CLFAGS = -std=c11

# this is for the source-header pairs helper
name = test

all: ${OBJECTS}
	@$(CC) src/main.c ${INCLUDES} ${OBJECTS} ${CLFAGS} -o ./$(TITLE)

# build objects
./build/%.o: ./src/%.c
	@$(CC) $< $(INCLUDES) ${CLFAGS} -o $@ -c

clean:
	@rm ./$(TITLE)
	@rm -rf ${OBJECTS}

# helper to make new source-header pairs
new:
	@printf "#ifndef smv_$(TITLE)_$(name)_h\n#define smv_$(TITLE)_$(name)_h\n\n\n#endif" > include/$(name).h
	@printf "#include \"$(name).h\"" > src/$(name).c