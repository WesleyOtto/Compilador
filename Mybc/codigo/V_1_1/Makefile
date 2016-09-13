CFLAGS=-I. -g

relocatables=lexer.o mybc.o parser.o

executable=mybc

$(executable): $(relocatables)
	cc -o $(executable) $(relocatables)
clean:
	$(RM)  $(relocatables)
mostlyclean: clean
	$(RM) $(executable) *~
