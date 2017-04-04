/*
 * Parser.h -- Parser类的接口
 * 作者：杨柳
 * 编辑时间：2014-12-25
 * version 0
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include "Token.h"
#include "Num.h"
#include "Word.h"
#include "Tag.h"
#include "Lexer.h"
#include "Table.h"
#include "Statement.h"

using namespace std;

class Parser {
	private:
		Lexer lex;                    // lexical analyzer for this parser
		Token* look;                  // lookahead token
		Tag tag;

		map<string, Table> tables;    // 数据库实例
		
		string tabname;
	   	vector<string> colname;
		vector<string> pkey;
		map<string, int> defval;
		vector<int> val;
		vector<map<string, Statement*> > whereArg;
		map<string, Statement*> stmts;
		bool isall;

		Token* tmp_look;
		Word* tmp_word;
		Num* tmp_num;
		int res_cal;

	public:
		Parser(Lexer l);
		
		void Move();
		bool Match(int t);
		void Program();
		bool CREATE_Rec();
		bool Primary_Rec();
		int INSERT_Col_Rec(int num);
		int INSERT_Val_Rec(int num);
		bool WHERE_Rec();
		bool SELECT_Rec();

		void Error(string s);
		void Clear();

		bool Calculator();

};

#endif
