PROJECT = tftpserver
SOURCE = tftpserver.c
HEADERS = wrapper.h

CFLAGS = -Wall -g
LDFLAGS =
OBJECTS = ${SOURCE:.c=.o}

$(PROJECT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(PROJECT)
	
$(OBJECTS): $(HEADERS)

clean:
	-rm -f $(PROJECT) $(OBJECTS) *.core
