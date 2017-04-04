/*
 * An interpreter for SSQL
 * Main Part
 * author : Austin Yang
 * create time : 2015-1-1
 */

#include<iostream>
#include<string>
#include<cstring>
#include<cstdio>

#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc == 2) {
	    freopen(argv[1], "r", stdin);	
	    Lexer lex;
	    Parser parse(lex);
	    parse.Program();
    }else{

        printf("请输入文件名!\n");
    }

    return 0;
}
