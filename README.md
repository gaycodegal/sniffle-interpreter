# The Sniffle Interpreter

Not well written, but functional and small.

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
- (or BOOL1 BOOL2 ... BOOLN)
- (and BOOL1 BOOL2 ... BOOLN)
- (set VAR VAL)
- (local VAR VAL)
- (lambda (VAR1 VAR2 ... VARN) PROG)
- (pprint EXP1 EXP2 ... EXPN)
- (+ NUM1 NUM2 ... NUMN)
- (- NUM1 NUM2 ... NUMN)
- (/ NUM1 NUM2 ... NUMN)
- (* NUM1 NUM2 ... NUMN)
- (< NUM1 NUM2)
- (> NUM1 NUM2)
- (<= NUM1 NUM2)
- (>= NUM1 NUM2)
- (= NUM1 NUM2)
- (= STR1 STR2)
- (begin EXP1 EXP2 ... EXPN)
- (while BOOL EXP1 EXP2 ... EXPN)
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

