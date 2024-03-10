# Structure
This is a compiler that implements a subset (for now) of the c99 compiler.
The stages involved in the same are as defined:

C code -> lexing & preprocessing -> recusrive descent parsing -> code
generation (x86 NASM probably).

There's a good change the code generation would first happen in a completely
imaginary language, but I'm trying to avoid that.
