# Grammar
Or to be more precise, **grammar that has been implemented so far**.

```
code -> statement*;

statement -> assignment;

assignment -> TYPE IDENT `=` expression;

expression => term ('+' term)*;

term => factor;

factor => NUM | IDENT | '(' expression ')';
```