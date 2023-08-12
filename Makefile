TITLE = smvcc

OBJECTS = ./build/compiler.o ./build/cprocess.o
INCLUDES = -I ./include

# this is for the source-header pairs helper
name = test

all: ${OBJECTS}
	@clang src/main.c ${INCLUDES} ${OBJECTS}  -o ./$(TITLE)

# build objects
./build/%.o: ./src/%.c
	@clang $< $(INCLUDES) -o $@ -g -c

clean:
	@rm ./$(TITLE)
	@rm -rf ${OBJECTS}

# helper to make new source-header pairs
new:
	@printf "#ifndef smv_$(TITLE)_$(name)_h\n#define smv_$(TITLE)_$(name)_h\n\n\n#endif" > include/$(name).h
	@printf "#include \"$(name).h\"" > src/$(name).c