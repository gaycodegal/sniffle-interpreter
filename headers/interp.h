#include "lispinclude.h"

/**
   evaluates if two things are equal.
*/
expression * eqFunc(expression * arglist, environment * env, environment * args);

/**
   evaluates if two things are >.
*/
expression * gtFunc(expression * arglist, environment * env, environment * args);

/**
   evaluates if two things are >=.
*/
expression * geFunc(expression * arglist, environment * env, environment * args);

/**
   evaluates if two things are <.
*/
expression * ltFunc(expression * arglist, environment * env, environment * args);

/**
   evaluates if two things are <=.
*/
expression * leFunc(expression * arglist, environment * env, environment * args);

/**
   A printing C_FUNC available in sniffle.
 */
expression * printAnyFunc(expression * arglist, environment * env, environment * args);

/**
   A setting C_FUNC available in sniffle.

   sets global vars.
 */
expression * setFunc(expression * arglist, environment * env, environment * args);

/**
   A setting C_FUNC available in sniffle.

   sets local (function scope) vars. possibly
   should depricate.
 */
expression * setFunc2(expression * arglist, environment * env, environment * args);

/**
   A lambda constructor C_FUNC available in sniffle.

   makes functions
 */
expression * lambdaMakerFunc(expression * arglist, environment * env, environment * args);

/**
   An or C_FUNC available in sniffle.

   evals as nesc left to right
 */
expression * orFunc(expression * arglist, environment * env, environment * args);

/**
   An if C_FUNC available in sniffle.
   
   only evals branch nesc.
 */
expression * ifFunc(expression * arglist, environment * env, environment * args);

/**
   An and C_FUNC available in sniffle.

   evals as nesc left to right
 */
expression * andFunc(expression * arglist, environment * env, environment * args);

/**
   An adding C_FUNC available in sniffle.

   only works on numbers.
   unbounded amount of nums
 */
expression * addFunc(expression * arglist, environment * env, environment * args);

/**
   A multiplying C_FUNC available in sniffle.

   only works on numbers.
   unbounded amount of nums
 */
expression * multiplyFunc(expression * arglist, environment * env, environment * args);

/**
   A subtracting C_FUNC available in sniffle.

   only works on numbers.
   subtract unbounded amount of nums from the first number

   if only one number will negate
 */
expression * subtractFunc(expression * arglist, environment * env, environment * args);

/**
   An adding C_FUNC available in sniffle.

   only works on numbers.
   add unbounded amount of nums
 */
expression * divideFunc(expression * arglist, environment * env, environment * args);

/**
   evaluate a slist of expressions.

   returns a new slist of the evaluated forms
 */
slist * evalList(slist * list, environment * env, environment * args);

/**
   evaluates a single expression.

   returns a new expression that must be deleted by caller.
 */
expression * evalAST(expression * prog, environment * env, environment *args);

/**
   delete an environment.
 */
void deleteEnv(environment * env);

/**
   create the standard environment with bindings for the sniffle language
 */
environment * createEnv();

/**
   run the repl
*/
void repl(environment * env);

/**
   run a program. Does not delete prog or ENV.
   prog must be a LIST_EXP containing various snippets to evaluate.
   ENV must be the environment to run the program in.
 */
void runProgram(expression * prog, environment * ENV);

/**
   parse and run a file in a pretty fashion
 */
int main(int argc, char ** argv);
