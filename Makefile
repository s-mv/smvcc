TITLE = smvcc

CC = g++

OBJECTS = ./build/program.o ./build/lexer.o

INCLUDES = -I ./include
CLFAGS = -std=c++11 -g

TESTS_PATH = ./tests

.PHONY: clean test production new

default: production # for now

test: ${OBJECTS}
	@$(CC) main.cpp ${INCLUDES} ${OBJECTS} ${CLFAGS} -o ./$(TITLE).test -Dsmv_smvcc_tests

production: ${OBJECTS}
	@$(CC) main.cpp ${INCLUDES} ${OBJECTS} ${CLFAGS} -o ./$(TITLE)

# build objects
./build/%.o: ./src/%.cpp
	@$(CC) $< $(INCLUDES) ${CLFAGS} -o $@ -c

clean:
	@rm ./$(TITLE)
	@rm -rf ${OBJECTS}

# helper to make new source-header pairs
name = newfile
new:
	@printf "#ifndef smv_$(TITLE)_$(name)_hpp\n#define smv_$(TITLE)_$(name)_hpp\n\n\n#endif" > include/$(name).hpp
	@printf "#include \"$(name).hpp\"" > src/$(name).cpp