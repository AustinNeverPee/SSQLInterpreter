#ifndef STATEMENT_H
#define STATEMENT_H

#include<iostream>
using namespace std;


class Statement
{

    public:
        Statement(){
            cal_token = '+';
            cal_num = 0;
            isNot = false;
            isColumnFront = true;
            isFront = true;
        };
        bool isNot;
        bool isColumnFront;
        int cal_token;
        int cal_num;
        bool isFront;
        int num;
        int token;

};


#endif
