#ifndef NUM_H
#define NUM_H

#include<iostream>
#include<stdio.h>
#include<string>
#include "Token.h"
using namespace std;
class Num : public Token
{
    public:
        int value;
        Num(int v) : Token(276){
            value = v;
        }

        virtual string toString(){
            char buf[20];
            sprintf(buf, "%d", value);
            string res(buf);
            return res;
        }

};




#endif
