# The Sniffle Interpreter

Not well written, but functional, small, and reasonably fast. Written in C/C++. Also available in JavaScript for kicks.

## Language Features (Current)

- Functions
  - Arguments and local variables overshadow global arguments
  - Lambdas (anonymous functions)
  - Have their own environment that is persistent throughout all function calls.
- Execution Environments
  - Specify which variables are available, able to be passed in alongside a program to execute.
  - Programs return a value, as well as having the ability to modify the passed environment.
- Function binding
  - Bind C++ and C methods directly into Sniffle as long as they are of the proper format

## Language Methods

- (if BOOL PATH1 PATH2)
  - computes PATH1 if BOOL is not NULL or 0
  - else PATH2
- (or BOOL1 BOOL2 ... BOOLN)
  - computes BOOL1 or BOOL2 or ... or BOOLN
  - evaluates until first true
- (and BOOL1 BOOL2 ... BOOLN)
  - computes BOOL1 and BOOL2 and ... and BOOLN
  - evaluates until first false
- (set VAR VAL)
  - update the value of a var in the global context
- (local VAR VAL)
  - update the value of a var in the local context
- (lambda (VAR1 VAR2 ... VARN) PROG)
  - a lambda function.
  - if you use local to update the value of
    any var, it will effect all instances of
    the lambda. Similarly, calling the lambda
    (even recursively) will erase overwrite the value
    of the vars.
- (pprint EXP1 EXP2 ... EXPN)
  - pretty prints expressions
- (+ NUM1 NUM2 ... NUMN)
  - evaluates NUM1 + NUM2
- (- NUM1 NUM2 ... NUMN)
  - evaluates NUM1 - NUM2
- (/ NUM1 NUM2 ... NUMN)
  - evaluates NUM1 / NUM2
- (* NUM1 NUM2 ... NUMN)
  - evaluates NUM1 * NUM2
- (< NUM1 NUM2)
  - evaluates NUM1 < NUM2
- (> NUM1 NUM2)
  - evaluates NUM1 > NUM2
- (<= NUM1 NUM2)
  - evaluates NUM1 <= NUM2
- (>= NUM1 NUM2)
  - evaluates NUM1 >= NUM2
- (= NUM1 NUM2)
  - evaluates NUM1 == NUM2
- (= STR1 STR2)
  - compares strings for equality
- (begin EXP1 EXP2 ... EXPN)
  - executes commands in sequence
  - returns the result of evaluation of EXPN
- (while BOOL EXP1 EXP2 ... EXPN)
  - will repeat EXP1 EXP2 ... EXPN while BOOL is true
  - returns the result of the last evaluation of EXPN
- (quote EXP)
  - returns the expression in unevaluated form
- (car EXP)
  - returns the first element of a list
- (cdr EXP)
  - removes the first element of a list
    - IMPORTANT, this causes side effects!
  - returns the remaining list
- (copy EXP)
  - copies an expression; useful for avoiding side effects of cdr
- (exit)
  - exits the REPL (Read-Eval-Print Loop)

## Rules important for use

- Don't store lambda functions inside lambda
  functions in a loop unless you break that loop before
  program termination

## Language Features (Goals)

- Fixed Memory Execution
  - Specify the amount of memory the execution of a program should be bounded by.
- Variable and fixed number of arguments into a function

## Usage

Compile via the ```make``` command.

Run via ```./interp <filenamehere>```

