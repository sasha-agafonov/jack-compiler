#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "lexer.h"
#include "parser.h"

#define HUNT 17
#define FORCE 71
#define TRANSTAR 717
#define THROTTLE 177


struct by_lightning {
  Token caller;
  Token subcaller;
  Token classn;
  Token subn;
  int a_mysterious_value; /*execution mode*/ };

struct comm {
  int mode;
  char class[64];
  char command[64];
  char segment[64];
  char argument[128];
  char target[64]; };


struct class {
  char id[64];
  int count;
  struct class_scope* scope; };


struct class_scope {
  char id[64];
  char type[64];
  char kind[64];
  int inx;
  int args;
  int vars;
  int count;
  struct subroutine_scope* scope; };


struct subroutine_scope {
  char id[64];
  char type[64];
  char kind[64];
  int inx; };


void Fold(Token classname);
void Cold(Token call, Token callsub, Token subname);
void Hold(Token callr, Token subcallr, Token id1, Token id2);
void Halt(Token call, Token callsub, Token classname, Token subname);
void Release(void);
void ReleaseComms(void);
void PrintClasses(void);
void Reconnaissance(void);
void Suspend(Token suspect);
void InsertClass(Token name);
void InsertClassScope(Token name, char* tp, char* kd);
void InsertSubroutineScope(Token name, char* tp, char* kd);
void NewCommand(char* cls, char* cmd, char* seg, char* arg, int mode, char* caller);
int IsClass(char* class);
int LookupClass(Token name);
int GetFuncVarCount(char* class, char* function);
int GetFuncArgCount(char* class, char* function);
int LookupClassScope(Token classname, Token scopename);
int LookupClassScopeVarType(Token classname, char* typ);
int LookupSubroutineScope(Token classname, Token scopename, Token subscopename);
int LookupSubroutineScopeVarType(Token classname, Token subname, char* typ);
int IsFuncConstr(Token class, Token subroute);
Token Hunt(void);
Token GetIdType(Token class, Token id);
Token GetVarIdByType(Token classname, Token typ);
Token GetVarTypeById(Token classname, Token aidi);
Token GetVarKindById(char* classname, char* subscope, char* aidi);
ParserInfo Showdown(void);
const char* GetEndClass(int inx);
const char* Traverse(const char* file);

#endif
