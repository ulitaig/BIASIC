/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include <vector>
#include <stdio.h>

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
#include "program.h"
#include "statement.h"
using namespace std;

EvalState allstate;
const string ims[] = { "REM","LET","PRINT","INPUT","END","GOTO","IF","THEN","RUN","LIST","CLEAR","QUIT","HELP" };
const int imsn = 13;


int get_value(TokenScanner &scanner)
{
	Expression *exp = parseExp(scanner);
	int value = exp->eval(allstate);
	delete exp;
	return value;
}

bool judge(const std::string &s)
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(s);

	string start = scanner.nextToken();
	if (start.empty()) return false;

	enum TokenType p = scanner.getTokenType(start);
	if (p != WORD) error("SYNTAX ERROR");

	if (start == "REM") return true;
	else if (start == "LET")
	{
		string var = scanner.nextToken();
		string tmp = scanner.nextToken();
		if (scanner.getTokenType(var) != WORD || tmp != "=") error("SYNTAX ERROR");
		for (int i = 0; i < imsn; i++) if (var == ims[i]) error("SYNTAX ERROR");
		Expression *exp = parseExp(scanner);
		delete exp;
	}
	else if (start == "PRINT") Expression *exp = parseExp(scanner);
	else if (start == "INPUT")
	{
		string var = scanner.nextToken();
		for (int i = 0; i < imsn; i++) if (var == ims[i]) error("SYNTAX ERROR");
		if (scanner.getTokenType(var) != WORD) error("SYNTAX ERROR");
	}
	else if (start == "END")
	{
		if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
	}
	else if (start == "GOTO")
	{
		string ss = scanner.nextToken();
		if (ss.empty()) error("SYNTAX ERROR");

		enum TokenType z = scanner.getTokenType(ss);
		if (z != NUMBER) error("SYNTAX ERROR");

		if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
	}
	else if (start == "IF")
	{
		int p0 = s.find("IF"), p1 = s.find("<");
		if (p1 == s.npos) p1 = s.find(">");
		if (p1 == s.npos) p1 = s.find("=");
		if (p1 == s.npos) error("SYNTAX ERROR");
		int p2 = s.find("THEN");
		if (p2 == s.npos) error("SYNTAX ERROR");
		string ls = s.substr(p0 + 2, p1 - p0 - 2), rs = s.substr(p1 + 1, p2 - p1 - 1);
		TokenScanner sc1, sc2;
		sc1.ignoreWhitespace();
		sc1.scanNumbers();
		sc1.setInput(ls);
		sc2.ignoreWhitespace();
		sc2.scanNumbers();
		sc2.setInput(rs);
		Expression *exp = parseExp(sc1);
		delete exp;
		exp = parseExp(sc2);
		delete exp;
		while (scanner.hasMoreTokens())
		{
			start = scanner.nextToken();
			if (start == "THEN") break;
		}
		if (start != "THEN") error("SYNTAX ERROR");
		start = scanner.nextToken();
		p = scanner.getTokenType(start);
		if (p != NUMBER) error("SYNTAX ERROR");
		if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
	}
	else error("SYNTAX ERROR");
	return true;

}
Program::Program()
{
	// Replace this stub with your own code
}

Program::~Program()
{
	// Replace this stub with your own code
}

void Program::insert(int num, const std::string &s)
{
	if (!judge(s)) list[num].clear();
	if (num >= list.size()) list.resize(num * 2);
	list[num] = s;
}

void Program::clear()
{
	allstate.clear();
	list.clear();
}

void Program::listall()
{
	for (int i = 0; i < list.size(); i++)
		if (!list[i].empty())
			cout << i << list[i] << endl;
}

void Program::run()
{
	for (int i = -1;;)
	{
		i++;
		if (i >= list.size()) return;
		if (!list[i].empty())
		{
			string s = list[i];

			TokenScanner scanner;
			scanner.ignoreWhitespace();
			scanner.scanNumbers();
			scanner.setInput(s);

			string start = scanner.nextToken();
			if (start.empty()) continue;

			enum TokenType p = scanner.getTokenType(start);
			if (p != WORD) error("SYNTAX ERROR");

			if (start == "REM") continue;
			else if (start == "LET")
			{
				string var = scanner.nextToken();
				string tmp = scanner.nextToken();
				if (scanner.getTokenType(var) != WORD || tmp != "=") error("SYNTAX ERROR");
				allstate.setValue(var, get_value(scanner));
			}
			else if (start == "PRINT") cout << get_value(scanner) << endl;
			else if (start == "INPUT")
			{
				string var = scanner.nextToken();
				if (scanner.getTokenType(var) != WORD) error("SYNTAX ERROR");
				allstate.getin(var);
			}
			else if (start == "END")
			{
				if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
				return;
			}
			else if (start == "GOTO")
			{
				string ss = scanner.nextToken();
				if (ss.empty()) error("SYNTAX ERROR");

				enum TokenType z = scanner.getTokenType(ss);
				if (z != NUMBER) error("SYNTAX ERROR");

				if (scanner.hasMoreTokens()) error("SYNTAX ERROR");

				stringstream sst(ss);
				int num;
				sst >> num;
				if (list[num].empty()) error("LINE NUMBER ERROR");
				i = num - 1;
			}
			else if (start == "IF")
			{
				int p0 = s.find("IF"), p1 = s.find("<");
				if (p1 == s.npos) p1 = s.find(">");
				if (p1 == s.npos) p1 = s.find("=");
				if (p1 == s.npos) error("SYNTAX ERROR");
				int p2 = s.find("THEN");
				if (p2 == s.npos) error("SYNTAX ERROR");
				string ls = s.substr(p0 + 2, p1 - p0 - 2), rs = s.substr(p1 + 1, p2 - p1 - 1);
				TokenScanner sc1, sc2;
				sc1.ignoreWhitespace();
				sc1.scanNumbers();
				sc1.setInput(ls);
				sc2.ignoreWhitespace();
				sc2.scanNumbers();
				sc2.setInput(rs);
				int l = get_value(sc1);
				int r = get_value(sc2);
				bool bo = true;
				while (scanner.hasMoreTokens())
				{
					start = scanner.nextToken();
					if (start == "<"&&l >= r) bo = false;
					if (start == ">"&&l <= r) bo = false;
					if (start == "="&&l != r) bo = false;
					if (start == "THEN") break;
				}
				if (start != "THEN") error("SYNTAX ERROR");
				start = scanner.nextToken();
				p = scanner.getTokenType(start);
				if (p != NUMBER) error("SYNTAX ERROR");
				if (scanner.hasMoreTokens()) error("SYNTAX ERROR");

				
				stringstream sst(start);
				int num;
				sst >> num;
				if (list[num].empty()) error("LINE NUMBER ERROR");
				if (!bo) continue;
				i = num - 1;
			}
			else error("SYNTAX ERROR");
		}
	}
}

void Program::addSourceLine(int lineNumber, string line)
{
	// Replace this stub with your own code
}

void Program::removeSourceLine(int lineNumber)
{
	// Replace this stub with your own code
}

string Program::getSourceLine(int lineNumber)
{
	return "";    // Replace this stub with your own code
}

void Program::setParsedStatement(int lineNumber, Statement *stmt)
{
	// Replace this stub with your own code
}

Statement *Program::getParsedStatement(int lineNumber)
{
	return NULL;  // Replace this stub with your own code
}

int Program::getFirstLineNumber()
{
	return 0;     // Replace this stub with your own code
}

int Program::getNextLineNumber(int lineNumber)
{
	return 0;     // Replace this stub with your own code
}
