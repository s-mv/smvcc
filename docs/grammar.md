# Grammar
Or to be more precise, **grammar that has been implemented so far**.

```
code -> statement*;

statement -> assignment `;` ;

assignment -> TYPE IDENT `=` expression;

expression => term (`+` expression)?;

term => factor (`*` term)?;

factor => NUM | IDENT | '(' expression ')';
```