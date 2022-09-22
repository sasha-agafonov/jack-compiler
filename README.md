# JACK Compiler

This is a Jack to virtual machine code compiler that I implemented in C. Jack is an object-oriented programming language, somewhat similar to Java.
The compiler itself consists of the following 4 components: lexer, parser, symbol table and virtual machine code generator. 

## Lexer
The first step is to turn the source code into a stream of tokens. A token is simply a structure that contains information about one lexeme - a lexeme may be a reserved keyword, an identifier, or a syntactic element, such as a bracket. A lexical error is thrown if the source code contains things like illegal symbols (e.g. £ not enclosed in apostrophes) or, for example, if an identifier starts with a digit (e.g. 2_happy). If the lexer detects no errors in the token stream, we proceed to the next stage.

## Parser
The parser is probably the most significant component of the compiler. Parser has access to a number of functions that pretty much define the syntax of Jack. When the parser receives a token, it first checks the token type and then identifies which other tokens are allowed to come after. For example, the parser may receive a token containing a reserved keyword "class" when a user wants to define a new class in Jack. According to the Jack syntax, the next token must then contain an identifier. If the expected token type does not match, the parser issues a syntactic error. Naturally, there are cases when multiple different types of tokens may follow. The only requirement is that the rules of the Jack grammar are obeyed - assuming, of course, that the source is free of lexical errors. The full Jack grammar can be found
[here](https://drive.google.com/file/d/1ujgcS7GoI-zu56FxhfkTAvEgZ6JT7Dxl/view).

## Symbol Table
The symbol table is used to perform the most difficult type of error checking - semantic analysis. But conceptually, it is not too complicated. We maintain a table for storing information about variables and methods, such as identifiers, types, parameters (if any), return values (if any) and so on. As we parse the source code and find, for example, a method declaration, we store information about it in our symbol table. Similarly, if we notice that the source code contains a call to a method, we access the table and search for that method to ensure that it has been declared. If this is not the case, we issue an error. We validate variables and objects in a similar manner. Once the source code passes this stage, it can be considered error-free. We then proceed to the final stage, where we perform the actual translation from Jack to virtual machine code.

## Code Generation
Assuming that the input Jack source is free from lexical, syntactic and semantic errors, the last step is to translate the source to VM (virtual machine) code. Unlike C and C++, Jack does not require manual memory management, so we are mostly concerned about pushing and popping VM commands on/from the stack. For example, to get the result of adding two variables, we push both variables and the sum command on the stack and then pop the result. 
