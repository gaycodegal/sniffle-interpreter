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


## Language Features (Goals)

- Fixed Memory Execution
  - Specify the amount of memory the execution of a program should be bounded by.
- Variable and fixed number of arguments into a function

## Usage

Compile via the ```make``` command.

Run via ```./interp <filenamehere>```

