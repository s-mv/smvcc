TITLE   = smvcc
# TODO, compile using smvcc
CC      = clang
CFLAGS  = -Wall -Wextra -Iinclude -g

SRC     = main.c src/lexer.c src/preprocessor.c
BUILD   = build
OBJ     = $(BUILD)/main.o $(BUILD)/lexer.o $(BUILD)/preprocessor.o
BIN     = $(BUILD)/$(TITLE)

.PHONY: all dev test install clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/main.o: main.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/lexer.o: src/lexer.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/preprocessor.o: src/preprocessor.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

dev: all
	./$(BIN)

test: all
	@echo "Testing is a TODO!"

install: all
	@echo "Installing $(BIN) to /usr/local/bin"
	install -m 755 $(BIN) /usr/local/bin/

clean:
	rm -rf $(BUILD)

