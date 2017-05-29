# The Sniffle Interpreter

Currently at the Abstract Syntax Tree (AST) level. Not well written - there is no notion of tokenizer/parser. Will be adding in the interpreter shortly.

## Language Features (Goals)

- Functions
  - Recursion
  - Arguments and local variables will overshadow global arguments
  - Lambdas (anonymous functions)
  - Environment settings
    - by default functions will be bound to the live global environment
    - can be rebound to a new custom environment
- Execution Environments
  - Specify which variables are available, able to be passed in alongside a program to execute.
  - Programs will return a value, as well as having the ability to modify the passed environment.
  - Fixed Memory Execution
  - Specify the amount of memory the execution of a program should be bounded by.
  - Custom environment specification (environment copying and extension)
- Automatic Reference Counting (Deallocation as soon as reference hits 0)
- Easy function binding
  - Bind C++ and C methods directly into Sniffle
- Variable and fixed number of arguments into a function

## Usage

Compile via the ```make``` command.

Run via ```./interp <filenamehere>```

