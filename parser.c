#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "symbols.h"

int ExpressVoid();
int TypeVoid();
int StatementVoid();

ParserInfo SemicolonExp();
ParserInfo OpenBraceExp();
ParserInfo OpenParentExp();
ParserInfo CloseBraceExp();
ParserInfo CloseParentExp();
ParserInfo CloseBracketExp();
ParserInfo IdExp();
ParserInfo Factor();
ParserInfo Term();
ParserInfo ArExpress();
ParserInfo RelExpress();
ParserInfo Express();
ParserInfo ExpressList();
ParserInfo SubCall();
ParserInfo Operand();
ParserInfo VarStat();
ParserInfo LetStat();
ParserInfo IfStat();
ParserInfo WhileStat();
ParserInfo DoStat();
ParserInfo ReturnStat();
ParserInfo Statement();
ParserInfo SubBody();
ParserInfo ParamList();
ParserInfo SubDec();
ParserInfo Type();
ParserInfo ClassVarDec();
ParserInfo MemberDec();
ParserInfo ClassDec();

int labler = 0;
int lebler = 0;
Token cclass;
Token cscope;

int InitParser (char* file_name) {
	InitLexer(file_name);
	return 1; }

int ExpressVoid(void) {
	Token tok = PeekNextToken();
	if (tok.tp == SYMBOL && (strcmp(tok.lx, "-") == 0 || strcmp(tok.lx, "~") == 0
		|| strcmp(tok.lx, "(") == 0)) return 1;
	else if (tok.tp == ID || tok.tp == STRING || tok.tp == INT) return 1;
	else if (tok.tp == RESWORD && (strcmp(tok.lx, "true") == 0 || strcmp(tok.lx, "false") == 0
		|| strcmp(tok.lx, "this") == 0 || strcmp(tok.lx, "null") == 0)) return 1;
	return 0; }


int TypeVoid(void) {
	Token tok = PeekNextToken();
	if (tok.tp == ID) return 1;
	else if (tok.tp == RESWORD && (strcmp(tok.lx, "int") == 0 || strcmp(tok.lx, "char") == 0
		|| strcmp(tok.lx, "boolean") == 0)) return 1;
	return 0; }


int StatementVoid(void) {
	Token tok = PeekNextToken();
	if (tok.tp == RESWORD) {
		if (strcmp(tok.lx, "var") == 0 || strcmp(tok.lx, "let") == 0 ||
				strcmp(tok.lx, "if") == 0 || strcmp(tok.lx, "while") == 0 ||
				strcmp(tok.lx, "do") == 0 || strcmp(tok.lx, "return") == 0) return 1;
				return 0; }
	return 0; }


ParserInfo SemicolonExp(void) {
	ParserInfo pok;
	Token tok = GetNextToken();
	pok.er = none;
	if (!(tok.tp == SYMBOL && strcmp(tok.lx, ";") == 0)) {
		pok.er = semicolonExpected;
		pok.tk = tok; }
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok; }
	return pok; }


ParserInfo OpenBraceExp(void) {
	ParserInfo pok;
	Token tok = GetNextToken();
	pok.er = none;
	if (!(tok.tp == SYMBOL && strcmp(tok.lx, "{") == 0)) {
		pok.er = openBraceExpected;
		pok.tk = tok; }
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok; }
	return pok; }


ParserInfo OpenParentExp(void) {
	ParserInfo pok;
	Token tok = GetNextToken();
	pok.er = none;
	if (!(tok.tp == SYMBOL && strcmp(tok.lx, "(") == 0)) {
		pok.er = openParenExpected;
		pok.tk = tok; }
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok; }
	return pok; }


ParserInfo CloseBraceExp(void) {
	ParserInfo pok;
	Token tok = GetNextToken();
	pok.er = none;
	if (!(tok.tp == SYMBOL && strcmp(tok.lx, "}") == 0)) {
		pok.er = closeBraceExpected;
		pok.tk = tok; }
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok; }
	return pok; }


ParserInfo CloseParentExp(void) {
	ParserInfo pok;
	Token tok = GetNextToken();
	pok.er = none;
	if (!(tok.tp == SYMBOL && strcmp(tok.lx, ")") == 0)) {
		pok.er = closeParenExpected;
		pok.tk = tok; }
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok; }
	return pok; }


ParserInfo CloseBracketExp(void) {
	ParserInfo pok;
	Token tok = GetNextToken();
	pok.er = none;
	if (!(tok.tp == SYMBOL && strcmp(tok.lx, "]") == 0)) {
		pok.er = closeBracketExpected;
		pok.tk = tok; }
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok; }
	return pok; }


ParserInfo IdExp(void) {
	ParserInfo pok;
	Token tok = GetNextToken();
	pok.er = none;
	if (tok.tp != ID) {
		pok.er = idExpected;
		pok.tk = tok; }
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok; }
	return pok; }


ParserInfo Factor(void) {
	ParserInfo pok;
	Token action;
	Token tok = PeekNextToken();
	action = tok;
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;
	if (tok.tp == SYMBOL && (strcmp(tok.lx, "-") == 0 || strcmp(tok.lx, "~") == 0)) tok = GetNextToken();
	pok = Operand();
	if (strcmp(action.lx, "-") == 0) NewCommand(cclass.lx, "neg", "-1", "-1", FORCE, cclass.lx);
	else if (strcmp(action.lx, "~") == 0) NewCommand(cclass.lx, "not", "-1", "-1", FORCE, cclass.lx);
	return pok; }


ParserInfo Term(void) {
	ParserInfo pok;
	Token action;
	pok.er = none;
	pok = Factor();
	if (pok.er != none) return pok;
	Token tok = PeekNextToken();
	action = tok;
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
		while (tok.tp == SYMBOL && (strcmp(tok.lx, "/") == 0 || strcmp(tok.lx, "*") == 0 )) {
			GetNextToken();
			pok = Factor();
			if (pok.er != none) return pok;
			if (strcmp(action.lx, "/") == 0) NewCommand("Math", "call", "divide", cclass.lx, HUNT, cclass.lx);
			else NewCommand("Math", "call", "multiply", cclass.lx, HUNT, cclass.lx);
			tok = PeekNextToken();
			action = tok;
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; } }
	return pok; }


ParserInfo ArExpress(void) {
	ParserInfo pok;
	Token action;
	pok.er = none;
	pok = Term();
	if (pok.er != none) return pok;
	Token tok = PeekNextToken();
	action = tok;
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
		while (tok.tp == SYMBOL && (strcmp(tok.lx, "+") == 0 || strcmp(tok.lx, "-") == 0 )) {
			GetNextToken();
			pok = Term();
			if (pok.er != none) return pok;
			if (strcmp(action.lx, "+") == 0) NewCommand(cclass.lx, "add", "-1", "-1", FORCE, cclass.lx);
			else NewCommand(cclass.lx, "sub", "-1", "-1", FORCE, cclass.lx);
			tok = PeekNextToken();
			action = tok;
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; } }
	return pok; }


ParserInfo RelExpress(void) {
	ParserInfo pok;
	Token action;
	pok.er = none;
	pok = ArExpress();
	if (pok.er != none) return pok;
	Token tok = PeekNextToken();
	action = tok;
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
		while (tok.tp == SYMBOL && (strcmp(tok.lx, "=") == 0 || strcmp(tok.lx, ">") == 0 || strcmp(tok.lx, "<") == 0)) {
			GetNextToken();
			pok = ArExpress();
			if (pok.er != none) return pok;
			if (strcmp(action.lx, "<") == 0) NewCommand(cclass.lx, "lt", "-1", "-1", FORCE, cclass.lx);
			else if (strcmp(action.lx, ">") == 0) NewCommand(cclass.lx, "gt", "-1", "-1", FORCE, cclass.lx);
			else NewCommand(cclass.lx, "eq", "-1", "-1", FORCE, cclass.lx);
			tok = PeekNextToken();
			action = tok;
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; } }
	return pok; }


ParserInfo Express(void) {
	ParserInfo pok;
	Token action;
	pok.er = none;
	pok = RelExpress();
	if (pok.er != none) return pok;
	Token tok = PeekNextToken();
	action = tok;
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
		while (tok.tp == SYMBOL && (strcmp(tok.lx, "&") == 0 || strcmp(tok.lx, "|") == 0 )) {
			GetNextToken();
			pok = RelExpress();
			if (pok.er != none) return pok;
			if (strcmp(action.lx, "&") == 0) NewCommand(cclass.lx, "and", "-1", "-1", FORCE, cclass.lx);
			else NewCommand(cclass.lx, "or", "-1", "-1", FORCE, cclass.lx);
			tok = PeekNextToken();
			action = tok;
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; } }
	return pok; }


ParserInfo ExpressList(void) {
	ParserInfo pok;
	pok.er = none;
	if (ExpressVoid() == 1) {
		pok = Express();
		if (pok.er != none) return pok;
		Token tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }
		while (tok.tp == SYMBOL && (strcmp(tok.lx, ",") == 0)) {
			GetNextToken();
			pok = Express();
			if (pok.er != none) return pok;
			tok = PeekNextToken();
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; } }
			return pok; }
	return pok; }


ParserInfo Operand(void) {
	ParserInfo pok;
	char acter[32];
	char coal[32];
	char alpha;
	int alpha_int;
	int magic_dot = 0;
	Token id;
	Token id2;
	Token tok = GetNextToken();
	id2.tp = ERR;

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == ID) {
		id = tok;
		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }

		if ((strcmp(tok.lx, ".") != 0) && (strcmp(tok.lx, "[") != 0) && strcmp(tok.lx, "(") != 0) NewCommand(cclass.lx, "push", cscope.lx, id.lx, THROTTLE, cclass.lx);

		if (tok.tp == SYMBOL && strcmp(tok.lx, ".") == 0) {
			magic_dot++;
			GetNextToken();
			id2 = PeekNextToken();
			pok = IdExp();
			if (pok.er != none) return pok; }

			tok = PeekNextToken();
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; }

		if (tok.tp == SYMBOL && strcmp(tok.lx, "[") == 0) {
			if (id2.tp == ERR) {
				if (LookupClassScope(cclass, id) == 0 && LookupSubroutineScope(cclass, cscope, id) == 0) {
					pok.er = undecIdentifier;
					pok.tk = id;
					return pok;	} }
				else Hold(cclass, cscope, id, id2);
			GetNextToken();
			pok = Express();

			if (magic_dot == 0) NewCommand(cclass.lx, "push", cscope.lx, id.lx, THROTTLE, cclass.lx);
			else NewCommand(id.lx, "push", cscope.lx, id2.lx, THROTTLE, cclass.lx);

			NewCommand(cclass.lx, "add", cscope.lx, cclass.lx, FORCE, cclass.lx);
			NewCommand(cclass.lx, "pop pointer 1", cscope.lx, cclass.lx, FORCE, cclass.lx);
			NewCommand(cclass.lx, "push that 0", cscope.lx, cclass.lx, FORCE, cclass.lx);

			if (pok.er != none) return pok;
			pok = CloseBracketExp(); }

			else if (tok.tp == SYMBOL && strcmp(tok.lx, "(") == 0) {
				if (id2.tp == ERR) Cold(cclass, cscope, id);
				else {
					if (LookupClassScope(cclass, id) == 0 && LookupSubroutineScope(cclass, cscope, id) == 0) Suspend(id);
					Halt(cclass, cscope, id, id2); }

				GetNextToken();
				NewCommand(cclass.lx, "push", cscope.lx, id.lx, TRANSTAR, cclass.lx);
				pok = ExpressList();

				if (magic_dot == 0) NewCommand(cclass.lx, "call", id.lx, cclass.lx, HUNT, cclass.lx);
				else { NewCommand(id.lx, "call", id2.lx, cclass.lx, HUNT, cclass.lx); }
				if (pok.er != none) return pok;
				pok = CloseParentExp(); }

			else {
				if (LookupClassScope(cclass, id) == 0 && LookupSubroutineScope(cclass, cscope, id) == 0) {
					pok.er = undecIdentifier;
					pok.tk = id;
					return pok; } }
			return pok; }

	else if (tok.tp == SYMBOL) {
		if (strcmp(tok.lx, "(") == 0) {
			pok = Express();
			if (pok.er != none) return pok;
			pok = CloseParentExp();
			return pok;	}
		else {
			pok.er = syntaxError;
			pok.tk = tok;
			return pok;	} }

	else if (tok.tp == RESWORD) {
		if (strcmp(tok.lx, "true") != 0 && strcmp(tok.lx, "false") != 0 && strcmp(tok.lx, "null") != 0 && strcmp(tok.lx, "this") != 0) {
			pok.er = syntaxError;
			pok.tk = tok; }
			if (strcmp(tok.lx, "true") == 0) {
				NewCommand(cclass.lx, "push constant 0", "-1", "-1", FORCE, cclass.lx);
				NewCommand(cclass.lx, "not", "-1", "-1", FORCE, cclass.lx); }
			else if ((strcmp(tok.lx, "false") == 0) || strcmp(tok.lx, "null") == 0) NewCommand(cclass.lx, "push constant 0", "-1", "-1", FORCE, cclass.lx);
			else if (strcmp(tok.lx, "this") == 0) NewCommand(cclass.lx, "push pointer 0", "-1", "-1", FORCE, cclass.lx);
		return pok; }

	else if (tok.tp == INT) {
		strcpy(acter, "push constant ");
		strcat(acter, tok.lx);
		NewCommand(cclass.lx, acter, "-1", "-1", FORCE, cclass.lx);
		return pok; }

	else if (tok.tp == STRING) {
		alpha_int = strlen(tok.lx);
		sprintf(coal, "%i", alpha_int);
		strcpy(acter, "push constant ");
		strcat(acter, coal);
		NewCommand(cclass.lx, acter, "-1", "-1", FORCE, cclass.lx);
		NewCommand("String", "call", "new", cclass.lx, HUNT, cclass.lx);
		for (int x = 0; x < strlen(tok.lx); x++) {
			alpha = tok.lx[x];
			alpha_int = (int)alpha;
		  sprintf(coal, "%i", alpha_int);
			strcpy(acter, "push constant ");
			strcat(acter, coal);
			NewCommand(cclass.lx, acter, "-1", "-1", FORCE, cclass.lx);
			NewCommand("String", "call", "appendChar", cclass.lx, HUNT, cclass.lx); } }
	return pok; }


	ParserInfo SubCall(void) {
		ParserInfo pok;
		Token id;
		Token type;
		Token kind;
		Token tok;
		int magic_dot = 0;
		char boom[32];

		id = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }
		pok.er = none;
		pok = IdExp();
		if (pok.er != none) return pok;

		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }

		if (strcmp(tok.lx, ".") != 0) NewCommand(cclass.lx, "push pointer 0", "-1", "-1", FORCE, cclass.lx);

		if (tok.tp == SYMBOL && strcmp(tok.lx, ".") == 0) {
			magic_dot++;
			GetNextToken();
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; }
			tok = PeekNextToken();
			if (LookupClassScope(cclass, id) == 0 && LookupSubroutineScope(cclass, cscope, id) == 0) Suspend(id);
			Halt(cclass, cscope, id, tok);
			pok = IdExp();
			if (pok.er != none) return pok; }

		else Cold(cclass, cscope, id);
		pok = OpenParentExp();
		if (pok.er != none) return pok;
		if (magic_dot > 0) NewCommand(cclass.lx, "push", cscope.lx, id.lx, TRANSTAR, cclass.lx);
		pok = ExpressList();
		if (pok.er != none) return pok;
		pok = CloseParentExp();
		if (magic_dot == 0)	NewCommand(cclass.lx, "call", id.lx, cclass.lx, HUNT, cclass.lx);
		else NewCommand(id.lx, "call", tok.lx, cclass.lx, HUNT, cclass.lx);

		NewCommand(cclass.lx, "pop temp 0", "-1", "-1", FORCE, cclass.lx);

		return pok; }


ParserInfo VarStat(void) {
	ParserInfo pok;
	Token type;
	Token kind;
	Token tok = GetNextToken();

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && (strcmp(tok.lx, "var")) == 0) {

		kind = tok;
		tok = PeekNextToken();

		strcpy(type.lx, tok.lx);

		pok = Type();
		if (pok.er != none) return pok;
		tok = PeekNextToken();
		if (LookupSubroutineScope(cclass, cscope, tok) == 1) {
			pok.er = redecIdentifier;
			pok.tk = tok;
			return pok; }
		InsertSubroutineScope(tok, type.lx, kind.lx);
		pok = IdExp();
		if (pok.er != none) return pok;
		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }
		while (tok.tp == SYMBOL && strcmp(tok.lx, ",") == 0) {
			GetNextToken();
			tok = PeekNextToken();
			if (LookupSubroutineScope(cclass, cscope, tok) == 1) {
				pok.er = redecIdentifier;
				pok.tk = tok;
				return pok; }
			InsertSubroutineScope(tok, type.lx, kind.lx);
			pok = IdExp();
			if (pok.er != none) return pok;
			tok = PeekNextToken();
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; } }
			pok = SemicolonExp();
			return pok; }
	else {
		pok.er = syntaxError;
		pok.tk = tok;
		return pok; } }


ParserInfo LetStat(void) {
	ParserInfo pok;
	Token id;
	int md = 0;
	Token tok = GetNextToken();

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && (strcmp(tok.lx, "let")) == 0) {
		tok = PeekNextToken();
		id = tok;
		if (LookupSubroutineScope(cclass, cscope, tok) == 0 && LookupClassScope(cclass, tok) == 0 && LookupClass(tok) == 0) {
			pok.er = undecIdentifier;
			pok.tk = tok;
			return pok; }
		pok = IdExp();
		if (pok.er != none) return pok;
		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }
		if (tok.tp == SYMBOL && (strcmp(tok.lx, "[") == 0 )) {
			md++;
			GetNextToken();
			pok = Express();
			NewCommand(cclass.lx, "push", cscope.lx, id.lx, THROTTLE, cclass.lx);
			NewCommand(cclass.lx, "add", cscope.lx, id.lx, FORCE, cclass.lx);
			if (pok.er != none) return pok;
			pok = CloseBracketExp();
			if (pok.er != none) return pok; }

		tok = GetNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }
			if (tok.tp == SYMBOL && strcmp(tok.lx, "=") == 0) {
				pok = Express();
				if (pok.er != none) return pok;
				pok = SemicolonExp();
				if (md == 0) NewCommand(cclass.lx, "pop", cscope.lx, id.lx, THROTTLE, cclass.lx);
				else {
					NewCommand(cclass.lx, "pop temp 0", cscope.lx, id.lx, FORCE, cclass.lx);
					NewCommand(cclass.lx, "pop pointer 1", cscope.lx, id.lx, FORCE, cclass.lx);
					NewCommand(cclass.lx, "push temp 0", cscope.lx, id.lx, FORCE, cclass.lx);
					NewCommand(cclass.lx, "pop that 0", cscope.lx, id.lx, FORCE, cclass.lx); }
				return pok; }
			else {
				pok.er = equalExpected;
				pok.tk = tok;
				return pok; } }
	else {
		pok.er = syntaxError;
		pok.tk = tok;
		return pok; } }


ParserInfo IfStat(void) {
	ParserInfo pok;
	char lbl[64];
	char lbl2[64];
	char label[64];
	char lebel[64];

	Token tok = GetNextToken();
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && strcmp(tok.lx, "if") == 0 ) {
		pok = OpenParentExp();
		if (pok.er != none) return pok;
		pok = Express();
		if (pok.er != none) return pok;
		pok = CloseParentExp();
		if (pok.er != none) return pok;

		sprintf(lebel, "%i", labler);
		strcpy(label, "if-goto M");
		strcat(label, lebel);
		NewCommand(cclass.lx, label, "-1", "-1", FORCE, cclass.lx);
		labler++;

		sprintf(lebel, "%i", labler);
		strcpy(label, "goto M");
		strcat(label, lebel);
		NewCommand(cclass.lx, label, "-1", "-1", FORCE, cclass.lx);

		sprintf(lebel, "%i", labler - 1);
		strcpy(label, "label M");
		strcat(label, lebel);
		NewCommand(cclass.lx, label, "-1", "-1", FORCE, cclass.lx);

		pok = OpenBraceExp();
		if (pok.er != none) return pok;
		while (StatementVoid() == 1) {
			pok = Statement();
			if (pok.er != none) return pok; }
		pok = CloseBraceExp();
		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }

    if (tok.tp == RESWORD && strcmp(tok.lx, "else") == 0) {
			GetNextToken();
			pok = OpenBraceExp();
			sprintf(lbl2, "%i", lebler);
			strcpy(lbl, "goto W");
			strcat(lbl, lbl2);
			NewCommand(cclass.lx, lbl, "-1", "-1", FORCE, cclass.lx);

			sprintf(lebel, "%i", labler);
			strcpy(label, "label M");
			strcat(label, lebel);
			NewCommand(cclass.lx, label, "-1", "-1", FORCE, cclass.lx);

			if (pok.er != none) return pok;
			while (StatementVoid() == 1) {
				pok = Statement();
				if (pok.er != none) return pok; }

			pok = CloseBraceExp();
			sprintf(lbl2, "%i", lebler);
			strcpy(lbl, "label W");
			strcat(lbl, lbl2);
			NewCommand(cclass.lx, lbl, "-1", "-1", FORCE, cclass.lx);
			lebler++;
			return pok; }

		sprintf(lebel, "%i", labler);
		strcpy(label, "label M");
		strcat(label, lebel);
		NewCommand(cclass.lx, label, "-1", "-1", FORCE, cclass.lx);
		labler++;

		return pok; }
	else {
		pok.er = syntaxError;
		pok.tk = tok;
	 	return pok; } }


ParserInfo WhileStat(void) {
	ParserInfo pok;
	Token tok = GetNextToken();
	char label[32];
	char lebel[32];

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	sprintf(lebel, "%i", labler);
	strcpy(label, "label M");
	strcat(label, lebel);
	NewCommand(cclass.lx, label, "-1", "-1", FORCE, cclass.lx);
	labler++;

	if (tok.tp == RESWORD && strcmp(tok.lx, "while") == 0) {
		pok = OpenParentExp();
		if (pok.er != none) return pok;
		pok = Express();
		if (pok.er != none) return pok;
		pok = CloseParentExp();
		if (pok.er != none) return pok;
		NewCommand(cclass.lx, "not", "-1", "-1", FORCE, cclass.lx);
		sprintf(lebel, "%i", labler);
		strcpy(label, "if-goto M");
		strcat(label, lebel);
		NewCommand(cclass.lx, label, "-1", "-1", FORCE, cclass.lx);

		pok = OpenBraceExp();
		if (pok.er != none) return pok;
		while (StatementVoid() == 1) {
			pok = Statement();
			if (pok.er != none) return pok; }
		pok = CloseBraceExp();

		sprintf(lebel, "%i", labler - 1);
		strcpy(label, "goto M");
		strcat(label, lebel);
		NewCommand(cclass.lx, label, "-1", "-1", FORCE, cclass.lx);

		sprintf(lebel, "%i", labler);
		strcpy(label, "label M");
		strcat(label, lebel);
		NewCommand(cclass.lx, label, "-1", "-1", FORCE, cclass.lx);
		labler++;

		return pok; }
	else {
		pok.er = syntaxError;
		pok.tk = tok;
		return pok; } }


ParserInfo DoStat(void) {
	ParserInfo pok;
	Token tok = GetNextToken();

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && strcmp(tok.lx, "do") == 0) {
		pok = SubCall();
		if (pok.er != none) return pok;
		pok = SemicolonExp();
		return pok; }
	else {
		pok.er = syntaxError;
		pok.tk = tok;
		return pok; } }


ParserInfo ReturnStat(void) {
	ParserInfo pok;
	Token tok = GetNextToken();

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && strcmp(tok.lx, "return") == 0) {
		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }

	if (tok.tp == ID || tok.tp == INT || tok.tp == STRING || (tok.tp == RESWORD &&
		(strcmp(tok.lx, "this") == 0 || strcmp(tok.lx, "true") == 0 ||
		strcmp(tok.lx, "false") == 0 || strcmp(tok.lx, "null") == 0)) ||
		(tok.tp == SYMBOL && strcmp(tok.lx, "(") == 0)) {
		pok = Express();

		if (pok.er != none) return pok; }
	else NewCommand(cclass.lx, "push constant 0", "-1", "-1", FORCE, cclass.lx);

	pok = SemicolonExp();
	NewCommand(cclass.lx, "return", "-1", "-1", FORCE, cclass.lx);
	return pok; }
	else {
		pok.er = syntaxError;
		pok.tk = tok;
		return pok; } }


ParserInfo Statement(void) {
	ParserInfo pok;
	Token tok = PeekNextToken();
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && strcmp(tok.lx, "var") == 0) {
		pok = VarStat();
		return pok; }

	else if (tok.tp == RESWORD && strcmp(tok.lx, "let") == 0) {
		pok = LetStat();
		return pok; }

	else if (tok.tp == RESWORD && strcmp(tok.lx, "if") == 0) {
		pok = IfStat();
		return pok; }

	else if (tok.tp == RESWORD && strcmp(tok.lx, "while") == 0) {
		pok = WhileStat();
		return pok; }

	else if (tok.tp == RESWORD && strcmp(tok.lx, "do") == 0) {
		pok = DoStat();
		return pok; }

	else if (tok.tp == RESWORD && strcmp(tok.lx, "return") == 0) {
		pok = ReturnStat();
		return pok; }

	else {
		pok.er = syntaxError;
		pok.tk = tok;
		return pok; } }


ParserInfo SubBody(void) {
	ParserInfo pok;
	pok.er = none;
	pok = OpenBraceExp();
	if (pok.er != none) return pok;
	while (StatementVoid() == 1) {
		pok = Statement();
		if (pok.er != none) return pok; }
	pok = CloseBraceExp();
	return pok; }


ParserInfo ParamList(void) {
	ParserInfo pok;
	Token type;
	Token kind;
	Token tok = PeekNextToken();

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == ID || (tok.tp == RESWORD && ((strcmp(tok.lx, "int") == 0) || (strcmp(tok.lx, "char") == 0) || (strcmp(tok.lx, "boolean") == 0)))) {
		if (tok.tp == ID) Fold(tok);
		type = tok;
		strcpy(kind.lx, "argument");
		GetNextToken();
		tok = PeekNextToken();
		if (LookupSubroutineScope(cclass, cscope, tok) == 1) {
			pok.er = redecIdentifier;
			pok.tk = tok;
			return pok; }
		InsertSubroutineScope(tok, type.lx, kind.lx);
		pok = IdExp();
		if (pok.er != none) return pok;
		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }

		while(tok.tp == SYMBOL && strcmp(tok.lx, ",") == 0) {
			GetNextToken();
			tok = PeekNextToken();
			if (tok.tp == ID)	Fold(tok);
			type = tok;
			pok = Type();
			if (pok.er != none) return pok;
			tok = PeekNextToken();
			if (LookupSubroutineScope(cclass, cscope, tok) == 1) {
				pok.er = redecIdentifier;
				pok.tk = tok;
				return pok; }
			InsertSubroutineScope(tok, type.lx, kind.lx);
			pok = IdExp();
			if (pok.er != none) return pok;
			tok = PeekNextToken();
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; } }
		return pok; }
	return pok; }


ParserInfo SubDec(void) {
	ParserInfo pok;
	Token type;
	Token kind;
	Token tok = GetNextToken();
	char boom[32];

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && (strcmp(tok.lx, "constructor") == 0 || strcmp(tok.lx, "function") == 0 || strcmp(tok.lx, "method") == 0)) {
		kind = tok;
		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
			return pok; }
		if (TypeVoid() == 1 || (tok.tp == RESWORD && strcmp(tok.lx, "void") == 0)) {
			type = tok;
			tok = GetNextToken();
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
				return pok; }
				tok = PeekNextToken();
				if (LookupClassScope(cclass, tok) == 1) {
					pok.er = redecIdentifier;
					pok.tk = tok;
					return pok; }
				InsertClassScope(tok, type.lx, kind.lx);
				cscope = tok;
				pok = IdExp();
				if (pok.er != none) return pok;
				pok = OpenParentExp();
				if (pok.er != none) return pok;
				pok = ParamList();
				if (pok.er != none) return pok;
				pok = CloseParentExp();
				NewCommand(cclass.lx, "function", tok.lx, cclass.lx, HUNT, cclass.lx);
				if (pok.er != none) return pok;
				pok = SubBody();
				return pok; }
		else {
			pok.er = syntaxError;
			pok.tk = tok;
			return pok; } }
	else {
		pok.er = subroutineDeclarErr;
		pok.tk = tok;
		return pok; } }


ParserInfo Type(void) {
	ParserInfo pok;
	Token tok = GetNextToken();
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;
	if (!((tok.tp == RESWORD && (strcmp(tok.lx, "int") == 0 || strcmp(tok.lx, "char") == 0
	|| strcmp(tok.lx, "boolean") == 0 )) || tok.tp == ID))  {
		pok.er = illegalType;
		pok.tk = tok; }
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok; }
	return pok; }


ParserInfo ClassVarDec(void) {
	ParserInfo pok;
	Token type;
	Token kind;
	Token tok = GetNextToken();

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && (strcmp(tok.lx, "static") == 0 || strcmp(tok.lx, "field") == 0 )) {
		kind = tok;
		type = PeekNextToken();
		if (type.tp == ID) Fold(type);
		pok = Type();
		if (pok.er != none) return pok;
		tok = PeekNextToken();
		if (LookupClassScope(cclass, tok) == 1) {
			pok.er = redecIdentifier;
			pok.tk = tok;
			return pok; }
		InsertClassScope(tok, type.lx, kind.lx);
		pok = IdExp();
		if (pok.er != none) return pok;
		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
		 	return pok; }
		while(tok.tp == SYMBOL && strcmp(tok.lx, ",") == 0) {
			GetNextToken();
			tok = PeekNextToken();
			if (LookupClassScope(cclass, tok) == 1) {
				pok.er = redecIdentifier;
				pok.tk = tok;
				return pok; }
			InsertClassScope(tok, type.lx, kind.lx);
			pok = IdExp();
			if (pok.er != none) return pok;
			tok = PeekNextToken();
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
			 	return pok; } }
			pok = SemicolonExp();
			return pok; }
	else {
		pok.er = classVarErr;
		pok.tk = tok;
	 	return pok; } }


ParserInfo MemberDec(void) {
	ParserInfo pok;
	Token tok = PeekNextToken();

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && (strcmp(tok.lx, "static") == 0 || strcmp(tok.lx, "field") == 0)) {
		pok = ClassVarDec();
		return pok; }
	else if (tok.tp == RESWORD && (strcmp(tok.lx, "constructor") == 0
		|| strcmp(tok.lx, "function") == 0 || strcmp(tok.lx, "method") == 0))	{
			pok = SubDec();
			return pok; }
	else {
		pok.er = memberDeclarErr;
		pok.tk = tok;
		return pok; } }


ParserInfo ClassDec(void) {
	ParserInfo pok;
	Token tok = GetNextToken();

	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok.er = none;

	if (tok.tp == RESWORD && strcmp(tok.lx, "class") == 0) {
		tok = PeekNextToken();
		if (LookupClass(tok) == 1) {
			pok.er = redecIdentifier;
			pok.tk = tok;
			return pok; }
		InsertClass(tok);
		cclass = tok;
		pok = IdExp();
		if (pok.er != none) return pok;
		pok = OpenBraceExp();
		if (pok.er != none) return pok;
		tok = PeekNextToken();
		if (tok.tp == ERR) {
			pok.er = lexerErr;
			pok.tk = tok;
		 	return pok; }

		while (tok.tp == RESWORD && (strcmp(tok.lx, "static") == 0 || strcmp(tok.lx, "field") == 0 ||
		strcmp(tok.lx, "constructor") == 0 || strcmp(tok.lx, "function") == 0 || strcmp(tok.lx, "method") == 0)) {
			pok = MemberDec();
			if (pok.er != none) return pok;
			tok = PeekNextToken();
			if (tok.tp == ERR) {
				pok.er = lexerErr;
				pok.tk = tok;
			 	return pok; } }
			pok = CloseBraceExp();
			return pok; }
	else {
		pok.er = classExpected;
		pok.tk = tok;
		return pok; } }


ParserInfo Parse(void) {
	ParserInfo pok;
 	Token tok = PeekNextToken();
	pok.er = none;
	if (tok.tp == EOFile) return pok;
	if (tok.tp == ERR) {
		pok.er = lexerErr;
		pok.tk = tok;
		return pok; }
	pok = ClassDec();
	return pok; }


int StopParser(void) {
	return 1; }


#ifndef TEST_PARSER

// int main(void) { return 0; }

#endif
