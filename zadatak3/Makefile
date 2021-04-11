CFLAGS = -Wall -g 


SRCS1 = tcpserver.c 
OBJS1 = $(SRCS1:.c=.o) 
LIBS = 
TARGET1 = tcpserver  


SRCS2 = tcpklijent.c 
OBJS2 = $(SRCS2:.c=.o) 
TARGET2 = tcpklijent 

all: $(TARGET1) $(TARGET2)



$(TARGET1): $(OBJS1) 
	$(CC) $(CFLAGS) $(OBJS1) -o $(TARGET1) $(LIBS)

 
 
$(TARGET2): $(OBJS2) 
	$(CC) $(CFLAGS) $(OBJS2) -o $(TARGET2) $(LIBS)



clean:
	-rm -f $(TARGET1) $(OBJS1) *.core
	-rm -f $(TARGET2) $(OBJS2) *.core
