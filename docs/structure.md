# Structure
This is a compiler that implements a subset (for now) of the c99 compiler.
The stages involved in the same are as defined:

C code -> lexing & preprocessing -> recusrive descent parsing -> code
generation (x86 NASM probably).

There's a good change the code generation would first happen in a completely
imaginary language, but I'm trying to avoid that.

## The Preprocessor - My Approach
A lexer may either have 2 passes:
1. Lex preprocessor directives as tokens.
2. Traverse token array and resolve directives.
(Biggest con: conditional directives are redundant overhead.)

...Or preprocess on the fly by lexing recursively and returning a pointer to
the lexer (add TOKEN_FILE pointing to another lexer with different source) and
on the go also make a registry of `#define`s.
(Biggest con: I am mentally incapable of thinking of a clean implementation!)

I ended up chosing the second way.

## The Lexer - My Approach
The lexer simply works as a really complicated finite state machine. Nothing
much to really say about that as of now.
