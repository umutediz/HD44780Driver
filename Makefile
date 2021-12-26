IDIR =.
CC=gcc
CFLAGS=-I$(IDIR) -g3
ODIR=.

LIBS=-lwiringPi

_DEPS = commands.h instructions.h fallbacks.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o commands.o instructions.o fallbacks.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
