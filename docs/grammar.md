# Grammar
Or to be more precise, **grammar that has been implemented yet**.

```
program: statement* ;

statement:
    block
    | if
    | declaration
    | function
    ;

expression: factor (OP factor)* ;

declaration: TYPE IDENTIFIER `(` arguments `)` `;` ;

function: TYPE IDENTIFIER `(` arguments `)` `{` statement* `}` ;

parameter:
    IDENFIFIER
    | expression
    ;

parameters: parameter (, parameter)* ;

argument: TYPE parameter ;

arguments: argument (, argument)* ;

call: IDENT `(` parameters `)` ;

return: `return` expression ;
```