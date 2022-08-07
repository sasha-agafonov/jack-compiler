#include "compiler.h"
#include "dirent.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "lexer.h"
#include "parser.h"
#include "symbols.h"

int InitCompiler () {
	return 1; }

ParserInfo compile (char* dir_name) {
	ParserInfo p;
	char path[128];
	char vm[128];
	char* pseudopaths[12];
	int pseudo_count = 0;
	DIR *dir;
	FILE *vmf;
	struct dirent *entry;
	dir = opendir(".");
	Release();
	ReleaseComms();

  for (int i = 0; i < 12; i++) pseudopaths[i] = malloc(sizeof(char) * 64);

	if (dir) {
		while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
			strcpy(path, entry->d_name);
			if (!(strcmp(path + strlen(path) - 5, ".jack"))) {
				InitParser(path);
				p = Parse();
				if (p.er != none)	return p;	} } } }
	ReleaseComms();
	strcpy(path, "./");
	strcat(path, dir_name);
	dir = opendir(dir_name);

	if (dir) {
		while ((entry = readdir(dir)) != NULL) {
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				strcat(path, "/");
				strcat(path, entry->d_name);
				if (!(strcmp(path + strlen(path) - 5, ".jack"))) {
					InitParser(path);
					p = Parse(); }
				if (!(strcmp(path + strlen(path) - 5, ".jack"))) {
					for (int i = 0; i < strlen(path) - 3; i++) {
						if (i == strlen(path) - 4) vm[i] = '\0';
						else vm[i] = path[i]; }
					strcat(vm, "vm");
					vmf = fopen(vm, "w+");
					strcpy(pseudopaths[pseudo_count++], vm);
					memset(vm, 0, strlen(vm)); }

				if (p.er != none ) return p;
				strcpy(path, "./");
				strcat(path, dir_name); } }
		closedir(dir); }

	fclose(vmf);
	p = Showdown();
	Reconnaissance();
	char* tmp[128];

	for (int j = 0; j < pseudo_count; j++) {
		vmf = fopen(pseudopaths[j], "w+");
		fprintf(vmf, "%s", Traverse(GetEndClass(pseudo_count - j)));
		fclose(vmf); }

	Release();
	ReleaseComms();
	return p; }

int StopCompiler ()
{
	return 1;
}


#ifndef TEST_COMPILER
int main ()
{
	InitCompiler ();
	ParserInfo p = compile ("Pong");
	PrintError (p);
	StopCompiler ();
	return 1;
}
#endif
