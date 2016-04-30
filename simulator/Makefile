# Makefile for project 1

CC = cc
CFLAGS = -g -c -Wall
LDFLAGS = 
SOURCES = simulator.c readerfile.c execution.c  convert.c operation.c
OBJECTS = $(SOURCES:.c = .o)
EXECUTABLE = pipeline

# Include .h file here
INCLUDES =  simulator.h  readerfile.h declaration.h function.h execution.c  convert.h operation.h
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
%.o: %.c ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE) *.bin *.rpt
