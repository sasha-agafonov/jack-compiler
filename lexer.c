#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define EMPTY -10
#define LINECOMM 456
#define BLOCKCOMM 752
#define APICOMM 947

char* buffer; // stores entire file
char fname[128]; // stores file name
int current_line; // current line
int current_char; // current char
int file_size; // file size
int char_count = 0; // total characters
int ernal_bleeding = 0;


int InitLexer (char* file_name) {
  FILE* fptr;
  char ch;
  int m = 0;
  fptr = fopen( file_name, "r");
  if (!fptr) return 0;
  strcpy(fname, file_name);
  fseek(fptr, 0, SEEK_END);
  file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);
  buffer = (char*)malloc(file_size + 1);
  fread(buffer, 1, file_size, fptr);
  buffer[file_size] = '\0';
  fclose(fptr);
  current_line = 1;
  current_char = 0;
  return 1; }


char* IsIdentifier(int posx) {
  if (isalpha(buffer[posx]) == 0 && buffer[posx] != '_') return NULL;
  char* unhappy_string = malloc(sizeof(char) * 128);
  for (int i = posx; i < file_size + 1; i++) {
    if (i != file_size ) {
      if (isalnum(buffer[i]) != 0 || buffer[i] == '_') unhappy_string[i - posx] = buffer[i];
      else {
        unhappy_string[i - posx] = '\0';
        return unhappy_string; } }
    else {
      unhappy_string[i - posx] = '\0';
      return unhappy_string; } }
  return NULL; }

char* IsInteger(int posx) {
  if (isdigit(buffer[posx]) == 0) return NULL;
  char* coal = malloc(sizeof(char) * 1024);
  for (int i = posx; i < file_size + 1; i++) {
    if (isdigit(buffer[i]) != 0) coal[i - posx] = buffer[i];
    else {
      coal[i - posx] = '\0';
      break; } }
  if (strlen(coal) == 0 ) return NULL;
  return coal; }


const char* IsBool(int posx) {
  char bull[6];
  for (int i = 0; i < 6; i++) bull[i] = buffer[posx + i];
  if (strncmp( bull, "null", strlen("null")) == 0 && isalnum(buffer[posx + strlen("null")]) == 0 && buffer[posx + strlen("null")] != '_') return "null";
  else if (strncmp( bull, "true", strlen("true")) == 0 && isalnum(buffer[posx + strlen("true")]) == 0 && buffer[posx + strlen("true")] != '_') return "true";
  else if (strncmp( bull, "false", strlen("false")) == 0 && isalnum(buffer[posx + strlen("false")]) == 0 && buffer[posx + strlen("false")] != '_') return "false";
  return NULL; }


char* GetString(int posx) {
  char* happy_string = malloc(sizeof(char) * 1024);
  for (int i = posx; i < file_size + 1; i++) {
    if (buffer[i] == '\"') {
      happy_string[i - posx] = '\0';
      return happy_string; }
    else if (buffer[i] == '\n') {
      happy_string[i - posx] = '\0';
      ernal_bleeding = 1;
      return happy_string; }
    happy_string[i - posx] = buffer[i]; }
  return NULL; }


int IsSymbol(char sym) {
  if (sym == '(' || sym == ')' || sym == '[' || sym == ']' || sym == '{'
   || sym == '}' || sym == ',' || sym == ';' || sym == '=' || sym == '.'
   || sym == '+' || sym == '-' || sym == '*' || sym == '/' || sym == '&'
   || sym == '|' || sym == '~' || sym == '<' || sym == '>' ) return 1;
  return 0; }


int IsEscapeSequence(char acter) {
  if (acter == '\a' || acter == '\b' || acter == '\e' || acter == '\f'
   || acter == '\n' || acter == '\r' || acter == '\t' || acter == '\v'
   || acter == '\"' ) return 1;
  return 0; }


int IsLegal(char coal) {
  if (IsSymbol(coal) == 1 || isalnum(coal) != 0 || IsEscapeSequence(coal) == 1
  || coal == '_' || coal == '/' || coal == '\"' || coal == ' ') return 1;
  return 0; }


const char* IsReserved(int pos) {
  char happy_string[12];
  for (int i = 0; i < 12; i++) happy_string[i] = buffer[pos + i];
  if (strncmp( happy_string, "class",       strlen("class"      )) == 0 && isalnum(buffer[pos + strlen("class"      )]) == 0 && buffer[pos + strlen("class"      )] != '_') return "class";
  if (strncmp( happy_string, "constructor", strlen("constructor")) == 0 && isalnum(buffer[pos + strlen("constructor")]) == 0 && buffer[pos + strlen("constructor")] != '_') return "constructor";
  if (strncmp( happy_string, "method",      strlen("method"     )) == 0 && isalnum(buffer[pos + strlen("method"     )]) == 0 && buffer[pos + strlen("method"     )] != '_') return "method";
  if (strncmp( happy_string, "function",    strlen("function"   )) == 0 && isalnum(buffer[pos + strlen("function"   )]) == 0 && buffer[pos + strlen("function"   )] != '_') return "function";
  if (strncmp( happy_string, "int",         strlen("int"        )) == 0 && isalnum(buffer[pos + strlen("int"        )]) == 0 && buffer[pos + strlen("int"        )] != '_') return "int";
  if (strncmp( happy_string, "boolean",     strlen("boolean"    )) == 0 && isalnum(buffer[pos + strlen("boolean"    )]) == 0 && buffer[pos + strlen("boolean"    )] != '_') return "boolean";
  if (strncmp( happy_string, "char",        strlen("char"       )) == 0 && isalnum(buffer[pos + strlen("char"       )]) == 0 && buffer[pos + strlen("char"       )] != '_') return "char";
  if (strncmp( happy_string, "void",        strlen("void"       )) == 0 && isalnum(buffer[pos + strlen("void"       )]) == 0 && buffer[pos + strlen("void"       )] != '_') return "void";
  if (strncmp( happy_string, "var",         strlen("var"        )) == 0 && isalnum(buffer[pos + strlen("var"        )]) == 0 && buffer[pos + strlen("var"        )] != '_') return "var";
  if (strncmp( happy_string, "static",      strlen("static"     )) == 0 && isalnum(buffer[pos + strlen("static"     )]) == 0 && buffer[pos + strlen("static"     )] != '_') return "static";
  if (strncmp( happy_string, "field",       strlen("field"      )) == 0 && isalnum(buffer[pos + strlen("field"      )]) == 0 && buffer[pos + strlen("field"      )] != '_') return "field";
  if (strncmp( happy_string, "let",         strlen("let"        )) == 0 && isalnum(buffer[pos + strlen("let"        )]) == 0 && buffer[pos + strlen("let"        )] != '_') return "let";
  if (strncmp( happy_string, "do",          strlen("do"         )) == 0 && isalnum(buffer[pos + strlen("do"         )]) == 0 && buffer[pos + strlen("do"         )] != '_') return "do";
  if (strncmp( happy_string, "if",          strlen("if"         )) == 0 && isalnum(buffer[pos + strlen("if"         )]) == 0 && buffer[pos + strlen("if"         )] != '_') return "if";
  if (strncmp( happy_string, "else",        strlen("else"       )) == 0 && isalnum(buffer[pos + strlen("else"       )]) == 0 && buffer[pos + strlen("else"       )] != '_') return "else";
  if (strncmp( happy_string, "while",       strlen("while"      )) == 0 && isalnum(buffer[pos + strlen("while"      )]) == 0 && buffer[pos + strlen("while"      )] != '_') return "while";
  if (strncmp( happy_string, "return",      strlen("return"     )) == 0 && isalnum(buffer[pos + strlen("return"     )]) == 0 && buffer[pos + strlen("return"     )] != '_') return "return";
  if (strncmp( happy_string, "true",        strlen("true"       )) == 0 && isalnum(buffer[pos + strlen("true"       )]) == 0 && buffer[pos + strlen("true"       )] != '_') return "true";
  if (strncmp( happy_string, "false",       strlen("false"      )) == 0 && isalnum(buffer[pos + strlen("false"      )]) == 0 && buffer[pos + strlen("false"      )] != '_') return "false";
  if (strncmp( happy_string, "null",        strlen("null"       )) == 0 && isalnum(buffer[pos + strlen("null"       )]) == 0 && buffer[pos + strlen("null"       )] != '_') return "null";
  if (strncmp( happy_string, "this",        strlen("this"       )) == 0 && isalnum(buffer[pos + strlen("this"       )]) == 0 && buffer[pos + strlen("this"       )] != '_') return "this";
  return NULL; }


// Get the next token from the source file
Token GetNextToken () {
	Token t;
  int mode = EMPTY;
  int skipper = 0;
  char lxm[128];
  int illegal;
  for (int i = current_char; i < file_size + 1; i++) {
    current_char++;

    if (skipper > 0) {
      skipper--;
      continue; }

    if (buffer[i] == ' ' || buffer[i] == '\t') continue;

    //end of file encountered
    if (buffer[i] == '\0') {
      if ( mode == APICOMM || mode == BLOCKCOMM) {
        t.tp = ERR;
        t.ec = EofInCom;
        strcpy(t.lx, "Error: unexpected eof in comment");
        break; }
      else if (mode == EMPTY) {
        t.tp = EOFile;
        strcpy(t.lx, "End of File"); }
        break; }

    // ignore new linez (not really)
    if (buffer[i] == '\n') {
      current_line++;
      if (mode == EMPTY || mode == APICOMM || mode == BLOCKCOMM) continue; }

    // end commentz
    if (mode == LINECOMM || mode == BLOCKCOMM || mode == APICOMM) {
      if (mode == LINECOMM) {
        if (buffer[i] == '\n') {
          mode = EMPTY;
          continue; }
        if (buffer[i] == '\0') {
          t.ec = EofInCom;
          break; }
        else continue; }
      else if (mode == BLOCKCOMM || mode == APICOMM) {
        if (buffer[i] == '*' && buffer[i + 1] == '/') {
          skipper++;
          mode = EMPTY;
          continue; }
        else continue; } }

    // check commentz
    if (buffer[i] == '/' ) {
      if (buffer[i + 1] == '/') {
        mode = LINECOMM;
        skipper++;
        continue; }
      else if (buffer[i + 1] == '*') {
        if (buffer[i + 2] == '*') {
          mode = APICOMM;
          skipper++;
          continue; }
        else {
          mode = BLOCKCOMM;
          skipper++;
          continue; } } }

    if (mode == EMPTY) {

      if (IsLegal(buffer[i]) == 0) {
        t.tp = ERR;
        t.ec = IllSym;
        strcpy(t.lx, "Error: illegal symbol in source file");
        break; }

      // symbolz
      if (IsSymbol(buffer[i])) {
        lxm[0] = buffer[i];
        lxm[1] = '\0';
        strcpy(t.lx, lxm);
        t.tp = SYMBOL;
        break; }

      //reswordz
      else if (IsReserved(i) != NULL) {
        t.tp = RESWORD;
        strcpy(t.lx, (char*)IsReserved(i));
        current_char += strlen( t.lx ) - 1;
        break; }

      // stringz
      else if (buffer[i] == '\"') {
        if (GetString(i + 1) != NULL) {
          if (ernal_bleeding == 1 ) {
            ernal_bleeding = 0;
            t.tp = ERR;
            t.ec= NewLnInStr;
            t.ln = current_line;
            strcpy(t.lx, "Error: new line in string constant");
            strcpy(t.fl, fname);
            return t; }
          else {
            t.tp = STRING;
            strcpy(t.lx, (char*)GetString(i + 1));
            current_char += strlen( t.lx ) + 1;
            break; } }
        else {
          t.tp = ERR;
          t.ec = EofInCom;
          t.ln = current_line;
          strcpy(t.lx, "Error: unexpected eof in string constant");
          strcpy(t.fl, fname);
          return t; } }

        // idz
      else if (IsIdentifier(i) != NULL) {
        t.tp = ID;
        strcpy(t.lx, (char*)IsIdentifier(i));
        current_char += strlen(t.lx) - 1;
        break; }

        // intz
      else if (IsInteger(i) != NULL) {
        t.tp = INT;
        strcpy(t.lx, (char*)IsInteger(i));
        current_char += strlen(t.lx) - 1;
        break; }
        // boolz
      else if (IsBool(i) != NULL) {
        t.tp = ID;
        strcpy(t.lx, (char*)IsBool(i));
        current_char += strlen(t.lx) - 1;
        break; } } }

  t.ln = current_line;
  strcpy(t.fl, fname);
  return t; }

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken () {
	Token t;
  int mode = EMPTY;
  int skipper = 0;
  char lxm[128];
  int illegal;
  int temp_line = current_line;
  for (int i = current_char; i < file_size + 1; i++) {

    if (skipper > 0) {
      skipper--;
      continue; }

    if (buffer[i] == ' ' || buffer[i] == '\t') continue;

    //end of file encountered
    if (buffer[i] == '\0') {
      if ( mode == APICOMM || mode == BLOCKCOMM) {
        t.tp = ERR;
        t.ec = EofInCom;
        strcpy(t.lx, "Error: unexpected eof in comment");
        break; }
      else if (mode == EMPTY) {
        t.tp = EOFile;
        strcpy(t.lx, "End of File"); }
        break; }

    // ignore new linez (not really)
    if (buffer[i] == '\n') {
      temp_line++;
      if (mode == EMPTY || mode == APICOMM || mode == BLOCKCOMM) continue; }

    // end commentz
    if (mode == LINECOMM || mode == BLOCKCOMM || mode == APICOMM) {
      if (mode == LINECOMM) {
        if (buffer[i] == '\n') {
          mode = EMPTY;
          continue; }
        if (buffer[i] == '\0') {
          t.ec = EofInCom;
          break; }
        else continue; }
      else if (mode == BLOCKCOMM || mode == APICOMM) {
        if (buffer[i] == '*' && buffer[i + 1] == '/') {
          skipper++;
          mode = EMPTY;
          continue; }
        else continue; } }

    // check commentz
    if (buffer[i] == '/' ) {
      if (buffer[i + 1] == '/') {
        mode = LINECOMM;
        skipper++;
        continue; }
      else if (buffer[i + 1] == '*') {
        if (buffer[i + 2] == '*') {
          mode = APICOMM;
          skipper++;
          continue; }
        else {
          mode = BLOCKCOMM;
          skipper++;
          continue; } } }

    if (mode == EMPTY) {

      if (IsLegal(buffer[i]) == 0) {
        t.tp = ERR;
        t.ec = IllSym;
        strcpy(t.lx, "Error: illegal symbol in source file");
        break; }

      // symbolz
      if (IsSymbol(buffer[i])) {
        lxm[0] = buffer[i];
        lxm[1] = '\0';
        strcpy(t.lx, lxm);
        t.tp = SYMBOL;
        break; }

      //reswordz
      else if (IsReserved(i) != NULL) {
        t.tp = RESWORD;
        strcpy(t.lx, (char*)IsReserved(i));
        break; }

      // stringz
      else if (buffer[i] == '\"') {
        if (GetString(i + 1) != NULL) {
          if (ernal_bleeding == 1 ) {
            ernal_bleeding = 0;
            t.tp = ERR;
            t.ec= NewLnInStr;
            t.ln = temp_line;
            strcpy(t.lx, "Error: new line in string constant");
            strcpy(t.fl, fname);
            return t; }
          else {
            t.tp = STRING;
            strcpy(t.lx, (char*)GetString(i + 1));
            break; } }
        else {
          t.tp = ERR;
          t.ec = EofInCom;
          t.ln = temp_line;
          strcpy(t.lx, "Error: unexpected eof in string constant");
          strcpy(t.fl, fname);
          return t; } }

        // idz
      else if (IsIdentifier(i) != NULL) {
        t.tp = ID;
        strcpy(t.lx, (char*)IsIdentifier(i));
        break; }

        // intz
      else if (IsInteger(i) != NULL) {
        t.tp = INT;
        strcpy(t.lx, (char*)IsInteger(i));
        break; }
        // boolz
      else if (IsBool(i) != NULL) {
        t.tp = ID;
        strcpy(t.lx, (char*)IsBool(i));
        break; } } }

  t.ln = temp_line;
  strcpy(t.fl, fname);
  return t; }

int StopLexer () {
  free(buffer);
	return 0; }

#ifndef TEST

// int main(void) { return 0; }

#endif

// I do apologize for my code
