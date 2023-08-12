TITLE = smvcc

OBJECTS = 
INCLUDES = -I ./include

all: ${OBJECTS}
	@gcc src/main.c ${INCLUDES} ${OBJECTS} -o ./$(TITLE)

clean:
	@rm ./$(TITLE)
	@rm -rf ${OBJECTS}

# helper 
new:
	@printf "#ifndef smv_$(TITLE)_$(name)_h\n#define smv_$(TITLE)_$(name)_h\n\n\n#endif" > include/$(name).h
	@printf "#include \"$(name).h\"" > src/$(name).c