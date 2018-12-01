/*
 * File: Basic.cpp
 * ---------------
 * Name: Basic
 * Section: Ulita
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */
#include <stdio.h>

#include <cctype>
#include <iostream>
#include <string>
#include <sstream>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line);
int get_value(TokenScanner &scanner);

/* Main program */
Program program;
extern EvalState allstate;
const string ims[] = { "REM","LET","PRINT","INPUT","END","GOTO","IF","THEN","RUN","LIST","CLEAR","QUIT","HELP" };
const int imsn = 13;
int main()
{
	
	while (true)
	{
		try
		{
			processLine(getLine());
		}
		catch (ErrorException & ex)
		{
			cerr << ex.getMessage() << endl;
		}
	}
	return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line)
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);

	string start = scanner.nextToken();
	if (start.empty()) return;

	enum TokenType p = scanner.getTokenType(start);
	if (p == WORD)
	{
		if (start == "HELP") puts("Fxxk off!");
		else if (start == "QUIT") exit(0);
		else if (start == "CLEAR") program.clear();
		else if (start == "LIST") program.listall();
		else if (start == "RUN") program.run();
		else if (start == "LET")
		{
			string var = scanner.nextToken();
			string tmp = scanner.nextToken();
			if(scanner.getTokenType(var)!= WORD||tmp!="=") error("SYNTAX ERROR");
			for(int i=0;i<imsn;i++) if(var==ims[i]) error("SYNTAX ERROR");
			allstate.setValue(var,get_value(scanner));
		}
		else if (start == "PRINT") cout << get_value(scanner) << endl;
		else if (start == "INPUT")
		{
			string var = scanner.nextToken();
			if (scanner.getTokenType(var) != WORD) error("SYNTAX ERROR");
			for (int i = 0; i < imsn; i++) if (var == ims[i]) error("SYNTAX ERROR");
			allstate.getin(var);
		}
		else error("SYNTAX ERROR");
		return;
	}
	if (p != NUMBER) error("SYNTAX ERROR");
	stringstream ss(start);
	int num;
	ss >> num;
	program.insert(num, line.substr(start.length()));
	//error("SYNTAX ERROR");
}

