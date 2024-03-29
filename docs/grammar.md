# Grammar
Or to be more precise, **grammar that has been implemented yet**.

```
expression = factor (OP factor)*
function = TYPE IDENTIFIER `(` arguments `)` `{` statement* `}`
parameter = (IDENFIFIER | expression)
parameters = parameter (, parameter)*
argument = TYPE parameter
arguments = argument (, argument)*
call = IDENT `(` parameters `)`
return = `return` expression;
```