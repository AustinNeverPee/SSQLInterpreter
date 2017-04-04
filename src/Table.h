#ifndef TABLE_H
#define TABLE_H
#include<iostream>
#include<cstring>
#include<string>
#include<cstdio>
#include<map>
#include<list>
#include<vector>
#include "Statement.h"

using namespace std;



class Table
{

    public:
        Table();
        Table(string tableName, vector<string> columnName, map<string, int> defaultValue, vector<string> pkey);

        void delete_s(vector<map<string,Statement*> > whereArg);

        void query(bool isAll, vector<string> column, vector<map<string,Statement*> > whereArg);

        void insert(map<string,int> record);

    private:
        
        bool isRowDataRight(vector<int> row_data, vector<map<string,Statement*> > whereArg);
        Statement dealingStmt(Statement stmt);
        string tableName;
        int columnCount;
        map<string, int> columnName;
        map<string, int> defaultValue;
        vector<vector<int> > data;
        vector<string> pkey;
};


#endif
