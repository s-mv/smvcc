# Grammar
Or to be more precise, **grammar that has been implemented yet**.

```
expression = factor (OP factor)*
function = TYPE IDENTIFIER `(` arguments `)` `{` statement* `}`
arguments = TYPE IDENFIFIER (, TYPE IDENTIFIER)*
parameters = (IDENFIFIER | expression) (, (IDENTIFIER | expression))*
call = IDENT `(` parameters `)`
return = `return` expression;
```