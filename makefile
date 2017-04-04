VPATH = src
CC = g++
OBJS = Lexer.o Parser.o Table.o 
HFILE = src/Tag.h src/Word.h src/Num.h src/Token.h src/Statement.h
PROG = ssql
$(PROG):$(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(HFILE) src/Main.cpp
Lexer.o: Lexer.h Lexer.cpp
	$(CC) -c src/Lexer.cpp

Parser.o: Parser.h Parser.cpp
	$(CC) -c src/Parser.cpp

Table.o: Table.h Table.cpp
	$(CC) -c src/Table.cpp

clean:
	rm -f $(PROG) $(OBJS) ssql
