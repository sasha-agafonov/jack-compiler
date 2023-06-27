#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"
#include "parser.h"
#include "lexer.h"
#define ELITE 1337

int kek = 0;
int classcnt = 0;
int comm_count = 0;
int tekken_count = 0;
int secret = 0;

Token * tekken;
struct class * global;
struct by_lightning * traitors;
struct comm * commands;

int Fields(char *class)
{
	int enfield = 0;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].kind, "field") == 0) enfield++;
			}
		}
	}

	return enfield;
}

const char *GetClassByFunc(char *function)
{
	for (int i = 0; i < classcnt; i++)
	{
		for (int j = 0; j < global[i].count; j++)
		{
			if (strcmp(global[i].scope[j].id, function) == 0) return global[i].id;
		}
	}

	return "ERROR__AT__THIS__POINT";
}

int IsClass(char *class)
{
	for (int i = 0; i < classcnt; i++)
		if (strcmp(global[i].id, class) == 0) return 1;
	return 0;
}

Token GetVarKindBySubId(char *class, char *subscope, char *
	var)
{
	Token tok;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, subscope) == 0)
				{
					for (int k = 0; global[i].scope[j].count; k++)
					{
						if (strcmp(global[i].scope[j].scope[k].id,
								var) == 0)
						{
							strcpy(tok.lx, global[i].scope[j].scope[k].kind);
							return tok;
						}
					}
				}
			}
		}
	}

	tok.tp = ERR;
	return tok;
}

Token GetClassByVar(char *class, char *
	var)
{
	Token tok;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if ((strcmp(global[i].scope[j].id,
						var) == 0) && ((strcmp(global[i].scope[j].kind, "static") == 0) || (strcmp(global[i].scope[j].kind, "field") == 0)))
				{
					strcpy(tok.lx, global[i].scope[j].type);
					return tok;
				}

				for (int k = 0; k < global[i].scope[j].count; k++)
				{
					if ((strcmp(global[i].scope[j].scope[k].id,
							var) == 0))
					{
						strcpy(tok.lx, global[i].scope[j].scope[k].type);
						return tok;
					}
				}
			}
		}
	}

	strcpy(tok.lx, "__FATAL__ERR__");
	return tok;
}

int GetClassVarOffset(char *class, char *classvar)
{
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if ((strcmp(global[i].scope[j].id, classvar) == 0) &&
					((strcmp(global[i].scope[j].kind, "static") == 0) ||
						(strcmp(global[i].scope[j].kind, "field") == 0)))
					return global[i].scope[j].inx;
			}
		}
	}

	return -200;
}

int GetVarOffset(char *class, char *
	var)
{
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				for (int k = 0; k < global[i].scope[j].count; k++)
				{
					if (strcmp(global[i].scope[j].scope[k].id,
							var) == 0)
					{
						return global[i].scope[j].scope[k].inx;
					}
				}
			}
		}
	}

	return -200;
}

int GetSubVarOffset(char *class, char *subroute, char *
	var)
{
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class) == 0)
		{
			for (int k = 0; k < global[i].count; k++)
			{
				if (strcmp(global[i].scope[k].id, subroute) == 0)
				{
					for (int x = 0; x < global[i].scope[k].count; x++)
					{
						if (strcmp(global[i].scope[k].scope[x].id,
								var) == 0)
						{
							return global[i].scope[k].scope[x].inx;
						}
					}
				}
			}
		}
	}

	return -100;
}

int GetFuncArgCount(char *class, char *function)
{
	int args = 0;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, function) == 0)
				{
					for (int k = 0; k < global[i].scope[j].count; k++)
					{
						if (strcmp(global[i].scope[j].scope[k].kind, "argument") == 0) args++;
					}
				}
			}
		}
	}

	return args;
}

int IsConstructor(char *class, char *arg)
{
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				for (int k = 0; k < global[i].scope[j].count; k++)
				{
					if ((strcmp(global[i].scope[j].scope[k].id, arg) == 0) && (strcmp(global[i].scope[j].scope[k].kind, "argument") == 0))
					{
						if (strcmp(global[i].scope[j].kind, "constructor") == 0) return 1;
						else return 0;
					}
				}
			}
		}
	}
}

int GetFuncVarCount(char *class, char *function)
{
	int vars = 0;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, function) == 0)
				{
					for (int k = 0; k < global[i].scope[j].count; k++)
					{
						if (strcmp(global[i].scope[j].scope[k].kind, "var") == 0) vars++;
					}
				}
			}
		}
	}

	return vars;
}

const char *GetEndClass(int inx)
{
	return global[classcnt - inx].id;
}

const char *Traverse(const char *file)
{
	char class[128];
	char *electro;
	char boom[32];
	char temp[128];
	int ccnt = 0;
	int debase = 0;
	strcpy(class, file);
	electro = malloc(sizeof(char) *128 *comm_count);
	electro[0] = '\0';
	for (int i = 0; i < comm_count; i++)
	{
		if (strcmp(commands[i].target, file) == 0)
		{
			if (commands[i].mode == HUNT)
			{
				if (strcmp(commands[i].command, "function") == 0)
				{
					Token cls;
					Token sub;
					strcpy(cls.lx, commands[i].class);
					strcpy(sub.lx, commands[i].segment);
					strcat(electro, commands[i].command);
					strcat(electro, " ");
					strcat(electro, file);
					strcat(electro, ".");
					strcat(electro, commands[i].segment);
					strcat(electro, " ");
					sprintf(boom, "%i", GetFuncVarCount(class, commands[i].segment));
					strcat(electro, boom);

					if (IsFuncConstr(cls, sub) == 2)
					{
						int mem = 0;
						strcat(electro, "\npush constant ");
						mem = Fields(commands[i].target);
						sprintf(boom, "%i", mem);
						strcat(electro, boom);
						strcat(electro, "\n");
						strcat(electro, "call Memory.alloc 1\n");
						strcat(electro, "pop pointer 0");
					}
					else if (IsFuncConstr(cls, sub) == 3)
					{
						strcat(electro, "\npush argument 0\n");
						strcat(electro, "pop pointer 0");
					}
				}
				else if (strcmp(commands[i].command, "call") == 0)
				{
					Token newtk;
					strcat(electro, commands[i].command);
					strcat(electro, " ");

					if (IsClass(commands[i].class) == 1)
					{
						strcat(electro, commands[i].class);
						strcpy(newtk.lx, commands[i].class);
					}
					else
					{
						newtk = GetClassByVar(commands[i].target, commands[i].class);
						strcat(electro, newtk.lx);
					}

					strcat(electro, ".");
					strcat(electro, commands[i].segment);
					strcat(electro, " ");
					sprintf(boom, "%i", GetFuncArgCount(newtk.lx, commands[i].segment));
					strcat(electro, boom);
				}
				else
				{
					strcat(electro, commands[i].command);
					strcat(electro, " ");
					strcat(electro, commands[i].segment);
					strcat(electro, " ");
					strcat(electro, commands[i].argument);
				}

				strcat(electro, "\n");
			}
			else if (commands[i].mode == FORCE)
			{
				strcat(electro, commands[i].command);
				strcat(electro, "\n");
			}
			else if (commands[i].mode == THROTTLE || commands[i].mode == TRANSTAR)
			{
				char off[16];
				int m2;

				if ((commands[i].mode == TRANSTAR) && (IsClass(commands[i].argument) != 0)) continue;

				strcat(electro, commands[i].command);
				strcat(electro, " ");

				Token tok = GetVarKindById(commands[i].class, commands[i].segment, commands[i].argument);

				if (strcmp(tok.lx, "field") == 0)
				{
					strcat(electro, "this");
					strcat(electro, " ");
					m2 = GetClassVarOffset(commands[i].class, commands[i].argument);
				}
				else if (strcmp(tok.lx, "static") == 0)
				{
					strcat(electro, "static");
					strcat(electro, " ");
					m2 = GetClassVarOffset(commands[i].class, commands[i].argument);
				}
				else if (strcmp(tok.lx, "argument") == 0)
				{
					strcat(electro, "argument");
					strcat(electro, " ");
					m2 = GetSubVarOffset(commands[i].class, commands[i].segment, commands[i].argument);
				}
				else if (strcmp(tok.lx, "var") == 0)
				{
					strcat(electro, "local");
					strcat(electro, " ");
					m2 = GetSubVarOffset(commands[i].class, commands[i].segment, commands[i].argument);
				}
				else strcat(electro, "__UNKNOWN__SEGMENT__");

				sprintf(off, "%i", m2);
				strcat(electro, off);
				strcat(electro, "\n");
			}
		}
	}

	strcat(electro, "\0");
	return electro;
}

void ReleaseComms(void)
{
	comm_count = 0;
}

void NewCommand(char *cls, char *cmd, char *seg, char *arg, int mode, char *caller)
{
	if (comm_count == 0) commands = malloc(sizeof(struct comm) *16);
	else if ((comm_count != 0) && (comm_count % 16 == 0)) commands = realloc(commands, sizeof(struct comm) *(comm_count + 16));
	struct comm new_comm;
	if (mode == FORCE) new_comm.mode = FORCE;
	else if (mode == HUNT) new_comm.mode = HUNT;
	else if (mode == THROTTLE) new_comm.mode = THROTTLE;
	else if (mode == TRANSTAR) new_comm.mode = TRANSTAR;
	strcpy(new_comm.class, cls);
	strcpy(new_comm.command, cmd);
	strcpy(new_comm.segment, seg);
	strcpy(new_comm.argument, arg);
	strcpy(new_comm.target, caller);
	commands[comm_count] = new_comm;
	comm_count++;
}

void Reconnaissance(void)
{
	int stick, filled, arg,
	var;
	for (int i = 0; i < classcnt; i++)
	{
		stick = 0;
		filled = 0;
		for (int j = 0; j < global[i].count; j++)
		{
			arg = 0;
			var = 0;
			if (strcmp(global[i].scope[j].kind, "static") == 0) global[i].scope[j].inx = stick++;
			else if (strcmp(global[i].scope[j].kind, "field") == 0) global[i].scope[j].inx = filled++;
			else
			{
				for (int k = 0; k < global[i].scope[j].count; k++)
				{
					if (strcmp(global[i].scope[j].scope[k].kind, "var") == 0) global[i].scope[j].scope[k].inx =
						var ++;
					else if (strcmp(global[i].scope[j].scope[k].kind, "argument") == 0) global[i].scope[j].scope[k].inx = arg++;
				}

				global[i].scope[j].args = arg;
				global[i].scope[j].vars =
					var;
			}
		}
	}
}

ParserInfo Showdown(void)
{
	ParserInfo poker;

	for (int i = 0; i < kek; i++)
	{
		if (traitors[i].a_mysterious_value == 2)
		{
			if (LookupClass(traitors[i].classn) == 0)
			{
				poker.er = undecIdentifier;
				poker.tk = traitors[i].classn;
				return poker;
			}
		}
		else if (traitors[i].a_mysterious_value == 1)
		{
			// method calls an object id
			if (LookupClass(traitors[i].classn) == 0)
			{
				if (LookupClassScope(traitors[i].caller, traitors[i].classn) == 0 &&
					LookupSubroutineScope(traitors[i].caller, traitors[i].subcaller, traitors[i].classn) == 0)
				{
					poker.er = undecIdentifier;
					poker.tk = traitors[i].classn;
					return poker;
				}

				Token tok = GetIdType(traitors[i].caller, traitors[i].classn);
				if (tok.tp == ERR)
				{
					poker.er = undecIdentifier;
					poker.tk = traitors[i].classn;
					return poker;
				}

				if (LookupClass(tok) == 0)
				{
					poker.er = undecIdentifier;
					poker.tk = tok;
					return poker;
				}

				if (LookupClassScope(tok, traitors[i].subn) == 0)
				{
					poker.er = undecIdentifier;
					poker.tk = traitors[i].subn;
					return poker;
				}
			}

			// method calls a class name
			else
			{
				if (LookupClassScope(traitors[i].classn, traitors[i].subn) == 0)
				{
					poker.er = undecIdentifier;
					poker.tk = traitors[i].subn;
					return poker;
				}

				if (IsFuncConstr(traitors[i].classn, traitors[i].subn) == 0)
				{
					poker.er = undecIdentifier;
					poker.tk = traitors[i].subn;
					return poker;
				}

				if (IsFuncConstr(traitors[i].classn, traitors[i].subn) == 2)
				{
					Token tkk = GetVarIdByType(traitors[i].caller, traitors[i].classn);
					if (LookupClassScope(traitors[i].caller, tkk) == 0 &&
						LookupSubroutineScope(traitors[i].caller, traitors[i].subcaller, tkk) == 0)
					{
						poker.er = undecIdentifier;
						poker.tk = traitors[i].classn;
						return poker;
					}
				}
			}
		}
		else if (traitors[i].a_mysterious_value == ELITE)
		{
			if (LookupClassScope(traitors[i].caller, traitors[i].classn) == 0 &&
				LookupSubroutineScope(traitors[i].caller, traitors[i].subcaller, traitors[i].classn) == 0)
			{
				poker.er = undecIdentifier;
				poker.tk = traitors[i].classn;
				return poker;
			}

			Token toki;
			toki = GetVarTypeById(traitors[i].caller, traitors[i].classn);

			if (toki.tp == ERR)
			{
				poker.er = undecIdentifier;
				poker.tk = traitors[i].classn;
				return poker;
			}

			if (LookupClass(toki) == 0)
			{
				poker.er = undecIdentifier;
				poker.tk = traitors[i].classn;
				return poker;
			}

			if (LookupClassScope(toki, traitors[i].subn) == 0)
			{
				poker.er = undecIdentifier;
				poker.tk = traitors[i].subn;
				return poker;
			}
		}
		else
		{
			if (LookupClassScope(traitors[i].caller, traitors[i].subn) == 0)
			{
				poker.er = undecIdentifier;
				poker.tk = traitors[i].subn;
				return poker;
			}
		}
	}

	Token izer = Hunt();
	if (izer.tp != ERR)
	{
		poker.er = undecIdentifier;
		poker.tk = izer;
		return poker;
	}

	poker.er = none;
	return poker;
}

void Suspend(Token suspect)
{
	if (tekken_count == 0) tekken = malloc(sizeof(Token) *16);
	else if (tekken_count != 0 && tekken_count % 16 == 0) tekken = realloc(tekken, sizeof(Token) *(tekken_count + 16));
	tekken_count++;
	tekken[tekken_count - 1] = suspect;
}

Token Hunt(void)
{
	Token tictac;
	tictac.tp = ERR;
	for (int j = 0; j < tekken_count; j++)
		if (LookupClass(tekken[j]) == 0) return tekken[j];
	return tictac;
}

Token VarKindById(char *classname, char *subscope, char *aidi)
{
	Token tok;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, classname) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, aidi) == 0)
				{
					strcpy(tok.lx, global[i].scope[j].kind);
					return tok;
				}

				for (int k = 0; k < global[i].scope[j].count; k++)
				{
					if (strcmp(global[i].scope[j].scope[k].id, aidi) == 0)
					{
						strcpy(tok.lx, global[i].scope[j].scope[k].kind);
						return tok;
					}
				}
			}
		}
	}

	tok.tp = ERR;
	return tok;
}

Token GetVarKindById(char *classname, char *subscope, char *aidi)
{
	Token tok;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, classname) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, subscope) == 0)
				{
					for (int k = 0; k < global[i].scope[j].count; k++)
					{
						if (strcmp(global[i].scope[j].scope[k].id, aidi) == 0)
						{
							strcpy(tok.lx, global[i].scope[j].scope[k].kind);
							return tok;
						}
					}
				}
			}
		}
	}

	for (int n = 0; n < classcnt; n++)
	{
		if (strcmp(global[n].id, classname) == 0)
		{
			for (int m = 0; m < global[n].count; m++)
			{
				if (strcmp(global[n].scope[m].id, aidi) == 0)
				{
					strcpy(tok.lx, global[n].scope[m].kind);
					return tok;
				}
			}
		}
	}

	tok.tp = ERR;
	strcpy(tok.lx, "__KIND__NOT__IDENTIFIED__");
	return tok;
}

Token GetClassNameByVarId(char *
	var)
{
	Token tok;
	for (int i = 0; i < classcnt; i++)
	{
		for (int j = 0; j < global[i].count; j++)
		{
			if (strcmp(global[i].scope[j].id,
					var) == 0)
			{
				strcpy(tok.lx, global[i].scope[j].type);
				return tok;
			}
		}
	}

	tok.tp = ERR;
	return tok;
}

Token GetVarTypeById(Token classname, Token aidi)
{
	Token tok;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, classname.lx) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, aidi.lx) == 0)
				{
					strcpy(tok.lx, global[i].scope[j].type);
					return tok;
				}

				for (int k = 0; k < global[i].scope[j].count; k++)
				{
					if (strcmp(global[i].scope[j].scope[k].id, aidi.lx) == 0)
					{
						strcpy(tok.lx, global[i].scope[j].scope[k].type);
						return tok;
					}
				}
			}
		}
	}

	tok.tp = ERR;
	return tok;
}

Token GetVarIdByType(Token classname, Token typ)
{
	Token tok;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, classname.lx) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].type, typ.lx) == 0)
				{
					strcpy(tok.lx, global[i].scope[j].id);
					return tok;
				}

				for (int k = 0; k < global[i].scope[j].count; k++)
				{
					if (strcmp(global[i].scope[j].scope[k].type, typ.lx) == 0)
					{
						strcpy(tok.lx, global[i].scope[j].scope[k].id);
						return tok;
					}
				}
			}
		}
	}

	tok.tp = ERR;
	return tok;
}

void Fold(Token classname)
{
	if (kek == 0) traitors = malloc(sizeof(struct by_lightning) *4);
	else if (kek != 0 && kek % 4 == 0) traitors = realloc(traitors, sizeof(struct by_lightning) *(kek + 4));
	struct by_lightning new;
	new.classn = classname;
	new.a_mysterious_value = 2;
	kek++;
	traitors[kek - 1] = new;
}

void Cold(Token call, Token callsub, Token subname)
{
	if (kek == 0) traitors = malloc(sizeof(struct by_lightning) *4);
	else if (kek != 0 && kek % 4 == 0) traitors = realloc(traitors, sizeof(struct by_lightning) *(kek + 4));
	struct by_lightning new;
	new.subcaller = callsub;
	new.caller = call;
	new.subn = subname;
	new.a_mysterious_value = 0;
	kek++;
	traitors[kek - 1] = new;
}

void Hold(Token callr, Token subcallr, Token id1, Token id2)
{
	if (kek == 0) traitors = malloc(sizeof(struct by_lightning) *4);
	else if (kek != 0 && kek % 4 == 0) traitors = realloc(traitors, sizeof(struct by_lightning) *(kek + 4));
	struct by_lightning new;
	new.subcaller = subcallr;
	new.caller = callr;
	new.classn = id1;
	new.subn = id2;
	new.a_mysterious_value = ELITE;
	kek++;
	traitors[kek - 1] = new;
}

void Halt(Token call, Token callsub, Token classname, Token subname)
{
	if (kek == 0) traitors = malloc(sizeof(struct by_lightning) *4);
	else if (kek != 0 && kek % 4 == 0) traitors = realloc(traitors, sizeof(struct by_lightning) *(kek + 4));
	struct by_lightning new;
	new.subcaller = callsub;
	new.caller = call;
	new.classn = classname;
	new.subn = subname;
	new.a_mysterious_value = 1;
	kek++;
	traitors[kek - 1] = new;
}

void InsertClass(Token name)
{
	if (classcnt == 0) global = malloc(sizeof(struct class) *4);
	else if ((classcnt != 0) && (classcnt % 4 == 0)) global = realloc(global, sizeof(struct class) *(classcnt + 4));
	struct class new;
	strcpy(new.id, name.lx);
	classcnt++;
	new.count = 0;
	new.scope = NULL;
	global[classcnt - 1] = new;
}

void InsertClassScope(Token name, char *tp, char *kd)
{
	if (classcnt == 0) return;
	else if (global[classcnt - 1].scope == NULL && global[classcnt - 1].count == 0)
		global[classcnt - 1].scope = malloc(sizeof(struct class_scope) *4);
	else if (global[classcnt - 1].count != 0 && (global[classcnt - 1].count % 4 == 0))
		global[classcnt - 1].scope = realloc(global[classcnt - 1].scope, sizeof(struct class_scope) *(global[classcnt - 1].count + 4));
	struct class_scope new;
	strcpy(new.id, name.lx);
	strcpy(new.type, tp);
	strcpy(new.kind, kd);
	new.count = 0;
	new.scope = NULL;
	new.inx = ELITE;
	new.args = ELITE;
	new.vars = ELITE;
	global[classcnt - 1].count++;
	global[classcnt - 1].scope[global[classcnt - 1].count - 1] = new;
	if (strcmp(kd, "method") == 0)
	{
		Token t;
		strcpy(t.lx, "this");
		InsertSubroutineScope(t, global[classcnt - 1].id, "argument");
	}
}

void InsertSubroutineScope(Token name, char *tp, char *kd)
{
	if (classcnt == 0) return;

	else if (global[classcnt - 1].scope[global[classcnt - 1].count - 1].count == 0 &&
		global[classcnt - 1].scope[global[classcnt - 1].count - 1].scope == NULL)
		global[classcnt - 1].scope[global[classcnt - 1].count - 1].scope = malloc(sizeof(struct subroutine_scope) *4);

	else if (global[classcnt - 1].scope[global[classcnt - 1].count - 1].count != 0 &&
		global[classcnt - 1].scope[global[classcnt - 1].count - 1].count % 4 == 0)
		global[classcnt - 1].scope[global[classcnt - 1].count - 1].scope = realloc(global[classcnt - 1].scope[global[classcnt - 1].count - 1].scope,
			sizeof(struct subroutine_scope) *(global[classcnt - 1].scope[global[classcnt - 1].count - 1].count + 4));

	struct subroutine_scope new;
	new.inx = ELITE;
	strcpy(new.id, name.lx);
	strcpy(new.type, tp);
	strcpy(new.kind, kd);
	global[classcnt - 1].scope[global[classcnt - 1].count - 1].count++;
	global[classcnt - 1].scope[global[classcnt - 1].count - 1].scope[global[classcnt - 1].scope[global[classcnt - 1].count - 1].count - 1] = new;
}

int LookupClass(Token name)
{
	if (classcnt == 0) return 0;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, name.lx) == 0) return 1;
	}

	return 0;
}

int LookupClassScope(Token classname, Token scopename)
{
	if (classcnt == 0) return 0;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, classname.lx) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, scopename.lx) == 0) return 1;
			}
		}
	}

	return 0;
}

int LookupClassScopeVarType(Token classname, char *typ)
{
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, classname.lx) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].type, typ) == 0) return 1;
			}
		}
	}

	return 0;
}

int LookupSubroutineScopeVarType(Token classname, Token subname, char *typ)
{
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, classname.lx) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, subname.lx) == 0)
				{
					for (int k = 0; k < global[i].scope[j].count; k++)
					{
						if (strcmp(global[i].scope[j].scope[k].type, typ) == 0) return 1;
					}
				}
			}
		}
	}

	return 0;
}

int LookupSubroutineScope(Token classname, Token scopename, Token subscopename)
{
	if (classcnt == 0) return 0;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, classname.lx) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, scopename.lx) == 0)
				{
					for (int k = 0; k < global[i].scope[j].count; k++)
					{
						if (strcmp(global[i].scope[j].scope[k].id, subscopename.lx) == 0) return 1;
					}
				}
			}
		}
	}

	return 0;
}

int IsFuncConstr(Token class, Token subroute)
{
	if (LookupClass(class) == 0 || LookupClassScope(class, subroute) == 0) return 0;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class.lx) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, subroute.lx) == 0)
				{
					if (strcmp(global[i].scope[j].kind, "function") == 0) return 1;
					if (strcmp(global[i].scope[j].kind, "constructor") == 0) return 2;
					if (strcmp(global[i].scope[j].kind, "method") == 0) return 3;
				}
			}
		}
	}

	return 0;
}

void Release(void)
{
	tekken_count = 0;
	classcnt = 0;
	kek = 0;
}

Token GetIdType(Token class, Token id)
{
	Token tok;
	for (int i = 0; i < classcnt; i++)
	{
		if (strcmp(global[i].id, class.lx) == 0)
		{
			for (int j = 0; j < global[i].count; j++)
			{
				if (strcmp(global[i].scope[j].id, id.lx) == 0)
				{
					strcpy(tok.lx, global[i].scope[j].type);
					return tok;
				}

				for (int k = 0; k < global[i].scope[j].count; k++)
				{
					if (strcmp(global[i].scope[j].scope[k].id, id.lx) == 0)
					{
						strcpy(tok.lx, global[i].scope[j].scope[k].type);
						return tok;
					}
				}
			}
		}
	}

	tok.tp = ERR;
	return tok;
}