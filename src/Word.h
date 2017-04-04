#ifndef WORD_H
#define WORD_H
#include<iostream>
#include<string>
#include<cstring>
#include<cstdio>
#include "Token.h"

using namespace std;
class Word : public Token
{
    public:
        string str;

	Word(string s, int tag) : Token(tag){
        str = s;
    }	

    virtual string toString(){
        return str;
    }
};

#endif
