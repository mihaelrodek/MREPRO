lab1: bot server CandC

bot: bot.o
	cc -o bot bot.o

bot.o: bot.c
	cc -c bot.c

UDP_server: server.o
	cc -o server server.o

UDP_server.o: server.c
	cc -c server.c

CandC: CandC.o
	cc -o CandC CandC.o

CandC.o: CandC.c
	cc -c CandC.c

clean:
	-rm -f bot.o bot server.o server CandC.o CandC *.core
