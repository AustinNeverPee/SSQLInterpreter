#include "Table.h"

Table::Table() 
{

}

Table::Table(string _tableName, vector<string> column,
        map<string, int> _defaultValue, vector<string> _pkey)
{
    tableName = _tableName;
    vector<string>::iterator iter;
    int count = 0;
    for(iter = column.begin(); iter != column.end(); iter++) {
       columnName[*iter] = count++; 
    }
    columnCount = column.size();


    defaultValue = _defaultValue;
    pkey = _pkey;

    printf("CREATE数据成功!\n");

}


void Table::insert(map<string, int> record)
{
    vector<int> row(columnCount, 0);

    map<string, int>::iterator iter;
    for(iter = defaultValue.begin(); iter != defaultValue.end(); iter++) {
        map<string, int>::iterator it = columnName.find(iter->first); 
        if(it != columnName.end()) {
           row[it->second] = iter->second; 
            
        } else {
            printf("列名不存在!\n");
            return;
        }
    }  

    map<string, int> pkey_val;

    for(iter = record.begin(); iter != record.end(); iter++) {
        map<string, int>::iterator it = columnName.find(iter->first);
        if(it != columnName.end()) {
            row[it->second] = iter->second;

        } else {
            printf("列名不存在\n");
            return;
        }

        vector<string>::iterator it_key;
        for(it_key = pkey.begin(); it_key != pkey.end(); it_key++) {
            if(*it_key == iter->first) {
                pkey_val[iter->first] = iter->second; 
            }
        }
    }

    bool isConflict = false;

    vector<vector<int> >::iterator it_data;
    for(it_data = data.begin(); it_data != data.end(); it_data++) {
        vector<int> row_data = *it_data;
        map<string, int>::iterator it_key = pkey_val.begin();
    
        bool isAllEqual = true;
        for( ; it_key != pkey_val.end(); it_key++) {
            int val = row_data[columnName[it_key->first]];
            if(val != it_key->second) {
                isAllEqual = false;
                break;
            }
        }
        if(isAllEqual == true) {
            isConflict = true;
            break;
        }

    }

    if(pkey.size() == 0 || isConflict == false) {
        data.push_back(row);
        printf("Insert数据成功!\n");
    } else {
        printf("存在相同的主键值！\n");
    }
}




void Table::query(bool isAll, vector<string> column, vector<map<string, Statement*> > whereArg)
{
    //判断选择查询的列名是否在表中
    if(isAll == false) {
        vector<string>::iterator it;
        for(it = column.begin(); it != column.end(); it++){
            map<string, int>::iterator iter = columnName.find(*it);
            if(iter == columnName.end()) {
                printf("查询不存在的列名!\n");
                return;
            }
        }
    }else {
        column.clear();
        map<string, int>::iterator it;
        for(it = columnName.begin(); it != columnName.end(); it++) {
            column.push_back(it->first);
        }
    }

    //where语句为空则查询全部
    if(whereArg.size() == 0) {
        int count = 0;
        for(int i = 0; i < column.size(); i++) {
            printf("|------------");
        }
        printf("|\n");

        for(vector<string>::iterator it = column.begin();
                it != column.end(); it++) {
            printf("|%-12s", (*it).c_str());
        }
        printf("|\n");

        for(int i = 0; i < column.size(); i++) {
            printf("|------------");
        }
        printf("|\n");


        vector<vector<int> >::iterator iter;
        for(iter = data.begin(); iter != data.end(); iter++) {
            vector<int> row = *iter;           
            for(vector<string>::iterator it = column.begin();
                    it != column.end(); it++) {
                int num = row[columnName[(*it)]];
                printf("|%-12d",num);
            }
            printf("|\n");

            for(int i = 0; i < column.size(); i++) {
                printf("|------------");
            }
            printf("|\n");
            count++;
        }
    
        printf("%d rows affected!\n", count); 
        return;
    }



    //判断where语句的列名是否在表中
    vector<map<string, Statement*> >::iterator it;
    for(it = whereArg.begin(); it != whereArg.end(); it++) {
        map<string, Statement*> map_and = *it;
        map<string, Statement*>::iterator iter;
        for(iter = map_and.begin(); iter != map_and.end(); iter++) {
            if(columnName.find(iter->first) == columnName.end()) {
                printf("列名不存在!\n");
                return;
            }
        }
    }

    int count = 0;

    for(int i = 0; i < column.size(); i++) {
            printf("|------------");
    }
    printf("|\n");

    for(vector<string>::iterator it = column.begin();
            it != column.end(); it++) {
        printf("|%-12s", (*it).c_str());
    }
    printf("|\n");

    for(int i = 0; i < column.size(); i++) {
        printf("|------------");
    }
    printf("|\n");



    //判断每一行数据是否符合where语句
    vector<vector<int> >::iterator it_data = data.begin();

    while(it_data != data.end()) {   //对每一行进行比较
        vector<int> row_data = *it_data;   //每一行的数据

        bool isRight = isRowDataRight(row_data, whereArg);
        if(isRight == true) {
            count++;
                     
            for(vector<string>::iterator it = column.begin();
                    it != column.end(); it++) {
                int num = row_data[columnName[*it]];
                printf("|%-12d",num);
            }
            printf("|\n");

            for(int i = 0; i < column.size(); i++) {
                printf("|------------");
            }
            printf("|\n");
        }
        it_data++;
    }
    printf("%d rows affected!\n", count);
}


void Table::delete_s(vector<map<string, Statement*> > whereArg)
{
    int count = 0;
    //where语句为空删除全部
    if(whereArg.size() == 0) {
        count = data.size();
        data.clear();
        printf("%d rows deleted!\n", count);
        return;
    }

    //检验where语句的列名是否在表中
    vector<map<string, Statement*> >::iterator it;
    for(it = whereArg.begin(); it != whereArg.end(); it++) {
        map<string, Statement*> map_and = *it;
        map<string, Statement*>::iterator iter;
        for(iter = map_and.begin(); iter != map_and.end(); iter++) {
            if(columnName.find(iter->first) == columnName.end()) {
                printf("列名不存在!\n");
                return;
            }
        }
    }

    //判断每一行数据是否符合where语句
    vector<vector<int> >::iterator it_data = data.begin();
    while(it_data != data.end()) {  //对每一行进行比较
        vector<int> row_data = *it_data;   //每一行的数据

        bool isRight = isRowDataRight(row_data, whereArg);
        if(isRight == true) {
            count++;
            it_data = data.erase(it_data);  //删除该行数据
        } else {
            it_data++;
        }
    }
    printf("%d rows deleted!\n", count);
}


//判断一行数据是否符合where语句
bool Table::isRowDataRight(vector<int> row_data, vector<map<string,Statement*> > whereArg)
{
    vector<map<string, Statement*> >::iterator it_where; 

    bool isOrRight = false;    //或的判断
    for(it_where = whereArg.begin(); it_where != whereArg.end(); it_where++) {
        map<string, Statement*> map_where = *it_where;  
        map<string, Statement*>::iterator it_and; 

        bool isAndRight = true;  //与的判断

        for(it_and = map_where.begin(); it_and != map_where.end(); it_and++) {
            Statement stmt = *(it_and->second);
            Statement s = dealingStmt(stmt);
            string name = it_and->first;
            int data_num = row_data[columnName[name]];
            if(s.cal_num == 0 && s.cal_token == '*') {
                data_num = 0;
            }
                    
            switch(s.token) {
                case 273:
                    if(!(s.num == data_num)) {
                        isAndRight = false;
                    }
                     break;
                case 271:
                    if(!(s.num != data_num)) {
                        isAndRight = false;
                    }
                    break;
                case 274:
                            
                    if(s.isColumnFront == true && (!(data_num >= s.num))) {
                        isAndRight = false;
                    }
                    if(s.isColumnFront == false && (!(s.num >= data_num))) {
                        isAndRight = false;
                    }
                    break;
                case 275:
                    if(s.isColumnFront == true && (!(data_num <= s.num))) {
                        isAndRight = false;
                    }
                    if(s.isColumnFront == false && (!(s.num <= data_num))) {
                        isAndRight = false;
                    }
                    break;
                case '>':
                    if(s.isColumnFront == true && (!(data_num > s.num))) {
                        isAndRight = false;
                    }
                    if(s.isColumnFront == false && (!(s.num > data_num))) {
                        isAndRight = false;
                    }
                    break;
                case '<':
                    if(s.isColumnFront == true && (!(data_num < s.num))) {
                        isAndRight = false;
                    }
                    if(s.isColumnFront == false && (!(s.num < data_num))) {
                        isAndRight = false;
                    }
                    break;
                default:
                    printf("不合法的操作符!\n");
                    return false;
                }

            if(s.isNot == true) {
                isAndRight = !isAndRight;
            }
            if(isAndRight == false) {
                break;
            }
        }
        if(isAndRight == true) {
            isOrRight = true;
            break;
        } 
    }

    return isOrRight;

}


Statement Table::dealingStmt(Statement stmt)
{
    Statement tmp;
    tmp.isNot = stmt.isNot;
    tmp.isColumnFront = stmt.isColumnFront;
    tmp.cal_token = stmt.cal_token;
    tmp.cal_num = stmt.cal_num;
    tmp.isFront = stmt.isFront;
    tmp.num = stmt.num;
    tmp.token = stmt.token;


    switch(stmt.cal_token) {
        case '+':
            tmp.num = stmt.num - stmt.cal_num;

            break;
        case '-':
            if(stmt.isFront) {
                tmp.num = stmt.num + stmt.cal_num;   
            } else {
                tmp.num = stmt.cal_num - stmt.num;
                tmp.isColumnFront = !tmp.isColumnFront;
            }
            break;
        case '*':
            if(stmt.cal_num != 0) {
                tmp.num = stmt.num / stmt.cal_num;
            }
            break;
        case '/':
            if(stmt.isFront) {
                tmp.num = stmt.num * stmt.cal_num;

            } else {
                if(stmt.num != 0) {                
                    tmp.num = stmt.cal_num / stmt.num;
                    tmp.isColumnFront = !tmp.isColumnFront;
                }
            }
            break;
        default:
            break;
    }

    return tmp;

}
