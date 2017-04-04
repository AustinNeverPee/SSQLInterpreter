CC = g++
OBJS = Lexer.o Parser.o Table.o 
HFILE = Tag.h Word.h Num.h Token.h Statement.h
PROG = ssql
$(PROG):$(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(HFILE) Main.cpp
Lexer.o: Lexer.h Lexer.cpp
	$(CC) -c Lexer.cpp

Parser.o: Parser.h Parser.cpp
	$(CC) -c Parser.cpp

Table.o: Table.h Table.cpp
	$(CC) -c Table.cpp

clean:
	rm -f $(PROG) $(OBJS)
