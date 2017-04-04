#include "Lexer.h"

int Lexer::line = 1;

Lexer::Lexer() {

    peek = ' ';
    words["CREATE"] = tag.CREATE;
    words["TABLE"] = tag.TABLE;
    words["INT"] = tag.INT;
    words["PRIMARY"] = tag.PRIMARY;
    words["DEFAULT"] = tag.DEFAULT;
    words["KEY"] = tag.KEY;
    words["INSERT"] = tag.INSERT;
    words["INTO"] = tag.INTO;
    words["VALUES"] = tag.VALUES;
    words["DELETE"] = tag.DELETE;
    words["FROM"] = tag.FROM;
    words["WHERE"] = tag.WHERE;
    words["SELECT"] = tag.SELECT;
    
}

Token* Lexer::scan() {
    for( ; ; getChar()) {
        if(peek == ' ' || peek == '\t')
            continue;
        else if(peek == '\n')
            line++;
        else
            break;

    }

    switch(peek) 
    {
        case '&':
            if(getChar('&'))
                return new Word("&&", tag.AND);
            else
                return new Token('&');
        case '|':
            if(getChar('|'))
                return new Word("||", tag.OR);
            else
                return new Token('|');
        case '<':
            if(getChar('='))
                return new Word("<=", tag.LE);
            else if(peek == '>'){
                peek = ' ';
                return new Word("<>", tag.NE);
            }
            else
                return new Token('<');
        case '>':
            if(getChar('='))
                return new Word(">=", tag.GE);
            else
                return new Token('>');
        case '=':
            if(getChar('='))
                return new Word("==", tag.EQ);
            else
                return new Token('=');
    }

    if(isDigit(peek))
    {
        int value = 0;
        do{
            value = value * 10 + digit(peek);
            getChar();

        }while(isDigit(peek));

        return new Num(value);

    }

    if(isLetterOrUnderline(peek))
    {
        string str;
        do{
            str.append(1, peek);
            getChar();

        }while(isDigit(peek) || isLetterOrUnderline(peek));

        if(words.find(str) == words.end()){
            return new Word(str, tag.ID);

        }else{
            return new Word(str, words[str]);
        }

    }
    Token* token = new Token(peek);
    peek = ' ';
    return token;

}

void Lexer::getChar()
{

    peek = getchar();

}

int Lexer::digit(char c) {

    return c - 48;

}


bool Lexer::getChar(char c) {

    getChar();
    
    if(peek != c) {

        return false;
    }
    peek = ' ';
    return true;

}

bool Lexer::isLetterOrUnderline(char c) {

    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}


bool Lexer::isDigit(char c) {
    return (c >= '0' && c <= '9');
}

