#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <map>
#include <cstdio>
#include "Token.h"
#include "Word.h"
#include "Num.h"
#include "Tag.h"
using namespace std;

class Lexer {
	public:
        	Lexer();
        	Token* scan();
        	void getChar();
		bool getChar(char c);
		int digit(char c);
		
		bool isLetterOrUnderline(char c);
		bool isDigit(char c);

		static int line;

	private:
        	char peek;
		Tag tag;
        	map<string, int> words;
};

#endif
