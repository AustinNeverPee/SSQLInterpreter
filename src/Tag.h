#ifndef TAG_H
#define TAG_H
#include<iostream>
using namespace std;

class Tag 
{
    public:
	enum {
	CREATE = 256, TABLE = 257, INT = 258, PRIMARY = 259, DEFAULT = 260, 
    KEY = 261, INSERT = 262, INTO = 263, VALUES = 264, DELETE = 265,
	FROM = 266, WHERE = 267, SELECT = 268, ID = 269, AND = 270,
	NE = 271, OR = 272, EQ = 273, GE = 274, LE = 275, NUM = 276
	};

};



#endif
