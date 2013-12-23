PROGRAM=test_gntshr_gnttab

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).c
	gcc -c -o $(PROGRAM).o $(PROGRAM).c
	gcc $(PROGRAM).o -I xenctrl.h -lxenctrl -o $(PROGRAM)

clean:
	rm -f $(PROGRAM) $(PROGRAM).o
