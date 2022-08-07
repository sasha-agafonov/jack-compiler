#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"
#include "symbols.h"

int InitCompiler ();
ParserInfo compile (char* dir_name);
int StopCompiler();

#endif
