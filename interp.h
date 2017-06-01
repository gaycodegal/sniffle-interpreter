#include "lispinclude.h"

void deleteEnv(environment * env);

expression * addFunc(expression * arglist, environment * env, environment * args);

slist * evalList(slist * list, environment * env, environment * args);

expression * evalAST(expression * prog, environment * env, environment *args);

int main(int argc, char ** argv);
