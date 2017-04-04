/*
 * Parser.cpp -- Parser类的实现
 * 作者：杨柳
 * 编辑时间：2014-12-25
 * Version 0
 */

#include "Parser.h"

/**
 * constructor
 */
Parser::Parser(Lexer l) {
	lex = l;
	Move();
}

void Parser::Move() {
	look = lex.scan();

//	string str = look->toString();
//	cout << look->tag << " " << str << "\n";
}

bool Parser::Match(int t) {
	if (look->tag == t) {
		Move();
		return true;
	} else {
		return false;
	}
}

/**
 * 语法分析主程序
 */
void Parser::Program() {
	while(look->tag != EOF) {
		if (Match(tag.CREATE)) {
			if (Match(tag.TABLE)) {
				tmp_word = (Word*)look;

				if (Match(tag.ID)) {
					tabname = tmp_word->str;

					if (Match('(')) {
						if (CREATE_Rec()) {
							if (Match(';')) {
								// 匹配CREATE语句成功
								map<string, Table>::iterator it_tab = tables.find(tabname);
								if (it_tab == tables.end()) {
									bool flag_error = false;
									vector<string>::iterator it_pk = pkey.begin();
									for (; it_pk != pkey.end(); it_pk++) {
										bool flag_pk = false;
										vector<string>::iterator it_col = colname.begin();
										for (; it_col != colname.end(); it_col++) {
											if (*it_col == *it_pk) {
												flag_pk = true;
												break;
											}
										}

										if (!flag_pk) {
											flag_error = true;
											break;
										}
									}

									if (!flag_error) {
										Table table(tabname, colname, defval, pkey);
										tables[tabname] = table;

										Clear();
									} else {
										Error("设为主键的列名不存在");
									}
								} else {
									Error("不能CRTEATE已存在的表名");
								}
							} else {
								Error("CREATE语句缺少;");
							}
						}
					} else {
						Error("CREATE语句缺少(");
					}
				} else {
					Error("CREATE语句缺少表名");
				}
			} else {
				Error("CREATE语句缺少TABLE");
			}
		} else if (Match(tag.INSERT)) {
			if (Match(tag.INTO)) {
				tmp_word = (Word*)look;

				if (Match(tag.ID)) {
					tabname = tmp_word->str;

					if (Match('(')) {
						//获取插入列的个数
						int num_col = INSERT_Col_Rec(0);
						if (num_col != -1) {
							if (Match(tag.VALUES)) {
								if (Match('(')) {
									//获取插入值的个数
									int num_val = INSERT_Val_Rec(0);
									if (num_val != -1) {
										if (num_col == num_val) {
											if (Match(';')) {
												// 匹配INSERT语句成功
												map<string, Table>::iterator it_tab = tables.find(tabname);
												if (it_tab == tables.end()) {
													cout << "INSERT不存在的表名" << endl;
												} else {
													map<string, int> record;
													vector<string>::iterator it_col = colname.begin();
													vector<int>::iterator it_val = val.begin();
													for (; it_col != colname.end(); it_col++, it_val++) {
														if (record.find(*it_col) != record.end()) {
															cout << "INSERT列名重复" << endl;
															break;
														} else {
															record[*it_col] = *it_val;
														}
													}
													tables[tabname].insert(record);
												}

												Clear();
											} else {
												Error("INSERT语句缺少;");
											}
										} else {
											Error("INSERT语句中插入列和插入值的个数不一致");
										}
									}
								} else {
									Error("INSERT语句缺少(");
								}
							} else {
								Error("INSERT语句缺少VALUES");
							}
						}
					} else {
						Error("INSERT语句缺少(");
					}
				} else {
					Error("INSERT语句缺少表名");
				}
			} else {
				Error("INSERT语句缺少INTO");
			}	
		} else if (Match(tag.DELETE)) {
			if (Match(tag.FROM)) {
				tmp_word = (Word*)look;

				if (Match(tag.ID)) {
					tabname = tmp_word->str;

					if (Match(';')) {
						// 匹配DELETE语句成功
						map<string, Table>::iterator it_tab = tables.find(tabname);
						if (it_tab == tables.end()) {
							cout << "DELETE不存在的表名" << endl;
						} else {
							tables[tabname].delete_s(whereArg);
						}

						Clear();
					} else if (Match(tag.WHERE)) {
						if (WHERE_Rec()) {
							// 匹配DELETE语句成功
							map<string, Table>::iterator it_tab = tables.find(tabname);
							if (it_tab == tables.end()) {
								cout << "DELETE不存在的表名" << endl;
							} else {

								tables[tabname].delete_s(whereArg);
							}

							Clear();
						}
					} else {
						Error("DELETE语句语法错误");
					}
				} else {
					Error("DELETE语句缺少表名");
				}
			} else {
				Error("DELETE语句缺少FROM");
			}
		} else if (Match(tag.SELECT)) {
			if (Match('*')) {
				isall = true;

				if (Match(tag.FROM)) {
					tmp_word = (Word*)look;

					if (Match(tag.ID)) {
						tabname = tmp_word->str;

						if (Match(';')) {
							// 匹配SELECT语句成功
							map<string, Table>::iterator it_tab = tables.find(tabname);
							if (it_tab == tables.end()) {
								cout << "SELECT不存在的表名" << endl;
							} else {
							//	vector<string>::iterator it_col = colname.begin();
							//	for (; it_col != colname.end(); it_col++) {
							//		cout << *it_col << endl;
							//	}
								tables[tabname].query(isall, colname, whereArg);
							}

							Clear();
						} else if (Match(tag.WHERE)) {
							if (WHERE_Rec()) {
								// 匹配SELECT语句成功
								map<string, Table>::iterator it_tab = tables.find(tabname);
								if (it_tab == tables.end()) {
									cout << "SELECT不存在的表名" << endl;
								} else {
							//		vector<string>::iterator it_col = colname.begin();
							//		for (; it_col != colname.end(); it_col++) {
							//			cout << *it_col << endl;
							//		}
									tables[tabname].query(isall, colname, whereArg);
								}

								Clear();
							}
						} else {
							Error("SELECT语句语法错误");
						}
					} else {
						Error("SELECT语句缺少表名");
					}
				} else {
					Error("SELECT语句缺少FROM");
				}
			} else if (SELECT_Rec()) {
				isall = false;
				tmp_word = (Word*)look;

				if (Match(tag.ID)) {
					tabname = tmp_word->str;

					if (Match(';')) {
						// 匹配SELECT语句成功
						map<string, Table>::iterator it_tab = tables.find(tabname);
						if (it_tab == tables.end()) {
							cout << "SELECT不存在的表名" << endl;
						} else {
							//vector<string>::iterator it_col = colname.begin();
							//for (; it_col != colname.end(); it_col++) {
							//	cout << *it_col << endl;
							//}
							tables[tabname].query(isall, colname, whereArg);
						}

						Clear();
					} else if (Match(tag.WHERE)) {
						if (WHERE_Rec()) {
							// 匹配SELECT语句成功
							map<string, Table>::iterator it_tab = tables.find(tabname);
							if (it_tab == tables.end()) {
								cout << "SELECT不存在的表名" << endl;
							} else {
								//vector<string>::iterator it_col = colname.begin();
								//for (; it_col != colname.end(); it_col++) {
								//	cout << *it_col << endl;
								//}
								tables[tabname].query(isall, colname, whereArg);
							}

							Clear();
						}
					} else {
						Error("SELECT语句语法错误");
					}
				} else {
					Error("SELECT语句缺少表名");
				}
			}
		} else {
			Move();
			Error("syntax error");
		}
	}
}

/**
 * 递归判断CREATE语句中()内语句的语法正误
 */
bool Parser::CREATE_Rec() {
	tmp_word = (Word*)look;

	if (Match(tag.ID)) {
		colname.push_back(tmp_word->str);

		if (Match(tag.INT)) {
			if (Match(',')) {
				return CREATE_Rec();
			} else if (Match(tag.DEFAULT)) {
				if (Match('=')) {
					if (Calculator()) {
						defval[colname.back()] = res_cal;
						if (tmp_look->tag == ',') {
							return CREATE_Rec();	
						} else if (tmp_look->tag == ')') {
							return true;
						} else {
							Error("CREATE语句中数字后语法错误");
							return false;
						}
					} else {
						return false;
					}
				} else {
					Error("CREATE语句缺少=");
					return false;
				}
			} else if (Match(')')) {
				return true;
			} else {
				Error("CREATE语句中INT后语法错误");
				return false;
			}
		} else {
			Error("CREATE语句缺少INT");
			return false;
		}
	} else if (Match(tag.PRIMARY)) {
		// 判断是否只含有一个PRIMARY KEY语句
		if (pkey.size() == 0) {
			if (Match(tag.KEY)) {
				if (Match('(')) {
					if (Primary_Rec()) {
						if (Match(')')) {
							return true;
						} if (Match(',')) {
							return CREATE_Rec();
						} else {
							Error("CREATE语句语法错误");
							return false;
						}
					} else {
						return false;
					}
				} else {
					Error("CREATE语句缺少(");
					return false;
				}
			} else {
				Error("CREATE语句缺少KEY");
				return false;
			}
		} else {
			Error("CREATE语句语法错误，含有多个PRIMARY KEY语句");
			return false;
		}
	} else {
		Error("CREATE语句语法错误");
		return false;
	}
}

/**
 * 递归判断CREATE语句中PRIMARY KEY()内语句的语法正误
 */
bool Parser::Primary_Rec() {
	tmp_word = (Word*)look;

	if (Match(tag.ID)) {
//		if (pkey.empty()) {
//			cout << "PKEY为空！" << endl;
//		} else {
//			cout << "PKEY不为空！" << endl;
//		}

		if (Match(')')) {
			pkey.push_back(tmp_word->str);
			return true;
		} else if (Match(',')) {
			pkey.push_back(tmp_word->str);
			return Primary_Rec();
		} else {
			Error("CREATE语句中PRIMARY KEY语句错误");
			return false;
		}
	} else {
		Error("CREATE语句中PRIMARY KEY语句缺少ID");
		return false;
	}
}

/**
 * 递归判断INSERT语句中()内语句的语法正误
 * 如果语法正确，则返回插入"列"的个数
 * 如果语法错误，则返回-1
 */
int Parser::INSERT_Col_Rec(int num) {
	tmp_word = (Word*)look;

	if (Match(tag.ID)) {
		colname.push_back(tmp_word->str);

		if (Match(',')) {
			return INSERT_Col_Rec(num + 1);
		} else if (Match(')')) {
			return num + 1;
		} else {
			Error("INSERT语句语法错误");
			return -1;
		}
	} else {
		Error("INSERT语句语法错误");
		return -1;
	}
}

/**
 * 递归判断INSERT语句中()内语句的语法正误
 * 如果语法正确，则返回插入"值"的个数
 * 如果语法错误，则返回-1
 */
int Parser::INSERT_Val_Rec(int num) {
	if (Calculator()) {
		val.push_back(res_cal);				

		if (tmp_look->tag == ',') {
			return INSERT_Val_Rec(num + 1);
		} else if (tmp_look->tag == ')') {
			return num + 1;
		} else {
			Error("INSERT语句语法错误");
			return -1;
		}
	} else {
		return -1;
	}
}

/**
 * 递归判断DELETE/SELECT语句中WHERE语句的语法正误
 */
bool Parser::WHERE_Rec() {
	Statement* tmpstmt = new Statement();

	if (Match('!')) {
		tmpstmt->isNot = true;
	}
	tmp_look = look;
	
	if (Match(tag.ID)) {
		tmp_word = (Word*)tmp_look;
		string tmpname = tmp_word->str;
		tmp_look = look;
		bool flag_cal = false;

		if (Match('+') || Match('-') || Match('*') || Match('/')) {
			tmpstmt->cal_token = tmp_look->tag;
			if (Calculator()) {
				tmpstmt->cal_num = res_cal;
				flag_cal = true;
			} else {
				return false;
			}
		}

		
		bool flag_match;
		if (flag_cal) {
			flag_match = tmp_look->tag == tag.EQ || tmp_look->tag == tag.GE ||tmp_look->tag ==tag.LE || tmp_look->tag == tag.NE || tmp_look->tag == '>' || tmp_look->tag == '<';
		} else {
			flag_match = Match(tag.EQ) || Match(tag.GE) || Match(tag.LE) || Match(tag.NE) || Match('>') || Match('<');
		}

		if (flag_match) {
			tmpstmt->token = tmp_look->tag;
			if (Calculator()) {
				tmpstmt->num = res_cal;

				if (tmp_look->tag == ';') {
					stmts[tmpname] = tmpstmt;
					whereArg.push_back(stmts);
					return true;
				} else if (tmp_look->tag == tag.AND) {
					stmts[tmpname] = tmpstmt;
					return WHERE_Rec();
				} else if (tmp_look->tag == tag.OR) {
					stmts[tmpname] = tmpstmt;
					whereArg.push_back(stmts);
					stmts.clear();
					return WHERE_Rec();
				} else {
					Error("WHERE语句语法错误");
					return false;
				}
			} else {
				Error("WHERE语句缺少数字");
				return false;
			}
		} else {
			Error("WHERE语句缺少算术运算符");
			return false;
		}
	} else {
		Error("WHERE语句语法错误");
		return false;
	}
}

/**
 * 递归判断SELECT语句中()内语句的语法正误
 */
bool Parser::SELECT_Rec() {
	tmp_word = (Word*)look;

	if (Match(tag.ID)) {
		if (Match(tag.FROM)) {
			colname.push_back(tmp_word->str);
			return true;
		} else if (Match(',')) {
			colname.push_back(tmp_word->str);
			return SELECT_Rec();
		} else {
			Error("SELECT语句缺少FROM");
			return false;
		}
	} else {
		Error("SELECT语句缺少ID");
		return false;
	}
}

/**
 * 遇到语法错误时显示行号和具体错误信息
 */
void Parser::Error(string s) {
	cout << "Line:" << lex.line << " " << s << endl;
	Clear();

	while (true) {
		if (!Match(';')) {
			Move();
		} else {
			break;	
		}
	}
}

/**
 * 清除临时变量内的赋值
 */
void Parser::Clear() {
	tabname = "";
	colname.clear();
	pkey.clear();
	defval.clear();
	val.clear();
	whereArg.clear();
	stmts.clear();
	isall = false;
}

/**
 * 计算复杂逻辑运算的值
 */
bool Parser::Calculator() {
	// 将中缀表达式转换为后缀表达式
	stack<Token*> op_stack;				// 存放操作符的堆栈
	vector<Token*> post;				// vector储存后缀表达式

	bool op_flag = false;				// 判断是否可以求值
	int pre_flag = 0;					// 判断左右括号是否匹配
	
	while (true) {
		tmp_look = look;

		if (Match(tag.NUM)) {
			post.push_back(tmp_look);
		} else if (Match('(')) {
			pre_flag++;
			
			op_stack.push(tmp_look);
		} else if (Match('+')) {
			if (!op_stack.empty() && (op_stack.top()->tag == '*' || op_stack.top()->tag == '/' || op_stack.top()->tag == '-')) {
				post.push_back(op_stack.top());
				op_stack.pop();
				op_stack.push(tmp_look); 
			} else {
				op_stack.push(tmp_look);
			}
		} else if (Match('-')) {
			if (!op_stack.empty() && (op_stack.top()->tag == '*' || op_stack.top()->tag == '/')) {
				post.push_back(op_stack.top());
				op_stack.pop();
				op_stack.push(tmp_look); 
			} else {
				op_stack.push(tmp_look);
			}
		} else if (Match('*') || Match('/')) {
			op_stack.push(tmp_look);
		} else if (Match(')')) {
			if (pre_flag == 0) {
				op_flag = true;
				break;
			} else {
				pre_flag--;
				
				while (op_stack.top()->tag != '(') {
					post.push_back(op_stack.top());
					op_stack.pop();
				}
				op_stack.pop();
			}
		} else if (Match(',') || Match(';') || Match(tag.AND) || Match(tag.OR) || Match(tag.EQ) || Match(tag.GE) || Match(tag.LE) || Match(tag.NE) || Match('>') || Match('<')) {
			op_flag = true;
			break;
		} else {
			Error("算术运算表达式语法错误");
			break;
		}
	}
	// 将栈中剩余元素依次弹出到后缀表达式数组中
	while (!op_stack.empty()) {
		post.push_back(op_stack.top());
		op_stack.pop();
	}


	// 通过后缀表达式求值
	if (op_flag) {
		stack<int> res_stack;			// 定义堆栈存放操作数每次运算的结果
		int tmp_a, tmp_b, tmp_c;		// 定义计算时使用的临时变量
		bool flag_0 = false;			// 判断零是否为除数

		vector<Token*>::iterator it_token = post.begin();
		for (; it_token != post.end(); it_token++) {
			if ((*it_token)->tag == tag.NUM) {
				tmp_num = (Num*)(*it_token);
				res_stack.push(tmp_num->value);
			} else if ((*it_token)->tag == '-') {
				tmp_b = res_stack.top();
				res_stack.pop();
				tmp_a = res_stack.top();
				res_stack.pop();
				tmp_c = tmp_a - tmp_b;
				res_stack.push(tmp_c);
			} else if ((*it_token)->tag == '+') {
				tmp_b = res_stack.top();
				res_stack.pop();
				tmp_a = res_stack.top();
				res_stack.pop();
				tmp_c = tmp_a + tmp_b;
				res_stack.push(tmp_c);
			} else if ((*it_token)->tag == '*') {
				tmp_b = res_stack.top();
				res_stack.pop();
				tmp_a = res_stack.top();
				res_stack.pop();
				tmp_c = tmp_a * tmp_b;
				res_stack.push(tmp_c);
			} else if ((*it_token)->tag == '/') {
				tmp_b = res_stack.top();
				res_stack.pop();
				tmp_a = res_stack.top();
				res_stack.pop();
				if (tmp_b == 0) {
					Error("0不能作为除数进行计算");
					flag_0 = true;
					break;
				}
				tmp_c = tmp_a / tmp_b;
				res_stack.push(tmp_c);
			}
		}

		if (!flag_0) {
			res_cal = res_stack.top();
			return true;
		}
	}

	return false; 
}
