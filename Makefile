# Adapt GUPATH to point to your gurobi installation
# or set the environment variable GUROBI_HOME accordingly
GUPATH=$(GUROBI_HOME)

GUINCLUDE=$(GUPATH)/include
GULIB=$(GUPATH)/lib/libgurobi.so.2.0.1

CC=gcc
CFLAGS= -O3 -std=c99 -pedantic -Wall -Wshadow -W -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wpointer-arith -Wnested-externs -Wundef -Wcast-qual -Wcast-align -Wwrite-strings -I$(GUINCLUDE)
OBJFILES=color.o graph.o greedy.o lpgurobi.o mwis.o mwis_grb.o mwis_grdy.o plotting.o

color: $(OBJFILES)
	$(CC) $(CFLAGS) -o color $(OBJFILES) $(GULIB) -lm -lpthread

stable: stable.o
	$(CC) $(CFLAGS) -o stable stable.o $(GULIB) -lm -lpthread


clean:
	rm -f *.o color stable mwis_gurobi.log look.lp vg.log*

color.o:     color.c color.h lp.h
graph.o:     graph.c graph.h
greedy.o:    greedy.c  color.h graph.h
lpgurobi.o:  lpgurobi.c color.h lp.h 
mwis.o:      mwis.c mwis.h color.h
mwis_grdy.o: mwis_grdy.c color.h graph.h
mwis_grb.o:  mwis_grb.c color.h lp.h
plotting.o:  plotting.c color.h
stable.o:    stable.c color.h lp.h

