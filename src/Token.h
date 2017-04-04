#ifndef TOKEN_H
#define TOKEN_H
#include<iostream>
#include<string>
using namespace std;

class Token { 
    public:
        int tag;
        Token(int t) {
            tag = t;
        }
        Token() {}
        virtual string toString(){
            char c = (char)tag;
            string str(1, c);
            return str;
        }
};


#endif
