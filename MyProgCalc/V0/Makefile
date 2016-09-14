CFLAGS=-I. -g

relocatables= lexer.o progcalc.o parser.o keywords.o

executable=progcalc

$(executable): $(relocatables)
	cc -o $(executable) $(relocatables)
clean:
	$(RM)  $(relocatables)
mostlyclean: clean
	$(RM) $(executable) *~
indent:
	indent -nfca -orig -nuts -ts4*.[ch]
